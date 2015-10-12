#ifndef MEMMAN__CONSTANTS_H
#define MEMMAN__CONSTANTS_H

#define MM_MAX_NAME 64
#define MM_MAX_ACCESS 64
#define MM_MAX_INPUT 512

// in bytes
#define MM_PAGE_SIZE 16 

typedef enum mm_segment_type_e {
  MM_SEG_FREE = 1,
  MM_SEG_PROCESS,
} mm_segment_type_e;

typedef enum mm_memory_type_e {
  MM_MEM_PHYSICAL = 1,
  MM_MEM_VIRTUAL,
} mm_memory_type_e;

typedef enum mm_algorithms_e {
  MM_ALG_FREE_FF = 1,
  MM_ALG_FREE_NF,
  MM_ALG_FREE_QF,
} mm_algorithms_e;

  /* MM_ALG_PSUBST_FIFO, */
  /* MM_ALG_PSUBST_LRUP, */
  /* MM_ALG_PSUBST_NRUP, */
  /* MM_ALG_PSUBST_SCP */
/* } mm_algorithms_e; */

#define MM_ERR_MALLOC "Couldn't allocate memory"
#define MM_ERR_MALFORMED_TRACE                                                 \
  "Malformed trace.\n"                                                         \
  "Should be: t0 name tf b p1 t1 [pn tn]"

#define MM_FILE_VIRTUAL "/tmp/mm.vir"
#define MM_FILE_PHYSICAL "/tmp/mm.mem"
#define MM_MEM_FNAME_SIZE sizeof(MM_FILE_VIRTUAL)

#endif
