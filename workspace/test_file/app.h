#include "target_test.h"

/*
 *  各タスクの優先度の定義
 */
#ifndef _TEST_FILE_H_
#define _TEST_FILE_H_

#define BLUE_STREET 0;
#define GREEN_STREET 1;
#define YELLOW_STREET 2;
#define RED_STREET 3;

#define REMOVESNOW 0;
#define BLUEMATERIAL 1;
#define BLACKMATERIAL 2;

typedef struct {
	int street;
	int section;
	int distance;
	int dash;
	int facing;
} position;

static tasks[4] = {0, 0, 0, 0};

extern void	main_task(intptr_t exinf);
#endif /* TOPPERS_MACRO_ONLY */
