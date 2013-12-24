/* sleep.c */
 
#define INCL_DOS
#include <os2.h>

void msleep(long ms)
{
  DosSleep(ms);
}

/* end of sleep.c */
