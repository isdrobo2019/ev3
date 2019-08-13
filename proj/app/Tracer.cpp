#include "Tracer.h"
#include "stdlib.h"
Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2) {
  }

void Tracer::init() {
  init_f("083_sample");
}

void Tracer::terminate() {
  msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
}

void Tracer::run(int target,int flg) {
	const	float	Kp = 0.8;
	float p = 0;
	float turn = 0;

	int pwm = 15;

	p = Kp * (colorSensor.getBrightness() - target);

	turn = p;

	if(pwm < labs(turn)){
		if(turn > 0 ){
			turn = pwm;
		} else {
			turn = -pwm;
		}
	}

	leftWheel.setPWM(pwm - turn);
	rightWheel.setPWM(pwm + turn);

}

