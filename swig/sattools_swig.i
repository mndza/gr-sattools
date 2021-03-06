/* -*- c++ -*- */

#define SATTOOLS_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "sattools_swig_doc.i"

%{
#include "sattools/asm_golay_decoder.h"
#include "sattools/async_crc32c.h"
#include "sattools/golay24_decode.h"
#include "sattools/reed_solomon_ccsds_decode.h"
%}

%include "sattools/asm_golay_decoder.h"
GR_SWIG_BLOCK_MAGIC2(sattools, asm_golay_decoder);
%include "sattools/async_crc32c.h"
GR_SWIG_BLOCK_MAGIC2(sattools, async_crc32c);
%include "sattools/golay24_decode.h"
GR_SWIG_BLOCK_MAGIC2(sattools, golay24_decode);
%include "sattools/reed_solomon_ccsds_decode.h"
GR_SWIG_BLOCK_MAGIC2(sattools, reed_solomon_ccsds_decode);


#ifdef ENABLE_SATTOOLS_ZEROMQ
%{
#include "sattools/csp_zmqhub_pub_sink.h"
%}

%include "sattools/csp_zmqhub_pub_sink.h"
GR_SWIG_BLOCK_MAGIC2(sattools, csp_zmqhub_pub_sink);
#endif
