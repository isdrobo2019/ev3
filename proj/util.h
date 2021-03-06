#ifndef MY_UNTL_H_
#define MY_UNTL_H_

#include "ev3api.h"
#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
#define S_FRONT 0
#define S_RIGHT 1
#define S_BACK 2
#define S_LEFT 3
#define S_FRONT_LEFT 4
#define S_FRONT_RIGHT 5
#define S_BACK_RIGHT 6
#define S_BACK_LEFT 7

extern void init_f(const char *str);
extern void msg_f(const char *str, int32_t line);
void msg(int32_t line, char* format, char* args, ...);
double getMax(int len, ...);
double getMin(int len, ...);
colorid_t rawColortoColorNumber(rgb_raw_t source, double* rgbCoef, double* redHSV, double* yellowHSV, double* greenHSV, double* blueHSV);
void rawColortoHSV(rgb_raw_t source, double* rgbCoef, double *HSV);
void rgb2hsv(int r, int g, int b);
void rgb2hsv(double r, double g, double b, double* h, double* s, double* v);
void normalizeRGB(rgb_raw_t source, double* rgbCoef);
void SaturationMin(double* redHSV, double* yellowHSV, double* greenHSV, double* blueHSV);
colorid_t HSVtoColorNumber(double h, double s, double v);
colorid_t HSVtoColorNumber2(double h, double s, double v, double* redHSV, double* yellowHSV, double* greenHSV, double* blueHSV);
void rotDir(int* dirx, int* diry, int mode);

#endif  // MY_UNTL_H_
