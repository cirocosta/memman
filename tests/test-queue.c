#include "memman/queue.h"
#include "memman/common.h"

void test1()
{
  mm_queue_t* q = mm_queue_create(3);
  char* hue = "hue";
  char* br = "br";
  char* front = NULL;

  ASSERT(mm_queue_empty(q) == 1, "Must start empty");

  ASSERT(mm_queue_insert(q, hue) == 0, "Must receive it's 'ticket' correctly");
  front = mm_queue_front(q);
  ASSERT(front == hue, "");

  ASSERT(mm_queue_insert(q, br) == 1, "Must receive it's 'ticket' correctly");
  ASSERT(mm_queue_empty(q) == 0, "Not empty after inserts");

  front = mm_queue_front(q);
  ASSERT(front == hue, "Didn't removed, must remain in front");

  mm_queue_remove(q);
  ASSERT(mm_queue_empty(q) == 0, "");

  front = mm_queue_front(q);
  ASSERT(front == br, "");

  mm_queue_remove(q);
  ASSERT(mm_queue_empty(q) == 1, "Ends empty after removing");

  mm_queue_destroy(q);
}

int main(int argc, char* argv[])
{
  TEST(test1, "general testing");

  return 0;
}
