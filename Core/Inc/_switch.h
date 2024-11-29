#ifndef _SWITCH_H
#define _SWITCH_H

#include "main.h"

#define S_M_THRESHOLD 700
#define M_L_THRESHOLD 2500
#define MID_TIMER 150
#define LONG_TIMER 20
#define BUZZER_TIMER 30
#define LOW_BUZZER 15000
#define MID_BUZZER 10000
#define HIGH_BUZZER 7500

typedef enum _clock_status {
  m_Clock = 0,
  m_Clock_Set,
  m_Alarm,
  m_Alarm_Set,
  m_Stopwatch
} Clock_status;
typedef enum _sw_press { None = 0, Short, Mid, Long } SW_press;
typedef enum _BZ_mode { bzNone = 0, bzLow, bzMid, bzHigh } BZ_mode;

typedef struct _SW_status {
  SW_press sw_press;
  BZ_mode bz_mode;
  uint8_t sw_on;
  uint8_t sw_rising;
  uint8_t sw_mid_thres;
  uint8_t sw_long_thres;
  uint8_t sw_mid_flag;
  uint8_t sw_long_flag;
  uint8_t bz_on;
  uint16_t sw_count;
  uint16_t sw_mid_count;
  uint16_t sw_long_count;
  uint16_t bz_count;
  uint16_t bz_sound;
  uint16_t temp;
} SW_status;

inline void SwitchRunning(TIM_HandleTypeDef *htim, uint8_t onbz);
void SwitchStatus(TIM_HandleTypeDef *htim, SW_status *sw, uint8_t onbz);
void BuzzerSq(TIM_HandleTypeDef *htim, SW_status *sw, uint8_t onbz);
void SwitchClear(SW_status *sw);
void SwitchCheck(SW_status *sw, GPIO_TypeDef *GPIOx, uint16_t pin_num);
void SwitchPressTimer(SW_status *sw);
void SwitchBuzzerTimer(SW_status *sw);

extern SW_status sw1;
extern SW_status sw2;
extern SW_status sw3;
extern SW_status sw4;
extern uint8_t bz_busy;

#endif