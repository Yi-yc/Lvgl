#ifndef _GT911_H_
#define _GT911_H_
#include "lcd.h"
#include "sys.h"
#include "touch.h"

void ctp_test(void);
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);
void Load_Drow_Dialog(void);

#endif
