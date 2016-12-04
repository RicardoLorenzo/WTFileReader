#include <stdio.h>
#include <string.h>
#include <iostream>

#include "include/wt_file.h"
#include "include/wt_utils.h"

WTBlockReader::WTBlockReader(struct wt_file_reader *reader) {
  wt_r = reader;
  assert(wt_r != NULL);
};

int WTBlockReader::readCell(wt_cell_unpack_t *unpack_c, int offset) {
  /* Get start of the cell from the mapped memory region */
  uint8_t* cell = static_cast<uint8_t *>(wt_r->map) + offset;

  unpack_c->prefix = 0;
  unpack_c->data = NULL;
  unpack_c->size = 0;
  unpack_c->len = 0;

  wt_cell_header_t c_head;
  std::memcpy(&c_head, cell, sizeof(wt_cell_header_t));
  unpack_c->desc = c_head.__chunk[0];

  /* Short cell type */
  switch (unpack_c->desc & WT_CELL_SHORT_TYPE_MASK) {
    case WT_CELL_KEY_SHORT:
    case WT_CELL_VALUE_SHORT:
      unpack_c->size = (unpack_c->desc >> WT_CELL_SHORT_SHIFT);
      unpack_c->data = new uint8_t[unpack_c->size];
      std::memcpy(unpack_c->data, (c_head.__chunk + 1), unpack_c->size);
      unpack_c->len = 1 + unpack_c->size; /* desc + data */
      return (unpack_c->len);
    case WT_CELL_KEY_SHORT_PFX:
      unpack_c->prefix = c_head.__chunk[1];
      unpack_c->size = (unpack_c->desc >> WT_CELL_SHORT_SHIFT);
      unpack_c->data = new uint8_t[unpack_c->size];
      std::memcpy(unpack_c->data, (c_head.__chunk + 2), unpack_c->size);
      unpack_c->len = 2 + unpack_c->size; /* desc + prefix + data */
      return (unpack_c->len);
  }

  /* Long cell type */

  /* Advance to the cell data */
  const uint8_t* cell_data = cell + 1;
  uint64_t sz;

  switch (unpack_c->desc & WT_CELL_LONG_TYPE_MASK) {
    /* TODO: Handle other cell types */
    case WT_CELL_ADDR_DEL:
    case WT_CELL_ADDR_INT:
    case WT_CELL_ADDR_LEAF:
    case WT_CELL_ADDR_LEAF_NO:
    case WT_CELL_KEY:
    case WT_CELL_KEY_PFX:
    case WT_CELL_VALUE:
      unpackUInt(&cell_data, &sz);
      if (unpack_c->desc == WT_CELL_KEY ||
          unpack_c->desc == WT_CELL_KEY_PFX ||
          (unpack_c->desc == WT_CELL_VALUE && !(unpack_c->desc & WT_CELL_64V)))
          sz += WT_CELL_SIZE_ADJUST;

      unpack_c->size = (uint32_t)sz;
      unpack_c->data = new uint8_t[unpack_c->size];
      std::memcpy(unpack_c->data, cell_data, unpack_c->size);
      unpack_c->len = (cell_data + unpack_c->size) - cell;
      return (unpack_c->len);
  }
  /* TODO: Handle unrecognized used case better */
  return -1;
}
