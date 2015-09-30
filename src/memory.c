#include "memman/memory.h"

mm_memory_t* mm_memory_create(unsigned size, mm_memory_type_e type)
{
  mm_memory_t* mem = malloc(sizeof(*mem));
  PASSERT(mem, MM_ERR_MALLOC);

  if (type == MM_MEM_PHYSICAL) {
    strncpy(mem->fname, MM_FILE_PHYSICAL, MM_MEM_FNAME_SIZE);
  } else {
    strncpy(mem->fname, MM_FILE_VIRTUAL, MM_MEM_FNAME_SIZE);
  }

  mem->type = type;
  mem->size = size;
  mem->file = NULL;

  return mem;
}

void mm_memory_close(mm_memory_t* mem)
{
  PASSERT(fclose(mem->file) != EOF, "Error while closing file %s", mem->fname);
  mem->file = NULL;
}

void mm_memory_destroy(mm_memory_t* mem)
{
  if (mem->file)
    mm_memory_close(mem);
  FREE(mem);
}

void mm_memory_init_file(mm_memory_t* mem)
{
  int8_t* buf = NULL;

  buf = calloc(mem->size, sizeof(*buf));
  memset(buf, -1, mem->size);

  PASSERT((mem->file = fopen(mem->fname, "wb")),
          "Error while opening(wb) file %s", mem->fname);
  fwrite(buf, sizeof(*buf), mem->size, mem->file);

  free(buf);
}

void mm_memory_assign(mm_memory_t* mem, unsigned base, unsigned length,
                      int value)
{
  // TODO
}
