/**
 * \file    ev3lcd.h
 * \brief	API for LCD
 * \author	ertl-liyixiao
 */

 /**
 * \~English
 * \defgroup ev3api-brick EV3 Intelligent Brick
 * @{ 
 *
 * \~Japanese
 * \defgroup ev3api-brick EV3本体機能
 * @{
 */
 
#pragma once

/**
 * \~English
 * \defgroup ev3api-lcd LCD
 * \brief    Definitions of API for LCD.
 * @{
 *
 * \~Japanese
 * \defgroup ev3api-lcd LCD
 * \brief    LCDに関するAPI．
 * @{
 */


/**
 * \~English
 * \brief Enumeration type for font size
 *
 * \~Japanese
 * \brief フォントを表す番号
 */
typedef enum {
	EV3_FONT_SMALL,  //!< \~English Font in small size  \~Japanese 小さいサイズのフォント
	EV3_FONT_MEDIUM, //!< \~English Font in medium size \~Japanese 普通サイズのフォント
} lcdfont_t;

/**
 * \~English
 * \brief Enumeration type for LCD color
 *
 * \~Japanese
 * \brief LCDカラーを表す番号
 */
typedef enum {
	EV3_LCD_WHITE = 0, //!< \~English White  \~Japanese 白
	EV3_LCD_BLACK = 1, //!< \~English Black  \~Japanese 黒
} lcdcolor_t;

#define EV3_LCD_WIDTH  (178) //!< \~English Width of LCD screen   \~Japanese LCD画面の幅
#define EV3_LCD_HEIGHT (128) //!< \~English Height of LCD screen  \~Japanese LCD画面の高さ

/**
 * \~English
 * \brief        Set the default font.
 * \param font   Font number
 * \retval E_OK  Successful completion
 * \retval E_ID  Invalid font number
 *
 * \~Japanese
 * \brief        デフォルトのフォントを設定する．
 * \param  font  フォントの番号
 * \retval E_OK  正常終了
 * \retval E_ID  不正ID番号
 */
ER ev3_lcd_set_font(lcdfont_t font);

/**
 * \~English
 * \brief           Get font size.
 * \param  font     Font number
 * \param  p_width  Pointer to a place to store the acquired font width. If it is NULL, it is not stored.
 * \param  p_height Pointer to the place to store the height of the acquired font. If it is NULL, it is not stored.
 * \retval E_OK     Successful completion
 * \retval E_ID     Invalid fomt number
 *
 * \~Japanese
 * \brief           フォントのサイズを取得する．
 * \param  font     フォントの番号
 * \param  p_width  取得したフォントの幅を格納する場所へのポインタ．NULLの場合は格納しない．
 * \param  p_height 取得したフォントの高さを格納する場所へのポインタ．NULLの場合は格納しない．
 * \retval E_OK     正常終了
 * \retval E_ID     不正ID番号
 */
ER ev3_font_get_size(lcdfont_t font, int32_t *p_width, int32_t *p_height);

/**
 * \~English
 * \brief Structure of the image object.
 *
 * \~Japanese
 * \brief 画像のオブジェクトの構造体．
 */
typedef struct {
    int32_t   width;
    int32_t   height;
    void* data;
} image_t;

/**
 * \~English
 * \brief            Load images from memory file.
 * \details          Create an image object from the specified memory file. Currently, it only supports monochrome image files in BMP format. If an error occurs , clear the data of \a p_image to NULL.
 * \param  p_memfile Object of memory file where image file is stored
 * \param  p_image   Pointer to the place to store the object of the generated image
 * \retval E_OK      Successful completion
 * \retval E_NOMEM   Insufficient memory
 * \retval E_NOSPT   Image file format is not supported
 * \retval E_OBJ     Image file is damaged
 * \retval E_PAR     Invalid memory file
 *
 * \~Japanese
 * \brief            メモリファイルから画像をロードする．
 * \details          指定したメモリファイルから画像のオブジェクトを生成する．現時点では，BMP形式のモノクロ画像ファイルしかサポートしない．
 *                   エラーが発生する場合，\a p_image の \a data はNULLにクリアする.
 * \param  p_memfile 画像ファイルが格納されるメモリファイルのオブジェクト
 * \param  p_image   生成した画像のオブジェクトを格納する場所へのポインタ
 * \retval E_OK      正常終了
 * \retval E_NOMEM   メモリ不足
 * \retval E_NOSPT   画像ファイルの形式は未対応
 * \retval E_OBJ     画像ファイルは破損
 * \retval E_PAR     メモリファイルは無効
 */
