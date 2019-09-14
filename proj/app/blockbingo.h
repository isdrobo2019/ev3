#include <vector>

void routeLastfix(int* route, int& routeLength) {
	if(route[routeLength - 1] == 1) {
		route[routeLength] = 0;
		routeLength++;
	} else if(route[routeLength - 1] == 3) {
		route[routeLength] == 4;
		routeLength++;
	}
}

  void goGarage(int* toGarage, int& toGarageLength, int nowx, int nowy, int goalx, int goaly, int dirx, int diry) {
	  int subx = goalx - nowx;
	  int suby = goaly - nowy;
	  bool firstturn = false;
	  char firstaxis = '\0';

	  // ←→
	  if(dirx != 0) {
		  if(
			  (dirx == -1 && subx >= 0) ||
			  (dirx == 1 && subx <= 0)
		  ) {
			firstturn = true;
		  }
		  firstaxis = 'x';
	  // ↑↓
	  } else if (diry != 0) {
		  if(
			  (diry == -1 && suby >= 0) ||
			  (diry == 1 && suby <= 0)
		  ) {
			firstturn = true;
		  }
		  firstaxis = 'y';
	  }
	  if(subx == 0 && suby == 0) firstturn = true;

		int p, q, r;
		if(firstaxis == 'x') {
			p = subx;
			q = suby;
			r = 1;
		} else {
			p = suby;
			q = subx;
			r = -1;
		}

		int i = 0;
		if(firstturn) {
			toGarage[i] = 5;
			i++;
		}
		int dxcount = abs(p);
		if(!firstturn) dxcount--;
		for(int dx = 0; dx < dxcount; dx++) {
			toGarage[i] = 2;
			i++;
		}
		if(q != 0) {
			toGarage[i] = (p * q * r> 0)? 0: 4;
			i++;
			for(int dy = 0; dy < abs(q) - 1; dy++) {
				toGarage[i] = 2;
				i++;
			}
			if(firstaxis == 'x') {
				toGarage[i] = (p * q * r> 0)? 4: 0;
			} else {
				toGarage[i] = 2;
			}
			i++;
		} else {
			if(firstaxis == 'x') {
				toGarage[i] = 2;
			} else {
				int s = (goalx>0?1:-1);
				if(p==0){
				int t = (diry==1?1:-1);
					toGarage[i] = (t * s> 0)? 0: 4;
				} else {
					toGarage[i] = (p * s> 0)? 4: 0;
				}
			}
			i++;
		}
		toGarageLength = i;
  }
  
  void blockBingoMethod(Clock clock, Tracer tracer, Motor& leftWheel, Motor& rightWheel, int* turning, int turningLength, int light_white, int light_black, double* rgbCoef
  , double* redHSV, double* yellowHSV, double* greenHSV, double* blueHSV) {
	  FILE *fp;
	char str[64];
	int sector = 0;
	int mode = 0;
  bool change = true;
  bool buttonEnable = false;
	int turningNow = 0;
	  int tmpCount = 0;

int myMotorPower = (Motor::PWM_MAX)/5;
int toGarage[64];
int toGarageLength = 0;
int* nowRoute;
int routeLength;
  // リフレクトセンサ・白
  int light_grey = light_white*(1.0f-GREYLEVEL) + light_black*GREYLEVEL;//(light_white + light_black * 3.0f) / 4.0f;

// カラーセンサ・ステージ
rgb_raw_t stageColor;

  //Clock clock;
  TouchSensor touchSensor(PORT_1);
  ColorSensor colorSensor(PORT_2);
//Motor leftWheel(PORT_C);
//Motor rightWheel(PORT_B);

	int nowx;
	int nowy;
	int goalx;
	int goaly;
	int dirx;
	int diry;

	if(tracer.getSelect()=='L') {
		nowx = -1;
		nowy = 2;
		goalx = 3;
		goaly = 2;
		dirx = 1;
		diry = 0;
	} else {
		nowx = 4;
		nowy = 2;
		goalx = 0;
		goaly = 1;
		dirx = -1;
		diry = 0;
	}

routeLastfix(turning, turningLength);
nowRoute = turning;
routeLength = turningLength;

int nowtime = 0;
int timeSum = 0;
uint timeSt;

fp=fopen("blockbingo_nowpos.log","w");
  // 難所whileループ部分
  while(true) {

	if(mode == 0) tracer.run(light_grey, 25);//20);
    // モード変わったときにモータのパワーをセットする
    if(change) {
      switch(mode) {
		  // ライントレースのみ
        case 0:
          leftWheel.setCount(0);
          rightWheel.setCount(0);
		  timeSt = clock.now();
          //leftWheel.setPWM(myMotorPower);
          //rightWheel.setPWM(myMotorPower);
        break;
		// カラーセンサ判定のみ
		case 10:
          leftWheel.setCount(0);
          rightWheel.setCount(0);
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(myMotorPower);
		break;
        case 1:
		switch(nowRoute[turningNow]) {
			case 0:
			timeSt = clock.now();
			case 1:
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(0);
			break;
			case 2:
			timeSt = clock.now();
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(myMotorPower);
			break;
			case 4:
			timeSt = clock.now();
			case 3:
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
		switch(nowRoute[turningNow]) {
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
		// 180
		case 5:
          leftWheel.setPWM(-myMotorPower);
          rightWheel.setPWM(myMotorPower);
          leftWheel.setCount(0);
          rightWheel.setCount(0);
		break;
		case 11:
          leftWheel.setCount(0);
          rightWheel.setCount(0);
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(myMotorPower);
		break;
		case 12:
          leftWheel.setCount(0);
          rightWheel.setCount(0);
          leftWheel.setPWM(-myMotorPower);
          rightWheel.setPWM(myMotorPower);
		break;
      }
      change = false;
    }
		//if(tmpCount%500==0)ev3_speaker_play_tone(100,30);
    int angle;
	float distance;
    switch(mode) {
	  // ライントレースのみ
      case 0:
		// if(advanceChk(12.0f) == 1) {
        //   ev3_speaker_play_tone(50,30);
        //   mode = 10;
        //   change = true;
		// }

		colorSensor.getRawColor(stageColor);
        if(rawColortoColorNumber(stageColor, rgbCoef, redHSV, yellowHSV, greenHSV, blueHSV) != COLOR_NONE) {
		  fprintf(fp, "case 0 (ライントレース)\n");
		  fprintf(fp, "st now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
          ev3_speaker_play_tone(50,30);
          mode = 1;
          change = true;
		  nowx += dirx;
		  nowy += diry;
		  	char str[64];
		  	//snprintf(str,64,"now = (%d, %d)", nowx, nowy);
			//ev3_lcd_draw_string(str,0,0);
		  fprintf(fp, "en now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
			timeSum += clock.now() - timeSt;
			nowtime++;
		}
		break;
		case 10:

		balancingAdvanceChk(distance, 1, myMotorPower);
		colorSensor.getRawColor(stageColor);
        if(rawColortoColorNumber(stageColor, rgbCoef, redHSV, yellowHSV, greenHSV, blueHSV) != COLOR_NONE) {
			
        //if(false) {
        //if(true) {
          ev3_speaker_play_tone(100,30);
          mode = 1;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(buttonEnable) {
              if (touchSensor.isPressed() == true ){
                break;
              }
          }
          //leftWheel.setPWM(myMotorPower);
          //rightWheel.setPWM(myMotorPower);
        }
      break;
      case 1:
	  switch(nowRoute[turningNow]){
		case 0:
		case 1:
		angle = nowRoute[turningNow]==0? 90: 45;
		//if(blockSetPhase==3) angle *= -1;
		if(tracer.getSelect()=='L' && nowRoute[turningNow]==0) angle += ANGLE_ASSIST;
        if(monoWheelRotChk(angle, 1) == 1) {
          ev3_speaker_play_tone(300,30);
          //mode = 0;
  		  mode = nowRoute[turningNow]==1? 2: 0;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(buttonEnable) {
              if (touchSensor.isPressed() == true){
                break;
              }
          }
   		  if(nowRoute[turningNow]==0) {
			fprintf(fp, "case 1 case 0(曲がり)\n");
			fprintf(fp, "st now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
			rotDir(&dirx, &diry, nowRoute[turningNow]);
			turningNow++;// = (turningNow + 1) % routeLength;
			fprintf(fp, "en now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
			timeSum += clock.now() - timeSt;
			nowtime++;
		  }
		  //if(blockSetPhase == 0 && nowRoute[turningNow] == 1) blockSetPhase = 1;
		  //if(blockSetPhase == 3) blockSetPhase = 0;
		  tracer.setSelect('R');
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(0);
        }
		break;
		case 2:
		distance = 10.0f;
		//if(blockSetPhase==2) distance*= -1;
		if(balancingAdvanceChk(distance, 1, myMotorPower) == 1) {
          ev3_speaker_play_tone(300,30);
          mode = 0;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(buttonEnable) {
              if (touchSensor.isPressed() == true ){
                break;
              }
          }
   		  turningNow++;// = (turningNow + 1) % routeLength;
    	  //if(blockSetPhase==1 || blockSetPhase==2) blockSetPhase++;
			timeSum += clock.now() - timeSt;
			nowtime++;
          leftWheel.setPWM(myMotorPower);
          rightWheel.setPWM(myMotorPower);
		}
		break;
		case 3:
		case 4:
		angle = nowRoute[turningNow]==3? 45: 90;
		//if(blockSetPhase==3) angle *= -1;

		if(tracer.getSelect()=='R' && nowRoute[turningNow]!=3) angle += ANGLE_ASSIST;
        if(monoWheelRotChk(angle, 0) == 1) {
          ev3_speaker_play_tone(300,30);
          //mode = 0;
		  mode = nowRoute[turningNow]==3? 2: 0;
          change = true;
          leftWheel.setPWM(0);
          rightWheel.setPWM(0);
          while(buttonEnable) {
              if (touchSensor.isPressed() == true ){
                break;
              }
          }
   		  if(nowRoute[turningNow]==4) {
			fprintf(fp, "case 1 case 4(曲がり)\n");
			fprintf(fp, "st now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
			rotDir(&dirx, &diry, nowRoute[turningNow]);
				 turningNow++;// = (turningNow + 1) % routeLength;
			fprintf(fp, "en now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
			timeSum += clock.now() - timeSt;
			nowtime++;
		  }
		  //if(blockSetPhase == 0 && nowRoute[turningNow] == 1) blockSetPhase = 1;
		  //if(blockSetPhase == 3) blockSetPhase = 0;
		  tracer.setSelect('L');
          leftWheel.setPWM(0);
          rightWheel.setPWM(myMotorPower);
        }
		break;
		// 180
		case 5:
        change = true;
		mode = 11;
		break;
	  }
      break;
	  // ブロサ前進
	  case 2:
		//if(advanceChk(20.0) == 1) {
		if(balancingAdvanceChk(20.0, 1, myMotorPower) == 1) {
          ev3_speaker_play_tone(400,30);
			mode = 3;
          change = true;
		}
	  break;
	  // ブロサ後退
 	  case 3:
		//if(advanceChk(-20.0) == 1) {
		if(balancingAdvanceChk(20.0, 1, -myMotorPower / 2) == 1) {
          ev3_speaker_play_tone(550,30);
			mode = 4;
          change = true;
		}
	  break;
	  // 逆ターン
 	  case 4:
			switch(nowRoute[turningNow]){
				case 1:
					angle = (tracer.getSelect()=='R')? 35: 55;
					if(monoWheelRotChk(angle, 1) == 1) {
			          ev3_speaker_play_tone(800,30);
						turningNow++;// = (turningNow + 1) % routeLength;
				    	change = true;
						mode = 1;
					}
				break;
				case 3:
					angle = (tracer.getSelect()=='L')? 35: 55;
					if(monoWheelRotChk(angle, 0) == 1) {
			          ev3_speaker_play_tone(800,30);
						turningNow++;// = (turningNow + 1) % routeLength;
          				change = true;
						mode = 1;
					}
				break;
			}
		
	  break;
	  // 180用若干前進
	  case 11:
		if(balancingAdvanceChk(2.5, 1, myMotorPower) == 1) {
          ev3_speaker_play_tone(400,30);
			mode = 12;
          change = true;
		}
	  break;
	  // 180実体
	  case 12:
			distance = WIDTH_RADIUS * M_PI;
			//if(balancingAdvanceChk(distance, 2, myMotorPower) == 1) {
			if(balancingAdvanceChk2(distance, 2, myMotorPower / 4 * 3, 1.25f) == 1) {
				fprintf(fp, "case 1 case 4(曲がり)\n");
				fprintf(fp, "st now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
				mode = 0;
				if(tracer.getSelect()=='L') tracer.setSelect('R');
				else tracer.setSelect('L');
				rotDir(&dirx, &diry, nowRoute[turningNow]);
				turningNow++;// = (turningNow + 1) % routeLength;
				fprintf(fp, "en now = (%d, %d) ,dir = (%d, %d)\n", nowx, nowy, dirx, diry);
			}
	  break;
    }
    clock.sleep(4);
	timecount += 4;
	// if(timecount >= 1000000 || turningNow == routeLength) {
	if(sector == 0 && change && mode == 0 && nowtime != 0) {
		float timeAve = (float)timeSum / (float)nowtime;
		goGarage(toGarage, toGarageLength, nowx, nowy, goalx, goaly, dirx, diry);
		// 115
		if(toGarageLength * timeAve + clock.now() >= 1000 * 25 || turningNow == routeLength || ev3_button_is_pressed(DOWN_BUTTON)) {
			fprintf(fp, "timeAve = %f\n", timeAve);
			fprintf(fp, "timeSum = %d\n", timeSum);
			fprintf(fp, "nowtime = %d\n", nowtime);

			ev3_speaker_play_tone(4000,300);
			nowRoute = toGarage;
			routeLength = toGarageLength;
			turningNow = 0;
			sector = 1;
		}
	}

	// if(sector == 0 && mode == 0 && (turningNow == routeLength || ev3_button_is_pressed(DOWN_BUTTON) || clock.now() + 1000 * 105 >= 1000 * 110) ) {
	// 	ev3_speaker_play_tone(4000,300);
	// 	goGarage(toGarage, toGarageLength, nowx, nowy, goalx, goaly, dirx, diry);
	// 	// for(int i = 0 ; i < toGarageLength ; i++){
	// 	// 	fprintf(fp, "toGarage[%d] = %d\n", i, toGarage[i]);
	// 	// }
	// 	nowRoute = toGarage;
	// 	routeLength = toGarageLength;
	// 	turningNow = 0;
	// 	sector = 1;
	// }
	if(sector == 1 && mode == 0 && turningNow == routeLength) {
        leftWheel.setPWM(0);
        rightWheel.setPWM(0);
		break;
	}
    tmpCount++;
  }
  fclose(fp);
  }