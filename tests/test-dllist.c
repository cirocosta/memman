#include "memman/dllist.h"

void test1()
{
  {
    mm_dllist_t* list = mm_dllist_create(NULL);

    ASSERT(list->next == NULL, "");
    ASSERT(list->prev == NULL, "");
    ASSERT(list->segment == NULL, "");

    mm_dllist_destroy(list);
  }
  {
    mm_segment_t* data = mm_segment_create(0, 0);
    mm_dllist_t* list = mm_dllist_create(data);

    ASSERT(list->next == NULL, "");
    ASSERT(list->prev == NULL, "");
    ASSERT(list->segment == data, "");

    mm_dllist_destroy(list);
  }
}

void test2()
{
  mm_segment_t* data = mm_segment_create(0, 0);
  mm_dllist_t* listA = mm_dllist_create(NULL);
  mm_dllist_t* listB = mm_dllist_insert_after(listA, data);

  ASSERT(listA->prev == NULL, "");
  ASSERT(listA->segment == NULL, "");
  ASSERT(listA->next == listB, "");
  ASSERT(listB->prev == listA, "");
  ASSERT(listB->next == NULL, "");

  mm_dllist_destroy(listA);
}

void test3()
{
  {
    mm_dllist_t* list = mm_dllist_create(NULL);

    ASSERT(list->next == NULL, "");
    ASSERT(list->prev == NULL, "");
    ASSERT(list->segment == NULL, "");

    mm_dllist_remove(list);
    mm_dllist_destroy(list);
  }
  {
    mm_dllist_t* list = mm_dllist_create(NULL);
    mm_dllist_t* elem1 = mm_dllist_insert_after(list, NULL);
    mm_dllist_t* elem2 = mm_dllist_insert_after(elem1, NULL);
    mm_dllist_t* elem3 = mm_dllist_insert_after(elem2, NULL);

    mm_dllist_remove(elem2);
    mm_dllist_destroy(elem2);

    ASSERT(list->prev == NULL, "");
    ASSERT(list->next == elem1, "");
    ASSERT(elem1->next == elem3, "");
    ASSERT(elem3->prev == elem1, "");
    ASSERT(elem3->next == NULL, "");

    mm_dllist_destroy(list);
  }
}

void test4()
{
  mm_dllist_t* list = mm_dllist_create(NULL);
  mm_dllist_t* elem1 = mm_dllist_insert_after(list, NULL);
  mm_dllist_t* elem2 = mm_dllist_insert_after(elem1, NULL);
  mm_dllist_t* elem3 = mm_dllist_insert_after(elem2, NULL);

  mm_dllist_remove(elem1);
  mm_dllist_destroy(elem1);

  ASSERT(list->prev == NULL, "");
  ASSERT(list->next == elem2, "");
  ASSERT(elem2->next == elem3, "");
  ASSERT(elem3->prev == elem2, "");
  ASSERT(elem3->next == NULL, "");

  mm_dllist_destroy(list);
}

void test5()
{
  mm_dllist_t* list = mm_dllist_create(NULL);
  mm_dllist_t* elem1 = mm_dllist_insert_after(list, NULL);
  mm_dllist_t* elem2 = mm_dllist_insert_after(elem1, NULL);

  mm_dllist_remove(elem2);
  mm_dllist_destroy(elem2);
  

  ASSERT(list->prev == NULL, "");
  ASSERT(list->next == elem1, "");
  ASSERT(elem1->next == NULL, "");
  ASSERT(elem1->prev == list, "");

  mm_dllist_destroy(list);
}

void test6()
{
  mm_dllist_t* list = mm_dllist_create(NULL);
  mm_dllist_t* elem1 = mm_dllist_insert_after(list, NULL);
  mm_dllist_t* list2 = mm_dllist_create(NULL);

  mm_dllist_append(list, list2);

  ASSERT(list->prev == NULL, "");
  ASSERT(list->next == list2, "");
  ASSERT(list2->next == elem1, "");
  ASSERT(elem1->prev == list2, "");
  ASSERT(list2->prev == list, "");
  ASSERT(elem1->next == NULL, "");

  mm_dllist_destroy(list);
}

int main(int argc, char* argv[])
{
  TEST(test1, "List Creation");
  TEST(test2, "List Insert After");
  TEST(test3, "List Delete between two");
  TEST(test4, "List Delete first after head");
  TEST(test5, "List Delete last element");
  TEST(test6, "List append");

  return 0;
}
