#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <vector>

#include "wt_types.h"

/* WiredTiger File Reader */
#define WT_EOF -1

typedef enum {
	FROM_START = 0,
	FROM_CURRENT = 1
} wt_reader_seek_from_t;

struct wt_file_reader {
	/* WT file information */
	char *f_name;		/* WT file name */
	int f_desc;		/* WT file descriptor */
	off_t f_size;		/* WT file size */
	wt_block_desc_t *f_block_desc;	/* WT file block description */

	void *map;		/* WT file Mapped data */
	int c_offset;		/* WT reader current offset */
};

class WTBlockReader {
private:
	struct wt_file_reader *wt_r;

public:
	WTBlockReader(struct wt_file_reader *reader);
	~WTBlockReader();
  int readCell(wt_cell_unpack_t *unpack_c, int offset);
};

class WTFileReader {
private:
	struct wt_file_reader *wt_r;
	WTBlockReader *wt_b;
  int n_read, s_offset, bh_offset;
  wt_page_t page;
  wt_page_header_t page_header;
  wt_block_header_t block_header;

  size_t getFileSize(const char* filename);
  void mapFile();

public:
	WTFileReader(const char* filename);
	~WTFileReader();
  int seek(int offset, wt_reader_seek_from_t from_pos);
  int readBlockDesc();
  int readHeaders(wt_page_t *page);
	void readEntries(wt_page_t *page, wt_cell_unpack_t *entries);
};

class WTFile {
private:
  WTFileReader *file_reader;
	wt_page_t *page;
  wt_page_header_t *page_header;
  wt_block_header_t *block_header;

public:
	WTFile(const char* filename);
  void parse();
  void printPage(wt_page_t *page, wt_cell_unpack_t *entries);
};
