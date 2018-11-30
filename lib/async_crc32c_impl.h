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

#ifndef INCLUDED_SATTOOLS_ASYNC_CRC32C_IMPL_H
#define INCLUDED_SATTOOLS_ASYNC_CRC32C_IMPL_H

#include <sattools/async_crc32c.h>

namespace gr {
  namespace sattools {

    class async_crc32c_impl : public async_crc32c
    {
     private:
      void pdu_check(pmt::pmt_t pdu);
      void pdu_generate(pmt::pmt_t pdu);

     public:
      async_crc32c_impl(const bool check);
      ~async_crc32c_impl();
    };

  } // namespace sattools
} // namespace gr

#endif /* INCLUDED_SATTOOLS_ASYNC_CRC32C_IMPL_H */

