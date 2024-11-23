//
// Created by cp176 on 11/9/2024.
//

#ifndef FONT_H
#define FONT_H

#include <stdint.h>

extern const int font_8bit[39][8];
void render_font(unsigned int *pixels, int x, int y, int number, int SCREEN_WIDTH);
void render_font_from_str(unsigned int *pixels, int x, int y, const char *str, int SCREEN_WIDTH);

#endif //FONT_H
