#include <stdio.h>
#include <string.h>
#include <iostream>

#include "include/wt_file.h"

WTBlockReader::WTBlockReader(struct wt_file_reader *reader) {
  wt_r = reader;
  assert(wt_r != NULL);
};

uint8_t* WTBlockReader::readBlock(int offset, int length) {
  uint8_t *data = new uint8_t[length];

  memcpy(data, (static_cast<uint8_t*>(wt_r->map) + offset), length);

  uint32_t desc = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
  std::cout << "desc: " << std::dec << desc << std::endl;

  return data;
}
