#include <string>
#include <iostream>

#include "include/wt_file.h"

WTFile::WTFile(const char* filename) {
  page = (wt_page_t *) std::malloc(sizeof(wt_page_t));
  page_header = (wt_page_header_t *) std::malloc(sizeof(wt_page_header_t));
  block_header = (wt_block_header_t *) std::malloc(sizeof(wt_block_header_t));

  page->page_header = page_header;
  page->block_header = block_header;

  file_reader = new WTFileReader(filename);
}

void WTFile::parse() {
  int offset = file_reader->readBlockDesc();
  while(offset != WT_EOF) {
    offset = file_reader->readHeaders(page);
    file_reader->readEntries(page);
    printPage(page);
  }
}

void WTFile::printPage(wt_page_t *page) {
  std::cout << "page recno: " << page->page_header->recno;
  std::cout << " write_gen: ";
  std::cout << page->page_header->write_gen;
  std::cout << " mem_size: 0x";
  std::cout << std::hex << page->page_header->mem_size;
  std::cout << " entries: ";
  std::cout << std::dec << page->page_header->entries;
  std::cout << " type: " << (unsigned int) page->page_header->type;
  std::cout << " flags: " << (unsigned int) page->page_header->flags;
  std::cout << std::endl;
  std::cout << "  block disk_size: 0x";
  std::cout << std::hex << page->block_header->disk_size;
  std::cout << " checksum: 0x";
  std::cout << std::hex << page->block_header->checksum;
  std::cout << " flags: " << (unsigned int) page->block_header->flags;
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  WTFile *file = new WTFile(argv[1]);
  file->parse();
}
