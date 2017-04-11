/*
 * cli_sensor.c
 *
 *  Created on: Jun 22, 2014
 *      Author: liyixiao
 */

#include "ev3api.h"
#include "app.h"
#include "syssvc/serial.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>

static
const CliMenuEntry* select_sensor() {
	static const CliMenuEntry entry_tab[] = {
		{ .key = '1', .title = "Sensor port 1", .exinf = EV3_PORT_1 },
		{ .key = '2', .title = "Sensor port 2", .exinf = EV3_PORT_2 },
		{ .key = '3', .title = "Sensor port 3", .exinf = EV3_PORT_3 },
		{ .key = '4', .title = "Sensor port 4", .exinf = EV3_PORT_4 },
		{ .key = 'Q', .title = "Cancel", .exinf = -1 },
	};

	static const CliMenu climenu = {
		.title     = "Select Port",
		.entry_tab = entry_tab,
		.entry_num = sizeof(entry_tab) / sizeof(CliMenuEntry),
	};

	show_cli_menu(&climenu, 0, 0, MENU_FONT);

	return select_menu_entry(&climenu, 0, MENU_FONT_HEIGHT * 1, MENU_FONT);
}

#define VIEW_SENSOR(func) do { \
	while(1) { \
		{func;} \
		if(ev3_button_is_pressed(BACK_BUTTON)) { \
			while(ev3_button_is_pressed(BACK_BUTTON)); \
			break; \
		} \
	} \
} while(0)

void connect_sensor(intptr_t unused) {
	const CliMenuEntry* cme_port = NULL;
	while(cme_port == NULL) {
//		fio_clear_screen();
//		fprintf(fio, "--- Connect Sensor ---\n");
		cme_port = select_sensor();
	}

	if(cme_port->exinf == -1) return;

#define I2C_SENSOR (TNUM_SENSOR_TYPE+1)

	static const CliMenuEntry entry_tab[] = {
		{ .key = '1', .title = "Ultrasonic", .exinf = ULTRASONIC_SENSOR },
		{ .key = '2', .title = "Gyro sensor", .exinf = GYRO_SENSOR },
		{ .key = '3', .title = "Touch sensor", .exinf = TOUCH_SENSOR },
		{ .key = '4', .title = "Color sensor", .exinf = COLOR_SENSOR },
		{ .key = '5', .title = "I2C sensor", .exinf = I2C_SENSOR },
		{ .key = '6', .title = "IR sensor", .exinf = INFRARED_SENSOR },
		{ .key = 'N', .title = "Not connected", .exinf = NONE_SENSOR },
		//{ .key = 'Q', .title = "Cancel", .exinf = -1 },
	};

	static const CliMenu climenu = {
		.title     = "Select Type",
		.entry_tab = entry_tab,
		.entry_num = sizeof(entry_tab) / sizeof(CliMenuEntry),
	};

	const CliMenuEntry* cme_type = NULL;
	while(cme_type == NULL) {
//		fio_clear_screen();
//		fprintf(fio, "--- Connect Sensor ---\n");
//		fprintf(fio, ">>> %s\n", cme_port->title);
		show_cli_menu(&climenu, 0, MENU_FONT_HEIGHT * 0, MENU_FONT);
		cme_type = select_menu_entry(&climenu, 0, MENU_FONT_HEIGHT * 1, MENU_FONT);

        // Sub-menu for I2C sensor
        if (cme_type->exinf == I2C_SENSOR) {
	        static const CliMenuEntry entry_tab[] = {
		        { .key = '1', .title = "HT Acc. sensor", .exinf = HT_NXT_ACCEL_SENSOR },
		        { .key = '2', .title = "NXT Temp. sensor", .exinf = NXT_TEMP_SENSOR },
		        { .key = 'Q', .title = "Cancel", .exinf = -1 },
	        };

	        static const CliMenu climenu = {
		        .title     = "Select Type",
		        .entry_tab = entry_tab,
		        .entry_num = sizeof(entry_tab) / sizeof(CliMenuEntry),
	        };

            cme_type = NULL;
            while (cme_type == NULL) {
		        show_cli_menu(&climenu, 0, MENU_FONT_HEIGHT * 0, MENU_FONT);
		        cme_type = select_menu_entry(&climenu, 0, MENU_FONT_HEIGHT * 1, MENU_FONT);
            }
            if (cme_type->exinf == -1) cme_type = NULL;
        }
	}

	if(cme_type->exinf == -1) return;

//	fprintf(fio, ">>> %s ==> %s\n", cme_port->title, cme_type->title);
	ev3_sensor_config(cme_port->exinf, cme_type->exinf);
}

