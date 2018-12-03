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
#include "golay24_decode_impl.h"

extern "C" {
#include "golay/golay.h"
}

namespace gr {
  namespace sattools {

    golay24_decode::sptr
    golay24_decode::make()
    {
      return gnuradio::get_initial_sptr
        (new golay24_decode_impl());
    }

    golay24_decode_impl::golay24_decode_impl()
      : gr::block("golay24_decode",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(0, 0, 0))
    {
      message_port_register_out(pmt::mp("out"));
      set_max_noutput_items(24);
    }

    golay24_decode_impl::~golay24_decode_impl()
    {
    }

    int
    golay24_decode_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const uint8_t *in = (const uint8_t *) input_items[0];
      pmt::pmt_t meta = pmt::PMT_F;

      // Only accept buffers containing the entire Golay codeword
      if (noutput_items != 24) {
        return 0;
      }

      // Pack bits
      uint32_t golay_in = 0;
      for (int i = 0; i < 24; ++i) { golay_in = (golay_in << 1) | in[i]; }

      int32_t golay_out = golay_decode(golay_in);
      if (golay_out != -1) {
        meta = pmt::make_dict();

        // Extract tags from stream and add to PDU metadata
        std::vector<tag_t> tags;
        get_tags_in_window(tags, 0, 0, noutput_items);
        for (tag_t& tag : tags) {
          meta = pmt::dict_add(meta, tag.key, tag.value);
        }

        // Add length of payload in bits
        uint32_t frame_bits = 8 * (golay_out & 0xFFF);
        meta = pmt::dict_add(meta, pmt::intern("frame_len"), pmt::from_long(frame_bits));
      }

      message_port_pub(pmt::mp("out"), meta);
      consume_each (noutput_items);
      return noutput_items;
    }

  } /* namespace sattools */
} /* namespace gr */

