#include "util.h"
#include <math.h>
#include <stdarg.h>

#define SATURATION_THRESHOLD 80.0
#define SATURATION_MARGIN 15.0
#define VALUE_THRESHOLD 80.0

double SATURATION_MIN = 20.0;
// 演習用のユーティリティ

// 初期処理用
void init_f(const char *str) {
  // フォントの設定と0行目の表示
  ev3_lcd_set_font(EV3_FONT_SMALL);
  ev3_lcd_draw_string(str, 0, 0);
}

/**
 * 行単位で引数の文字列を表示
 * @param str 表示する文字列
 * @param line 20ドットごとの行番号（1から5）
 */
void msg_f(const char *str, int32_t line) {
  const int8_t line_height = 20;
  ev3_lcd_fill_rect(0, line * line_height, EV3_LCD_WIDTH, line_height, EV3_LCD_WHITE);
  ev3_lcd_draw_string(str, 0, line * line_height);
}

void msg(int32_t line, char* format, char* args, ...) {
  va_list ap;
  char str[256];
  vsnprintf(str, 256, format, ap);

  msg_f(str, line);
}

double getMax(int len, ...) {
    int i;
    double _max, num;
    va_list ap;
    va_start(ap, len);

    _max = va_arg(ap, double);
    for (i = 1; i < len; i++) {
        num = va_arg(ap, double);
        if (num > _max)
            _max = num;
    }
    va_end(ap);
    return _max;
}

double getMin(int len, ...) {
    int i;
    double _min, num;
    va_list ap;
    va_start(ap, len);
    FILE *tmpfp;

    if(len == 4) {
      tmpfp = fopen("getmin_test.log","w");
    fprintf(tmpfp, "len = %d\n", len);
    }
    _min = va_arg(ap, double);
    if(len == 4) fprintf(tmpfp, "val[0] = %f\n\n", _min);
    
    for (i = 1; i < len; i++) {
        num = va_arg(ap, double);
        if(len == 4) fprintf(tmpfp, "val[%d] = %f\n", i, num);
        if (num < _min)
            _min = num;
    }
    if(len == 4) fclose(tmpfp);
    va_end(ap);
    return _min;
}

colorid_t rawColortoColorNumber(rgb_raw_t source, double* rgbCoef, double* redHSV, double* yellowHSV, double* greenHSV, double* blueHSV) {
  double realR = (double)source.r * rgbCoef[0];
  double realG = (double)source.g * rgbCoef[1];
  double realB = (double)source.b * rgbCoef[2];
  double H, S, V;
char str[64];
			snprintf(str,64,"RGB :[%3u, %3u, %3u]", source.r, source.g, source.b);
			//ev3_lcd_draw_string(str,0,100);
			snprintf(str,64,"realRGB :[%3.2f, %3.2f, %3.2f]",realR, realG, realB);
			//ev3_lcd_draw_string(str,0,110);
  rgb2hsv(realR, realG, realB, &H, &S, &V);

  //HSVtoColorNumber2(H, S, V, redHSV, yellowHSV, greenHSV, blueHSV);
  HSVtoColorNumber(H, S, V);
}

void rawColortoHSV(rgb_raw_t source, double* rgbCoef, double *HSV) {
  double realR = (double)source.r * rgbCoef[0];
  double realG = (double)source.g * rgbCoef[1];
  double realB = (double)source.b * rgbCoef[2];
  //double H, S, V;
char str[64];
			snprintf(str,64,"RGB :[%3u, %3u, %3u]", source.r, source.g, source.b);
			ev3_lcd_draw_string(str,0,100);
			snprintf(str,64,"realRGB :[%3.2f, %3.2f, %3.2f]",realR, realG, realB);
			ev3_lcd_draw_string(str,0,110);
  rgb2hsv(realR, realG, realB, &HSV[0], &HSV[1], &HSV[2]);
}

void rgb2hsv(int r, int g, int b){
  // rgb2hsv((float)r, (float)g, (float)b);
}

/**
 h:0~360 s:0~100 v:0~100
 */
