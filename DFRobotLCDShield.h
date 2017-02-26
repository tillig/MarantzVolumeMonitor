#pragma once

/*
DFRobot LCD Shield
https://www.dfrobot.com/product-51.html
https://www.dfrobot.com/wiki/index.php/LCD_KeyPad_Shield_For_Arduino_SKU:_DFR0009
D4-D7    -> LCD Data transmission
D8       -> Register Select
D9       -> Enable pin
D10      -> Backlight control
*/
#define LCD_DB4 4
#define LCD_DB5 5
#define LCD_DB6 6
#define LCD_DB7 7
#define LCD_RS 8
#define LCD_ENABLE 9
#define LCD_BACKLIGHT 10
#define NUM_BUTTONS 6
#define BUTTON_PIN A0

#define BUTTON_NONE 0
#define BUTTON_RIGHT 1
#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_LEFT 4
#define BUTTON_SELECT 5

// Tolerances are based on the readings
// from pin A0 and add 50 to allow variance.
// My board didn't match the specs on the
// wiki page at DFRobot.
#define BUTTON_RIGHT_TOLERANCE 50 // Reading: 0
#define BUTTON_UP_TOLERANCE 150 // Reading: 100
#define BUTTON_DOWN_TOLERANCE 305 // Reading: 256
#define BUTTON_LEFT_TOLERANCE 460 // Reading: 409
#define BUTTON_SELECT_TOLERANCE 690 // Reading: 640

