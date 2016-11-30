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

/* WT Page */

#define WT_PAGE_SIZE    4096

typedef struct wt_page {
	int offset;
	wt_page_header_t *page_header;
	wt_block_header_t *block_header;
	uint8_t *entries;
} wt_page_t;

#endif // WT_TYPES_H_
