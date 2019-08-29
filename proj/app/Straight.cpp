#include "Straight.h"
#include "stdlib.h"
Straight::Straight():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2) {
  }

void Straight::init() {
}

void Straight::terminate() {
//   msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
}

void Straight::run(int target,int flg) {

	int pwm = 15;

	if(pwm < labs(turn)){
		if(turn > 0 ){
			turn = pwm;
		} else {
			turn = -pwm;
		}
	}

	int turn = leftWheel.getCount() - rightWheel.getCount();
	turn /= abs(turn);

	leftWheel.setPWM(pwm - turn);
	rightWheel.setPWM(pwm + turn);

}