typedef enum {
	US_DIST_CM = 0,
	US_DIST_IN = 1,
	US_LISTEN  = 2,
} ULTRASONIC_SENSOR_MODES;

static
void test_ultrasonic_sensor(sensor_port_t port) {
	static const CliMenuEntry entry_tab[] = {
		{ .key = '1', .title = "Distance (cm)", .exinf = US_DIST_CM },
		{ .key = '2', .title = "Listen", .exinf = US_LISTEN },
		{ .key = 'Q', .title = "Cancel", .exinf = -1 },
	};

	static const CliMenu climenu = {
		.title     = "Select Mode",
		.entry_tab = entry_tab,
		.entry_num = sizeof(entry_tab) / sizeof(CliMenuEntry),
	};

	bool_t change_mode = true;

	while(change_mode) {
		const CliMenuEntry* cme_mode = NULL;
		while (cme_mode == NULL) {
#if 1
			show_cli_menu(&climenu, 0, MENU_FONT_HEIGHT * 0, MENU_FONT);
			cme_mode = select_menu_entry(&climenu, 0, MENU_FONT_HEIGHT * 1, MENU_FONT);
#else
			fio_clear_screen();
			fprintf(fio, "--- Test Ultrasonic Sensor @ Port%d ---\n", port + 1);
			show_cli_menu(&climenu);
			cme_mode = select_menu_entry(&climenu);
#endif
		}

		if (cme_mode->exinf == -1)
			return;

		// Draw title
		char msgbuf[100];
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE); // Clear menu area
		ev3_lcd_draw_string("Test Sensor", (EV3_LCD_WIDTH - strlen("Test Sensor") * MENU_FONT_WIDTH) / 2, 0);
		sprintf(msgbuf, "Type: Ultrasonic");
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 1);
		sprintf(msgbuf, "Port: %c", '1' + port);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 2);
		switch (cme_mode->exinf) {
		case US_DIST_CM:
            ev3_lcd_draw_string("UP Button: Reset", 0, MENU_FONT_HEIGHT * 4);
			VIEW_SENSOR({
				int16_t val = ev3_ultrasonic_sensor_get_distance(port);
				sprintf(msgbuf, "Distance: %-3d cm", val);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
                // Reset sensor if UP button pressed
		        if(ev3_button_is_pressed(UP_BUTTON)) {
			        while(ev3_button_is_pressed(UP_BUTTON));
	                ev3_sensor_config(port, HT_NXT_ACCEL_SENSOR);
	                ev3_sensor_config(port, ULTRASONIC_SENSOR);
				    ev3_ultrasonic_sensor_get_distance(port); // Wait first data
		        }
				tslp_tsk(10);
			});
			break;
		case US_LISTEN:
			VIEW_SENSOR({
				bool_t val = ev3_ultrasonic_sensor_listen(port);
				sprintf(msgbuf, "Signal: %d", val);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				tslp_tsk(10);
			});
			break;
		default:
			assert(false);
		}
	}
}

static void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}

static void lcd_rect(int x1, int y1, int x2, int y2, int fill)
{
	if (x1 > x2) swap(&x1, &x2);
	if (y1 > y2) swap(&y1, &y2);
	
	if (fill)
	{
		ev3_lcd_fill_rect(x1, y1, x2-x1, y2-y1, EV3_LCD_BLACK);
		return;
	}
	
	ev3_lcd_draw_line(x1, y1, x1, y2);
	ev3_lcd_draw_line(x2, y1, x2, y2);
	ev3_lcd_draw_line(x1, y1, x2, y1);
	ev3_lcd_draw_line(x1, y2, x2, y2);
	ev3_lcd_fill_rect(x1+1, y1+1, x2-x1-1, y2-y1-1, EV3_LCD_WHITE);
}

typedef enum {
	IR_DIST   = 0,
	IR_SEEK   = 1,
	IR_REMOTE = 2,
} INFRARED_SENSOR_SENSOR_MODES;

