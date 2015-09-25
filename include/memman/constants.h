#ifndef MEMMAN__CONSTANTS_H
#define MEMMAN__CONSTANTS_H

#define MM_MAX_NAME 64
#define MM_MAX_ACCESS 64
#define MM_MAX_INPUT 512

typedef enum mm_seg_type {
  MM_SEG_FREE = 1,
  MM_SEG_PROCESS,
} mm_seg_type;

#define MM_ERR_MALLOC "Couldn't allocate memory"
#define MM_ERR_MALFORMED_TRACE                                                 \
  "Malformed trace.\n"                                                         \
  "Should be: t0 name tf b p1 t1 [pn tn]"

#endif
