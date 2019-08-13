#include "Motor.h"
#include "ColorSensor.h"
#include "util.h"

using namespace ev3api;

class Tracer {
public:
  Tracer();
  void run(int target, int flg);
  void init();
  void terminate();

private:
  Motor leftWheel;
  Motor rightWheel;
  ColorSensor colorSensor;
  const int8_t mThreshold = 20;
//  const int8_t pwm = (Motor::PWM_MAX) / 6;
  const float integral = 0;
  int diff[2] = {};
};