static
void test_infrared_sensor(sensor_port_t port) {
	static const CliMenuEntry entry_tab[] = {
		{ .key = '1', .title = "Distance (%)", .exinf = IR_DIST },
		{ .key = '2', .title = "Seek", .exinf = IR_SEEK },
		{ .key = '3', .title = "Remote", .exinf = IR_REMOTE },
		{ .key = 'Q', .title = "Cancel", .exinf = -1 },
	};

	static const CliMenu climenu = {
		.title     = "Select Mode",
		.entry_tab = entry_tab,
		.entry_num = sizeof(entry_tab) / sizeof(CliMenuEntry),
	};

	bool_t change_mode = true;

	while(change_mode) {
		const CliMenuEntry* cme_mode = NULL;
		while (cme_mode == NULL) {
#if 1
			show_cli_menu(&climenu, 0, MENU_FONT_HEIGHT * 0, MENU_FONT);
			cme_mode = select_menu_entry(&climenu, 0, MENU_FONT_HEIGHT * 1, MENU_FONT);
#else
			fio_clear_screen();
			fprintf(fio, "--- Test Infra-Red Sensor @ Port%d ---\n", port + 1);
			show_cli_menu(&climenu);
			cme_mode = select_menu_entry(&climenu);
#endif
		}

		if (cme_mode->exinf == -1)
			return;

		// Draw title
		char msgbuf[100];
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE); // Clear menu area
		ev3_lcd_draw_string("Test Sensor", (EV3_LCD_WIDTH - strlen("Test Sensor") * MENU_FONT_WIDTH) / 2, 0);
		sprintf(msgbuf, "Type: Infra-Red");
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 1);
		sprintf(msgbuf, "Port: %c", '1' + port);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 2);
		switch (cme_mode->exinf) {
		case IR_DIST:
			VIEW_SENSOR({
				int8_t val = ev3_infrared_sensor_get_distance(port);
//				fio_clear_line();
				sprintf(msgbuf, "Distance: %-3d %%", val);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				tslp_tsk(10);
			});
			break;
		case IR_SEEK:
			VIEW_SENSOR({
				ir_seek_t val = ev3_infrared_sensor_seek(port);
//				fio_clear_line();
				for(int c = 0; c < 4; c++)
				{
					int16_t h = val.heading[c];
					int16_t d = val.distance[c];
					sprintf(msgbuf, "%d H:%d D:%d      ", c + 1, h, d);
					ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * (3 + c));
				}
				tslp_tsk(10);
			});
			break;
		case IR_REMOTE:
			VIEW_SENSOR({
				ir_remote_t val = ev3_infrared_sensor_get_remote(port);
				msgbuf[1] = 0;
				for(int c = 0; c < 4; c++)
				{
					int beacon   = (val.channel[c] & IR_BEACON_BUTTON)    ? 1 : 0;
					int redup    = (val.channel[c] & IR_RED_UP_BUTTON)    ? 1 : 0;
					int reddown  = (val.channel[c] & IR_RED_DOWN_BUTTON)  ? 1 : 0;
					int blueup   = (val.channel[c] & IR_BLUE_UP_BUTTON)   ? 1 : 0;
					int bluedown = (val.channel[c] & IR_BLUE_DOWN_BUTTON) ? 1 : 0;
					int top = MENU_FONT_HEIGHT * 3 + 2;
					
					lcd_rect(c * 45,      top     , c * 45 + 40, top + 68, 0);
					lcd_rect(c * 45 + 2,  top + 2,  c * 45 + 38, top + 14, beacon);
					lcd_rect(c * 45 + 2,  top + 16, c * 45 + 14, top + 40, redup);
					lcd_rect(c * 45 + 2,  top + 42, c * 45 + 14, top + 66, reddown);
					lcd_rect(c * 45 + 26, top + 16, c * 45 + 38, top + 40, blueup);
					lcd_rect(c * 45 + 26, top + 42, c * 45 + 38, top + 66, bluedown);
					msgbuf[0] = '1' + c;
					ev3_lcd_draw_string(msgbuf, c * 45 + 16, top + 32);
				}
				tslp_tsk(10);
			});
			break;
		default:
			assert(false);
		}
	}
}

typedef enum {
	GYRO_ANG  = 0,
	GYRO_RATE = 1,
	GYRO_GnA  = 3,
	GYRO_CAL  = 4,
} GYRO_SENSOR_MODES;

