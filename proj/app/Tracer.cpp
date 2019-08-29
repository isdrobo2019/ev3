#include "Tracer.h"
#include "stdlib.h"
Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2){
  }

void Tracer::init(char course) {
	
	select = course;
	// init_f("Tracer");
}

// tag::tracer_impl[]
void Tracer::terminate() {
//   msg_f("Stopped.", 4);
  leftWheel.stop();
  rightWheel.stop();
}

void Tracer::run(int target, int mode) {
	float	Kp = 0;
	float	Ki = 0;
	float	Kd = 0;
	
	float p = 0;
	float i = 0;
	float d = 0;
	int pwm = 25;
	float	integral = 0;
	
	if(mode <= sizeof(deltaP) / sizeof(int)) {
//P��D�̒l�͂��̂܂܂œ��삵�܂����B
		Kp = deltaP[mode];
//		Ki = deltaI[mode];
		Kd = deltaD[mode];
//		pwm = speed[mode];
		
	}
	
	if(mode == 4 ){//|| mode == 9) {
		target = target + 2;
	}
	

	diff[0] = diff[1];
	diff[1] = colorSensor.getBrightness() - target;
//	float turn = Kp * diff[1] + bias;
	
	integral += (diff[1] + diff[0]) / 2.0*0.005;
	
	p = Kp * diff[1];
	i = Ki * integral;
	d = Kd * (diff[1] - diff[0]) / 0.005;
	
	float turn = p + i + d;

	if(pwm < labs(turn)) {
		if(turn > 0 ){
			turn = pwm;
		} else {
			turn = -pwm;
		}
	}

	if(select == 'L'){
		turn = -turn;
	}
	
	leftWheel.setPWM(pwm - turn);
    rightWheel.setPWM(pwm + turn);

}
