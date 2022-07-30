 #define PI 3.1415926
 #include "main.h"
 #include "pan.h"
 
 
 void send_L_angle(int angle){
		int counter_trans = 500+error_fix+(angle*2000)/180;
	 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, counter_trans);
	 printf("counter_trans:%d\r\n",counter_trans);
	 		printf("error_fix:%d\r\n",error_fix);
	 	 printf("angle*2000/180:%f\r\n",angle*2000.0/180);
 }
 
 void send_H_angle(int angle){
		int counter_trans = 500+error_fix+((100-angle)*2000)/180;
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, counter_trans);
		printf("counter_trans:%d\r\n",counter_trans);
				printf("error_fix:%d\r\n",error_fix);
 }
	
float trans_anglea(int x,int y)
{
	float dx = x - 150.0;
	float anglea = atan(dx / y) / PI * 180;
	//printf("dx/y: %f\r\n", dx / y);
	printf("angleA: %f\r\n", anglea);
	return anglea;
}

float trans_angleb(int x,int y)
{
	float angleb = atan(sqrt((150-x)*(150-x)+y*y)/100)/PI*180;
	//printf("dx/y: %f\r\n", x / y);
	printf("angleB: %f\r\n", angleb);
	return angleb;
}
