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
    size_t total_entries = page->page_header->entries;
    /*
     * TODO: For the moment store the entries but we probably want
     *    to change that later.
     */
    wt_cell_unpack_t *entries = new wt_cell_unpack_t[total_entries];
    file_reader->readEntries(page, entries);
    printPage(page, entries);
  }
}

void WTFile::printPage(wt_page_t *page, wt_cell_unpack_t *entries) {
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

  if (page->page_header->type == WT_PAGE_ROW_LEAF) {
      for (int i=0; i < page->page_header->entries; i++) {
        wt_cell_unpack_t cell = entries[i];
        /*
         * TODO: For the moment format the cell data as [0x ... 0x] showing
         *    only the first and last bytes. Change that to match mdb_wt.py
         *    output. Also need to add utility function to get the string repr
         *    of the cell type (key or value).
         */
        std::cout << "    cell desc: 0x"
                  << std::hex << (unsigned int)(cell.desc)
                  << " data: <sz=" << std::dec << cell.size
                  << " v=[0x" << std::hex << (unsigned int)cell.data[0]
                  << " ... 0x"
                  << std::hex << (unsigned int)cell.data[cell.size -1]
                  << "]>" << std::endl;
      }
  }
}

int main(int argc, char** argv) {
  WTFile *file = new WTFile(argv[1]);
  file->parse();
}