static
void test_gyro_sensor(sensor_port_t port) {
	static const CliMenuEntry entry_tab[] = {
		{ .key = '1', .title = "Angle", .exinf = GYRO_ANG },
		{ .key = '2', .title = "Rate", .exinf = GYRO_RATE },
		{ .key = 'Q', .title = "Cancel", .exinf = -1 },
	};

	static const CliMenu climenu = {
		.title     = "Select Mode",
		.entry_tab = entry_tab,
		.entry_num = sizeof(entry_tab) / sizeof(CliMenuEntry),
	};

	bool_t change_mode = true;

	while(change_mode) {
		const CliMenuEntry* cme_mode = NULL;
		while (cme_mode == NULL) {
#if 1
			show_cli_menu(&climenu, 0, MENU_FONT_HEIGHT * 0, MENU_FONT);
			cme_mode = select_menu_entry(&climenu, 0, MENU_FONT_HEIGHT * 1, MENU_FONT);
#else
			fio_clear_screen();
			fprintf(fio, "--- Test Gyro Sensor @ Port%d ---\n", port + 1);
			show_cli_menu(&climenu);
			cme_mode = select_menu_entry(&climenu);
#endif
		}

		if (cme_mode->exinf == -1)
			return;

		// Draw title
		char msgbuf[100];
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE); // Clear menu area
		ev3_lcd_draw_string("Test Sensor", (EV3_LCD_WIDTH - strlen("Test Sensor") * MENU_FONT_WIDTH) / 2, 0);
		sprintf(msgbuf, "Type: Gyroscope");
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 1);
		sprintf(msgbuf, "Port: %c", '1' + port);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 2);
		switch (cme_mode->exinf) {
		case GYRO_ANG:
			ev3_gyro_sensor_reset(port);
			VIEW_SENSOR({
				int16_t val = ev3_gyro_sensor_get_angle(port);
//				fio_clear_line();
				sprintf(msgbuf, "Angle: %-4d deg", val);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				tslp_tsk(10);
			});
			break;
		case GYRO_RATE:
			ev3_gyro_sensor_reset(port);
			VIEW_SENSOR({
				int16_t val = ev3_gyro_sensor_get_rate(port);
//				fio_clear_line();
				sprintf(msgbuf, "Rate: %-4d deg/s", val);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				tslp_tsk(10);
			});
			break;
		default:
			assert(false);
		}
	}
}

static
void test_touch_sensor(sensor_port_t port) {
	// Draw title
	char msgbuf[100];
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE); // Clear menu area
	ev3_lcd_draw_string("Test Sensor", (EV3_LCD_WIDTH - strlen("Test Sensor") * MENU_FONT_WIDTH) / 2, 0);
	sprintf(msgbuf, "Type: Touch");
	ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 1);
	sprintf(msgbuf, "Port: %c", '1' + port);
	ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 2);

	VIEW_SENSOR({
		bool_t val = ev3_touch_sensor_is_pressed(port);
	//				fio_clear_line();
		sprintf(msgbuf, "Pressed: %d", val);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
		tslp_tsk(10);
	});
}

typedef enum {
	COL_REFLECT = 0,
	COL_AMBIENT = 1,
	COL_COLOR   = 2,
    COL_RGBRAW  = 4,
} COLOR_SENSOR_MODES;

static
void test_color_sensor(sensor_port_t port) {
	static const char *colorstr[] = {
	    [COLOR_NONE]   = "Unknown",
	    [COLOR_BLACK]  = "Black",
	    [COLOR_BLUE]   = "Blue",
	    [COLOR_GREEN]  = "Green",
	    [COLOR_YELLOW] = "Yellow",
	    [COLOR_RED]    = "Red",
	    [COLOR_WHITE]  = "White",
	    [COLOR_BROWN]  = "Brown",
	};

	static const CliMenuEntry entry_tab[] = {
		{ .key = '1', .title = "Reflect", .exinf = COL_REFLECT },
		{ .key = '2', .title = "Ambient", .exinf = COL_AMBIENT },
		{ .key = '3', .title = "Color", .exinf = COL_COLOR },
		{ .key = '4', .title = "RGB", .exinf = COL_RGBRAW },
		{ .key = 'Q', .title = "Cancel", .exinf = -1 },
	};

	static const CliMenu climenu = {
		.title     = "Select Mode",
		.entry_tab = entry_tab,
		.entry_num = sizeof(entry_tab) / sizeof(CliMenuEntry),
	};

	bool_t change_mode = true;

	while(change_mode) {
		const CliMenuEntry* cme_mode = NULL;
		while (cme_mode == NULL) {
#if 1
			show_cli_menu(&climenu, 0, MENU_FONT_HEIGHT * 0, MENU_FONT);
			cme_mode = select_menu_entry(&climenu, 0, MENU_FONT_HEIGHT * 1, MENU_FONT);
#else
			fio_clear_screen();
			fprintf(fio, "--- Test Color Sensor @ Port%d ---\n", port + 1);
			show_cli_menu(&climenu);
			cme_mode = select_menu_entry(&climenu);
#endif
		}

		if (cme_mode->exinf == -1)
			return;

		// Draw title
		char msgbuf[100];
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE); // Clear menu area
		ev3_lcd_draw_string("Test Sensor", (EV3_LCD_WIDTH - strlen("Test Sensor") * MENU_FONT_WIDTH) / 2, 0);
		sprintf(msgbuf, "Type: Color");
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 1);
		sprintf(msgbuf, "Port: %c", '1' + port);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 2);
		switch (cme_mode->exinf) {
		case COL_REFLECT:
			VIEW_SENSOR({
				uint8_t val = ev3_color_sensor_get_reflect(port);
//				fio_clear_line();
				sprintf(msgbuf, "Reflect: %-4d", val);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				tslp_tsk(10);
			});
			break;
		case COL_AMBIENT:
			VIEW_SENSOR({
				uint8_t val = ev3_color_sensor_get_ambient(port);
//				fio_clear_line();
				sprintf(msgbuf, "Ambient: %-4d", val);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				tslp_tsk(10);
			});
			break;
		case COL_COLOR:
			VIEW_SENSOR({
				colorid_t val = ev3_color_sensor_get_color(port);
//				fio_clear_line();
				sprintf(msgbuf, "Color: %-7s", colorstr[val]);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				tslp_tsk(10);
			});
			break;
		case COL_RGBRAW:
			VIEW_SENSOR({
				rgb_raw_t val;
                ev3_color_sensor_get_rgb_raw(port, &val);
//				fio_clear_line();
				sprintf(msgbuf, "Red:   %-4d", val.r);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
				sprintf(msgbuf, "Green: %-4d", val.g);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 4);
				sprintf(msgbuf, "Blue:  %-4d", val.b);
				ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 5);
				tslp_tsk(10);
			});
			break;
		default:
			assert(false);
		}
	}
}

