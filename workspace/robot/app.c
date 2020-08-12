#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
//#include "t_syslog.h"
//#include "syssvc/syslog.h"
#include "ev3api.h"
#include "robot.h"
#include "app.h"


static char lcdstr[100];
static long degree;
static rgb_raw_t rgb;
static uint8_t color;

void app_display_motor_readings() {
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    sprintf(lcdstr, "L: %ld", ev3_motor_get_counts(EV3_PORT_B));
	ev3_lcd_draw_string(lcdstr, 0, 0);
    sprintf(lcdstr, "R: %ld", ev3_motor_get_counts(EV3_PORT_C));
	ev3_lcd_draw_string(lcdstr, 0, 3);
    sprintf(lcdstr, "Move: %d  %f", robot_get_move_degree(), robot_get_move_distance());
	ev3_lcd_draw_string(lcdstr, 0, 5);
    sprintf(lcdstr, "Trip: %d  %f", robot_get_trip_degree(), robot_get_trip_distance());
	ev3_lcd_draw_string(lcdstr, 0, 7);
	
	ev3_speaker_play_tone(NOTE_E4, 200);
	while (true) {
		if (ev3_button_is_pressed(ENTER_BUTTON)) {
			ev3_speaker_play_tone(NOTE_C4, 200);
			while(ev3_button_is_pressed(ENTER_BUTTON));
			ev3_speaker_play_tone(NOTE_C5, 200);
			break;
		}
	}
}

static void print_motor_counts() {
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sprintf(lcdstr, "%lu   %lu", ev3_motor_get_counts(EV3_PORT_B), ev3_motor_get_counts(EV3_PORT_C));
	ev3_lcd_draw_string(lcdstr, 10, 10);

	ev3_speaker_play_tone(NOTE_E4, 200);
	while (true) {
		if (ev3_button_is_pressed(ENTER_BUTTON)) {
			ev3_speaker_play_tone(NOTE_C4, 200);
			while(ev3_button_is_pressed(ENTER_BUTTON));
			ev3_speaker_play_tone(NOTE_C5, 200);
			break;
		}
	}
}

void idle_task(intptr_t unused) {
    while(1) {
    	tslp_tsk(1000);
    }
}

