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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "asm_golay_decoder_impl.h"

namespace gr {
  namespace sattools {

    asm_golay_decoder::sptr
    asm_golay_decoder::make(
      const int bitrate, const std::string& asm_bits, const int asm_threshold,
      const bool ccsds_randomize, const bool ccsds_rs, const bool crc32c
    ) {
      return gnuradio::get_initial_sptr
        (new asm_golay_decoder_impl(bitrate, asm_bits, asm_threshold, ccsds_randomize, ccsds_rs, crc32c));
    }

    /*
     * The private constructor
     */
    asm_golay_decoder_impl::asm_golay_decoder_impl(
      const int bitrate, const std::string& asm_bits, const int asm_threshold,
      const bool ccsds_randomize, const bool ccsds_rs, const bool crc32c
    ) : gr::hier_block2("asm_golay_decoder",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(0, 0, 0))
    {
      message_port_register_hier_out(pmt::mp("out"));

      // internal blocks
      asm_tagger = gr::digital::correlate_access_code_tag_bb::make(asm_bits, asm_threshold, "asm_found");
      hdr_pld_demux = gr::digital::header_payload_demux::make(
        24, 1, 0, "frame_len", "asm_found", false, sizeof(uint8_t), "rx_time", bitrate);
      golay_decoder = gr::sattools::golay24_decode::make();
      pack_bits = gr::blocks::repack_bits_bb::make(1, 8, "frame_len", false, gr::GR_MSB_FIRST);
      to_pdu = gr::blocks::tagged_stream_to_pdu::make(gr::blocks::pdu::byte_t, "frame_len");

      if (ccsds_randomize) {
        derandomizer = gr::digital::additive_scrambler_bb::make(0xA9, 0xFF, 7, 0, 1, "frame_len");
      }

      if (ccsds_rs) {
        rs_decoder = gr::sattools::reed_solomon_ccsds_decode::make();
      }

      if (crc32c) {
        crc32c_check = gr::sattools::async_crc32c::make(true);
      }

      // block connections
      connect(self(), 0, asm_tagger, 0);
      connect(asm_tagger, 0, hdr_pld_demux, 0);
      connect(hdr_pld_demux, 0, golay_decoder, 0);
      msg_connect(golay_decoder, "out", hdr_pld_demux, "header_data");
      if (ccsds_randomize) {
        connect(hdr_pld_demux, 1, derandomizer, 0);
        connect(derandomizer, 0, pack_bits, 0);
      } else {
        connect(hdr_pld_demux, 1, pack_bits, 0);
      }
      connect(pack_bits, 0, to_pdu, 0);
      if (ccsds_rs && crc32c) {
        msg_connect(to_pdu, "pdus", rs_decoder, "in");
        msg_connect(rs_decoder, "out", crc32c_check, "in");
        msg_connect(crc32c_check, "out", self(), "out");
      } else if (ccsds_rs && !crc32c) {
        msg_connect(to_pdu, "pdus", rs_decoder, "in");
        msg_connect(rs_decoder, "out", self(), "out");
      } else if (!ccsds_rs && crc32c) {
        msg_connect(to_pdu, "pdus", crc32c_check, "in");
        msg_connect(crc32c_check, "out", self(), "out");
      } else if (!ccsds_rs && !crc32c) {
        msg_connect(to_pdu, "pdus", self(), "out");
      }
    }

    /*
     * Our virtual destructor.
     */
    asm_golay_decoder_impl::~asm_golay_decoder_impl()
    {
    }


  } /* namespace sattools */
} /* namespace gr */

