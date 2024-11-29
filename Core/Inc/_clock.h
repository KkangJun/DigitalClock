#ifndef _CLOCK_H
#define _CLOCK_H

#include "7SEG.h"
#include "CLCD.h"
#include "_led.h"
#include "_switch.h"
#include "main.h"

#define FORM_12H 0
#define FORM_24H 1

typedef struct _clock_buzzer {
  uint8_t bz_on;
  uint16_t bz_count;
  uint16_t bz_sound;
} Clock_Buzzer;

void ClockInit();
void ClockRunning();
void DateCheck();
void YearCheck();
void LeapCheck();
void ClockBuzzerSq(TIM_HandleTypeDef *htim, uint8_t onbz);
void ClockBuzzerTimer();
void ClockSwitchSq();
void ClockDisplay();

void UpSetCheck();
void DownSetCheck();
void SetDateCheck();
void ClockSetSwitchSq();
void ClockSetDisplay();
void ClockSetTimer();

extern Clock_Buzzer clock_bz;
extern uint8_t isleap;
extern uint8_t clock_form;
extern uint16_t clock_count;
extern char strleap[];
extern char str12H[];
extern char strbz[];
extern int clock_time[6];

extern Clock_status clock_mode;
extern uint8_t str1[20];
extern uint8_t str2[20];
extern uint8_t ondp;
extern uint8_t onbz;
extern char strdp;
extern uint8_t bz_busy;

extern uint16_t clock_set_timer;
extern int clock_set;

#endif