void rgb2hsv(double r, double g, double b, double* h, double* s, double* v){
  double maxVal = getMax(3, r, g, b);
  double minVal = getMin(3, r, g, b);

if(maxVal >= 255.0) {
  r*=255.0/maxVal;
  g*=255.0/maxVal;
  b*=255.0/maxVal;
}

char str[64];
			snprintf(str,64,"maxmin :[%3.2f, %3.2f]",maxVal, minVal);
			ev3_lcd_draw_string(str,0,0);

  *h = 0.0;
  *s = 0.0;
  *v = 0.0;

  if( maxVal != minVal){
    if(maxVal == r) *h = 60.0 * (g - b) / (maxVal - minVal);
    if(maxVal == g) *h = 60.0 * (b - r) / (maxVal - minVal) + 120.0;
    if(maxVal == b) *h = 60.0 * (r - g) / (maxVal - minVal) + 240.0;
    
  } else {
    *h = 0.0;
  }

  if(*h < 0.0){
    *h += 360.0;
  }

    *s = (maxVal - minVal) / maxVal;
    *v=maxVal;

  *h = roundf(*h);
  *s = roundf(*s * 100.0);
  *v = roundf(*v / 255.0 * 100.0);

}

void normalizeRGB(rgb_raw_t source, double* rgbCoef) {
  //double rgbmax = getMax((double)source.r, (double)source.g, (double)source.b);
  
  // rgbCoef[0] = rgbmax / (double)source.r;
  // rgbCoef[1] = rgbmax / (double)source.g;
  // rgbCoef[2] = rgbmax / (double)source.b;
  rgbCoef[0] = 255.0 / (double)source.r;
  rgbCoef[1] = 255.0 / (double)source.g;
  rgbCoef[2] = 255.0 / (double)source.b;

}

void SaturationMin(double* redHSV, double* yellowHSV, double* greenHSV, double* blueHSV) {


  SATURATION_MIN = getMin(4, redHSV[1], yellowHSV[1], greenHSV[1], blueHSV[1]) - SATURATION_MARGIN;
if(SATURATION_MIN < 0.0) SATURATION_MIN = 0.0;

      char str[64];
			snprintf(str,64,"smin = %3.2f",SATURATION_MIN);
			ev3_lcd_draw_string(str,0,10);

}

colorid_t HSVtoColorNumber(double h, double s, double v) {

char str[64];
			snprintf(str,64,"hsv :[%3.2f, %3.2f, %3.2f]",h, s, v);
			ev3_lcd_draw_string(str,0,120);
			snprintf(str,64,"smin %3.2f", SATURATION_MIN);
			ev3_lcd_draw_string(str,0,130);

  // if(s < SATURATION_THRESHOLD) {
  if(s < SATURATION_MIN) {
      return COLOR_NONE;
    // if(v > VALUE_THRESHOLD) {
    //   return COLOR_NONE;
    // } else {
    //   return COLOR_NONE;
    // }
  }

  return COLOR_RED;
}

colorid_t HSVtoColorNumber2(double h, double s, double v, double* redHSV, double* yellowHSV, double* greenHSV, double* blueHSV) {
double dh = 15.0;
double ds = 15.0;
double dv = 15.0;

char str[64];
			snprintf(str,64,"hsv :[%3.2f, %3.2f, %3.2f]",h, s, v);
			ev3_lcd_draw_string(str,0,120);

  if(redHSV[0] - dh < h && h < redHSV[0] + dh &&
  redHSV[1] - ds < s && s < redHSV[1] + ds &&
  redHSV[2] - dv < v && v < redHSV[2] + dv
  ) {
    return COLOR_RED;
  }
  if(yellowHSV[0] - dh < h && h < yellowHSV[0] + dh &&
  yellowHSV[1] - ds < s && s < yellowHSV[1] + ds &&
  yellowHSV[2] - dv < v && v < yellowHSV[2] + dv
  ) {
    return COLOR_YELLOW;
  }
  if(greenHSV[0] - dh < h && h < greenHSV[0] + dh &&
  greenHSV[1] - ds < s && s < greenHSV[1] + ds &&
  greenHSV[2] - dv < v && v < greenHSV[2] + dv
  ) {
    return COLOR_GREEN;
  }
  if(blueHSV[0] - dh < h && h < blueHSV[0] + dh &&
  blueHSV[1] - ds < s && s < blueHSV[1] + ds &&
  blueHSV[2] - dv < v && v < blueHSV[2] + dv
  ) {
    return COLOR_BLUE;
  }
  return COLOR_NONE;
}

void rotDir(int* dirx, int* diry, int mode){

  int tmpx = *dirx;
  int tmpy = *diry;
  switch(mode) {
    case S_RIGHT:
      *dirx = tmpy * -1;
      *diry = tmpx;
    break;
    case S_LEFT:
      *dirx = tmpy;
      *diry = tmpx * -1;
    break;
    case S_BACK:
      *dirx *= -1;
      *diry *= -1;
    break;
  }
}
