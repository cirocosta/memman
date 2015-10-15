#include "memman/memory.h"
#include <unistd.h>
#include <linux/stat.h>

void fdelete(const char* fname) { PASSERT(!unlink(fname), "fdelete:"); }
int fexists(const char* fname)
{
  if (!access(fname, F_OK))
    return 1;
  return 0;
}

int fsize(FILE* file)
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

int has_expected_value(FILE* file, int base, int length, int value)
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

void test1()
{
  unsigned bytes = 400;
  unsigned actual_bytes = 0;
  mm_memory_t* mem = mm_memory_create(bytes, MM_MEM_PHYSICAL);

  if (fexists(mem->fname))
    fdelete(mem->fname);

  mm_memory_init_file(mem);
  actual_bytes = fsize(mem->file);
  ASSERT(actual_bytes == bytes, "%d != %u", actual_bytes, bytes);

  mm_memory_destroy(mem);
}

void test2()
{
  unsigned bytes = 128;
  mm_memory_t* mem = mm_memory_create(bytes, MM_MEM_PHYSICAL);

  if (fexists(mem->fname))
    fdelete(mem->fname);

  mm_memory_init_file(mem);
  mm_memory_assign(mem, 16, 32, 5);
  ASSERT(has_expected_value(mem->file, 16, 32, 5),
         "Must have put the right things at the right place");

  mm_memory_destroy(mem);
}

int main(int argc, char* argv[])
{
  /* TEST(test1, "File initialization"); */
  TEST(test2, "Memory Assign");

  return 0;
}
