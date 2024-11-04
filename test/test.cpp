#include "slip.hpp"
#include <iostream>

int main(int argc, char **argv) {
  Slip s = Slip(255, 255);

  char test_buffer[]{1, 2, 3, 4};

  std::cout << "Data source size: " << sizeof(test_buffer) << std::endl;
  uint16_t ps = s.PackedSize(test_buffer, sizeof(test_buffer));
  std::cout << "Packed size would be: " << ps << std::endl;

  std::vector<char> packedData(ps);
  uint16_t len = s.Pack(test_buffer, packedData.data(), sizeof(test_buffer));
  std::cout << "Len written in buffer: " << len << std::endl;

  return 0;
}
