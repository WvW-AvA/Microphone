 #include "main.h"
 #include "pan.h"
 
 
 void send_L_angle(int angle){
		int counter_trans = 1500+error_fix+angle*180/2000;
	 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, counter_trans);
 }
 
  void send_H_angle(int angle){
		int counter_trans = 1500+error_fix+angle*180/2000;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, counter_trans);
 }
	
