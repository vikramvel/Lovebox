#ifndef _SYSTEM_H_
#define _SYSTEM_H_

// <!-- Here be dragons  -->

#define sclk 18   // marked SCL or CK on OLED board
#define mosi 23   // marked SDA or SI on OLED board
#define cs   5    // marked CS or OC on OLED board
#define rst  16   // marked RES or R on OLED board
#define dc   17   // marked DC or sometimes (confusingly) RS on OLED board

#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define NDOTS 256      // Number of dots 512
#define SCALE 4096 //4096
#define INCREMENT 512//512
#define PI2 6.283185307179586476925286766559
#define RED_COLORS (32)
#define GREEN_COLORS (64)
#define BLUE_COLORS (32)
#define PAUSE 1000


#define clear() fillScreen(0)


void initialize (void);
void display_setup(SEMU_SSD1331 disp);
void matrix (int16_t xyz[3][NDOTS], uint16_t col[NDOTS]);
void rotate (int16_t xyz[3][NDOTS], uint16_t angleX, uint16_t angleY, uint16_t angleZ);
void draw(int16_t xyz[3][NDOTS], uint16_t col[NDOTS], SEMU_SSD1331 disp);

#endif