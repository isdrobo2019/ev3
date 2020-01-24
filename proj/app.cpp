// tag::tracer_def[]
int timecount = 0;
int* btway = nullptr;
int btcount;
// bluetoothをtrue: 使用する false: 使用しない（手動で用意する）
bool usebt = true;

// ボタン待ちをtrue: 使用する false: 使用しない
bool buttonEnable = false;

// ライントレース区間をtrue: 有効にする false: 有効にしない
bool lineTraceEnable = true;

// ブロックビンゴ区間をtrue: 有効にする false: 有効にしない
bool blockBingoEnable = true;


bool settingLoaded = false;

#include "const.h"
#include "app.h"
#include "target_test.h"
#include "btclient/btapp.h"
#include "Tracer.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
// #include <unistd.h>

#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
using namespace ev3api;

#include "app/sakata_init.h"
#include "app/calibration.h"
#include "app/blockbingo.h"
#include "app/garage.h"

Tracer tracer;                  // Tracerのインスタンスを作成
ColorSensor colorSensor(PORT_2);

Clock clock;
Motor leftWheel(PORT_C);
Motor rightWheel(PORT_B);

//pid値確認用
FILE *fp;
float pidtest_sensor[100] = {0};
float pidtest_p[100] = {0};
float pidtest_i[100] = {0};
float pidtest_d[100] = {0};


// リフレクトセンサ・白
int light_white = 0;

// リフレクトセンサ・黒
int light_black = 0;

// リフレクトセンサ・中間
int light_target = 0;
int first_target = 0;

float tracer_cm[15] = {55, 75, 50, 80, 90, 95, 115, 80, 80, 20, 170, 60, 140, 65, 0};
float reserve_cm[15] = {55, 75, 50, 80, 115, 0, 110, 30, 25, 107, 150, 20, 170, 65, 0};
// float tracer_cm[15] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
// float reserve_cm[15] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};

int mode = 0;
int pwm = 25;//50;
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
	
/*
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
*/
	ext_tsk();
}

