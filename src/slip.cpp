/**
 * @file slip.cpp
 *
 * Library to pack/unpack data with SLIP protocol.
 *
 * @author Alexandre Maurer (alexmaurer@madis.ch)
 * @license MIT
 *
 */
#include "slip.hpp"

Slip::Slip() : Slip(255, 255){};

Slip::Slip(uint16_t rx_buffer_size, uint16_t tx_buffer_size) {
  decoding_buffer = std::make_unique<std::vector<char>>(rx_buffer_size);
  encoding_buffer = std::make_unique<std::vector<char>>(tx_buffer_size);
}

Slip::~Slip() {
  decoding_buffer.reset();
  encoding_buffer.reset();
}

/**
 * @brief Try to unpack from a buffer of size len.
 *
 * @param data
 * @param len
 * @return uint16_t return the size of unpacked data or 0 if no valid packet has
 * been found.
 */
uint16_t Slip::unpack(char *data, uint16_t len) {
  if (len <= 0)
    return 0;

  // inPacket_ = 0;
  // inEscape_ = 0;

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
  if (inPacket_ && inEscape_) {
    inEscape_ = false;

    // In escape mode, only TFEND or TFESC can be received otherwise discard
    // char and pending data in buffer
    switch (b) {
    case SLIP_TFEND:
      decoding_buffer->push_back(SLIP_FEND);
      break;

    case SLIP_TFESC:
      decoding_buffer->push_back(SLIP_FESC);
      break;

    default: {
      inPacket_ = false;
      inEscape_ = false;
      unpack_index_ = 0;
    } break;
    }
  }
  // Not in escape mode but already inside packet
  else if (inPacket_) {
    switch (b) {
    case SLIP_FESC:
      inEscape_ = true;
      break;

    // end of packet
    case SLIP_FEND: {
      if (unpack_index_ > 0) {
        inPacket_ = false;
        return decoding_buffer->size();
      }
    } break;

    // Append char as-is
    default:
      decoding_buffer->push_back(b);
      break;
    }
  } else {
    // Beginning of packet
    if (b == SLIP_FEND) {
      inPacket_ = true;
      decoding_buffer->clear();
    }
  }

  return 0;
}

/**
 * @brief Return the calculated size that would be occupied after packing with
 * SLIP protocol
 *
 * @param src data source to pack
 * @param len data size
 * @return uint16_t size of packed data
 */
uint16_t Slip::PackedSize(char *src, uint16_t len) {
  // 2 bytes for the packet delimiters
  uint32_t ret = 2;

  // Every byte that must be escaped in the data will take 2 bytes after packing
  while (len--) {
    if (*src == SLIP_FEND || *src == SLIP_FESC)
      ret += 2;
    else
      ret++;

    src++;
  }

  if (ret > 0xFFFF)
    throw std::overflow_error("Packed size is bigger than uint16_t");

  return ret;
}

/**
 * @brief Read src data, pack and write result into dst.
 *
 * @param src data source to be packed
 * @param dst destination buffer for packed data
 * @param len data len to process
 * @return uint16_t return len of data written to the dst buffer
 */
uint16_t Slip::Pack(char *src, char *dst, uint16_t src_len) {
  if (src_len <= 0)
    return 0;

  char *start = dst;

  // Starting of frame
  *dst++ = SLIP_FEND;

  while (src_len--) {
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
