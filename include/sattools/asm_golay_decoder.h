/* -*- c++ -*- */
/* 
 * Copyright 2018 Diego Hurtado de Mendoza.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_SATTOOLS_ASM_GOLAY_DECODER_H
#define INCLUDED_SATTOOLS_ASM_GOLAY_DECODER_H

#include <sattools/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
  namespace sattools {

    /*!
     * \brief <+description of block+>
     * \ingroup sattools
     *
     */
    class SATTOOLS_API asm_golay_decoder : virtual public gr::hier_block2
    {
     public:
      typedef boost::shared_ptr<asm_golay_decoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sattools::asm_golay_decoder.
       *
       * To avoid accidental use of raw pointers, sattools::asm_golay_decoder's
       * constructor is in a private implementation
       * class. sattools::asm_golay_decoder::make is the public interface for
       * creating new instances.
       */
      static sptr make(
        const int bitrate, const std::string& asm_bits, const int asm_threshold,
        const bool ccsds_randomize, const bool ccsds_rs, const bool crc32c
      );
    };

  } // namespace sattools
} // namespace gr

#endif /* INCLUDED_SATTOOLS_ASM_GOLAY_DECODER_H */

