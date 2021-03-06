/* -*- c++ -*- */
/*
 * Copyright 2021 Diego Hurtado de Mendoza.
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
#include "reed_solomon_ccsds_decode_impl.h"

extern "C" {
  #include "rs/decode_rs_8.h"
}

namespace gr {
  namespace sattools {

    static const unsigned int N = 255;
    static const unsigned int K = 255 - 32;

    reed_solomon_ccsds_decode::sptr
    reed_solomon_ccsds_decode::make()
    {
      return gnuradio::get_initial_sptr
        (new reed_solomon_ccsds_decode_impl());
    }

    reed_solomon_ccsds_decode_impl::reed_solomon_ccsds_decode_impl()
      : gr::block("reed_solomon_ccsds_decode",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
      message_port_register_out(pmt::mp("out"));
      message_port_register_in(pmt::mp("in"));
      set_msg_handler(pmt::mp("in"),
        boost::bind(&reed_solomon_ccsds_decode_impl::pdu_decode, this, _1));
    }

    reed_solomon_ccsds_decode_impl::~reed_solomon_ccsds_decode_impl()
    {
    }

    void
    reed_solomon_ccsds_decode_impl::pdu_decode(pmt::pmt_t pdu) {
        // Internal variables
        const pmt::pmt_t frame = pmt::cdr(pdu);
        size_t frame_len(0);
        const uint8_t* bytes_in = pmt::u8vector_elements(frame, frame_len);
        uint8_t data[N] = { 0 };

        // Sanity check
        if (frame_len > N) {
          GR_LOG_INFO(d_logger, boost::format("frame length exceeds N (%1% > %2%), truncating") \
            % frame_len % N);
          frame_len = N;
        } else if (frame_len < (N-K)) {
          GR_LOG_INFO(d_logger, boost::format("frame length too small (%1% < %2%), extending") \
            % frame_len % (N-K));
          frame_len = N-K;
        }

        // Try to decode data using intermediate buffer
        // *NOT* dual-basis representation
        memcpy(data, bytes_in, frame_len);
        int rs_res = decode_rs_8(data, NULL, 0);

        // Ignore packet if not able to decode
        if (rs_res < 0) {
          GR_LOG_INFO(d_logger, "unable to decode frame");
          return;
        }

        // Strip parity data
        frame_len -= (N - K);

        // Send decoded PDU
        message_port_pub(pmt::mp("out"),
            pmt::cons(pmt::car(pdu), pmt::init_u8vector(frame_len, data)));
    }

  } /* namespace sattools */
} /* namespace gr */

