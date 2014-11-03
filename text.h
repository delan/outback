#ifndef CG200_TEXT_H
#define CG200_TEXT_H

#include "const.h"

void cg200_begin_text(int w, int h);
void cg200_end_text(void);
void cg200_draw_text(const char *text, int x, int y, int w, int h);

#endif
