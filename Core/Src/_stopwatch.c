#include "_stopwatch.h"

void LapInit() {
  for (int i = 0; i < MAX_LAP_SAVE; i++) {
    lap_save[i].lap_index = i + 1;
    lap_save[i].lap_time[0] = 0;
    lap_save[i].lap_time[1] = 0;
    lap_save[i].lap_time[2] = 0;
    lap_save[i].lap_time[3] = 0;
  }
}

void StopwSwitchSq() {
  if (sw1.sw_rising) {
    sw1.sw_rising = 0;
    clock_mode = m_Clock;
    LED_Clear();
    CLCD_Clear();
  }
  if (sw2.sw_rising) {
    sw2.sw_rising = 0;
    switch (stopw_status) {
      case Stopped:
      case Paused:
        stopw_status = Running;
        L_BLUE_OFF();
        L_GREEN_ON();
        break;
      case Running:
        stopw_status = Paused;
        L_GREEN_OFF();
        L_BLUE_ON();
        break;
    }
  }
  if (sw3.sw_rising) {
    sw3.sw_rising = 0;
    switch (stopw_status) {
      case Stopped:
        break;
      case Paused:
        stopw_status = Stopped;
        L_BLUE_OFF();
        R_RED_OFF();
        CLCD_Clear();
        break;
      case Running:
        LapSave();
        break;
    }
  }
  if (sw4.sw_rising) {
    sw4.sw_rising = 0;
    R_RED_TOGGLE();
    LapDisplay();
  }
}

void StopwDisplay() {
  strdp = ondp ? '.' : ' ';
  sprintf(str1, "STW %02d:%02d:%02d%c%03d", stopw_time[0], stopw_time[1],
          stopw_time[2], strdp, stopw_time[3]);
  CLCD_Puts(0, 0, str1);
  _7SEG_SetNumber(DGT1, stopw_time[2], ondp);
  _7SEG_SetNumber(DGT2, stopw_time[3] / 100, OFF);
}

void StopwCount() {
  switch (stopw_status) {
    case Stopped:  // Initializing Code (최적화 문제 시 수정 필요)
      stopw_time[0] = 0;
      stopw_time[1] = 0;
      stopw_time[2] = 0;
      stopw_time[3] = 0;
      lap_count = 0;
      lap_display_count = 0;
      stopw_count = 0;
      ondp = 1;
      break;
    case Paused:
      break;
    case Running:
      stopw_time[3] = stopw_count;
      if (stopw_count > 999) {
        stopw_count = 0;
        stopw_time[2]++;
      }
      if (stopw_time[2] > 59) {
        stopw_time[2] = 0;
        stopw_time[1]++;
      }
      if (stopw_time[1] > 59) {
        stopw_time[1] = 0;
        stopw_time[0]++;
      }
      if (stopw_time[0] > 99) {
        stopw_time[0] = 0;
      }
      ondp = stopw_count < 500 ? 1 : 0;
      break;
  }
}

void LapSave() {
  if (lap_count >= MAX_LAP_SAVE) {
    sprintf(str2, "LAP FULL (%d/%d)  ", lap_count, MAX_LAP_SAVE);
    CLCD_Puts(0, 1, str2);
  } else {
    lap_save[lap_count].lap_time[0] = stopw_time[0];
    lap_save[lap_count].lap_time[1] = stopw_time[1];
    lap_save[lap_count].lap_time[2] = stopw_time[2];
    lap_save[lap_count].lap_time[3] = stopw_time[3];
    sprintf(str2, "LP%d %02d:%02d:%02d.%03d", lap_save[lap_count].lap_index,
            lap_save[lap_count].lap_time[0], lap_save[lap_count].lap_time[1],
            lap_save[lap_count].lap_time[2], lap_save[lap_count].lap_time[3]);
    CLCD_Puts(0, 1, str2);
    lap_count++;
  }
}

void LapDisplay() {
  if (lap_count == 0) {
    CLCD_Puts(0, 1, "NO LAP          ");
  } else {
    if (lap_display_count >= lap_count) {
      lap_display_count = 0;
    }
    sprintf(str2, "%d/%d %02d:%02d:%02d.%03d",
            lap_save[lap_display_count].lap_index, lap_count,
            lap_save[lap_display_count].lap_time[0],
            lap_save[lap_display_count].lap_time[1],
            lap_save[lap_display_count].lap_time[2],
            lap_save[lap_display_count].lap_time[3]);
    CLCD_Puts(0, 1, str2);
    lap_display_count++;
  }
}

void StopwTimer() {
  if (stopw_status != Stopped || sw1.sw_on || sw2.sw_on || sw3.sw_on ||
      sw4.sw_on) {
    stopw_timer = 0;
  } else if (stopw_timer > 30000) {
    {
      clock_mode = m_Clock;
      stopw_timer = 0;
    }
  }
}