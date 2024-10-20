//
// COMP-GENG 421 - Tom Lupfer
//
// LCD module header file
//

#ifndef LCD_H_
#define LCD_H_

#define LCD_PIXEL_WIDTH				128		// width of the LCD, in pixels
#define LCD_PIXEL_HEIGHT			32		// height of the LCD, in pixels
#define LCD_PAGE_PIXEL_HEIGHT		8		// height of each LCD page, in pixels

#define LCD_PAGE_HEIGHT				(LCD_PIXEL_HEIGHT / LCD_PAGE_PIXEL_HEIGHT)	// height of the LCD, in pages

#define LCD_CHAR_PIXEL_WIDTH		6		// width of each character, in pixels
#define LCD_CHAR_PIXEL_HEIGHT		8		// height of each character, in pixels

#define LCD_CHAR_WIDTH				(LCD_PIXEL_WIDTH / LCD_CHAR_PIXEL_WIDTH)	// width of the LCD, in characters
#define LCD_CHAR_HEIGHT				(LCD_PIXEL_HEIGHT / LCD_CHAR_PIXEL_HEIGHT)	// height of the LCD, in characters

#define LCD_CHAR_NORMAL				FALSE
#define LCD_CHAR_REVERSE			TRUE

#define LCD_GOBBLER_PIXEL_WIDTH		8
#define LCD_GOBBLER_PIXEL_HEIGHT	8

#define LCD_GOBLET_PIXEL_WIDTH		4
#define LCD_GOBLET_PIXEL_HEIGHT		4

#define LCD_OBJ_GOBBLER				0
#define LCD_OBJ_GOBLET				1

#define LCD_OBJ_RENDER				TRUE
#define LCD_OBJ_ERASE				FALSE


void LcdInit(void);
void LcdClear(void);
void LcdReverse(Bool ReverseDisplay);
void LcdRenderObject(Int Object, Int PosX, Int PosY, Bool RenderFlag);
Int LcdPrintf(Int CharRow, Int CharCol, Bool ReverseFlag, char * FmtStrPtr, ...);

#endif	// LCD_H_
