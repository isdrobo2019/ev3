// tag::tracer_def[]
int timecount = 0;
int* btway;
int btcount;
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
#include "app/garage.h"

Tracer tracer;                  // Tracerのインスタンスを作成
ColorSensor colorSensor(PORT_2);

Clock clock;
Motor leftWheel(PORT_C);
Motor rightWheel(PORT_B);

// リフレクトセンサ・白
int light_white = 0;

// リフレクトセンサ・黒
int light_black = 0;

// リフレクトセンサ・中間
int light_target = 0;
int first_target = 0;

float tracer_cm[15] = {55, 75, 50, 80, 90, 95, 115, 80, 80, 20, 170, 60, 140, 70, 10};
float reserve_cm[15] = {55, 75, 50, 80, 115, 0, 110, 30, 25, 107, 150, 20, 170, 70, 10};

int mode = 0;
int pwm = 40;//50;
int inside_pwm = 20;//25;

// カラーセンサ・白
rgb_raw_t stageWhite;

double redHSV[3];
double yellowHSV[3];
double greenHSV[3];
double blueHSV[3];

// カラーセンサ補正倍率
double rgbCoef[3];
int flg = 1;

int counting = 0;
void reserve_cyc(intptr_t exinf) {

	timecount += 4;
	if(mode == 5){
		//回転
		leftWheel.setPWM(-20);
		rightWheel.setPWM(20);
		if(rotationChk(177)){
			ev3_speaker_play_tone(100,30);
	  		leftWheel.setCount(0);
	  		rightWheel.setCount(0);
			mode++;
			
			tracer.setSelect('L');
		}
	}else{
		
		if(advanceChk(reserve_cm[mode])){
			ev3_speaker_play_tone(500,30);
		  	leftWheel.setCount(0);
		  	rightWheel.setCount(0);

			mode++;
				
		}
		
		if(mode == 0){
			tracer.run(first_target, mode);
		} if(mode == 7){
			tracer.run(light_target - 4, mode);
		}else {
		
			tracer.run(light_target, mode);
		}
	}

	if(mode ==  sizeof(reserve_cm) / sizeof(int)){
		ev3_speaker_play_tone(300,30);
		wup_tsk(MAIN_TASK);
	}

	ext_tsk();
}

// ライントレースタスク
void tracer_cyc(intptr_t exinf) {

	timecount += 4;
	if(advanceChk(tracer_cm[mode])){
		ev3_speaker_play_tone(500,30);
	  	leftWheel.setCount(0);
	  	rightWheel.setCount(0);

		mode++;
	}
	
	if(mode == 0){
		tracer.run(first_target, mode);
	} else {
		
		tracer.run(light_target, mode);
	}

	if(mode ==  sizeof(tracer_cm) / sizeof(int)){
		ev3_speaker_play_tone(300,30);
		wup_tsk(MAIN_TASK);         // 左ボタン押下でメインを起こす
	}
	
	ext_tsk();
}

// void blockbingo_cyc(intptr_t exinf) {

// }

