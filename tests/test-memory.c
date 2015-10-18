#include "memman/memory.h"
#include <unistd.h>
#include <linux/stat.h>
#include "./file-utils.h"


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
  ASSERT(fhas_expected_value(mem->file, 16, 32, 5),
         "Must have put the right things at the right place");

  mm_memory_destroy(mem);
}

int main(int argc, char* argv[])
{
  TEST(test1, "File initialization");
  TEST(test2, "Memory Assign");

  return 0;
}
