#include "_clock.h"

void UpSetCheck() {
  LeapCheck();
  switch (clock_set) {
    case 0:
      if (clock_time[0] > 9999) {
        clock_time[0] = 0;
      }
      break;
    case 1:
      if (clock_time[1] > 12) {
        clock_time[1] = 1;
      }
      SetDateCheck();
      break;
    case 2:
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
          }
          break;
        case 4:
        case 6:
        case 9:
        case 11:
          if (clock_time[2] > 30) {
            clock_time[2] = 1;
          }
          break;
        case 2:
          if (clock_time[2] > 28) {
            if (isleap) {
              if (clock_time[2] > 29) {
                clock_time[2] = 1;
              }
            } else {
              clock_time[2] = 1;
            }
          }
          break;
      }
      break;
    case 3:
      if (clock_time[3] > 23) {
        clock_time[3] = 0;
      }
      break;
    case 4:
      if (clock_time[4] > 59) {
        clock_time[4] = 0;
      }
      break;
    case 5:
      clock_time[5] = 0;
      clock_count = 0;
      break;
  }
}
void DownSetCheck() {
  LeapCheck();
  switch (clock_set) {
    case 0:
      if (clock_time[0] < 0) {
        clock_time[0] = 9999;
      }
      break;
    case 1:
      if (clock_time[1] < 1) {
        clock_time[1] = 12;
      }
      SetDateCheck();
      break;
    case 2:
      switch (clock_time[1]) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          if (clock_time[2] < 1) {
            clock_time[2] = 31;
          }
          break;
        case 4:
        case 6:
        case 9:
        case 11:
          if (clock_time[2] < 1) {
            clock_time[2] = 30;
          }
          break;
        case 2:
          if (clock_time[2] < 1) {
            if (isleap) {
              clock_time[2] = 29;
            } else {
              clock_time[2] = 28;
            }
          }
          break;
      }
      break;
    case 3:
      if (clock_time[3] < 0) {
        clock_time[3] = 23;
      }
      break;
    case 4:
      if (clock_time[4] < 0) {
        clock_time[4] = 59;
      }
      break;
    case 5:
      clock_time[5] = 0;
      clock_count = 0;
      break;
  }
}

void SetDateCheck() {
  switch (clock_time[1]) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      if (clock_time[2] > 31) {
        clock_time[2] = 31;
      }
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      if (clock_time[2] > 30) {
        clock_time[2] = 30;
      }
      break;
    case 2:
      if (clock_time[2] > 28) {
        if (isleap) {
          if (clock_time[2] > 29) {
            clock_time[2] = 29;
          }
        } else {
          clock_time[2] = 28;
        }
      }
      break;
  }
}

void ClockSetSwitchSq() {
  if (sw1.sw_mid_thres) {
    sw1.sw_rising = 0;
    sw1.sw_mid_thres = 0;
    clock_mode = m_Clock;
    LED_Clear();
    CLCD_Clear();
  }
  if (sw2.sw_rising) {
    sw2.sw_rising = 0;
    clock_set--;
    if (clock_set < 0) {
      clock_set = 5;
    }
  }
  if (sw3.sw_on) {
    if (sw3.sw_rising) {
      sw3.sw_rising = 0;
      clock_time[clock_set]++;
    }
    switch (sw3.sw_press) {
      case Short:
        break;
      case Mid:
        if (sw3.sw_mid_flag) {
          sw3.sw_mid_flag = 0;
          clock_time[clock_set]++;
        }
        break;
      case Long:
        if (sw3.sw_long_flag) {
          sw3.sw_long_flag = 0;
          clock_time[clock_set]++;
        }
        break;
    }
    UpSetCheck();
  }
  if (sw4.sw_on) {
    if (sw4.sw_rising) {
      sw4.sw_rising = 0;
      clock_time[clock_set]--;
    }
    switch (sw4.sw_press) {
      case Short:
        break;
      case Mid:
        if (sw4.sw_mid_flag) {
          sw4.sw_mid_flag = 0;
          clock_time[clock_set]--;
        }
        break;
      case Long:
        if (sw4.sw_long_flag) {
          sw4.sw_long_flag = 0;
          clock_time[clock_set]--;
        }
        break;
    }
    DownSetCheck();
  }
}

void ClockSetDisplay() {
  if (sw3.sw_on || sw4.sw_on) {
    ondp = 1;
  } else {
    ondp = clock_count < 500 ? 1 : 0;
  }

  switch (clock_set) {
    case 0:
      ondp ? sprintf(str1, "CLOCK %4d.%2d.%2d", clock_time[0], clock_time[1],
                     clock_time[2])
           : sprintf(str1, "CLOCK     .%2d.%2d", clock_time[1], clock_time[2]);

      sprintf(str2, "SET     %02d:%02d:%02d", clock_time[3], clock_time[4],
              clock_time[5]);
      break;
    case 1:
      ondp ? sprintf(str1, "CLOCK %4d.%2d.%2d", clock_time[0], clock_time[1],
                     clock_time[2])
           : sprintf(str1, "CLOCK %4d.  .%2d", clock_time[0], clock_time[2]);

      sprintf(str2, "SET     %02d:%02d:%02d", clock_time[3], clock_time[4],
              clock_time[5]);
      break;
    case 2:
      ondp ? sprintf(str1, "CLOCK %4d.%2d.%2d", clock_time[0], clock_time[1],
                     clock_time[2])
           : sprintf(str1, "CLOCK %4d.%2d.  ", clock_time[0], clock_time[1]);

      sprintf(str2, "SET     %02d:%02d:%02d", clock_time[3], clock_time[4],
              clock_time[5]);
      break;
    case 3:
      sprintf(str1, "CLOCK %4d.%2d.%2d", clock_time[0], clock_time[1],
              clock_time[2]);
      ondp
          ? sprintf(str2, "SET     %02d:%02d:%02d", clock_time[3],
                    clock_time[4], clock_time[5])
          : sprintf(str2, "SET       :%02d:%02d", clock_time[4], clock_time[5]);
      break;
    case 4:
      sprintf(str1, "CLOCK %4d.%2d.%2d", clock_time[0], clock_time[1],
              clock_time[2]);
      ondp
          ? sprintf(str2, "SET     %02d:%02d:%02d", clock_time[3],
                    clock_time[4], clock_time[5])
          : sprintf(str2, "SET     %02d:  :%02d", clock_time[3], clock_time[5]);
      break;
    case 5:
      sprintf(str1, "CLOCK %4d.%2d.%2d", clock_time[0], clock_time[1],
              clock_time[2]);
      ondp
          ? sprintf(str2, "SET     %02d:%02d:%02d", clock_time[3],
                    clock_time[4], clock_time[5])
          : sprintf(str2, "SET     %02d:%02d:  ", clock_time[3], clock_time[4]);
      break;
  }

  CLCD_Puts(0, 0, str1);
  CLCD_Puts(0, 1, str2);
  _7SEG_SetNumber(DGT1, clock_time[5] / 10, OFF);
  _7SEG_SetNumber(DGT2, clock_time[5] % 10, ondp);
}

void ClockSetTimer() {
  if (sw1.sw_on || sw2.sw_on || sw3.sw_on || sw4.sw_on) {
    clock_set_timer = 0;
  } else if (clock_set_timer > 30000) {
    {
      clock_mode = m_Clock;
      clock_set_timer = 0;
    }
  }
}