void main_task(intptr_t unused) {

char courseLR;
	TouchSensor touchSensor(PORT_1);
	Motor arm(PORT_A);
	ev3_speaker_set_volume(5);



	//カラーセンサの角度制御
	arm.reset();
	arm.setPWM(-8);

	int arm_counting = 0;
	while(1){
		if(arm_counting >= 100){
			arm.reset();
			arm.stop();
			break;
		}
		clock.sleep(4);
		arm_counting++;

	}

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
	courseLR = course;

	tracer.init(course);
	
  // カラーキャリブレーション
	colorCalibration(stageWhite, light_black, light_white, light_target, rgbCoef
	, redHSV, yellowHSV, greenHSV, blueHSV
	);
	
	clock.reset();
	leftWheel.setCount(0);
	rightWheel.setCount(0);
	
	
	//現在の反射光を取得
	first_target = colorSensor.getBrightness();
	
	clock.sleep(200);

	
	// //走行開始
	// if(course== 'R'){
	// 	//ライントレース(回転)
	// 	ev3_sta_cyc(RESERVE_CYC);
	// 	slp_tsk();
	// 	ev3_stp_cyc(RESERVE_CYC);
	// } else {
	// 	//ライントレース
	// 	ev3_sta_cyc(TRACER_CYC);
	// 	slp_tsk();
	// 	ev3_stp_cyc(TRACER_CYC);
	// }

//走行終了
//tracer.terminate();
// slp_tsk();

  // ブロックビンゴ仮
  //float constvaltest = WIDTH_RADIUS;
  // leftWheel.setPWM((Motor::PWM_MAX)/5);
  // rightWheel.setPWM((Motor::PWM_MAX)/5);

  //int blockSetPhase = -1;
	//int turning[5] = {0, 1, 2, 3, 4};
	//0~4 右～左, 5 ターン
	//int turning[3] = {2, 1, 4};
	//int turning[4] = {0, 1, 4, 5};
	//int turning[3] = {2, 4, 4};
	//int turning_count = 3;

	//int turning[9] = {4, 0, 4, 0, 2, 0, 0, 4, 0};
	//int turning_count = 9;

	int turning[64];// = {2, 5, 5, 5, 5, 5, 5, 5, 5};
	int turning_count;
	if(courseLR == 'L') {
		turning[0] = S_FRONT_RIGHT;
		turning[1] = S_RIGHT;
		turning[2] = S_LEFT;
		turning[3] = S_LEFT;
		turning[4] = S_FRONT_LEFT;

		turning[5] = S_FRONT;
		turning[6] = S_FRONT_LEFT;
		turning[7] = S_FRONT;
		turning[8] = S_LEFT;
		turning[9] = S_BACK;

		turning[10] = S_FRONT_RIGHT;
		turning[11] = S_FRONT;
		turning[12] = S_FRONT_RIGHT;
		turning_count = 13;		
	} else {
		turning[0] = S_FRONT;
		turning[1] = S_FRONT;
		turning[2] = S_FRONT_LEFT;
		turning[3] = S_LEFT;
		turning[4] = S_LEFT;

		turning[5] = S_FRONT_LEFT;
		turning[6] = S_FRONT;
		turning[7] = S_LEFT;
		turning[8] = S_FRONT_LEFT;
		turning[9] = S_FRONT;

		turning[10] = S_FRONT_LEFT;
		turning[11] = S_FRONT;
		turning[12] = S_LEFT;
		turning[13] = S_LEFT;
		turning[14] = S_FRONT;

		turning[15] = S_FRONT_RIGHT;
		turning_count = 16;		
	}
turning[ 0]=0;
turning[ 1]=1;
turning[ 2]=5;
turning[ 3]=0;
turning[ 4]=7;
turning[ 5]=3;
turning[ 6]=3;
turning[ 7]=6;
turning[ 8]=1;
turning[ 9]=3;
turning[10]=4;
turning[11]=3;
turning[12]=3;
turning[13]=5;
turning[14]=0;
turning[15]=3;
turning[16]=3;
turning[17]=4;
turning[18]=3;
turning[19]=0;
turning[20]=0;
turning[21]=2;
turning[22]=0;
turning[23]=5;
turning[24]=3;
turning[25]=0;
turning[26]=2;
turning[27]=0;
turning[28]=5;

		turning_count = 29;		


  btmain(unused);
FILE *btfp;
btfp = fopen("btresult.log", "w");
for(int i=0;i<btcount;i++){
	fprintf(btfp, "btway[%d]=[%d]\n", i, btway[i]);
}
fclose(btfp);

	 blockBingoMethod(clock, tracer, leftWheel, rightWheel, turning, turning_count, light_white, light_black, rgbCoef
	 , redHSV, yellowHSV, greenHSV, blueHSV);
	//  blockBingoMethod(clock, tracer, leftWheel, rightWheel, btway, btcount, light_white, light_black, rgbCoef
	//  , redHSV, yellowHSV, greenHSV, blueHSV);

	garage(leftWheel, rightWheel, courseLR);

	ev3_speaker_play_tone(200,30);
	clock.sleep(80);
	ev3_speaker_play_tone(300,30);
	clock.sleep(80);
	ev3_speaker_play_tone(400,30);
	clock.sleep(80);

	delete [] btway;
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

int rotationChk(float angle) {

	//直径（タイヤ間
	float diameter = 14.5f;
	float radius = 5.0f;
	float rightTravel;
	float leftTravel;
	float Travel;

	float distance = diameter * M_PI * angle / 360.0f;
	rightTravel = rightWheel.getCount() * 2.0f * radius * M_PI / 360.0f;
	leftTravel = leftWheel.getCount() * 2.0f * radius * M_PI / 360.0f;
	Travel = (labs(rightTravel) + labs(leftTravel)) /2.0f;
	
	
	if(Travel >= distance){
		return 1;
	}
	return 0;
}

int advanceChk(float distance) {
	return balancingAdvanceChk(distance, 0, 0);
}

int balancingAdvanceChk(float distance, int balancing, int pwm) {
	balancingAdvanceChk2(distance, balancing, pwm, 1.0f);
}

int balancingAdvanceChk2(float distance, int balancing, int pwm, float pwmRate) {
	
	float radius = 5.0f;
	float position = distance;

	float balancer = 1.0f;
	float lowP = 1.0f;
	float highP = 1.2f;

	float leftTravel;
	float rightTravel;
	float progress = 0.0f;
	float progress_ratio = 0.0f;

	leftTravel = leftWheel.getCount() * 2.0f * radius * M_PI / 360.0f;
	rightTravel = rightWheel.getCount() * 2.0f * radius * M_PI / 360.0f;

	if(balancing == 1 || balancing == 2) {
		//balancer = pwm>0? 1.05: 0.95;
		//lowP = pwm>0? 1.05: 0.95;
		//highP = pwm>0? 0.95: 1.05;
		if(labs(leftTravel) * pwmRate > labs(rightTravel) / pwmRate) {
			if(counting%100==0) ev3_speaker_play_tone(100,30);
			// leftWheel.setPWM(pwm-balancer);
			// rightWheel.setPWM(pwm+balancer);
			leftWheel.setPWM((int)(pwm*lowP) * ((balancing == 1)? 1: -1) / pwmRate);
			rightWheel.setPWM((int)(pwm*highP) * pwmRate);
			
		} else {
			if(counting%100==0) ev3_speaker_play_tone(800,30);
			// leftWheel.setPWM(pwm+balancer);
			// rightWheel.setPWM(pwm-balancer);
			leftWheel.setPWM((int)(pwm*highP) * ((balancing == 1)? 1: -1) / pwmRate);
			rightWheel.setPWM((int)(pwm*lowP) * pwmRate);
		}
	}

	progress = (labs(leftTravel) * pwmRate + labs(rightTravel) / pwmRate) / 2.0f;
	progress_ratio = progress / position;

	counting++;
  	if(labs(progress) > labs(position)){
  		return 1;
  	}
  	return 0;
}
