void garage(Motor& leftWheel, Motor& rightWheel, char LR) {

  Clock clock;
	int myMotorPower = (Motor::PWM_MAX)/5;
	float distance;
	int angle;

  if(LR == 'L') {
	leftWheel.setCount(0);
	rightWheel.setCount(0);
	leftWheel.setPWM(myMotorPower);
	rightWheel.setPWM(myMotorPower);

	distance = 26.0f;
	while(balancingAdvanceChk(distance, 1, myMotorPower) != 1) clock.sleep(4);
	
	leftWheel.setCount(0);
	rightWheel.setCount(0);
	leftWheel.setPWM(0);
	rightWheel.setPWM(myMotorPower);
	
	angle = 90;
	while(monoWheelRotChk(angle, 0) != 1) clock.sleep(4);
	leftWheel.setPWM(0);
	rightWheel.setPWM(0);
  } else {

	leftWheel.setCount(0);
	rightWheel.setCount(0);
	leftWheel.setPWM(myMotorPower);
	rightWheel.setPWM(myMotorPower);
	distance = 18.0f;
	while(balancingAdvanceChk(distance, 1, myMotorPower) != 1) clock.sleep(4);

	leftWheel.setCount(0);
	rightWheel.setCount(0);
	leftWheel.setPWM(0);
	rightWheel.setPWM(myMotorPower);

	angle = 90;
	while(monoWheelRotChk(angle, 0) != 1) clock.sleep(4);

	leftWheel.setCount(0);
	rightWheel.setCount(0);
	leftWheel.setPWM(myMotorPower);
	rightWheel.setPWM(myMotorPower);
	distance = 25.0f;
	while(balancingAdvanceChk(distance, 1, myMotorPower) != 1) clock.sleep(4);
	leftWheel.setPWM(0);
	rightWheel.setPWM(0);
  }
}