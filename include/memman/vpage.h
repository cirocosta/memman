#ifndef MEMMAN__VPAGE_H
#define MEMMAN__VPAGE_H 

typedef struct mm_vpage_t {
  unsigned char r : 1; // read
  unsigned char m : 1; // modified
  unsigned char p : 1; // present
  unsigned phys_page;  // physical page (mapping)
} mm_vpage_t;

static const mm_vpage_t mm_vpage_zeroed = { 0 };

#endif
