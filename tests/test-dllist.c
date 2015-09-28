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
  struct data_t data = {.id = 30 };
  mm_dllist_t* listA = mm_dllist_create(NULL);
  mm_dllist_t* listB = mm_dllist_insert_after(listA, &data);

  ASSERT(listA->prev == NULL, "");
  ASSERT(listA->data == NULL, "");
  ASSERT(listA->next == listB, "");
  ASSERT(listB->prev == listA, "");
  ASSERT(listB->next == NULL, "");

  ASSERT(((struct data_t*)listA->next->data)->id == 30, "");
  ASSERT(((struct data_t*)listB->data)->id == 30, "");

  mm_dllist_destroy(listA);
}

void test3()
{
  {
    mm_dllist_t* list = mm_dllist_create(NULL);

    ASSERT(list->next == NULL, "");
    ASSERT(list->prev == NULL, "");
    ASSERT(list->data == NULL, "");

    mm_dllist_remove(list);
    mm_dllist_destroy(list);
  }
  {
    mm_dllist_t* list = mm_dllist_create(NULL);
    mm_dllist_t* elem1 = mm_dllist_insert_after(list, NULL);
    mm_dllist_t* elem2 = mm_dllist_insert_after(elem1, NULL);
    mm_dllist_t* elem3 = mm_dllist_insert_after(elem2, NULL);

    mm_dllist_remove(elem2);
    FREE(elem2);

    ASSERT(list->prev == NULL, "");
    ASSERT(list->next == elem1, "");
    ASSERT(elem1->next == elem3, "");
    ASSERT(elem3->prev == elem1, "");
    ASSERT(elem3->next == NULL, "");

    mm_dllist_destroy(list);
  }
}

int main(int argc, char* argv[])
{
  TEST(test1, "List Creation");
  TEST(test2, "List Insert After");
  TEST(test3, "List Delete");

  return 0;
}
