/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2018 Roman Lebedev
    Copyright (C) 2018 Stefan Hoffmeister

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "common/Common.h"                                  // for uint32
#include "decompressors/AbstractParallelizedDecompressor.h" // for Abstract...
#include "io/ByteStream.h"                                  // for ByteStream

namespace rawspeed {

class RawImage;

class PanasonicDecompressorV5 final : public AbstractParallelizedDecompressor {
  static constexpr uint32 SerializationBlockSize = 0x4000;
  static constexpr uint32 SerializationBlockSizeMask =
      0x3FFF; // == 0x4000 - 1, set all bits

  // The RW2 raw image buffer is built from individual blocks of size
  // SerializationBlockSize bytes. These blocks themselves comprise of two
  // sections, split at swapped at split into sections:
  //     bytes:
  //     [0..section_split_offset-1][section_split_offset..SerializationBlockSize-1]
  //     pixels: [a..b][0..a-1]
  // When reading, these two sections need to be swapped to enable linear
  // prcessing..
  static constexpr uint32 section_split_offset = 0x2008;

  static constexpr uint32 PixelDataBlockSize = 16;

  struct DataPump;

  void decompressThreaded(const RawDecompressorThread* t) const final;

  ByteStream input;

  const uint32 bps;
  uint32 encodedDataSize;

public:
  PanasonicDecompressorV5(const RawImage& img, const ByteStream& input_,
                          uint32 bps_);
};

} // namespace rawspeed
