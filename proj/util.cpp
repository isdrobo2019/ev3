#include "util.h"
#include <stdarg.h>
#include <math.h>

// 演習用のユーティリティ

// 初期処理用
void init_f(const char *str) {
  // フォントの設定と0行目の表示
  ev3_lcd_set_font(EV3_FONT_MEDIUM);
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
  // va_start(ap, count);
  // // char** str;

  // str = new char*[count];

  // for(int i = 0; i < count; i++){
  //     str[i] = va_arg(ap, char*);
  // }
  char str[256];
  vsnprintf(str, 256, format, ap);

  msg_f(str, line);
//   va_end(ap);
}

double getMax(double len, ...) {
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

double getMin(double len, ...) {
    int i;
    double _min, num;
    va_list ap;
    va_start(ap, len);

    _min = va_arg(ap, double);
    for (i = 1; i < len; i++) {
        num = va_arg(ap, double);
        if (num < _min)
            _min = num;
    }
    va_end(ap);
    return _min;
}

void rgb2hsv(int r, int g, int b){
  rgb2hsv((float)r, (float)g, (float)b);
}

void rgb2hsv(float r, float g, float b, float* h, float* s, float* v){
  float maxVal = getMax(r, g, b);
  float minVal = getMin(r, g, b);

  *h = 0.0f;
  *s = 0.0f;
  *v = 0.0f;

  if( maxVal != minVal){
    if(maxVal == r) *h = 60.0f * (g - b) / (maxVal - minVal);
    if(maxVal == g) *h = 60.0f * (b - r) / (maxVal - minVal) + 120.0f;
    if(maxVal == b) *h = 60.0f * (r - g) / (maxVal - minVal) + 240.0f;
    
    *s = (maxVal - minVal) / maxVal;
  }

  if(*h < 0.0f){
    *h += 360.0f;
  }

  *h = roundf(*h);
  *s = roundf(*s * 100.0f);
  *v = roundf(*v / 255.0f * 100.0f);

}