// ライントレースタスク
void tracer_cyc(intptr_t exinf) {
		// ev3_speaker_set_volume(3);

	if(tracer.getSelect() == 'R') {
		// ev3_speaker_play_tone(200,30);
	}

	timecount += 4;
	if(advanceChk(tracer_cm[mode])){
	
	char modestr[64];
	snprintf(modestr,64,"mode is %d", mode);
	ev3_lcd_draw_string(modestr,0,0);

	  	leftWheel.setCount(0);
	  	rightWheel.setCount(0);

		if(mode <15 && mode >=12) {
		ev3_speaker_play_tone(1000,30);
		}
		else {
		ev3_speaker_play_tone(500,30);
		}
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

/*
	if(dateCnt >= 99) {
		wup_tsk(MAIN_TASK);
	}
*/
	ext_tsk();
}

// void blockbingo_cyc(intptr_t exinf) {

// }

void main_task(intptr_t unused) {

// FILE *pwdfp = fopen("pwd.log", "w");
// char pwddir[256];
// getcwd(pwddir, 256);
// fprintf(pwdfp, "pwd = %s", pwddir);
// fclose(pwdfp);

settingLoad();

char courseLR;
	TouchSensor touchSensor(PORT_1);
	Motor arm(PORT_A);
	ev3_speaker_set_volume(5);

	if(settingLoaded) {
		ev3_speaker_play_tone(500,100);
		ev3_speaker_play_tone(50,100);
	} else {
		ev3_speaker_play_tone(50,100);
		ev3_speaker_play_tone(500,100);
	}

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

	
	mode = 0;
	//走行開始
	// if(course== 'R'){
	// 	//ライントレース(回転)
	// 	ev3_sta_cyc(RESERVE_CYC);
	// 	slp_tsk();
	// 	ev3_stp_cyc(RESERVE_CYC);
	// } else {
		//ライントレース
		if(lineTraceEnable) {
			ev3_sta_cyc(TRACER_CYC);
			slp_tsk();
			ev3_stp_cyc(TRACER_CYC);
		}
	// }

/*/pid値確認用
	if((fp = fopen("test.csv","w")) != NULL) {
		
		fprintf(fp,"%s,%s,%s,%s\n","sensor", "p", "i", "d");
		
		for(int i = 0; i < 100; i++) {
			
			fprintf(fp,"%f,%f,%f,%f\n",pidtest_sensor[i], pidtest_p[i], pidtest_i[i], pidtest_d[i]);
		}
		fclose(fp);
	}
*/


if(!blockBingoEnable) {
	//走行終了
	tracer.terminate();
	slp_tsk();
}
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
		
if(blockBingoEnable) {
	int* turning;// = {2, 5, 5, 5, 5, 5, 5, 5, 5};

	// 受け取る
	if(usebt) {
		btmain(unused);
	}
	else {
		// 経路を手動で用意
		btway = new int[64];
		// btway[0] = 5;
		// btway[1] = 3;
		// btway[2] = 6;
		// btway[3] = 0;
		// btway[4] = 1;
		// btway[5] = 5;
		// btway[6] = 0;
		// btway[7] = 1;
		// btway[8] = 6;
		// btway[9] = 3;
		// btway[10] = 6;
		// btway[11] = 1;
		// btway[12] = 5;
		// btway[13] = 1;
		// btway[14] = 0;
		// btway[15] = 3;
		// btway[16] = 7;
		// btway[17] = 1;
		// btway[18] = 2;
		// btway[19] = 4;
		// btway[20] = 0;
		// btway[21] = 3;
		// btway[22] = 5;
btway[0] = 5;
btway[1] = 3;
btway[2] = 6;
btway[3] = 0;
btway[4] = 1;
btway[5] = 5;
btway[6] = 0;
btway[7] = 0;
btway[8] = 1;
btway[9] = 5;
btway[10] = 0;
btway[11] = 5;
btway[12] = 1;
btway[13] = 0;
btway[14] = 3;
btway[15] = 7;
btway[16] = 3;
btway[17] = 3;
btway[18] = 3;
btway[19] = 1;
btway[20] = 4;
btway[21] = 1;
btway[22] = 1;
btway[23] = 1;
btway[24] = 4;
btway[25] = 0;
btway[26] = 3;
btway[27] = 3;
btway[28] = 3;
btway[29] = 0;
btway[30] = 1;
btway[31] = 4;
		btcount = 32;
		if(courseLR == 'L') {
			// btway[0] = S_FRONT;
			// btway[0] = S_FRONT_RIGHT;
			// btway[1] = S_RIGHT;
			// btway[2] = S_LEFT;
			// btway[3] = S_LEFT;
			// btway[4] = S_FRONT_LEFT;

			// btway[5] = S_FRONT;
			// btway[6] = S_FRONT_LEFT;
			// btway[7] = S_FRONT;
			// btway[8] = S_LEFT;
			// btway[9] = S_BACK;

			// btway[10] = S_FRONT_RIGHT;
			// btway[11] = S_FRONT;
			// btway[12] = S_FRONT_RIGHT;

			//btcount = 13;		
		} else {
			// btway[0] = S_FRONT;
			// btway[1] = S_FRONT;
			// btway[2] = S_FRONT_LEFT;
			// btway[3] = S_LEFT;
			// btway[4] = S_LEFT;

			// btway[5] = S_FRONT_LEFT;
			// btway[6] = S_FRONT;
			// btway[7] = S_LEFT;
			// btway[8] = S_FRONT_LEFT;
			// btway[9] = S_FRONT;

			// btway[10] = S_FRONT_LEFT;
			// btway[11] = S_FRONT;
			// btway[12] = S_LEFT;
			// btway[13] = S_LEFT;
			// btway[14] = S_FRONT;

			// btway[15] = S_FRONT_RIGHT;
			// btcount = 16;		
		}
	}

	FILE *btfp;
	btfp = fopen("btresult.log", "w");
	fprintf(btfp, "usebt = %d\n", usebt);
	for(int i=0;i< btcount;i++){
		fprintf(btfp, "way[%d]=[%d]\n", i, btway[i]);
	}
	fclose(btfp);

	ev3_speaker_play_tone(1000,30);
	 blockBingoMethod(clock, tracer, leftWheel, rightWheel, btway, btcount, light_white, light_black, rgbCoef
	 , redHSV, yellowHSV, greenHSV, blueHSV);


// FILE *btfp;
// btfp = fopen("btresult.log", "w");
// for(int i=0;i<btcount;i++){
// 	fprintf(btfp, "btway[%d]=[%d]\n", i, btway[i]);
// }
// fclose(btfp);

	garage(leftWheel, rightWheel, courseLR);

	ev3_speaker_play_tone(200,30);
	clock.sleep(80);
	ev3_speaker_play_tone(300,30);
	clock.sleep(80);
	ev3_speaker_play_tone(400,30);
	clock.sleep(80);
}
	if(btway != nullptr) delete [] btway;
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
