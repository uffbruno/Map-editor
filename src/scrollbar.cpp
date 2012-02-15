#include <iostream>

#include <allegro5/allegro_primitives.h>
#include "scrollbar.hpp"

scrollbar::scrollbar(float x, float y, int width, int height): pos(x, y),
                                                               width(width),
                                                               height(height) {

}

scrollbar::~scrollbar() {
}

void scrollbar::scroll(float dx, float dy) {
    pos.x += dx;
    pos.y += dy;
}

void scrollbar::draw() {
    al_draw_filled_rectangle(pos.x, pos.y, 
        pos.x + width, pos.y + height, al_map_rgb(255,255,255));
}

bool scrollbar::is_over(float x, float y) {
    if (x < pos.x)          return false;
    if (x > pos.x + width)  return false;
    if (y < pos.y)          return false;
    if (y > pos.y + height) return false;
    
    return true;
}