/* -*- c++ -*- */
/*
 * Copyright 2024 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PAVELDEMINSDR_SOURCE_IMPL_H
#define INCLUDED_PAVELDEMINSDR_SOURCE_IMPL_H

#include <paveldeminsdr/source.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef SOCKET
#define SOCKET int
#define INVSOC (-1)
#endif
#define BUF_SIZE_BYTES 65536

void paveldeminsdr_send_commands( SOCKET socket, uint32_t *bufptr );

namespace gr {
  namespace paveldeminsdr {

    class source_impl : public source
    {
     private:
      double _rate;
      double _freqs[8];
      uint32_t _rate_value;
      uint32_t _freq_values[8];
      gr_complex *_buf;
      SOCKET _socket;

     public:
      source_impl(const std::string &host, unsigned short port, unsigned int samp_rate, unsigned int freq1, unsigned int freq2, unsigned int freq3, unsigned int freq4, unsigned int freq5, unsigned int freq6, unsigned int freq7, unsigned int freq8);
      ~source_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
      
      int set_center_freqs(unsigned int freq1, unsigned int freq2, unsigned int freq3, unsigned int freq4, unsigned int freq5, unsigned int freq6, unsigned int freq7, unsigned int freq8);
    };

  } // namespace paveldeminsdr
} // namespace gr

#endif /* INCLUDED_PAVELDEMINSDR_SOURCE_IMPL_H */
