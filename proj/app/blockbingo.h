  void blockBingoMethod(Tracer tracer, Motor& leftWheel, Motor& rightWheel, int* turning, int turningLength, int light_white, int light_black, double* rgbCoef) {
	char str[64];
	int mode = 0;
  bool change = true;
	int turningNow = 0;
	  int tmpCount = 0;

int myMotorPower = (Motor::PWM_MAX)/8;

  // リフレクトセンサ・白
  int light_grey = light_white*(1.0f-GREYLEVEL) + light_black*GREYLEVEL;//(light_white + light_black * 3.0f) / 4.0f;

// カラーセンサ・ステージ
rgb_raw_t stageColor;

  Clock clock;
  TouchSensor touchSensor(PORT_1);
  ColorSensor colorSensor(PORT_2);
//Motor leftWheel(PORT_C);
//Motor rightWheel(PORT_B);


  // 難所whileループ部分
  while(true) {

	if(mode == 0) tracer.run(light_grey, 20);
    // モード変わったときにモータのパワーをセットする
    if(change) {
      switch(mode) {
        case 0:
          //leftWheel.setPWM(myMotorPower);
          //rightWheel.setPWM(myMotorPower);
        break;
        case 1:
		switch(turning[turningNow]) {
			case 0:
			case 1:
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(0);
			break;
			case 2:
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(myMotorPower);
			break;
			case 3:
			case 4:
          leftWheel.setPWM(0);
          rightWheel.setPWM(myMotorPower);
			break;
		}
			//snprintf(str,64,"now :[%3.2f]");
			//ev3_lcd_draw_string(str,0,10);
          leftWheel.setCount(0);
          rightWheel.setCount(0);
        break;
		case 2:
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(myMotorPower);
          leftWheel.setCount(0);
          rightWheel.setCount(0);
		break;
		case 3:
          leftWheel.setPWM(-myMotorPower);
          rightWheel.setPWM(-myMotorPower);
          leftWheel.setCount(0);
          rightWheel.setCount(0);
		break;
		case 4:
		switch(turning[turningNow]) {
			case 1:
          leftWheel.setPWM(-myMotorPower);
          rightWheel.setPWM(0);
			break;
			case 3:
          leftWheel.setPWM(0);
          rightWheel.setPWM(-myMotorPower);
			break;
		}
			//snprintf(str,64,"now :[%3.2f]");
			//ev3_lcd_draw_string(str,0,10);
          leftWheel.setCount(0);
          rightWheel.setCount(0);
		break;
      }
      change = false;
    }
		//if(tmpCount%500==0)ev3_speaker_play_tone(100,30);
    int angle;
	float distance;
    switch(mode) {
      case 0:
		colorSensor.getRawColor(stageColor);
		
        if(rawColortoColorNumber(stageColor, rgbCoef) == COLOR_NONE) {
			
        //if(false) {
        //if(true) {
          ev3_speaker_play_tone(100,30);
          mode = 1;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(true) {
              if (touchSensor.isPressed() == true ){
                break;
              }
          }
          //leftWheel.setPWM(myMotorPower);
          //rightWheel.setPWM(myMotorPower);
        }
      break;
      case 1:
	  switch(turning[turningNow]){
		case 0:
		case 1:
		angle = turning[turningNow]==0? 90: 45;
		//if(blockSetPhase==3) angle *= -1;
        if(monoWheelRotChk(angle, 1) == 1) {
          ev3_speaker_play_tone(300,30);
          //mode = 0;
  		  mode = turning[turningNow]==1? 2: 0;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(true) {
              if (touchSensor.isPressed() == true){
                break;
              }
          }
   		  if(turning[turningNow]==0) turningNow = (turningNow + 1) % turningLength;
		  //if(blockSetPhase == 0 && turning[turningNow] == 1) blockSetPhase = 1;
		  //if(blockSetPhase == 3) blockSetPhase = 0;
		  tracer.setSelect('R');
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(0);
        }
		break;
		case 2:
		distance = 15.0f;
		//if(blockSetPhase==2) distance*= -1;
		if(balancingAdvanceChk(distance, 1, myMotorPower) == 1) {
          ev3_speaker_play_tone(300,30);
          mode = 0;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(true) {
              if (touchSensor.isPressed() == true ){
                break;
              }
          }
   		  turningNow = (turningNow + 1) % turningLength;
    	  //if(blockSetPhase==1 || blockSetPhase==2) blockSetPhase++;
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(myMotorPower);
		}
		break;
		case 3:
		case 4:
		angle = turning[turningNow]==3? 45: 90;
		//if(blockSetPhase==3) angle *= -1;

        if(monoWheelRotChk(angle, 0) == 1) {
          ev3_speaker_play_tone(300,30);
          //mode = 0;
		  mode = turning[turningNow]==3? 2: 0;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(true) {
              if (touchSensor.isPressed() == true ){
                break;
              }
          }
   		  if(turning[turningNow]==4) turningNow = (turningNow + 1) % turningLength;
		  //if(blockSetPhase == 0 && turning[turningNow] == 1) blockSetPhase = 1;
		  //if(blockSetPhase == 3) blockSetPhase = 0;
		  tracer.setSelect('L');
          leftWheel.setPWM(0);
          rightWheel.setPWM(myMotorPower);
        }
		break;
	  }
      break;
	  // ブロサ前進
	  case 2:
		if(advanceChk(20.0) == 1) {
          ev3_speaker_play_tone(400,30);
			mode = 3;
          change = true;
		}
	  break;
	  // ブロサ後退
 	  case 3:
		if(advanceChk(-20.0) == 1) {
          ev3_speaker_play_tone(550,30);
			mode = 4;
          change = true;
		}
	  break;
	  // 逆ターン
 	  case 4:
			switch(turning[turningNow]){
				case 1:
					if(monoWheelRotChk(45, 1) == 1) {
			          ev3_speaker_play_tone(800,30);
						turningNow = (turningNow + 1) % turningLength;
				    	change = true;
						mode = 0;
					}
				break;
				case 3:
					if(monoWheelRotChk(45, 0) == 1) {
			          ev3_speaker_play_tone(800,30);
						turningNow = (turningNow + 1) % turningLength;
          				change = true;
						mode = 0;
					}
				break;
			}
		
	  break;
    }
    clock.sleep(4);
    tmpCount++;
  }
  }