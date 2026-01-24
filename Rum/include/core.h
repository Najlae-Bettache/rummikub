#ifndef CORE_H
#define CORE_H

void core_init(void);
void core_run(void);
void core_dispatch_events(void);
void core_log(const char *msg, int level);

#endif