void main_task(intptr_t unused) {
	SYSTIM start_time, stop1, stop2;
	ER ercd;

	robot_add_back_button_on_clicked();

	ev3_motor_config(EV3_PORT_B, LARGE_MOTOR);
	ev3_motor_config(EV3_PORT_C, LARGE_MOTOR);
	ev3_sensor_config(EV3_PORT_2, COLOR_SENSOR);
	//ev3_sensor_config(EV3_PORT_4, HT_NXT_COLOR_SENSOR);
	ev3_lcd_set_font(EV3_FONT_MEDIUM);
	ev3_speaker_set_volume(100);

	ev3_motor_reset_counts(EV3_PORT_B);
	ev3_motor_reset_counts(EV3_PORT_C);
	ev3_color_sensor_get_reflect(EV3_PORT_2);
	//ht_nxt_color_sensor_measure_rgb(EV3_PORT_4, &rgb);

	ercd = get_tim(&start_time);
	assert(ercd == E_OK);


	// line follower

	
	/*
	// turn robot in arc, r = 30
	int i = 1;
	for (i = 1; i <= 8; i++) {
		ev3_motor_reset_counts(EV3_PORT_B);
		ev3_motor_reset_counts(EV3_PORT_C);
		ev3_motor_set_power(EV3_PORT_B, (30.0+5.4)*30/(30.0-5.4));
		ev3_motor_set_power(EV3_PORT_C, 30);
		int target = 720;
		while (ev3_motor_get_counts(EV3_PORT_C) < target) {
			tslp_tsk(10);
		}
		ev3_motor_stop(EV3_PORT_B, true);
		ev3_motor_stop(EV3_PORT_C, true);
		print_motor_counts();
	}
	*/

	/*
	// ev3_motor_reset_counts() stop ev3_motor_steer() immediately
	ev3_motor_steer(EV3_PORT_B, EV3_PORT_C, 1440, 30, 0);
	tslp_tsk(500);
	ev3_speaker_play_tone(NOTE_C4, 300);
	ev3_motor_reset_counts(EV3_PORT_B);
	tslp_tsk(500);
	ev3_speaker_play_tone(NOTE_C5, 300);
	ev3_motor_reset_counts(EV3_PORT_C);
	tslp_tsk(10000);
	ev3_motor_stop(EV3_PORT_B, true);
	ev3_motor_stop(EV3_PORT_C, true);

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sprintf(lcdstr, "%lu    %lu", ev3_motor_get_counts(EV3_PORT_B), ev3_motor_get_counts(EV3_PORT_C));
	ev3_lcd_draw_string(lcdstr, 10, 10);
	*/

	/*
	// after ev3_motor_reset_counts(), stop ev3_motor_rotate() continues till the set position
	ev3_motor_rotate(EV3_PORT_B, 1440, 30, false);
	tslp_tsk(3000);
	ev3_speaker_play_tone(NOTE_C4, 300);
	ev3_motor_reset_counts(EV3_PORT_B);
	tslp_tsk(10000);
	ev3_motor_stop(EV3_PORT_B, true);

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sprintf(lcdstr, "%lu    %lu", ev3_motor_get_counts(EV3_PORT_B), ev3_motor_get_counts(EV3_PORT_C));
	ev3_lcd_draw_string(lcdstr, 10, 10);
	*/

	/*
	// ev3_motor_steer
	ev3_motor_steer(EV3_PORT_B, EV3_PORT_C, 30, 0);
	while (ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C) < 1440) {
		tslp_tsk(1);
	}
	ev3_motor_stop(EV3_PORT_B, true);
	ev3_motor_stop(EV3_PORT_C, true);

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sprintf(lcdstr, "%lu    %lu", ev3_motor_get_counts(EV3_PORT_B), ev3_motor_get_counts(EV3_PORT_C));
	ev3_lcd_draw_string(lcdstr, 10, 10);
	*/

	/*
	// ev3_set_power
	ev3_motor_set_power(EV3_PORT_B, 30);
	ev3_motor_set_power(EV3_PORT_C, 30);
	while (ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C) < 1440) {
		tslp_tsk(1);
	}
	ev3_motor_stop(EV3_PORT_B, true);
	ev3_motor_stop(EV3_PORT_C, true);

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sprintf(lcdstr, "%lu    %lu", ev3_motor_get_counts(EV3_PORT_B), ev3_motor_get_counts(EV3_PORT_C));
	ev3_lcd_draw_string(lcdstr, 10, 10);
	*/

	/*
	// ev3_motor_rotate
	ev3_motor_rotate(EV3_PORT_B, 730, 30, false);
	ev3_motor_rotate(EV3_PORT_C, 730, 30, false);
	while (true) {
		if (ev3_motor_get_counts(EV3_PORT_B) >= 730) {
			get_tim(&stop1);
		}
		if (ev3_motor_get_counts(EV3_PORT_C) >= 730) {
			get_tim(&stop2);
		}
		if (ev3_motor_get_counts(EV3_PORT_B) >= 730 && ev3_motor_get_counts(EV3_PORT_C) >= 730) {
			break;
		}
		tslp_tsk(1);
	}
	ev3_motor_stop(EV3_PORT_B, true);
	ev3_motor_stop(EV3_PORT_C, true);

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sprintf(lcdstr, "%lu %lu", ev3_motor_get_counts(EV3_PORT_B), ev3_motor_get_counts(EV3_PORT_C));
	ev3_lcd_draw_string(lcdstr, 0, 0);
	*/

	/*
	//ev3_color_sensor_get_color(EV3_PORT_2);
	//ev3_color_sensor_get_reflect(EV3_PORT_2);
	//ev3_color_sensor_get_ambient(EV3_PORT_2);
	//ev3_color_sensor_get_rgb_raw(EV3_PORT_2, &rgb);
	//ht_nxt_color_sensor_measure_color(EV3_PORT_4, &color);
	ht_nxt_color_sensor_measure_rgb(EV3_PORT_4, &rgb);

	ercd = get_tim(&stop1);
	assert(ercd == E_OK);

	// method speed
	for (int i = 0; i < 100000; i++) {
		//if ((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_B))/2 > 300) {
		//	break;
		//}
		//ev3_motor_set_power(EV3_PORT_B, 30);
		//ev3_motor_reset_counts(EV3_PORT_B);
		//ev3_motor_steer(EV3_PORT_B, EV3_PORT_C, 30, 0);
		//ev3_motor_stop(EV3_PORT_B, false);

		//ev3_color_sensor_get_color(EV3_PORT_2);
		//ev3_color_sensor_get_reflect(EV3_PORT_2);
		//ev3_color_sensor_get_ambient(EV3_PORT_2);
		//ev3_color_sensor_get_rgb_raw(EV3_PORT_2, &rgb);
		//ht_nxt_color_sensor_measure_color(EV3_PORT_4, &color);
		ht_nxt_color_sensor_measure_rgb(EV3_PORT_4, &rgb);
	} 
	ercd = get_tim(&stop2);
	assert(ercd == E_OK);

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sprintf(lcdstr, "%lu    %lu", (stop1 - start_time), (stop2 - stop1));
	ev3_lcd_draw_string(lcdstr, 10, 10);
	*/

	/*
	// ev3_motor_rotate() - speed setting vs. speed output
	for (int i = 1; i < 11;i++) {
		ercd = get_tim(&start_time);
		assert(ercd == E_OK);

		ev3_motor_rotate(EV3_PORT_B, 360, 10 * i, true);

		ercd = get_tim(&stop2);
		assert(ercd == E_OK);

		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
		char lcdstr[100];
		sprintf(lcdstr, "L: %ld    t: %ld\n", ev3_motor_get_counts(EV3_PORT_B), (stop2 - start_time));
		ev3_lcd_draw_string(lcdstr, 0, i*10);

		tslp_tsk(10000);
	}
	*/

	/*
    FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);

    // Start task for printing message while idle
	act_tsk(IDLE_TASK);

	sus_tsk(IDLE_TASK);
	fprintf(bt, "fprintf to bt\n");
	rsm_tsk(IDLE_TASK);
	*/
	
	/*
	while (!ev3_bluetooth_is_connected()) {
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
		tslp_tsk(300);
		ev3_lcd_draw_string((char*)"Waiting BT...", 0, 0);
		tslp_tsk(1000);
	}

	FILE *fptr = ev3_serial_open_file(EV3_SERIAL_BT);
	fprintf(fptr, "%s", "a message from ev3rt via bluetooth");
	fclose(fptr);
	*/

	/*
	setlogmask (LOG_UPTO (LOG_DEBUG));

	openlog ("robot.log", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

	syslog (LOG_DEBUG, "LOG_DEBUG");
	syslog (LOG_NOTICE, "LOG_NOTICE");
	syslog (LOG_INFO, "LOG_INFO");

	closelog ();
	*/

  	/*
	// leep spped
	FILE* fp = fopen("/ev3rt/log/robot.log", "w");
	char lcdstr[100];
	sprintf(lcdstr, "%d\t%d\n", 0, CLOCKS_PER_SEC);
	fputs(lcdstr, fp);

	SYSTIM start_time;
	ER ercd = get_tim(&start_time);
	assert(ercd == E_OK);

	int i = 0;
	while (i++ < 1000) {
		SYSTIM stop2;
		ER ercd = get_tim(&stop2);
		assert(ercd == E_OK);
		sprintf(lcdstr, "%d\t%d\n", i, (stop2 - start_time));
		fputs(lcdstr, fp);
	} 
	fclose(fp);

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	char lcdstr[100];
    sprintf(lcdstr, "L: %ld", ev3_motor_get_counts(EV3_PORT_B));
	ev3_lcd_draw_string(lcdstr, 0, 0);
	*/

	//syslog(LOG_NOTICE, "LOG_NOTICE");
	//syslog(LOG_ERROR, "LOG_ERROR");

	/*
	robot_move(50, NEITHER_SIDE);
	
	robot_reset_trip();
	ev3_motor_reset_counts(EV3_PORT_B);
	ev3_motor_reset_counts(EV3_PORT_C);
	app_display_motor_readings();
	*/

	tslp_tsk(10000);
	exit(0);
}


