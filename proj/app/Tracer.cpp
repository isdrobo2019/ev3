#include "Tracer.h"
#include "stdlib.h"

int dateCnt = 0;

Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2){
  }

void Tracer::init(char course) {
	
	select = course;
	select2 = course;
	integral = 0;
}

// tag::tracer_impl[]
void Tracer::terminate() {
//   msg_f("Stopped.", 4);
  leftWheel.stop();
  rightWheel.stop();
}

char Tracer::getSelect() {
	return select;
}

void Tracer::setSelect(char select) {
	this->select = select;
}

// char Tracer::getLineBlock() {
// 	return lineBlock;
// }

// void Tracer::setLineBlock(char lineBlock) {
// 	this->lineBlock = lineBlock;
// }

void Tracer::run(int target, int mode) {
	
	char str[64];
	
	//ブロックビンゴ時
	float	Kp = 0.6;
	float	Ki = 0.2;
	float	Kd = 0.01;
	
	float* deltaP;
	float* deltaI;
	float* deltaD;
	
	float p = 0;
	float i = 0;
	float d = 0;
	int pwm = 15;
	
//	if(select2 = 'R'){
		deltaP = RdeltaP;
		deltaI = RdeltaI;
		deltaD = RdeltaD;
//	}else{
//		deltaP = LdeltaP;
//		deltaI = LdeltaI;
//		deltaD = LdeltaD;
//	}

	if(mode <= sizeof(speed) / sizeof(int)) {
		Kp = deltaP[mode];
		Ki = deltaI[mode];
		Kd = deltaD[mode];
		pwm = speed[mode];
	}

	snprintf(str,64,"S[%d] P[%2.1f] D[%2.1f] I[%2.1f]",pwm , Kp, Kd, Ki);
	ev3_lcd_draw_string(str,0,30);
	
/*	if(mode == 4 ){
		target = target + 2;
	}
*/
	diff[0] = diff[1];
	diff[1] = colorSensor.getBrightness() - target;
//	float turn = Kp * diff[1] + bias;
	
	integral += (diff[1] + diff[0]) / 2.0*0.004;
	
	p = Kp * diff[1];
	i = Ki * integral;
	d = Kd * (diff[1] - diff[0]) / 0.004;
	
	float turn = p + i + d;
	if(pwm < labs(turn)) {
		if(turn > 0 ){
			turn = pwm;
		} else {
			turn = -pwm;
		}
	}
/*
	if(100 <= labs(turn)) {
		if(turn > 0 ){
			turn = 100;
		} else {
			turn = -100;
		}
	}
*/	
	if(select == 'L'){
		turn = -turn;

	} else {
		ev3_speaker_play_tone(1000,30);
	}
	
	leftWheel.setPWM(pwm - turn);
    rightWheel.setPWM(pwm + turn);

/*
	pidtest_sensor[dateCnt] = integral;
	pidtest_p[dateCnt] = p;
	pidtest_i[dateCnt] = i;
	pidtest_d[dateCnt] = d;

	//pidtest[dateCnt] = turn;
	dateCnt++;
*/

}
