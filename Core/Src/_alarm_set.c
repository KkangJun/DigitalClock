#include "_alarm.h"

void AlarmUpSet() {
  switch (alarm_set) {
    case 0:
      alarm[alarm_count].alarm_on = !(alarm[alarm_count].alarm_on);
      break;
    case 1:
      alarm[alarm_count].alarm_rpt = !(alarm[alarm_count].alarm_rpt);
      break;
    case 2:
      alarm[alarm_count].alarm_time[0]++;
      if (alarm[alarm_count].alarm_time[0] > 23) {
        alarm[alarm_count].alarm_time[0] = 0;
      }
      break;
    case 3:
      alarm[alarm_count].alarm_time[1]++;
      if (alarm[alarm_count].alarm_time[1] > 59) {
        alarm[alarm_count].alarm_time[1] = 0;
      }
      break;
    case 4:
      alarm_count++;
      if (alarm_count > 4) {
        alarm_count = 0;
      }
      break;
  }
}

void AlarmDownSet() {
  switch (alarm_set) {
    case 0:
      alarm[alarm_count].alarm_on = !(alarm[alarm_count].alarm_on);
      break;
    case 1:
      alarm[alarm_count].alarm_rpt = !(alarm[alarm_count].alarm_rpt);
      break;
    case 2:
      alarm[alarm_count].alarm_time[0]--;
      if (alarm[alarm_count].alarm_time[0] < 0) {
        alarm[alarm_count].alarm_time[0] = 23;
      }
      break;
    case 3:
      alarm[alarm_count].alarm_time[1]--;
      if (alarm[alarm_count].alarm_time[1] < 0) {
        alarm[alarm_count].alarm_time[1] = 59;
      }
      break;
    case 4:
      alarm_count--;
      if (alarm_count < 0) {
        alarm_count = 4;
      }
      break;
  }
}

void AlarmSetSwitchSq() {
  if (sw1.sw_mid_thres) {
    sw1.sw_rising = 0;
    sw1.sw_mid_thres = 0;
    clock_mode = m_Alarm;
    LED_Clear();
    CLCD_Clear();
  }
  if (sw2.sw_rising) {
    sw2.sw_rising = 0;
    alarm_set--;
    if (alarm_set < 0) {
      alarm_set = 4;
    }
  }
  if (sw3.sw_on) {
    if (sw3.sw_rising) {
      sw3.sw_rising = 0;
      AlarmUpSet();
    }
    switch (sw3.sw_press) {
      case Short:
        break;
      case Mid:
        if (sw3.sw_mid_flag) {
          sw3.sw_mid_flag = 0;
          AlarmUpSet();
        }
        break;
      case Long:
        if (sw3.sw_long_flag) {
          sw3.sw_long_flag = 0;
          AlarmUpSet();
        }
        break;
    }
  }
  if (sw4.sw_on) {
    if (sw4.sw_rising) {
      sw4.sw_rising = 0;
      AlarmDownSet();
    }
    switch (sw4.sw_press) {
      case Short:
        break;
      case Mid:
        if (sw4.sw_mid_flag) {
          sw4.sw_mid_flag = 0;
          AlarmDownSet();
        }
        break;
      case Long:
        if (sw4.sw_long_flag) {
          sw4.sw_long_flag = 0;
          AlarmDownSet();
        }
        break;
    }
  }
}

