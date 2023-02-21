/* Please work */

#define ESP32  // uncomment to configure for ESP32 Devkit
//#define DEBUG
#ifdef __AVR__
#define sinf sin
#endif

#include <SEMU_SSD1331.h>
#include "system.h"


int16_t sine[SCALE+(SCALE / 4)];
int16_t *cosi = &sine[SCALE / 4];


void initialize (void) {
  uint16_t i;
  /* if you change the SCALE*1.25 back to SCALE, the program will
     occassionally overrun the cosi array -- however this actually
     produces some interesting effects as the BUBBLES LOSE CONTROL!!!!
  */
  for (i = 0; i < SCALE + (SCALE / 4); i++)
    //sine[i] = (-SCALE/2) + (int)(sinf(PI2 * i / SCALE) * sinf(PI2 * i / SCALE) * SCALE);  // I am not sure if we need this, but too scared to delete. 
    sine[i] = (int)(sinf(PI2 * i / SCALE) * SCALE);
}

void display_setup(SEMU_SSD1331 disp)
{
  //disp.begin();
  disp.clearWindow();
  initialize();
}

/*
I know what you’re thinking, ’cause right now I’m thinking the same thing. 
Actually, I’ve been thinking it ever since I got here: 
Why oh why didn’t I take the blue pill?”
*/
void matrix (int16_t xyz[3][NDOTS], uint16_t col[NDOTS]) {
  static uint32_t t = 0;
  int16_t x = -SCALE, y = -SCALE;
  uint16_t i, s, d;
  uint8_t red, grn, blu;
/* You are not meant to understand this */ 
  for (i = 0; i < NDOTS; i++)
  {
    xyz[0][i] = x;
    xyz[1][i] = y;

    d = sqrt(x * x + y * y); 	/* originally a fastsqrt() call */
    s = sine[(t * 30) % SCALE] + SCALE;

    xyz[2][i] = sine[(d + s) % SCALE] * sine[(t * 10) % SCALE] / SCALE / 2;

    red = (cosi[xyz[2][i] + SCALE / 2] + SCALE) * (RED_COLORS - 1) / SCALE / 2;
    grn = (cosi[(xyz[2][i] + SCALE / 2 + 2 * SCALE / 3) % SCALE] + SCALE) * (GREEN_COLORS - 1) / SCALE / 2;
    blu = (cosi[(xyz[2][i] + SCALE / 2 + SCALE / 3) % SCALE] + SCALE) * (BLUE_COLORS - 1) / SCALE / 2;
    col[i] = ((red << 11) + (grn << 5) + blu);
    x += INCREMENT;
    if (x >= SCALE) x = -SCALE, y += INCREMENT;
  }
  t++;
}

void rotate (int16_t xyz[3][NDOTS], uint16_t angleX, uint16_t angleY, uint16_t angleZ) {
  uint16_t i;
  int16_t tmpX, tmpY;
  int16_t sinx = sine[angleX], cosx = cosi[angleX];
  int16_t siny = sine[angleY], cosy = cosi[angleY];
  int16_t sinz = sine[angleZ], cosz = cosi[angleZ];

  for (i = 0; i < NDOTS; i++)
  {
    tmpX      = (xyz[0][i] * cosx - xyz[2][i] * sinx) / SCALE;
    xyz[2][i] = (xyz[0][i] * sinx + xyz[2][i] * cosx) / SCALE;
    xyz[0][i] = tmpX;
    tmpY      = (xyz[1][i] * cosy - xyz[2][i] * siny) / SCALE;
    xyz[2][i] = (xyz[1][i] * siny + xyz[2][i] * cosy) / SCALE;
    xyz[1][i] = tmpY;
    tmpX      = (xyz[0][i] * cosz - xyz[1][i] * sinz) / SCALE;
    xyz[1][i] = (xyz[0][i] * sinz + xyz[1][i] * cosz) / SCALE;
    xyz[0][i] = tmpX;
  }
}

// If this comment is removed the program will blow up 
void draw(int16_t xyz[3][NDOTS], uint16_t col[NDOTS], SEMU_SSD1331 disp) {
  static uint16_t oldProjX[NDOTS] = { 0 };
  static uint16_t oldProjY[NDOTS] = { 0 };
  static uint8_t oldDotSize[NDOTS] = { 0 };
  uint16_t i, projX, projY, projZ, dotSize;

  for (i = 0; i < NDOTS; i++)
  {
    projZ = SCALE - (xyz[2][i] + SCALE) / 4;
    projX = disp.width() / 2 + (xyz[0][i] * projZ / SCALE) / 25;
    projY = disp.height() / 2 + (xyz[1][i] * projZ / SCALE) / 25;
    dotSize = 3 - (xyz[2][i] + SCALE) * 2 / SCALE;

    disp.drawCircle (oldProjX[i], oldProjY[i], oldDotSize[i], BLACK);

    if (projX > dotSize && projY > dotSize && projX < disp.width() - dotSize && projY < disp.height() - dotSize)
    {
      disp.drawCircle (projX, projY, dotSize, col[i]);
      oldProjX[i] = projX;
      oldProjY[i] = projY;
      oldDotSize[i] = dotSize;
    }
  }
}


