#include "memman/dllist.h"

struct data_t {
  int id;
} data_t;

void test1()
{
  {
    mm_dllist_t* list = mm_dllist_create(NULL);

    ASSERT(list->next == NULL, "");
    ASSERT(list->prev == NULL, "");
    ASSERT(list->data == NULL, "");

    mm_dllist_destroy(list);
  }
  {
    struct data_t data = {.id = 30 };
    mm_dllist_t* list = mm_dllist_create((void*)&data);

    ASSERT(list->next == NULL, "");
    ASSERT(list->prev == NULL, "");
    ASSERT(list->data == &data, "");

    mm_dllist_destroy(list);
  }
}

void test2()
{
  {
    struct data_t data = {.id = 30 };
    mm_dllist_t* listA = mm_dllist_create(NULL);
    mm_dllist_t* listB = mm_dllist_insert_after(listA, &data);

    ASSERT(listA->prev == NULL, "");
    ASSERT(listA->data == NULL, "");
    ASSERT(listA->next->next == NULL, "");

    {
      struct data_t* elem_data = listA->next->data;

      ASSERT(listA->next->data == &data, "");
      ASSERT(elem_data->id == 30, "");
      ASSERT(listB->prev == listA, "");
    }

    mm_dllist_destroy(listA);
  }
}

int main(int argc, char* argv[])
{
  TEST(test1, "List Creation");
  TEST(test2, "List Append");

  return 0;
}
