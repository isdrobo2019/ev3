// tag::tracer_def[]
#include "app.h"
#include "Tracer.h"
#include <math.h>

#include "ColorSensor.h"
#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
using namespace ev3api;

Tracer tracer;                  // Tracerのインスタンスを作成
ColorSensor colorSensor(PORT_2);
TouchSensor touchSensor(PORT_1);
Motor arm(PORT_A);
Clock clock;


  Motor leftWheel(PORT_C);
  Motor rightWheel(PORT_B);

char str[64];
int light_white = 0;
int light_black = 0;
int light_target = 0;

int flg = 1;

void tracer_cyc(intptr_t exinf) {
	
	if(advanceN(80) == 1){
		ev3_speaker_play_tone(100,30);
		wup_tsk(MAIN_TASK);         // 左ボタン押下でメインを起こす
	} else {
	
		tracer.run(light_target, flg);               // 走行
	}
	ext_tsk();
	
//	act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t exinf) {

}

void main_task(intptr_t unused) {

	tracer.init();
	ev3_speaker_set_volume(5);
	arm.reset();
	arm.setPWM(5);

	while(1){
		if(arm.getCount() >= 30){
			arm.reset();
			arm.stop();
			break;
		}
	}
	while(1) {

		//タッチセンサが押下された場合
		if (touchSensor.isPressed() == true && light_black == 0) {
			
			ev3_speaker_play_tone(100,30);
			light_black = colorSensor.getBrightness();
			snprintf(str,64,"black:[%d]",light_black);
			ev3_lcd_draw_string(str,0,30);
			clock.sleep(500);
		}
		
		//タッチセンサが押下され、黒の値が取得済みの場合
		if (touchSensor.isPressed() == true && (light_white == 0 && light_black != 0)) {
			ev3_speaker_play_tone(100,30);
			
			light_white = colorSensor.getBrightness();
			light_target = (light_black + light_white) / 2;
			
			snprintf(str,64,"white:[%d]",light_white);
			ev3_lcd_draw_string(str,0,60);
			snprintf(str,64,"target:[%d]",light_target);
			ev3_lcd_draw_string(str,0,90);
			clock.sleep(500);
		}

		// 左ボタンを押すとリセット
		if (ev3_button_is_pressed(LEFT_BUTTON)) {
			light_black = 0;
			light_white = 0;
			snprintf(str,64,"black:         ");
	 		ev3_lcd_draw_string(str,0,30);
			snprintf(str,64,"white:         ");
			ev3_lcd_draw_string(str,0,60);
			snprintf(str,64,"target:         ");
			ev3_lcd_draw_string(str,0,90);
		}

		// タッチセンサーを押すと走行を開始する
		if (touchSensor.isPressed() == true && (light_white != 0 && light_black != 0)) {
		 	break;
		}
	}
	//走行開始
	ev3_sta_cyc(TRACER_CYC);
	slp_tsk();
	
	//走行終了
	ev3_stp_cyc(TRACER_CYC);
	tracer.terminate();
	ext_tsk();
}


int advanceN(float distance) {
	
	float radius = 5.0f;
	float position = distance;
	int reviseL = 0;
	int reviseR = 0;
	Clock clock;

	float balancer = 0;
	const int8_t pwm = (Motor::PWM_MAX)/5;

	init_f(__FILE__);
	char str[64];


	float leftTravel;
	float rightTravel;
	float progress = 0.0f;
	float progress_ratio = 0.0f;

	leftTravel = leftWheel.getCount() * 2.0f * radius * M_PI / 360.0f;
	rightTravel = rightWheel.getCount() * 2.0f * radius * M_PI / 360.0f;

	progress = (leftTravel + rightTravel) / 2.0f;
	progress_ratio = progress / position;
	  	
	if(progress > position){
  		return 1;
  	}
  	return 0;
}
