#include "memman/timer.h"

void mm_timer_init()
{
  sigset_t block_set;

  // FIXME WE'RE LIMITED TO SIGNALS THAT DO NOT START AT 0
  //       maybe use sigaction
  if (!~sigemptyset(&block_set) || !~sigaddset(&block_set, SIG_PROCESS_NEW) ||
      !~sigaddset(&block_set, SIG_PROCESS_ACCESS) ||
      !~sigaddset(&block_set, SIG_PROCESS_END) ||
      !~sigaddset(&block_set, SIG_PROCESS_QUANTUM) ||
      !~sigprocmask(SIG_BLOCK, &block_set, NULL)) {
    perror("error setting singnal mask:");
    exit(EXIT_FAILURE);
  }
}

timer_t mm_timer_schedule(int event, unsigned when, void* data)
{
  long long nanosecs = BILLION * when;
  struct itimerspec ts;
  struct sigevent se;
  timer_t tid;

  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = event;
  se.sigev_value.sival_ptr = data;

  ts.it_value.tv_sec = nanosecs / BILLION;
  ts.it_value.tv_nsec = nanosecs % BILLION;
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;

  PASSERT(!timer_create(CLOCK_MONOTONIC, &se, &tid),
          "timer_create: Couldn't create timer\n");
  PASSERT(!timer_settime(tid, 0, &ts, 0),
          "timer_settime: Couldn't activate timer\n");

  return tid;
}

timer_t mm_timer_schedule_repeating(int event, unsigned interval, void* data)
{
  long long nanosecs = interval * MILLION;
  struct itimerspec ts;
  struct sigevent se;
  timer_t tid;

  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = event;
  se.sigev_value.sival_ptr = data;

  ts.it_value.tv_sec = nanosecs / BILLION;
  ts.it_value.tv_nsec = nanosecs % BILLION;
  ts.it_interval.tv_sec = ts.it_value.tv_sec;
  ts.it_interval.tv_nsec = ts.it_value.tv_nsec;

  PASSERT(!timer_create(CLOCK_MONOTONIC, &se, &tid),
          "timer_create: Couldn't create timer\n");
  PASSERT(!timer_settime(tid, 0, &ts, 0),
          "timer_settime: Couldn't activate timer\n");

  return tid;
}

void mm_timer_wait(unsigned max_sigs, void* data, mm_timer_handler handler)
{
  siginfo_t sig;
  sigset_t mask;

  if (!~sigemptyset(&mask) || !~sigaddset(&mask, SIG_PROCESS_NEW) ||
      !~sigaddset(&mask, SIG_PROCESS_ACCESS) ||
      !~sigaddset(&mask, SIG_PROCESS_END) ||
      !~sigaddset(&mask, SIG_PROCESS_QUANTUM)) {
    perror("error setting singnal &mask:");
    exit(EXIT_FAILURE);
  }

  // TODO invatigate why SIG_PROCESS_NEW is not develired when
  //      t0 = 0. It should as sigwaitinfo removes enqueued stuff
  //      from the pending signals queue.

  while (max_sigs) {
    sigwaitinfo(&mask, &sig);
    handler(&sig, data);

    if (sig.si_signo != SIG_PROCESS_QUANTUM)
      max_sigs--;
    LOGERR("max_sigs = %u", max_sigs);
  }
}
