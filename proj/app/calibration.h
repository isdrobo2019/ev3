
void stageSelect(char& course) {
	  Clock clock;
	char str[64];
	    // ステージセレクト
	while(1){
		//左ボタンを押下
		if (ev3_button_is_pressed(LEFT_BUTTON)){
			course = 'L';
			snprintf(str,64,"course:[%c]", course);
			ev3_lcd_draw_string(str,0,30);
			clock.sleep(500);
		}
		
		//右ボタンを押下
		if (ev3_button_is_pressed(RIGHT_BUTTON)){
			course = 'R';
			snprintf(str,64,"course:[%c]", course);
			ev3_lcd_draw_string(str,0,30);
			clock.sleep(500);
		}
		
		//中央ボタンを押下
		if (ev3_button_is_pressed(ENTER_BUTTON) && course != '\0'){
			
			snprintf(str,64,"select_course:[%c]", course);
			ev3_lcd_draw_string(str,0,30);
	
			clock.sleep(500);
			break;
		}
	}
}
// カラーキャリブレーション
void colorCalibration(rgb_raw_t& stageWhite, int& light_black, int& light_white, int& light_target, double* rgbCoef) {

  int calibrationMode = 0;
  light_black = -1;
  light_white = -1;
  light_target = -1;
  stageWhite.r = 0;
  stageWhite.g = 0;
  stageWhite.b = 0;
  Clock clock;
  TouchSensor touchSensor(PORT_1);
  ColorSensor colorSensor(PORT_2);
  char str[64];

  while(true){
    if (touchSensor.isPressed()) {
      switch(calibrationMode) {
        case 0:
          ev3_speaker_play_tone(100,30);
    			light_black = colorSensor.getBrightness();
    			clock.sleep(500);
        break;      
        case 1:
          ev3_speaker_play_tone(100,30);
    			light_white = colorSensor.getBrightness();
    			light_target = (light_black + light_white) / 2;
    			clock.sleep(500);
        break;      
        case 2:
          ev3_speaker_play_tone(100,30);
    			colorSensor.getRawColor(stageWhite);
				normalizeRGB(stageWhite, rgbCoef);
    			clock.sleep(500);
        break;      
      }
      calibrationMode++;
    }
    if(calibrationMode == 4) break;
    if (ev3_button_is_pressed(LEFT_BUTTON)) {
      calibrationMode = 0;
      light_black = -1;
      light_white = -1;
      light_target = -1;
	  stageWhite.r = 0;
	  stageWhite.g = 0;
	  stageWhite.b = 0;
    }
	switch(calibrationMode) {
		case 1:
			snprintf(str,64,"black :[%d]",light_black);
			ev3_lcd_draw_string(str,0,10);
		case 2:
			snprintf(str,64,"white :[%d]",light_white);
			ev3_lcd_draw_string(str,0,20);
			snprintf(str,64,"target:[%d]",light_target);
			ev3_lcd_draw_string(str,0,30);
		case 3:
			snprintf(str,64,"r     :[%u]",stageWhite.r);
			ev3_lcd_draw_string(str,0,40);
			snprintf(str,64,"g     :[%u]",stageWhite.g);
			ev3_lcd_draw_string(str,0,50);
			snprintf(str,64,"b     :[%u]",stageWhite.b);
			ev3_lcd_draw_string(str,0,60);

			snprintf(str,64,"rCoef :[%f]",rgbCoef[0]);
			ev3_lcd_draw_string(str,0,70);
			snprintf(str,64,"gCoef :[%f]",rgbCoef[1]);
			ev3_lcd_draw_string(str,0,80);
			snprintf(str,64,"bCoef :[%f]",rgbCoef[2]);
			ev3_lcd_draw_string(str,0,90);
			break;
			}
  }
}