#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

#define MAIN_PRIORITY    TMIN_APP_TPRI + 1
#define TRACER_PRIORITY  TMIN_APP_TPRI + 2

#ifndef STACK_SIZE
#define STACK_SIZE      4096
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void tracer_task(intptr_t exinf);
extern void tracer_cyc(intptr_t exinf);

extern int monoWheelRotChk(int degree, int leftRight);
extern int advanceN(float distance);
	
#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
