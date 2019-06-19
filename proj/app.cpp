#include "app.h"
#include "util.h"

#include "Motor.h"
#include "Clock.h"

using namespace ev3api;

/**
 * メインタスク
 */

void main_task(intptr_t unused) {
  ext_tsk();
}
