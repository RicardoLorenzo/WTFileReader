#ifndef WT_TYPES_H_
#define WT_TYPES_H_

#include <stdint.h>

#define WT_BLOCK_MAGIC  120897
#define WT_BLOCK_MAJOR_VERSION  1
#define WT_BLOCK_MINOR_VERSION  0
#define WT_BLOCK_DESC_SIZE 16

typedef struct wt_block_desc {
	uint32_t magic;
	uint16_t major_version;
	uint16_t minor_version;
	uint32_t checksum;
	uint32_t unused;
} wt_block_desc_t;

/* WT Page Header */

#define WT_PAGE_INVALID   0 /* Invalid page */
#define WT_PAGE_BLOCK_MANAGER 1 /* Block-manager page */
#define WT_PAGE_COL_FIX   2  /* Col-store fixed-len leaf */
#define WT_PAGE_COL_INT   3  /* Col-store internal page */
#define WT_PAGE_COL_VAR   4  /* Col-store var-length leaf page */
#define WT_PAGE_OVFL      5  /* Overflow page */
#define WT_PAGE_ROW_INT   6  /* Row-store internal page */
#define WT_PAGE_ROW_LEAF  7  /* Row-store leaf page */

#define WT_PAGE_HEADER_SIZE 28
#define	WT_RECNO_OOB	0

typedef struct wt_page_header {
	uint64_t recno;
	uint64_t write_gen;
	uint32_t mem_size;
	uint32_t entries;
	uint8_t type;
	uint8_t flags;
	uint8_t unused[2];
} wt_page_header_t;

/* WT Block Header */

#define WT_BLOCK_HEADER_SIZE 12

typedef struct wt_block_header {
	uint32_t disk_size;
	uint32_t checksum;
	uint8_t flags;
	uint8_t unused[3];
} wt_block_header_t;

/* WT Cell */

#define WT_INTPACK64_MAXSIZE    ((int)sizeof(int64_t) + 1)
#define WT_INTPACK32_MAXSIZE    ((int)sizeof(int32_t) + 1)

#define WT_CELL_SHORT_TYPE_MASK 0x03U /* Short cell type bitmask */
#define WT_CELL_SHORT_SHIFT 2       /* Shift for short key/value */

#define WT_CELL_KEY_SHORT   0x01
#define WT_CELL_KEY_SHORT_PFX   0x02
#define WT_CELL_VALUE_SHORT 0x03

#define WT_CELL_64V  0x04  /* Associated value */

#define WT_CELL_LONG_TYPE_MASK  (0x0FU << 4)  /* Maximum 16 cell types */

#define WT_CELL_ADDR_DEL     (0)  /* Address: deleted */
#define WT_CELL_ADDR_INT     (1 << 4)  /* Address: internal  */
#define WT_CELL_ADDR_LEAF    (2 << 4)  /* Address: leaf */
#define WT_CELL_ADDR_LEAF_NO     (3 << 4)  /* Address: leaf no overflow */
#define WT_CELL_DEL      (4 << 4)  /* Deleted value */
#define WT_CELL_KEY      (5 << 4)  /* Key */
#define WT_CELL_KEY_OVFL     (6 << 4)  /* Overflow key */
#define WT_CELL_KEY_OVFL_RM (12 << 4)  /* Overflow key (removed) */
#define WT_CELL_KEY_PFX      (7 << 4)  /* Key with prefix byte */
#define WT_CELL_VALUE        (8 << 4)  /* Value */
#define WT_CELL_VALUE_COPY   (9 << 4)  /* Value copy */
#define WT_CELL_VALUE_OVFL  (10 << 4)  /* Overflow value */
#define WT_CELL_VALUE_OVFL_RM   (11 << 4) /* Overflow value (removed) */

#define	WT_CELL_SIZE_ADJUST  64
#define WT_CELL_HEADER_SIZE (                                                 \
    1 + 1 + WT_INTPACK64_MAXSIZE + WT_INTPACK32_MAXSIZE)

typedef struct wt_cell_header {
    uint8_t __chunk[WT_CELL_HEADER_SIZE];
} wt_cell_header_t;

typedef struct wt_cell_unpack {
    uint8_t desc;        /* Cell descriptor */
    uint8_t *data;       /* Cell data */
    uint32_t size;       /* Cell data size */
    uint32_t len;        /* Cell + data length */
    uint8_t prefix;      /* Cell prefix length */
} wt_cell_unpack_t;

/* WT Page */

#define WT_PAGE_SIZE    4096

typedef struct wt_page {
	int offset;
	wt_page_header_t *page_header;
	wt_block_header_t *block_header;
} wt_page_t;

#endif // WT_TYPES_H_
