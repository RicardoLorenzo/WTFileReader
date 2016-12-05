#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

#include "include/wt_file.h"

WTFileReader::WTFileReader(const char* filename) {
  wt_r = (struct wt_file_reader *)std::malloc(sizeof(struct wt_file_reader));
  assert(wt_r != NULL);

  wt_r->f_name = const_cast<char *>(filename);
  wt_r->f_desc = -1;
  wt_r->c_offset = 0;
  wt_r->f_block_desc = (wt_block_desc_t *) malloc(sizeof(wt_r->f_block_desc));
  assert(wt_r->f_block_desc != NULL);

  mapFile();
  wt_b = new WTBlockReader(wt_r);
};

WTFileReader::~WTFileReader() {
  free(wt_r->f_block_desc);
  free(wt_r);
};

size_t WTFileReader::getFileSize(const char* filename) {
    struct stat st;
    assert(stat(filename, &st) != -1);
    return st.st_size;
}

void WTFileReader::mapFile() {
  assert(wt_r->f_name != NULL);
  wt_r->f_size = getFileSize(wt_r->f_name);
  wt_r->f_desc = open(wt_r->f_name, O_RDONLY, 0);
  assert(wt_r->f_desc != -1);

  wt_r->map = mmap((caddr_t)0, wt_r->f_size, PROT_READ,
    MAP_PRIVATE | MAP_NORESERVE, wt_r->f_desc, 0);
  assert(wt_r->map != MAP_FAILED);
}

int WTFileReader::seek(int offset, wt_reader_seek_from_t from_pos) {
	int n_offset, s_pos;

	switch (from_pos) {
	case FROM_START:
		s_pos = 0;
		break;
	case FROM_CURRENT:
		s_pos = wt_r->c_offset;
	}

	n_offset = s_pos + offset;
	/* WT pages are 4KB aligned */
	if (n_offset % WT_PAGE_SIZE)
		return -1;
	/* First 4KB are reserved for WT file block description */
	if (!(n_offset >= WT_PAGE_SIZE && n_offset <= wt_r->f_size))
		return -1;

	wt_r->c_offset = n_offset;
	return n_offset;
}

int WTFileReader::readBlockDesc() {
	/*
	 * Use fixed WT_BLOCK_DESC_SIZE instead of computing the size from structs
	 * in case compiler inserts padding.
	 */
  wt_r->c_offset = 0;
	memcpy(wt_r->f_block_desc, (static_cast<unsigned char*>(wt_r->map) + wt_r->c_offset), WT_BLOCK_DESC_SIZE);

  assert(wt_r->f_block_desc->magic == WT_BLOCK_MAGIC);

  /* First WT page located at WT_PAGE_SIZE offset. */
	wt_r->c_offset += WT_PAGE_SIZE;
  return WT_PAGE_SIZE;
}

int WTFileReader::readHeaders(wt_page_t *page) {
  int p_offset;

	if (wt_r->c_offset >= wt_r->f_size)
		return WT_EOF;

  page->offset = wt_r->c_offset;
	/*
	 * Use fixed WT_PAGE_HEADER_SIZE & WT_BLOCK_HEADER_SIZE instead of computing
	 * the size from structs in case compiler inserts padding.
	 */
  memcpy(page->page_header, (static_cast<uint8_t*>(wt_r->map) + wt_r->c_offset),
	       WT_PAGE_HEADER_SIZE);
  memcpy(page->block_header, (static_cast<uint8_t*>(wt_r->map) +
    (wt_r->c_offset + WT_PAGE_HEADER_SIZE)), WT_BLOCK_HEADER_SIZE);

  if (page->page_header->recno == WT_RECNO_OOB)
		p_offset = page->block_header->disk_size;
	else
		p_offset = WT_PAGE_SIZE;

  wt_r->c_offset += p_offset;
  return p_offset;
}

void WTFileReader::readEntries(wt_page_t *page, wt_cell_unpack_t *entries) {
  int offset = page->offset + WT_PAGE_HEADER_SIZE + WT_BLOCK_HEADER_SIZE;

  if (page->page_header->type == WT_PAGE_ROW_LEAF) {
      for (int i=0; i < page->page_header->entries; i++) {
        wt_cell_unpack_t cell;
        offset += wt_b->readCell(&cell, offset);
        entries[i] = cell;
      }
  }
}
