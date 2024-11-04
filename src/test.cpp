#include "slip.hpp"

int main(int argc, char **argv) {
  Slip s = Slip();

  char test_buffer[]{1, 2, 3, 4};
  char out[100];

  std::cout << "pointer " << s.decoding_buffer << std::endl;
  std::cout << "pointer " << s.decoding_buffer.get() << std::endl;
  std::cout << "Packed size would be "
            << s.packedSize(test_buffer, sizeof(test_buffer)) << std::endl;

  uint16_t len = s.pack(test_buffer, out, sizeof(test_buffer));
  std::cout << "Len written in buffer " << len << std::endl;
  uint16_t unpack_size = s.unpack(out, len);
  std::cout << "Unpack size " << unpack_size << std::endl;

  return 0;
}
