
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

// #include <Arduino.h>
#include <stdint.h>

#define SLIP_FEND (char)0xC0
#define SLIP_FESC (char)0xDB
#define SLIP_TFEND (char)0xDC
#define SLIP_TFESC (char)0xDD

class Slip {
public:
  // uint16_t unpackedSize(char *data, uint16_t len);
  uint16_t unpack(char b);
  uint16_t unpack(char *data, uint16_t len);

  uint16_t packedSize(char *src, uint16_t len);
  uint16_t pack(char *src, char *dst, uint16_t len);

  char dataDecoded[255];
  char dataEncoded[255];

private:
  // Flags
  char inPacket_ = false;
  char inEscape_ = false;
  // Pointer for unpacking
  char *p_;
};

#endif