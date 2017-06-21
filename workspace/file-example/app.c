/**
 * This sample program shows how to create file and work with them (read and write data)
 */

#include "app.h"
#include <stdlib.h>

void main_task(intptr_t unused) {
    char lcd_text[100];

    // Set LCD font and get width and height of font
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    int32_t LCD_FONT_WIDTH, LCD_FONT_HEIGHT;
    ev3_font_get_size(EV3_FONT_MEDIUM, &LCD_FONT_WIDTH, &LCD_FONT_HEIGHT);
    int charSizeWidth = EV3_LCD_WIDTH/LCD_FONT_WIDTH;

    // Title on LCD
    sprintf(lcd_text, "File test");
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * 0);
    
    // write to file by fputc() => put "ABCDEFGHJK"
    FILE * file_desc = fopen("/test.txt", "a+"); 
    // "a+" => write to file and append data, create file if not exist
    int counter = 0;
    static char buf[256];
    while (counter < 10) {
        fputc(counter + 'A', file_desc);
        sprintf(lcd_text, "fputc_cnt: %d", counter);
        ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * 1);
        counter++;
        tslp_tsk(10);
    }
    fclose(file_desc);

    // read file by fread() => get char[3]
    file_desc = fopen("/test.txt", "r");
    int read_char_cnt = 3;
    fread(buf, 1 , read_char_cnt, file_desc);
    sprintf(lcd_text, "fread(): %s", buf);
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * 2);
    fclose(file_desc);


    // read file by fgetc() => get one byte
    file_desc = fopen("/test.txt", "r");
    int ch = fgetc(file_desc);
    sprintf(lcd_text, "fgetc(): %c", ch);
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * 3);
    fclose(file_desc);

    // read file by fgetc() in cycle => read all file
    file_desc = fopen("/test.txt", "r");
    read_char_cnt = 0;
    int lcd_row = 5;
    ch = EOF;
    while((ch = fgetc(file_desc)) !=  EOF) {
        sprintf(lcd_text, "Read_cnt: %d", read_char_cnt);
        ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * 4);
        
        sprintf(lcd_text, "%c", ch);
        ev3_lcd_draw_string(lcd_text, 
                            LCD_FONT_WIDTH * (read_char_cnt % charSizeWidth), 
                            LCD_FONT_HEIGHT * lcd_row);
        read_char_cnt++;
        if(read_char_cnt % charSizeWidth == 0) { lcd_row++; }

        tslp_tsk(10);
    }
    fclose(file_desc);  

    ev3_lcd_draw_string("END", 0, LCD_FONT_HEIGHT * 7);  
}



