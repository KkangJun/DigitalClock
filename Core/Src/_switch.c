#include "_switch.h"

void SwitchRunning(TIM_HandleTypeDef *htim, uint8_t onbz) {
  SwitchStatus(htim, &sw1, onbz);
  SwitchStatus(htim, &sw2, onbz);
  SwitchStatus(htim, &sw3, onbz);
  SwitchStatus(htim, &sw4, onbz);
}

void SwitchStatus(TIM_HandleTypeDef *htim, SW_status *sw, uint8_t onbz) {
  if (sw->sw_on) {
    if (sw->sw_count <= S_M_THRESHOLD) {
      sw->sw_press = Short;
      if (sw->bz_mode == bzLow) {
        sw->bz_on = 1;
        sw->bz_mode = bzMid;
        sw->bz_sound = LOW_BUZZER;
      }
    } else if (sw->sw_count <= M_L_THRESHOLD) {
      sw->sw_press = Mid;
      if (sw->bz_mode == bzMid) {
        sw->bz_on = 1;
        sw->sw_mid_thres = 1;
        sw->bz_mode = bzHigh;
        sw->bz_sound = MID_BUZZER;
      }
    } else {
      sw->sw_press = Long;
      if (sw->bz_mode == bzHigh) {
        sw->bz_on = 1;
        sw->sw_long_thres = 1;
        sw->bz_mode = bzNone;
        sw->bz_sound = HIGH_BUZZER;
      }
    }
    BuzzerSq(htim, sw, onbz);
  } else {
    SwitchClear(sw);
  }
}

void BuzzerSq(TIM_HandleTypeDef *htim, SW_status *sw, uint8_t onbz) {
  if (onbz && sw->bz_on) {
    TIM2->PSC = sw->bz_sound;
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
  } else {
    HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
  }
}

void SwitchClear(SW_status *sw) {
  sw->sw_press = None;
  sw->bz_mode = bzNone;
  sw->sw_mid_thres = 0;
  sw->sw_long_thres = 0;
  sw->sw_mid_flag = 0;
  sw->sw_long_flag = 0;
  sw->sw_count = 0;
  sw->bz_count = 0;
}

void SwitchCheck(SW_status *sw, GPIO_TypeDef *GPIOx, uint16_t pin_num) {
  if (HAL_GPIO_ReadPin(GPIOx, pin_num)) {
    sw->sw_on = 1;
    sw->sw_rising = 1;
    sw->bz_mode = bzLow;
  } else {
    sw->sw_on = 0;
    sw->bz_mode = bzNone;
  }
}

void SwitchPressTimer(SW_status *sw) {
  if (sw->sw_count <= M_L_THRESHOLD) {
    sw->sw_count++;
  }
  if (sw->sw_press == Mid) {
    sw->sw_mid_count++;
    if (sw->sw_mid_count > MID_TIMER) {
      sw->sw_mid_count = 0;
      sw->sw_mid_flag = 1;
    }
  } else if (sw->sw_press == Long) {
    sw->sw_long_count++;
    if (sw->sw_long_count > LONG_TIMER) {
      sw->sw_long_count = 0;
      sw->sw_long_flag = 1;
    }
  }
}

void SwitchBuzzerTimer(SW_status *sw) {
  if (sw->bz_on) {
    sw->bz_count++;
    if (sw->bz_count > BUZZER_TIMER) {
      sw->bz_on = 0;
      sw->bz_count = 0;
    }
  }
}