#include "_clock.h"

void ClockInit() {
  LeapCheck();
  DateCheck();
}

void ClockRunning() {
  // optimization
  if (clock_count > 999) {
    clock_count = 0;
    clock_time[5]++;

    if (clock_time[5] > 59) {
      clock_time[5] = 0;
      clock_time[4]++;

      if (clock_time[4] > 59) {
        clock_time[4] = 0;
        clock_time[3]++;

        if (clock_time[3] > 23) {
          clock_time[3] = 0;
          clock_time[2]++;

          if (clock_mode == m_Clock) {
            clock_bz.bz_sound = HIGH_BUZZER;
            clock_bz.bz_on = 1;
          }

          DateCheck();
        } else {
          if (clock_mode == m_Clock) {
            clock_bz.bz_sound = MID_BUZZER;
            clock_bz.bz_on = 1;
          }
        }
      }
    }
  }
}

void DateCheck() {
  switch (clock_time[1]) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      if (clock_time[2] > 31) {
        clock_time[2] = 1;
        clock_time[1]++;
      }
      YearCheck();
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      if (clock_time[2] > 30) {
        clock_time[2] = 1;
        clock_time[1]++;
      }
      YearCheck();
      break;
    case 2:
      if (clock_time[2] > 28) {
        if (isleap) {
          if (clock_time[2] > 29) {
            clock_time[2] = 1;
            clock_time[1]++;
          }
        } else {
          clock_time[2] = 1;
          clock_time[1]++;
        }
      }
      YearCheck();
      break;
  }
}

void YearCheck() {
  if (clock_time[1] > 12) {
    clock_time[1] = 1;
    clock_time[0]++;

    if ((clock_time[0] % 4) == 0 &&
        ((clock_time[0] % 100) != 0 || (clock_time[0] % 400) == 0)) {
      isleap = 1;
    } else {
      isleap = 0;
    }
  }
}

void LeapCheck() {
  if ((clock_time[0] % 4) == 0 &&
      ((clock_time[0] % 100) != 0 || (clock_time[0] % 400) == 0)) {
    isleap = 1;
  } else {
    isleap = 0;
  }
}

void ClockBuzzerSq(TIM_HandleTypeDef *htim, uint8_t onbz) {
  if (onbz && clock_bz.bz_on) {
    TIM2->PSC = clock_bz.bz_sound;
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
  } else {
    HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
  }
}

void ClockBuzzerTimer() {
  if (clock_bz.bz_count > BUZZER_TIMER) {
    clock_bz.bz_on = 0;
    clock_bz.bz_count = 0;
  }
}

void ClockSwitchSq() {
  if (sw1.sw_mid_thres) {
    sw1.sw_rising = 0;
    sw1.sw_mid_thres = 0;
    clock_set = 5;
    clock_mode = m_Clock_Set;
    LED_Clear();
    CLCD_Clear();
  } else if (!sw1.sw_on && sw1.sw_rising) {
    sw1.sw_rising = 0;
    clock_mode = m_Alarm;
    LED_Clear();
    CLCD_Clear();
  }
  if (sw2.sw_rising) {
    sw2.sw_rising = 0;
    onbz = !onbz;
  }
  if (sw3.sw_rising) {
    sw3.sw_rising = 0;
    clock_form = !clock_form;
  }
}

void ClockDisplay() {
  ondp = clock_count < 500 ? 1 : 0;
  isleap ? strcpy(strleap, "LEAP") : strcpy(strleap, "    ");
  onbz ? strcpy(strbz, "ON ") : strcpy(strbz, "OFF");
  strdp = ondp ? ':' : ' ';

  sprintf(str1, "%s  %4d.%2d.%2d", strleap, clock_time[0], clock_time[1],
          clock_time[2]);

  if (clock_form == FORM_12H) {
    uint8_t hour12H = clock_time[3];
    if (clock_time[3] > 12) {
      strcpy(str12H, "PM");
      hour12H = clock_time[3] - 12;
    } else {
      strcpy(str12H, "AM");
    }

    sprintf(str2, "BZ %s  %s %02d%c%02d", strbz, str12H, hour12H, strdp,
            clock_time[4]);
  } else {
    sprintf(str2, "BZ %s     %02d%c%02d", strbz, clock_time[3], strdp,
            clock_time[4]);
  }
  CLCD_Puts(0, 0, str1);
  CLCD_Puts(0, 1, str2);
  _7SEG_SetNumber(DGT1, clock_time[5] / 10, OFF);
  _7SEG_SetNumber(DGT2, clock_time[5] % 10, ondp);
}