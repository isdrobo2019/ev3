#include "util.h"
#include <math.h>
#include <stdarg.h>

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

double getMax(int len, double val, ...) {
    int i;
    double _max, num;
    va_list ap;
    va_start(ap, val);

    _max = va_arg(ap, double);
    for (i = 1; i < len; i++) {
        num = va_arg(ap, double);
        if (num > _max)
            _max = num;
    }
    va_end(ap);
    return _max;
}

double getMin(int len, double val, ...) {
    int i;
    double _min, num;
    va_list ap;
    va_start(ap, val);

    _min = va_arg(ap, double);
    for (i = 1; i < len; i++) {
        num = va_arg(ap, double);
        if (num < _min)
            _min = num;
    }
    va_end(ap);
    return _min;
}

colorid_t rawColortoColorNumber(rgb_raw_t source, double* rgbCoef) {
  double realR = (double)source.r * rgbCoef[0];
  double realG = (double)source.g * rgbCoef[1];
  double realB = (double)source.b * rgbCoef[2];
  double H, S, V;
char str[64];
			snprintf(str,64,"RGB :[%3u, %3u, %3u]", source.r, source.g, source.b);
			ev3_lcd_draw_string(str,0,100);
			snprintf(str,64,"realRGB :[%3.2f, %3.2f, %3.2f]",realR, realG, realB);
			ev3_lcd_draw_string(str,0,110);
  rgb2hsv(realR, realG, realB, &H, &S, &V);

  HSVtoColorNumber(H, S, V);
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

colorid_t HSVtoColorNumber(double h, double s, double v) {

char str[64];
			snprintf(str,64,"hsv :[%3.2f, %3.2f, %3.2f]",h, s, v);
			ev3_lcd_draw_string(str,0,120);

  if(s < 30.0) {
    if(v > 50.0) {
      return COLOR_WHITE;
    } else {
      return COLOR_BLACK;
    }
  }

  return COLOR_NONE;
}