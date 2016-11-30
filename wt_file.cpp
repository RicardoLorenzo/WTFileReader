#include <string>
#include <iostream>

#include "include/wt_file.h"

WTFile::WTFile(const char* filename) {
  file_reader = new WTFileReader(filename);
}

void WTFile::parse() {
  int offset = file_reader->readBlockDesc();
  offset = file_reader->readPage(page);
  //printPage(page);
}

void WTFile::printPage(wt_page_t *page) {
  std::cout << "recno: ";
  std::cout << page->page_header->recno;
  std::cout << " csum: 0x";
  std::cout << std::hex << page->block_header->checksum;
  std::cout << " flags: " << page->block_header->flags;
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  WTFile *file = new WTFile(argv[1]);
  file->parse();
}
