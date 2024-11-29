#ifndef _STOPWATCH_H
#define _STOPWATCH_H

#include "7SEG.h"
#include "CLCD.h"
#include "_led.h"
#include "_switch.h"
#include "main.h"

#define MAX_LAP_SAVE 9

typedef enum _stopw_status { Stopped = 0, Paused, Running } Stopw_status;

typedef struct _lap {
  uint8_t lap_index;
  uint16_t lap_time[4];
} Lap;

void LapInit();
void StopwSwitchSq();
void StopwDisplay();
void StopwCount();
void StopwTimer();

extern Stopw_status stopw_status;
extern Lap lap_save[MAX_LAP_SAVE];
extern uint8_t lap_count;
extern uint8_t lap_display_count;
extern uint16_t stopw_count;
extern uint16_t stopw_time[4];
extern uint16_t stopw_timer;

extern Clock_status clock_mode;
extern uint8_t str1[20];
extern uint8_t str2[20];
extern uint8_t ondp;
extern char strdp;

#endif