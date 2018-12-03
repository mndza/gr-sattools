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

#ifndef INCLUDED_SATTOOLS_ASM_GOLAY_DECODER_IMPL_H
#define INCLUDED_SATTOOLS_ASM_GOLAY_DECODER_IMPL_H

#include <sattools/asm_golay_decoder.h>

#include <gnuradio/digital/correlate_access_code_tag_bb.h>
#include <gnuradio/digital/header_payload_demux.h>
#include <gnuradio/digital/additive_scrambler_bb.h>
#include <gnuradio/blocks/repack_bits_bb.h>
#include <gnuradio/blocks/tagged_stream_to_pdu.h>
#include <sattools/golay24_decode.h>
#include <sattools/reed_solomon_ccsds_decode.h>
#include <sattools/async_crc32c.h>

namespace gr {
  namespace sattools {

    class asm_golay_decoder_impl : public asm_golay_decoder
    {
     private:
      gr::digital::correlate_access_code_tag_bb::sptr asm_tagger;
      gr::digital::header_payload_demux::sptr hdr_pld_demux;
      gr::sattools::golay24_decode::sptr golay_decoder;
      gr::blocks::repack_bits_bb::sptr pack_bits;
      gr::blocks::tagged_stream_to_pdu::sptr to_pdu;
      gr::digital::additive_scrambler_bb::sptr derandomizer;
      gr::sattools::reed_solomon_ccsds_decode::sptr rs_decoder;
      gr::sattools::async_crc32c::sptr crc32c_check;

     public:
      asm_golay_decoder_impl(
        const int bitrate, const std::string& asm_bits, const int asm_threshold,
        const bool ccsds_randomize, const bool ccsds_rs, const bool crc32c);
      ~asm_golay_decoder_impl();
    };

  } // namespace sattools
} // namespace gr

#endif /* INCLUDED_SATTOOLS_ASM_GOLAY_DECODER_IMPL_H */

