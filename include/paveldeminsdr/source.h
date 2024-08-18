/* -*- c++ -*- */
/*
 * Copyright 2024 hnch@gmx.net.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PAVELDEMINSDR_SOURCE_H
#define INCLUDED_PAVELDEMINSDR_SOURCE_H

#include <paveldeminsdr/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace paveldeminsdr {

    /*!
     * \brief <+description of block+>
     * \ingroup paveldeminsdr
     *
     */
    class PAVELDEMINSDR_API source : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of paveldeminsdr::source.
       *
       * To avoid accidental use of raw pointers, paveldeminsdr::source's
       * constructor is in a private implementation
       * class. paveldeminsdr::source::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &host, unsigned short port, unsigned int samp_rate, unsigned int freq1, unsigned int freq2, unsigned int freq3, unsigned int freq4, unsigned int freq5, unsigned int freq6, unsigned int freq7, unsigned int freq8);
      
      virtual int set_center_freqs(unsigned int freq1, unsigned int freq2, unsigned int freq3, unsigned int freq4, unsigned int freq5, unsigned int freq6, unsigned int freq7, unsigned int freq8) = 0;
      
    };

  } // namespace paveldeminsdr
} // namespace gr

#endif /* INCLUDED_PAVELDEMINSDR_SOURCE_H */
