#include <allegro5/allegro_primitives.h>
#include "button.hpp"

button::button(const std::string& label, ALLEGRO_FONT *font, int x, int y, int w, int h) :
    label(label), font(font), x(x), y(y), w(w), h(h), ct(0),
    color(al_map_rgb(255,255,255)), 
    mouseover_color(al_map_rgb(255,255,255)) {
}

button::~button() {
}

void button::handle_input(const ALLEGRO_EVENT &ev) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (ct && is_over(ev.mouse.x, ev.mouse.y)) {
            ct();
        }
    }
    
}

bool button::is_over(int px, int py) const {
    if (px < x)      return false;
    if (px > x + w)  return false;
    if (py < y)      return false;
    if (py > y + h)  return false;
    
    return true;
}

void button::draw() {
    al_draw_rectangle(x, y, x+w, y+h, color, 0);
    
    int ascent = al_get_font_ascent(font);
    al_draw_textf(font, al_map_rgb(255,255,255), x+w/2, y+h/2 - ascent/2, ALLEGRO_ALIGN_CENTRE, label.c_str());
}