/**
 * @file slip.cpp
 *
 * Library to pack/unpack data with SLIP protocol.
 *
 * @author Alexandre Maurer (alexmaurer@madis.ch)
 * @license MIT
 *
 */
#include "slip.h"

/**
 * @brief Try to unpack a complete frame. Only the first frame in buffer will be
 * processed by this function. The remaining data will not be processed.
 *
 * @param data
 * @param len
 * @return uint16_t return the size of unpacked data or 0 if no valid packet has
 * been found.
 */
uint16_t Slip::unpack(char *data, uint16_t len) {
  if (len <= 0)
    return 0;

  inPacket_ = 0;
  inEscape_ = 0;

  while (len--) {
    uint16_t ret = unpack(*data++);
    if (ret)
      return ret;
  }

  return 0;
}

/**
 *
 * @brief Try to unpack, byte after byte.
 *
 * @param b
 * @return uint16_t len of frame decoded
 */
uint16_t Slip::unpack(char b) {
  // Serial.printf("%02X-", b);

  if (inPacket_ && inEscape_) {
    inEscape_ = false;

    // In escape mode, only TFEND or TFESC can be received. Otherwise, discard
    // all data !
    switch (b) {
    case SLIP_TFEND:
      *p_++ = SLIP_FEND;
      break;

    case SLIP_TFESC:
      *p_++ = SLIP_FESC;
      break;

    default: {
      inPacket_ = 0;
      inEscape_ = 0;
      p_ = dataDecoded;
    } break;
    }
  } else if (inPacket_) {
    switch (b) {
    case SLIP_FESC:
      inEscape_ = true;
      break;

    case SLIP_FEND: {
      uint16_t frameLen = p_ - dataDecoded;
      if (frameLen > 0) {
        inPacket_ = false;
        p_ = dataDecoded;
        return frameLen;
      }
    } break;

    default:
      *p_++ = b;
      break;
    }
  } else {
    if (b == SLIP_FEND) {
      inPacket_ = true;
      p_ = dataDecoded;
    }
  }

  return 0;
}

/**
 * @brief Return the calculated size that would be occupied after packing with
 * SLIP protocol
 *
 * @param src
 * @param len
 * @return uint16_t
 */
uint16_t Slip::packedSize(char *src, uint16_t len) {
  // 2 bytes for the packet delimiters (0xC0 and the beginning and end of
  // packet)
  uint16_t ret = 2;

  // Every byte that must be escaped in the data will take 2 bytes after packing
  while (len--) {
    if (*src == SLIP_FEND || *src == SLIP_FESC)
      ret += 2;
    else
      ret++;

    src++;
  }

  return ret;
}

/**
 * @brief Read src data, pack and write result into dst.
 *
 * @param src pointer to data to be packed
 * @param dst pointer to output buffer
 * @param len len of data to process
 * @return uint16_t return len of data written to the dst buffer
 */
uint16_t Slip::pack(char *src, char *dst, uint16_t len) {
  if (len <= 0)
    return 0;

  char *start = dst;

  // Starting of frame
  *dst++ = SLIP_FEND;

  while (len--) {
    if (*src == SLIP_FEND) {
      *dst++ = SLIP_FESC;
      *dst++ = SLIP_TFEND;
    } else if (*src == SLIP_FESC) {
      *dst++ = SLIP_FESC;
      *dst++ = SLIP_TFESC;
    } else
      *dst++ = *src;

    src++;
  }

  // End of frame
  *dst++ = SLIP_FEND;

  return dst - start;
}
