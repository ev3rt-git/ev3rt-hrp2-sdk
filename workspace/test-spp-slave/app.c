#include "ev3api.h"
#include "app.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int32_t default_menu_font_width;
int32_t default_menu_font_height;

#define TEST_MOTOR_PORT  EV3_PORT_A
#define TEST_SENSOR_PORT EV3_PORT_2

void bluetooth_task(intptr_t unused) {
    FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

    static char buf[256];
    while (fgets(buf, 256, bt)) {
        if (strncmp("SENSOR", buf, strlen("SENSOR")) == 0) { // Send touch sensor (Port2) value
            //syslog(LOG_ERROR, "send SENSOR data");
            fprintf(bt, "SENSOR %d\n", ev3_touch_sensor_is_pressed(TEST_SENSOR_PORT));
        } else if (strncmp("MOTOR", buf, strlen("MOTOR")) == 0) { // Set motor speed
            int power;
            if (sscanf(buf, "MOTOR %d", &power) == 1) {
                ev3_motor_set_power(TEST_MOTOR_PORT, power);
            } else assert(false);
        } else assert(false);
    }
    // Bluetooth should have been disconnected
    assert(!ev3_bluetooth_is_connected());
}

void main_task(intptr_t unused) {
    ev3_motor_config(TEST_MOTOR_PORT, LARGE_MOTOR);
    ev3_sensor_config(TEST_SENSOR_PORT, TOUCH_SENSOR);
    ev3_lcd_set_font(MENU_FONT);
    ev3_font_get_size(MENU_FONT, &default_menu_font_width, &default_menu_font_height);

	// Clear menu area
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);

	// Draw title
	const char *title = "BT SPP SLAVE";
	int offset_x = 0, offset_y = 0;
	if (EV3_LCD_WIDTH - offset_x > strlen(title) * MENU_FONT_WIDTH)
		offset_x += (EV3_LCD_WIDTH - offset_x - strlen(title) * MENU_FONT_WIDTH) / 2;
	ev3_lcd_draw_string(title, offset_x, offset_y);

	// Draw name
	offset_x = 0;
	offset_y += MENU_FONT_HEIGHT;
	ev3_lcd_draw_string("Connected: ", offset_x, offset_y + MENU_FONT_HEIGHT * 0);

	// Draw status
	char lcdstr[100];
    bool_t connected = false;
	while (1) {
        if (connected != ev3_bluetooth_is_connected()) { // Status changed
            connected = !connected;
            if (connected)
                act_tsk(BT_TASK);
            else
                ter_tsk(BT_TASK);
        }
		sprintf(lcdstr, "%c", connected ? 'Y' : 'N');
		ev3_lcd_draw_string(lcdstr, offset_x + strlen("Connected: ") * MENU_FONT_WIDTH, offset_y + MENU_FONT_HEIGHT * 0);
        tslp_tsk(10);
	}
}

