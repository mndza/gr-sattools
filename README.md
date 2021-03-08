# gr-sattools

## pybombs recipe
```
category: common
depends: 
- gnuradio
- zeromq
source: git+https://github.com/mndza/gr-sattools.git
gitbranch: master
inherit: cmake
description: GNURadio OOT module with tools for satellite decoding
```

## manual build

There are no dependencies besides GNURadio 3.8. For GNURadio 3.7, please use the old `maint-3.7` branch.

Follow the usual GNURadio OOT module installation steps, modifying the `CMAKE_INSTALL_PREFIX` path if necessary (`/usr` for GNURadio installed through Ubuntu/Debian repositories, `/usr/local` by default when not set):

```
mkdir build/
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr ../
make -j4
sudo make install
sudo ldconfig
```
**NOTE:** Blocks requiring ZeroMQ will be built if ZeroMQ is found in the system. There is a CMake option to disable this, in case you do not need it:
```
cmake -DENABLE_SATTOOLS_ZEROMQ=OFF ../
```

## usage
see `examples/` directory

## blocks

### ASM+Golay Decoder

![ASM+Golay Decoder block](docs/asm_golay_block.png)

Decoder for ASM+Golay mode.

* **Input**: raw bitstream from the demodulator in unpacked bytes (1 bit/byte). 

* **Output**: messages containing CSP packets, which can be fed to the CSP zmqhub PUB sink block.

This is a hierarchical block roughly equivalent to the following blocks.

![ASM+Golay Decoder Internals](docs/asm_golay_hier_internals.png)

### Async CRC32-Castagnoli

Computes or checks CRC32-Castagnoli in async messages.

### Golay24 Decoder

Block used for header decoding in ASM+Golay mode. Expects 24 unpacked bits 
in the input port, which then are decoded using extended binary Golay code.
* If decoding is **successful**, a message is sent with a dictionary containing 
all tags from the input and a `frame_len` pair containing the frame length in bits.
* If decoding **fails**, a `PMT_F` message is sent.

### Reed-Solomon CCSDS Decoder

Decodes async messages using CCSDS Reed-Solomon variant (**NO** dual-basis representation).

### CSP zmqhub PUB sink

This block depends on ZeroMQ.
Connects PDUs containing CSP packets to a zmqhub interface from libcsp.
