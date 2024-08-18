/* -*- c++ -*- */
/*
 * Copyright 2024 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "source_impl.h"

void paveldeminsdr_send_commands( SOCKET socket, uint32_t *bufptr)
{
  std::stringstream message;

  ssize_t total = 10*sizeof(uint32_t);
  ssize_t size;
  size = ::send( socket, bufptr, total, MSG_NOSIGNAL );

  if ( size != total )
  {
    message << "Sending command failed.";
    throw std::runtime_error( message.str() );
  }
}

namespace gr {
  namespace paveldeminsdr {

    using output_type = gr_complex;
    source::sptr
    source::make(const std::string &host, unsigned short port, unsigned int samp_rate, unsigned int freq1, unsigned int freq2, unsigned int freq3, unsigned int freq4, unsigned int freq5, unsigned int freq6, unsigned int freq7, unsigned int freq8)
    {
      return gnuradio::make_block_sptr<source_impl>(
        host, port, samp_rate, freq1, freq2, freq3, freq4, freq5, freq6, freq7, freq8);
    }


    /*
     * The private constructor
     */
    source_impl::source_impl(const std::string &host, unsigned short port, unsigned int samp_rate, unsigned int freq1, unsigned int freq2, unsigned int freq3, unsigned int freq4, unsigned int freq5, unsigned int freq6, unsigned int freq7, unsigned int freq8)
      : gr::sync_block("source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(8 /* min outputs */, 8 /*max outputs */, sizeof(output_type)))
    {
      std::string host_used = "192.168.1.100";
      std::stringstream message;
      struct sockaddr_in addr;
      
      if ( 48000 == samp_rate ) _rate_value = 0;
      else if ( 96000 == samp_rate ) _rate_value = 1;
      else if ( 192000 == samp_rate ) _rate_value = 2;
      else if ( 384000 == samp_rate ) _rate_value = 3;
      else throw std::runtime_error( "Invalid sampling rate." );
      
      _rate = samp_rate;
      
      if ( !host.length() )
        host_used = "192.168.1.100";
      else
        host_used = host;

      if ( 0 == port )
        port = 1001;

      if ( ( _socket = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
        throw std::runtime_error( "Could not create TCP socket." );

      memset( &addr, 0, sizeof(addr) );
      addr.sin_family = AF_INET;
      inet_pton( AF_INET, host_used.c_str(), &addr.sin_addr );
      addr.sin_port = htons( port );

      if ( ::connect( _socket, (struct sockaddr *)&addr, sizeof(addr) ) < 0 )
      {
        message << "Could not connect to " << host_used << ":" << port << ".";
        throw std::runtime_error( message.str() );
      }
  
      _buf = (gr_complex *)malloc(BUF_SIZE_BYTES);
      if (!_buf)
      {
        message << "Could not allocate buffer.";
        throw std::runtime_error( message.str() );
      }
  
      set_center_freqs(freq1, freq2, freq3, freq4, freq5, freq6, freq7, freq8);
  
      }

    /*
     * Our virtual destructor.
     */
    source_impl::~source_impl()
    {
      free(_buf);
  
      ::close( _socket);
    }

    int
    source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out;
      int items_fetched;

      ssize_t size;
      ssize_t total = 8 * sizeof(gr_complex) * noutput_items;
      if (total > BUF_SIZE_BYTES)
        total = BUF_SIZE_BYTES;
      size = ::recv( _socket, _buf, total, MSG_WAITALL );

      if ( size != total )
        throw std::runtime_error( "Receiving samples failed." );
  
      items_fetched = (size / (8 * sizeof(gr_complex)));
  
      for(int kk=0; kk<items_fetched;++kk)
        for(int nn=0; nn<8;++nn)
        {
          out = (gr_complex *)output_items[nn];
          out[kk]=_buf[8*kk+nn];
        }

      return items_fetched;
    }
    
    int source_impl::set_center_freqs(unsigned int freq1, unsigned int freq2, unsigned int freq3, unsigned int freq4, unsigned int freq5, unsigned int freq6, unsigned int freq7, unsigned int freq8)
    {
      uint32_t buffer[10] = {0,0,0,0,0,0,0,0,0,0};

      _freqs[0] = _freq_values[0] = freq1;
      _freqs[1] = _freq_values[1] = freq2;
      _freqs[2] = _freq_values[2] = freq3;
      _freqs[3] = _freq_values[3] = freq4;
      _freqs[4] = _freq_values[4] = freq5;
      _freqs[5] = _freq_values[5] = freq6;
      _freqs[6] = _freq_values[6] = freq7;
      _freqs[7] = _freq_values[7] = freq8;
      
      buffer[1] = _rate_value;
      for(int kk=0; kk<8; kk++)
        buffer[kk+2] = _freq_values[kk];
      paveldeminsdr_send_commands( _socket, buffer );

      return 0;
}

  } /* namespace paveldeminsdr */
} /* namespace gr */