static
void test_ht_nxt_accel_sensor(sensor_port_t port) {
	// Draw title
	char msgbuf[100];
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE); // Clear menu area
	ev3_lcd_draw_string("Test Sensor", (EV3_LCD_WIDTH - strlen("Test Sensor") * MENU_FONT_WIDTH) / 2, 0);
	sprintf(msgbuf, "Type: HT Acc.");
	ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 1);
	sprintf(msgbuf, "Port: %c", '1' + port);
	ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 2);

	int16_t axes[3];

	VIEW_SENSOR({
		bool_t val = ht_nxt_accel_sensor_measure(port, axes);
		assert(val);
		sprintf(msgbuf, "X: %-4d", axes[0]);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
		sprintf(msgbuf, "Y: %-4d", axes[1]);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 4);
		sprintf(msgbuf, "Z: %-4d", axes[2]);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 5);
		tslp_tsk(10);
	});
}

static
void test_nxt_temp_sensor(sensor_port_t port) {
	// Draw title
	char msgbuf[100];
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE); // Clear menu area
	ev3_lcd_draw_string("Test Sensor", (EV3_LCD_WIDTH - strlen("Test Sensor") * MENU_FONT_WIDTH) / 2, 0);
	sprintf(msgbuf, "Type: NXT Temp.");
	ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 1);
	sprintf(msgbuf, "Port: %c", '1' + port);
	ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 2);

    float temp;

	VIEW_SENSOR({
		bool_t val = nxt_temp_sensor_measure(port, &temp);
		assert(val);
		sprintf(msgbuf, "Temp.: %8.4f", temp);
		ev3_lcd_draw_string(msgbuf, 0, MENU_FONT_HEIGHT * 3);
		tslp_tsk(10);
	});
}
void test_sensor(intptr_t unused) {
	const CliMenuEntry* cme_port = NULL;
	while(cme_port == NULL) {
//		fio_clear_screen();
//		fprintf(fio, "--- Test Sensor ---\n");
		cme_port = select_sensor();
	}

	if(cme_port->exinf == -1) return;

	switch(ev3_sensor_get_type(cme_port->exinf)) {
	case ULTRASONIC_SENSOR:
		test_ultrasonic_sensor(cme_port->exinf);
		break;
	case INFRARED_SENSOR:
		test_infrared_sensor(cme_port->exinf);
		break;
	case GYRO_SENSOR:
		test_gyro_sensor(cme_port->exinf);
		break;
	case TOUCH_SENSOR:
		test_touch_sensor(cme_port->exinf);
		break;
	case COLOR_SENSOR:
		test_color_sensor(cme_port->exinf);
		break;
	case HT_NXT_ACCEL_SENSOR:
		test_ht_nxt_accel_sensor(cme_port->exinf);
		break;
	case NXT_TEMP_SENSOR:
		test_nxt_temp_sensor(cme_port->exinf);
		break;
	case NONE_SENSOR: {
		char msgbuf[100];
		sprintf(msgbuf, "%s is not connected.", cme_port->title);
		show_message_box("No Sensor", msgbuf);
		}
//		fprintf(fio, "%s is not connected.\n", cme_port->title);
//		tslp_tsk(500);
		break;

	default:
		assert(false);
	}

}

