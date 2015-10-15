#ifndef MEMMAN__TIMER_H
#define MEMMAN__TIMER_H

#include "memman/common.h"
#include <time.h>
#include <signal.h>

typedef void (*mm_timer_handler)(siginfo_t* signal, void* data);

void mm_timer_init();
timer_t mm_timer_schedule(int event, unsigned when, void* data);
timer_t mm_timer_schedule_repeating(int event, unsigned interval, void* data);
void mm_timer_wait(unsigned max_sigs, void* data, mm_timer_handler handler);

#endif
