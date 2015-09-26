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

int fsize(char* file)
{
  int size;
  FILE* fh;

  PASSERT((fh = fopen(file, "rb")), "fsize");

  if (fseek(fh, 0, SEEK_END)) {
    fclose(fh);
    return -1;
  }

  size = ftell(fh);
  fclose(fh);
  return size;
}

void SET_UP(mm_memory_t* mem)
{
  if (fexists(mem->fname))
    fdelete(mem->fname);
}

void TEAR_DOWN(mm_memory_t* mem) { fdelete(mem->fname); }

void test1()
{
  unsigned bytes = 400;
  unsigned actual_bytes = 0;
  mm_memory_t* mem = mm_memory_create(bytes, MM_MEM_PHYSICAL);

  SET_UP(mem);

  mm_memory_init_file(mem);
  mm_memory_destroy(mem);

  actual_bytes = fsize(mem->fname);

  ASSERT(actual_bytes == bytes, "%d != %u", actual_bytes, bytes);

  /* TEAR_DOWN(mem); */
}

int main(int argc, char* argv[])
{
  TEST(test1, "File initialization");

  return 0;
}
