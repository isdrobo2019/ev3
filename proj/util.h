#ifndef MY_UNTL_H_
#define MY_UNTL_H_

#include "ev3api.h"

extern void init_f(const char *str);
extern void msg_f(const char *str, int32_t line);
void msg(int32_t line, char* format, char* args, ...);
double getMax(int len, double val, ...);
double getMin(int len, double val, ...);
colorid_t rawColortoColorNumber(rgb_raw_t source, double* rgbCoef);
void rgb2hsv(int r, int g, int b);
void rgb2hsv(double r, double g, double b, double* h, double* s, double* v);
void normalizeRGB(rgb_raw_t source, double* rgbCoef);
colorid_t HSVtoColorNumber(double h, double s, double v);

#endif  // MY_UNTL_H_
