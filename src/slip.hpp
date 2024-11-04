
/**
 * @file slip.h
 *
 * Library to pack/unpack data with SLIP protocol.
 *
 * @author Alexandre Maurer (alexmaurer@madis.ch)
 * @license MIT
 *
 */

#ifndef __ALEXMAURER_SLIP_H
#define __ALEXMAURER_SLIP_H

#include <memory>
#include <stdexcept>
#include <vector>

#define SLIP_FEND (char)0xC0
#define SLIP_FESC (char)0xDB
#define SLIP_TFEND (char)0xDC
#define SLIP_TFESC (char)0xDD

class Slip {
public:
  Slip();
  Slip(uint16_t rxbufferSize, uint16_t txBufferSize);
  ~Slip();

  uint16_t unpack(char b);
  uint16_t unpack(char *data, uint16_t len);

  uint16_t PackedSize(char *src, uint16_t src_len);
  uint16_t Pack(char *src, char *dst, uint16_t src_len);

  std::unique_ptr<std::vector<char>> decoding_buffer;
  std::unique_ptr<std::vector<char>> encoding_buffer;

private:
  // Flags
  bool inPacket_ = false;
  bool inEscape_ = false;
  // Pointer for unpacking
  char *p_;
  uint16_t unpack_index_;
};

#endif