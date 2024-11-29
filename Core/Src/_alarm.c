#include "_alarm.h"

void AlarmInit() {
  for (int i = 0; i < MAX_ALARM_SAVE; i++) {
    alarm[i].alarm_index = i + 1;
    alarm[i].alarm_time[0] = 0;
    alarm[i].alarm_time[1] = 0;
    alarm[i].alarm_time[2] = 0;
    alarm[i].alarm_on = 0;
    alarm[i].alarm_rpt = 0;
  }
}

void AlarmRunning() {
  AlarmCheck(&alarm[0]);
  AlarmCheck(&alarm[1]);
  AlarmCheck(&alarm[2]);
  AlarmCheck(&alarm[3]);
  AlarmCheck(&alarm[4]);
}

void AlarmCheck(Alarm *alarm) {
  if (alarm->alarm_on) {
    if (clock_time[3] == alarm->alarm_time[0] &&
        clock_time[4] == alarm->alarm_time[1] &&
        clock_time[5] == alarm->alarm_time[2] && alarm_bz == 0) {
      clock_mode = m_Alarm;
      alarm_bz = 1;
      alarm_bz_tic = 1;
      alarm_sw_flag = 1;
      if (!(alarm->alarm_rpt)) {
        alarm->alarm_on = 0;
      }
    }
  }
}

void AlarmTimer() {
  if (sw1.sw_on || sw2.sw_on || sw3.sw_on || sw4.sw_on) {
    alarm_timer = 0;
    if (alarm_bz) {
      alarm_bz_off_flag = 1;
      alarm_bz_count = 0;
      alarm_bz_tic = 0;
      alarm_sw_flag = 0;
    }
  } else if (alarm_timer > 30000) {
    {
      clock_mode = m_Clock;
      alarm_timer = 0;
    }
  }
}

void AlarmBuzzer(TIM_HandleTypeDef *htim, uint8_t onbz) {
  TIM2->PSC = HIGH_BUZZER;
  if (onbz && alarm_bz_tic) {
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
  } else {
    HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
  }
}

void AlarmBuzzerTimer() {
  alarm_bz_count++;
  if (alarm_bz_count > 30) {
    alarm_bz_tic = !alarm_bz_tic;
    alarm_bz_count = 0;
    L_ALL_TOGGLE();
    R_ALL_TOGGLE();
  }
}

void AlarmSwitchSq() {
  if (alarm_sw_flag) {
    sw1.sw_rising = 0;
    sw2.sw_rising = 0;
    sw3.sw_rising = 0;
    sw4.sw_rising = 0;
  } else {
    if (sw1.sw_mid_thres) {
      sw1.sw_rising = 0;
      sw1.sw_mid_thres = 0;
      alarm_set = 4;
      clock_mode = m_Alarm_Set;
      LED_Clear();
      CLCD_Clear();
    } else if (!sw1.sw_on && sw1.sw_rising) {
      sw1.sw_rising = 0;
      alarm_count = 0;
      clock_mode = m_Stopwatch;
      LED_Clear();
      CLCD_Clear();
    }
    if (sw2.sw_rising) {
      sw2.sw_rising = 0;
      alarm_count++;
      if (alarm_count > MAX_ALARM_SAVE - 1) {
        alarm_count = 0;
      }
    }
    if (sw3.sw_rising) {
      sw3.sw_rising = 0;
      alarm[alarm_count].alarm_on = !(alarm[alarm_count].alarm_on);
    }
    if (sw4.sw_rising) {
      sw4.sw_rising = 0;
      alarm[alarm_count].alarm_rpt = !(alarm[alarm_count].alarm_rpt);
    }
  }
}

void AlarmDisplay() {
  ondp = clock_count < 500 ? 1 : 0;
  alarm[alarm_count].alarm_on ? strcpy(stralarm, "ON ")
                              : strcpy(stralarm, "OFF");
  alarm[alarm_count].alarm_rpt ? strcpy(strrpt, "RPT") : strcpy(strrpt, "ONC");

  if (alarm_bz) {
    if (ondp) {
      sprintf(str1, "ALARM TRIGGERED!");
    } else {
      sprintf(str1, "                ");
    }
    sprintf(str2, "                ");
  } else {
    sprintf(str1, "ALARM #%d %s %s", alarm[alarm_count].alarm_index, stralarm,
            strrpt);

    if (clock_form == FORM_12H) {
      hour12H = alarm[alarm_count].alarm_time[0];
      if (alarm[alarm_count].alarm_time[0] > 12) {
        strcpy(str12H, "PM");
        hour12H = alarm[alarm_count].alarm_time[0] - 12;
      } else {
        strcpy(str12H, "AM");
      }
      sprintf(str2, "        %s %02d:%02d", str12H, hour12H,
              alarm[alarm_count].alarm_time[1]);
    } else {
      sprintf(str2, "           %02d:%02d", alarm[alarm_count].alarm_time[0],
              alarm[alarm_count].alarm_time[1]);
    }
  }
  CLCD_Puts(0, 0, str1);
  CLCD_Puts(0, 1, str2);
  _7SEG_SetNumber(DGT1, clock_time[5] / 10, OFF);
  _7SEG_SetNumber(DGT2, clock_time[5] % 10, ondp);
}