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
#include "async_crc32c_impl.h"

extern "C" {
  #include "crc32c/crc32c.h"
}

namespace gr {
  namespace sattools {

    async_crc32c::sptr
    async_crc32c::make(const bool check)
    {
      return gnuradio::get_initial_sptr
        (new async_crc32c_impl(check));
    }

    async_crc32c_impl::async_crc32c_impl(const bool check)
      : gr::block("async_crc32c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_out(pmt::mp("out"));
        message_port_register_in(pmt::mp("in"));
        if (check) {
          set_msg_handler(pmt::mp("in"),
            boost::bind(&async_crc32c_impl::pdu_check, this, _1));
        } else {
          set_msg_handler(pmt::mp("in"),
            boost::bind(&async_crc32c_impl::pdu_generate, this, _1));
        }
    }

    async_crc32c_impl::~async_crc32c_impl()
    {
    }

    void
    async_crc32c_impl::pdu_check(pmt::pmt_t pdu) {
        const pmt::pmt_t frame = pmt::cdr(pdu);
        size_t frame_len(0);
        const uint8_t* bytes_in = pmt::u8vector_elements(frame, frame_len);

        const uint32_t crc = crc32c_update(0, bytes_in, frame_len - 4);

        const uint32_t crc_rx = ((bytes_in[frame_len - 4] << 24) |
                                 (bytes_in[frame_len - 3] << 16) |
                                 (bytes_in[frame_len - 2] <<  8) |
                                 (bytes_in[frame_len - 1]));
        
        if (crc == crc_rx) {
          pmt::pmt_t output = pmt::init_u8vector(frame_len - 4, bytes_in);
          pmt::pmt_t msg_pair = pmt::cons(pmt::car(pdu), output);
          message_port_pub(pmt::mp("out"), msg_pair);
        }
    }

    void
    async_crc32c_impl::pdu_generate(pmt::pmt_t pdu) {
        const pmt::pmt_t frame = pmt::cdr(pdu);
        size_t frame_len(0);
        const uint8_t* bytes_in = pmt::u8vector_elements(frame, frame_len);

        const uint32_t crc = crc32c_update(0, bytes_in, frame_len);

        uint8_t* bytes_out = (uint8_t*)malloc(4 + frame_len);
        
        // Copy data
        memcpy((void*)bytes_out, (const void*)bytes_in, frame_len);

        // Copy CRC
        // TODO: endianness option?
        frame_len += 4;
        bytes_out[frame_len - 4] = static_cast<uint8_t>((crc >> 24) & 0xFF);
        bytes_out[frame_len - 3] = static_cast<uint8_t>((crc >> 16) & 0xFF);
        bytes_out[frame_len - 2] = static_cast<uint8_t>((crc >>  8) & 0xFF);
        bytes_out[frame_len - 1] = static_cast<uint8_t>( crc        & 0xFF);

        pmt::pmt_t output = pmt::init_u8vector(frame_len, bytes_out);
        pmt::pmt_t msg_pair = pmt::cons(pmt::car(pdu), output);
        message_port_pub(pmt::mp("out"), msg_pair);
        free(bytes_out);
    }

  } /* namespace sattools */
} /* namespace gr */

