#include "target_test.h"

/*
 *  各タスクの優先度の定義
 */
#ifndef _TEST_FILE_H_
#define _TEST_FILE_H_

typedef struct {
	int street;
	int section;
	int distance;
	int dash;
	int facing;
} position;

extern void	main_task(intptr_t exinf);
#endif /* TOPPERS_MACRO_ONLY */
