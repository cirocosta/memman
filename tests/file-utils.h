#ifndef MEMMAN__TEST__FILE_UTILS_H
#define MEMMAN__TEST__FILE_UTILS_H 

#include <stdio.h>
#include "memman/common.h"

inline static void fdelete(const char* fname) { PASSERT(!unlink(fname), "fdelete:"); }
inline static int fexists(const char* fname)
{
  if (!access(fname, F_OK))
    return 1;
  return 0;
}

inline static int fsize(FILE* file)
{
  int size;
  int last_pos;

  PASSERT(~fseek(file, 0, SEEK_SET), "");
  last_pos = ftell(file);
  PASSERT(~fseek(file, 0, SEEK_END), "");
  size = ftell(file);
  PASSERT(~fseek(file, last_pos, SEEK_SET), "");

  return size;
}

inline static int fhas_expected_value(FILE* file, int base, int length, int value)
{
  unsigned char* buf = calloc(length, sizeof(*buf));
  PASSERT(buf, MM_ERR_MALLOC);

  fseek(file, base, SEEK_SET);
  fread(buf, sizeof(*buf), length, file);

  while (length-- > 0)
    if (buf[length] != value)
      return 0;

  free(buf);

  return 1;
}

#endif
