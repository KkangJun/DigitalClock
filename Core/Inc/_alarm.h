#ifndef _ALARM_H
#define _ALARM_H

#include "7SEG.h"
#include "CLCD.h"
#include "_clock.h"
#include "_led.h"
#include "_switch.h"
#include "main.h"

#define MAX_ALARM_SAVE 5

typedef struct _alarm {
  uint8_t alarm_on;
  uint8_t alarm_rpt;
  uint8_t alarm_index;
  int8_t alarm_time[3];
} Alarm;

void AlarmInit();
void AlarmRunning();
void AlarmCheck(Alarm *alarm);
void AlarmTimer();
void AlarmBuzzer(TIM_HandleTypeDef *htim, uint8_t onbz);
void AlarmBuzzerTimer();
void AlarmSwitchSq();
void AlarmDisplay();

void AlarmUpSet();
void AlarmDownSet();
void AlarmSetSwitchSq();
void AlarmSetDisplay();
void AlarmSetTimer();

extern Alarm alarm[MAX_ALARM_SAVE];
extern uint8_t alarm_bz;
extern uint8_t alarm_bz_off_flag;
extern uint8_t alarm_bz_tic;
extern uint8_t alarm_sw_flag;
extern uint8_t hour12H;
extern uint16_t alarm_timer;
extern uint16_t alarm_bz_count;
extern int8_t alarm_count;
extern char stralarm[];
extern char strrpt[];

extern uint8_t bz_busy;

extern uint16_t alarm_set_timer;
extern int alarm_set;

#endif