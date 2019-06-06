#include "app.h"
#include "util.h"

#include "Motor.h"
#include "Clock.h"

using namespace ev3api;

/**
 * メインタスク
 */
// tag::main_task_1[]
void main_task(intptr_t unused) {

  float PI = 3.1415f;
  float radius = 5.0f;
  float position = 40.0f;
  int reviseL = 0;
  int reviseR = 0;
  Motor leftWheel(PORT_C);
  Motor rightWheel(PORT_B);
  Clock clock;

  float balancer = 0;
  const int8_t pwm = (Motor::PWM_MAX)/20;
  const uint32_t duration = 2000;

  init_f(__FILE__);
  char str[64];

  leftWheel.setCount(0);
  rightWheel.setCount(0);
  while(1) {
    leftWheel.setPWM(pwm - (int)balancer);
    rightWheel.setPWM(pwm + (int)balancer);
    clock.sleep(unused);
  	
  	float leftTravel = leftWheel.getCount() * 2.0f * radius * PI / 360.0f;
  	float rightTravel = rightWheel.getCount() * 2.0f * radius * PI / 360.0f;
  	balancer = leftTravel - rightTravel;
  	if(balancer>0.0f){
  		balancer=1.0f;
  		reviseR++;
  	}
  	if(balancer<0.0f){
  		balancer=-1.0f;
  		reviseL++;
  	}
  	if((leftTravel + rightTravel) / 2.0f > position)break;
  	// snprintf(str,64,"leftTravel = %f, revise = %d", leftTravel, reviseL);//leftWheel.getCount());
  	snprintf(str,64,"%f, %d", leftTravel, reviseL);//leftWheel.getCount());
  	ev3_lcd_draw_string(str,0,20);
  	// snprintf(str,64,"rightTravel = %f, revise = %d", rightTravel, reviseR);//rightWheel.getCount());
  	snprintf(str,64,"%f, %d", rightTravel, reviseR);//rightWheel.getCount());
  	ev3_lcd_draw_string(str,0,40);

    // 左ボタンを長押し、それを捕捉する
    if (ev3_button_is_pressed(LEFT_BUTTON)) {
      break;
    }
  }

  //msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
  while(ev3_button_is_pressed(LEFT_BUTTON)) {
    ;
  }

  ext_tsk();
}
// end::main_task_2[]