void AlarmSetDisplay() {
  if (sw3.sw_on || sw4.sw_on) {
    ondp = 1;
  } else {
    ondp = clock_count < 500 ? 1 : 0;
  }

  alarm[alarm_count].alarm_on ? strcpy(stralarm, "ON ")
                              : strcpy(stralarm, "OFF");
  alarm[alarm_count].alarm_rpt ? strcpy(strrpt, "RPT") : strcpy(strrpt, "ONC");

  if (clock_form == FORM_12H) {
    hour12H = alarm[alarm_count].alarm_time[0];
    if (alarm[alarm_count].alarm_time[0] > 12) {
      strcpy(str12H, "PM");
      hour12H = alarm[alarm_count].alarm_time[0] - 12;
    } else {
      strcpy(str12H, "AM");
    }
  }

  switch (alarm_set) {
    case 0:
      ondp ? sprintf(str1, "ALARM #%d %s %s", alarm[alarm_count].alarm_index,
                     stralarm, strrpt)
           : sprintf(str1, "ALARM #%d     %s", alarm[alarm_count].alarm_index,
                     strrpt);
      clock_form ? sprintf(str2, "SET        %02d:%02d",
                           alarm[alarm_count].alarm_time[0],
                           alarm[alarm_count].alarm_time[1])
                 : sprintf(str2, "SET     %s %02d:%02d", str12H, hour12H,
                           alarm[alarm_count].alarm_time[1]);
      break;
    case 1:
      ondp ? sprintf(str1, "ALARM #%d %s %s", alarm[alarm_count].alarm_index,
                     stralarm, strrpt)
           : sprintf(str1, "ALARM #%d %s    ", alarm[alarm_count].alarm_index,
                     stralarm);

      clock_form ? sprintf(str2, "SET        %02d:%02d",
                           alarm[alarm_count].alarm_time[0],
                           alarm[alarm_count].alarm_time[1])
                 : sprintf(str2, "SET     %s %02d:%02d", str12H, hour12H,
                           alarm[alarm_count].alarm_time[1]);
      break;
    case 2:
      sprintf(str1, "ALARM #%d %s %s", alarm[alarm_count].alarm_index, stralarm,
              strrpt);
      if (ondp) {
        clock_form ? sprintf(str2, "SET        %02d:%02d",
                             alarm[alarm_count].alarm_time[0],
                             alarm[alarm_count].alarm_time[1])
                   : sprintf(str2, "SET     %s %02d:%02d", str12H, hour12H,
                             alarm[alarm_count].alarm_time[1]);
      } else {
        clock_form ? sprintf(str2, "SET          :%02d",
                             alarm[alarm_count].alarm_time[1])
                   : sprintf(str2, "SET     %s   :%02d", str12H,
                             alarm[alarm_count].alarm_time[1]);
      }
      break;
    case 3:
      sprintf(str1, "ALARM #%d %s %s", alarm[alarm_count].alarm_index, stralarm,
              strrpt);
      if (ondp) {
        clock_form ? sprintf(str2, "SET        %02d:%02d",
                             alarm[alarm_count].alarm_time[0],
                             alarm[alarm_count].alarm_time[1])
                   : sprintf(str2, "SET     %s %02d:%02d", str12H, hour12H,
                             alarm[alarm_count].alarm_time[1]);
      } else {
        clock_form
            ? sprintf(str2,
                      "SET        %02d:  ", alarm[alarm_count].alarm_time[0])
            : sprintf(str2, "SET     %s %02d:  ", str12H, hour12H);
      }
      break;
    case 4:
      ondp ? sprintf(str1, "ALARM #%d %s %s", alarm[alarm_count].alarm_index,
                     stralarm, strrpt)
           : sprintf(str1, "ALARM #  %s %s", stralarm, strrpt);
      clock_form ? sprintf(str2, "SET        %02d:%02d",
                           alarm[alarm_count].alarm_time[0],
                           alarm[alarm_count].alarm_time[1])
                 : sprintf(str2, "SET     %s %02d:%02d", str12H, hour12H,
                           alarm[alarm_count].alarm_time[1]);
      break;
  }

  CLCD_Puts(0, 0, str1);
  CLCD_Puts(0, 1, str2);
  _7SEG_SetNumber(DGT1, clock_time[5] / 10, OFF);
  _7SEG_SetNumber(DGT2, clock_time[5] % 10, ondp);
}

void AlarmSetTimer() {
  if (sw1.sw_on || sw2.sw_on || sw3.sw_on || sw4.sw_on) {
    alarm_set_timer = 0;
  } else if (alarm_set_timer > 30000) {
    {
      clock_mode = m_Alarm;
      alarm_set_timer = 0;
    }
  }
}