ER ev3_image_load(const memfile_t *p_memfile, image_t *p_image);

/**
 * \~English
 * \brief            Release the image object.
 * \details          Free resources (memory area) secured by the object of the image. In case of normal \a termination , data of \a p_image is cleared to NULL.
 * \param  p_image   Pointer to released object
 * \retval E_OK      Successful completion
 * \retval E_PAR     \a p_image is NULL
 *
 * \~Japanese
 * \brief            画像のオブジェクトを解放する．
 * \details          画像のオブジェクトにより確保されたリソース（メモリ領域）を解放する．正常終了の場合，\a p_image の \a data はNULLにクリアする．
 * \param  p_image   解放する画像のオブジェクトのポインタ
 * \retval E_OK      正常終了
 * \retval E_PAR     \a p_image はNULL
 */
ER ev3_image_free(image_t *p_image);

/**
 * \~English
 * \brief             Draws a character string at the specified position.
 * \param  x          Horizontal position of upper left corner
 * \param  y          Vertical position of upper left corner
 * \param  str        String (pointer to array of characters)
 * \retval E_OK       Successful completion
 *
 * \~Japanese
 * \brief             指定位置で文字列を描く．
 * \param  x          左上隅の水平方向の位置
 * \param  y          左上隅の垂直方向の位置
 * \param  str        文字列
 * \retval E_OK       正常終了
 */
ER ev3_lcd_draw_string(const char *str, int32_t x, int32_t y);

/**
 * \~English
 * \brief             Draw a line at specified coordinates.
 * \param  x0         Horizontal position of start point
 * \param  y0         Vertical position of start point
 * \param  x1         Horizontal position of the end point
 * \param  y1         Vertical position of the end point
 * \retval E_OK       Successful completion
 *
 * \~Japanese
 * \brief             指定座標で線を引く．
 * \param  x0         始点の水平方向の位置
 * \param  y0         始点の垂直方向の位置
 * \param  x1         終点の水平方向の位置
 * \param  y1         終点の垂直方向の位置
 * \retval E_OK       正常終了
 */
ER ev3_lcd_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1);

/**
 * \~English
 * \brief            Draw a rectangle
 * \param  x         Horizontal position of upper left corner
 * \param  y         Vertical position of upper left corner
 * \param  w         Width of the rectangle
 * \param  h         Height of the rectangle
 * \param  color     Color [TODO: color of filling?]
 * \retval E_OK      Successful completion
 *
 * \~Japanese
 * \brief            矩形を描いて色を塗る．
 * \param  x         左上隅の水平方向の位置
 * \param  y         左上隅の垂直方向の位置
 * \param  w         矩形の幅
 * \param  h         矩形の高さ
 * \param  color     カラー
 * \retval E_OK      正常終了
 */
ER ev3_lcd_fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, lcdcolor_t color);

/**
 * \~English
 * \brief            Display the image at specified coordinates.
 * \param  p_image   Image object
 * \param  x         Horizontal position of upper left corner
 * \param  y         Vertical position of upper left corner
 * \retval E_OK      Successful completion
 * \retval E_PAR     Image object invalid
 *
 * \~Japanese
 * \brief            指定座標で画像を表示する．
 * \param  p_image   画像のオブジェクト
 * \param  x         左上隅の水平方向の位置
 * \param  y         左上隅の垂直方向の位置
 * \retval E_OK      正常終了
 * \retval E_PAR     画像のオブジェクトは無効
 */
ER ev3_lcd_draw_image(const image_t *p_image, int32_t x, int32_t y);

/**
 * @} // End of group ev3api-lcd
 */
 
/** @} // End of group ev3api-brick */

#if 0 // Legacy or not implemented

ER ev3_image_bitblt(const image_t *src, int32_t sx, int32_t sy, image_t *dst, int32_t dx, int32_t dy, int32_t w, int32_t h, int32_t rop);

lcdcolor_t ev3_image_pixel(const image_t *p_image, int32_t x, int32_t y);

#endif
