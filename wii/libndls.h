#ifdef _TINSPIRE
    #include_next <libndls.h>
#else
    #ifndef LIBNDLS_H
    #define LIBNDLS_H

    #include <stdint.h>
	#include <ogcsys.h>
	#include <ogc/lwp_watchdog.h>

	#include <gxflux/gfx.h>
	#include <gxflux/gfx_con.h>

    typedef enum {
        SCR_TYPE_INVALID=-1,
        SCR_320x240_565=0,
        SCR_320x240_4=1,
        SCR_240x320_565=2,
        SCR_320x240_16=3,
        SCR_320x240_8=4,
        SCR_TYPE_COUNT=5
    } scr_type_t;

    #define lcd_type(x) SCR_320x240_565

    #define isKeyPressed(x) 0
    #define is_classic 0
    #define is_cx 1
    #define touchpad_scan(x) do{}while(0)
    #define is_touchpad 0
    #define touchpad_arrow_pressed(x) 0

    #ifndef KEYS_H
    #define KEYS_H

    typedef struct {
        unsigned char contact; /* "touched". TRUE or FALSE */
        unsigned char proximity;
        uint16_t x;
        uint16_t y;
        unsigned char x_velocity;
        unsigned char y_velocity;
        uint16_t dummy;
        unsigned char pressed; /* "key pressed". TRUE or FALSE */
        unsigned char arrow; /* area of the pad currently touched. see tpad_arrow_t. */
    } touchpad_report_t;

    /***********************************
     * Keys (key=(offset, 2^bit #)
     ***********************************/

    typedef enum tpad_arrow 
    {
        TPAD_ARROW_NONE,
        TPAD_ARROW_UP, TPAD_ARROW_UPRIGHT,
        TPAD_ARROW_RIGHT, TPAD_ARROW_RIGHTDOWN,
        TPAD_ARROW_DOWN, TPAD_ARROW_DOWNLEFT,
        TPAD_ARROW_LEFT, TPAD_ARROW_LEFTUP,
        TPAD_ARROW_CLICK
    } tpad_arrow_t;

    typedef struct {
        int row, col, tpad_row, tpad_col;
        tpad_arrow_t tpad_arrow;
    } t_key;

    /* Used when the row and column are the same for both models */
    #define KEY_(row, col) {row, col, row, col, TPAD_ARROW_NONE}
    #define KEYTPAD_(row, col, tpad_row, tpad_col) {row, col, tpad_row, tpad_col, TPAD_ARROW_NONE}
    #define KEYTPAD_ARROW_(row, col, tpad_arrow) {row, col, row, col, tpad_arrow}

    /* Used to fill up a nonexistent key on a model */
    /* Used to fill up a nonexistent key on a model */
    #define _KEY_DUMMY_ROW 0x1C
    #define _KEY_DUMMY_COL 0x400
    
    #define KEY_NSPIRE_8 PAD_BUTTON_UP
    #define KEY_NSPIRE_2 PAD_BUTTON_DOWN
   
	/*#define KEY_NSPIRE_4 SDLK_TAB
    #define KEY_NSPIRE_6 SDLK_BACKSPACE*/

	#define KEY_NSPIRE_4 0
    #define KEY_NSPIRE_6 0

	#define KEY_NSPIRE_3 601
    #define KEY_NSPIRE_1 600
  
	#define KEY_NSPIRE_PERIOD PAD_BUTTON_START

    #define KEY_NSPIRE_UP 555
    #define KEY_NSPIRE_DOWN 556

    #define KEY_NSPIRE_LEFT PAD_BUTTON_LEFT
    #define KEY_NSPIRE_RIGHT PAD_BUTTON_RIGHT
     
    #define KEY_NSPIRE_DOC PAD_BUTTON_X
    #define KEY_NSPIRE_5 0
    #define KEY_NSPIRE_CLICK PAD_BUTTON_A
    

    #define KEY_NSPIRE_ESC PAD_TRIGGER_Z

    #define KEY_NSPIRE_MENU 1

    #define KEY_NSPIRE_7 PAD_BUTTON_A
    #define KEY_NSPIRE_9 PAD_BUTTON_B
    
    #define KEY_NSPIRE_MINUS 0
    #define KEY_NSPIRE_PLUS 0
    #define KEY_NSPIRE_0 0
    
    #define KEY_NSPIRE_CTRL 0


    #endif // !KEYS_H


    #endif
#endif
