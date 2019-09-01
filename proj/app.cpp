// tag::tracer_def[]
#include "const.h"
#include "app.h"
#include "target_test.h"
#include "btclient/btapp.h"
#include "Tracer.h"
#include <math.h>
#include <stdlib.h>

#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
using namespace ev3api;


#include "app/calibration.h"
#include "app/blockbingo.h"

Tracer tracer;                  // Tracerのインスタンスを作成
ColorSensor colorSensor(PORT_2);

Motor leftWheel(PORT_C);
Motor rightWheel(PORT_B);

// リフレクトセンサ・白
int light_white = 0;

// リフレクトセンサ・黒
int light_black = 0;

// リフレクトセンサ・中間
int light_target = 0;
int first_target = 0;

float cm[15] = {55, 75, 50, 80, 90, 95, 115, 80, 80, 20, 170, 60, 140, 70, 10};

int mode = 0;

// カラーセンサ・白
rgb_raw_t stageWhite;


// カラーセンサ補正倍率
double rgbCoef[3];
int flg = 1;

void tracer_cyc(intptr_t exinf) {
	act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t exinf) {

	if(advanceChk(cm[mode])){
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

// void blockbingo_cyc(intptr_t exinf) {

// }

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

	stageSelect(course);
	
	tracer.init(course);
	
  // カラーキャリブレーション
	colorCalibration(stageWhite, light_black, light_white, light_target, rgbCoef);

	//走行開始
	// ev3_sta_cyc(TRACER_CYC);
	// slp_tsk();
	
	// //走行終了
	// ev3_stp_cyc(TRACER_CYC);
	// tracer.terminate();

  // ブロックビンゴ仮？
  //rgb2hsv(0,0,0);
  //float constvaltest = WIDTH_RADIUS;
  // leftWheel.setPWM((Motor::PWM_MAX)/5);
  // rightWheel.setPWM((Motor::PWM_MAX)/5);

  //int blockSetPhase = -1;
	//int turning[5] = {0, 1, 2, 3, 4};
	//0~5 右～左
	int turning[3] = {2, 1, 4};

  btmain(unused);

	blockBingoMethod(tracer, leftWheel, rightWheel, turning, 3, light_white, light_black, rgbCoef);

  leftWheel.setPWM(0);
  rightWheel.setPWM(0);
	ext_tsk();
}

/**
 * leftRight: 0左, 1右
 */
int monoWheelRotChk(int degree, int leftRight) {
  float travel = (leftRight != 0)? leftWheel.getCount(): rightWheel.getCount();
  			char str[64];
			  snprintf(str,64,"degree :[%d], travel :[%3.2f]",degree, travel);
			ev3_lcd_draw_string(str,0,10);

  return (labs(travel) >= (float)abs(degree) * WIDTH / WHEEL_RADIUS)? 1: 0;
}

int advanceChk(float distance) {
	balancingAdvanceChk(distance, 0, 0);
}

int balancingAdvanceChk(float distance, int balancing, int pwm) {
	
	float radius = 5.0f;
	float position = distance;

	int balancer = 0;

	float leftTravel;
	float rightTravel;
	float progress = 0.0f;
	float progress_ratio = 0.0f;

	leftTravel = leftWheel.getCount() * 2.0f * radius * M_PI / 360.0f;
	rightTravel = rightWheel.getCount() * 2.0f * radius * M_PI / 360.0f;

	if(balancing == 1) {
		balancer = pwm>0? 1: -1;
		if(labs(leftTravel) > labs(rightTravel)) {
			leftWheel.setPWM(pwm-balancer);
			rightWheel.setPWM(pwm+balancer);
		} else {
			leftWheel.setPWM(pwm+balancer);
			rightWheel.setPWM(pwm-balancer);
		}
	}

	progress = (leftTravel + rightTravel) / 2.0f;
	progress_ratio = progress / position;

  	if(labs(progress) > labs(position)){
  		return 1;
  	}
  	return 0;
}

