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
#include "csp_zmqhub_pub_sink_impl.h"

namespace gr {
  namespace sattools {

    csp_zmqhub_pub_sink::sptr
    csp_zmqhub_pub_sink::make(const char *pub_endpoint)
    {
      return gnuradio::get_initial_sptr
        (new csp_zmqhub_pub_sink_impl(pub_endpoint));
    }

    csp_zmqhub_pub_sink_impl::csp_zmqhub_pub_sink_impl(const char *pub_endpoint)
      : gr::block("csp_zmqhub_pub_sink",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
      message_port_register_in(pmt::mp("pdu"));
      set_msg_handler(pmt::mp("pdu"),
        boost::bind(&csp_zmqhub_pub_sink_impl::pdu_handle, this, _1));

      // ZeroMQ initialization
      d_context = new zmq::context_t(1);
      d_pub_socket = new zmq::socket_t(*d_context, ZMQ_PUB);
      d_pub_socket->connect(pub_endpoint);
    }

    csp_zmqhub_pub_sink_impl::~csp_zmqhub_pub_sink_impl()
    {
      d_pub_socket->close();
      delete d_pub_socket;
      delete d_context;
    }

    void
    csp_zmqhub_pub_sink_impl::pdu_handle(pmt::pmt_t pdu) {
      // Internal variables
      const pmt::pmt_t frame = pmt::cdr(pdu);
      size_t frame_len(0);
      const uint8_t* bytes_in = pmt::u8vector_elements(frame, frame_len);

      // Convert CSP header to native endian and extract destination address
      const uint32_t csp_hdr = be32toh(*(uint32_t*)bytes_in);
      const uint8_t dst_addr = (csp_hdr >> 20) & 0x1F;
      
      // Prepare message with format from CSP zmqhub interface
      // - CSP destination address (1 byte)
      // - CSP header in machine endian (4 bytes)
      // - Packet payload
      zmq::message_t zmq_msg(1 + frame_len);
      uint8_t * zmq_buf = (uint8_t*) zmq_msg.data();
      zmq_buf[0] = dst_addr; 
      memcpy(&zmq_buf[1], (void*)&csp_hdr, sizeof(csp_hdr));
      memcpy(&zmq_buf[5], (void*)&bytes_in[4], frame_len - 4); 
      
      // Send to subscriber
      d_pub_socket->send(zmq_msg);
    }

  } /* namespace sattools */
} /* namespace gr */

