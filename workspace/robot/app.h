/*
 * Target dependent definition
 */
#include "target_test.h"

/*
 * Definition of priority of each task
 */
#define MAIN_PRIORITY 5 /* Main task priority, Must be higher than HIGH_PRIORITY */
#define HIGH_PRIORITY 9 /* Priority of tasks executed in parallel */
#define MID_PRIORITY 10
#define LOW_PRIORITY 11

/*
 * Definition of constants that may depend on the target
 */
#ifndef STACK_SIZE
#define STACK_SIZE 4096 /* task stack size */
#endif /* STACK_SIZE */

#ifndef LOOP_REF
#define LOOP_REF ULONG_C(1000000) /* Number of loops for speed measurement */
#endif /* LOOP_REF */

/*
 * Function prototype declaration
 */
#ifndef TOPPERS_MACRO_ONLY
extern void	task(intptr_t exinf);
extern void main_task(intptr_t exinf);
extern void idle_task(intptr_t exinf);
#endif /* TOPPERS_MACRO_ONLY */

