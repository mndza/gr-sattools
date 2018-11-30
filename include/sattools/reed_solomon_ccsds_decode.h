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


#ifndef INCLUDED_SATTOOLS_REED_SOLOMON_CCSDS_DECODE_H
#define INCLUDED_SATTOOLS_REED_SOLOMON_CCSDS_DECODE_H

#include <sattools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace sattools {

    /*!
     * \brief <+description of block+>
     * \ingroup sattools
     *
     */
    class SATTOOLS_API reed_solomon_ccsds_decode : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<reed_solomon_ccsds_decode> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sattools::reed_solomon_ccsds_decode.
       *
       * To avoid accidental use of raw pointers, sattools::reed_solomon_ccsds_decode's
       * constructor is in a private implementation
       * class. sattools::reed_solomon_ccsds_decode::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace sattools
} // namespace gr

#endif /* INCLUDED_SATTOOLS_REED_SOLOMON_CCSDS_DECODE_H */

