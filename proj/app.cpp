// tag::tracer_def[]
#include "app.h"
#include "Tracer.h"
#include <math.h>

#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
using namespace ev3api;

Tracer tracer;                  // Tracerのインスタンスを作成
ColorSensor colorSensor(PORT_2);

Motor leftWheel(PORT_C);
Motor rightWheel(PORT_B);

int light_white = 0;
int light_black = 0;
int light_target = 0;
int first_target = 0;

float cm[15] = {55, 75, 50, 80, 90, 95, 115, 80, 80, 20, 170, 60, 140, 70, 10};

int mode = 0;

void tracer_cyc(intptr_t exinf) {
	act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t exinf) {

	if(advanceN(cm[mode])){
		ev3_speaker_play_tone(100,30);
	  	leftWheel.setCount(0);
	  	rightWheel.setCount(0);

		mode++;
	}
	
	if(mode == 0){
		tracer.run(first_target, mode);
	} else {
		
		tracer.run(light_target, mode);
	}
	
	tracer.run(light_target, mode);

	if(mode ==  sizeof(cm) / sizeof(int)){
		ev3_speaker_play_tone(100,30);
		wup_tsk(MAIN_TASK);         // 左ボタン押下でメインを起こす
	}
	
	ext_tsk();
}

void main_task(intptr_t unused) {

	TouchSensor touchSensor(PORT_1);
	Motor arm(PORT_A);
	Clock clock;
	ev3_speaker_set_volume(5);

	arm.reset();
	arm.setPWM(5);

	//カラーセンサの角度制御
	while(1){
		if(arm.getCount() >= 30){
			arm.reset();
			arm.stop();
			break;
		}
	}
	
	char str[64];
	snprintf(str,64,"course:         ");
	ev3_lcd_draw_string(str,0,0);
	
	char course = '\0';

	while(1){
		//左ボタンを押下
		if (ev3_button_is_pressed(LEFT_BUTTON)){
			course = 'L';
			snprintf(str,64,"course:[%c]", course);
			ev3_lcd_draw_string(str,0,30);
			clock.sleep(500);
		}
		
		//右ボタンを押下
		if (ev3_button_is_pressed(RIGHT_BUTTON)){
			course = 'R';
			snprintf(str,64,"course:[%c]", course);
			ev3_lcd_draw_string(str,0,30);
			clock.sleep(500);
		}
		
		//中央ボタンを押下
		if (ev3_button_is_pressed(ENTER_BUTTON) && course != '\0'){
			
			snprintf(str,64,"select_course:[%c]", course);
			ev3_lcd_draw_string(str,0,30);
	
			clock.sleep(500);
			break;
		}
	}
	
	tracer.init(course);
	
	//キャリブレーション
	while(1) {

		if (touchSensor.isPressed() == true && light_black == 0) {
			
			snprintf(str,64,"black:         ");
	 		ev3_lcd_draw_string(str,0,30);
			
			ev3_speaker_play_tone(100,30);
			light_black = colorSensor.getBrightness();
			snprintf(str,64,"black:[%d]",light_black);
			ev3_lcd_draw_string(str,0,30);
			clock.sleep(500);
		}
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
		// 左ボタンを長押し、それを捕捉する
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
			
			first_target = colorSensor.getBrightness();
		 	break;
		}
	}
	
	leftWheel.setCount(0);
	rightWheel.setCount(0);

	ev3_sta_cyc(TRACER_CYC);
	slp_tsk();                    // 起きたら、走行をやめる
	ev3_stp_cyc(TRACER_CYC);
	tracer.terminate();

	ext_tsk();
}

//距離測定用
int advanceN(float distance) {
	
	float radius = 5.0f;
	float position = distance;

	float balancer = 0;

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

