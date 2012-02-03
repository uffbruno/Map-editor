#include <iostream>
#include <allegro5/allegro_primitives.h>

#include "mapgrid.hpp"

mapgrid::mapgrid(int x, int y, int visiblewidth, int visibleheight): x(x),
                                                                     y(y),
                                                                     visiblewidth(visiblewidth),
                                                                     visibleheight(visibleheight),
                                                                     mapx(0),
                                                                     mapy(0),
                                                                     width(0),
                                                                     height(0),
                                                                     m(),
                                                                     map_bitmap(0),
                                                                     visible_bitmap(0) {
    visible_bitmap = al_create_bitmap(visiblewidth, visibleheight);
}

mapgrid::~mapgrid() {
}

bool mapgrid::load_map(const std::string& mapfile) {
    if (!m.load_map(mapfile)) {
        return false;
    }
    
    width = m.get_width();
    height = m.get_height();

    if (map_bitmap) {
        al_destroy_bitmap(map_bitmap);
    }
    
    map_bitmap = al_create_bitmap(width, height);
    
    return true;
}

bool mapgrid::save_map(const std::string& mapfile) {
    return m.save_map(mapfile);
}

void mapgrid::draw() {
    if (!map_bitmap || !visible_bitmap) return;
    
    //save the previous bitmap target
    ALLEGRO_BITMAP *old_target = al_get_target_bitmap();

    //draws to our custom bitmap
    al_set_target_bitmap(map_bitmap);
    m.draw();
    
    //draws to our visible bitmap
    al_set_target_bitmap(visible_bitmap);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_bitmap_region(map_bitmap, mapx, mapy, visiblewidth, visibleheight, 0, 0, 0);
    
    //restore older target bitmap
    al_set_target_bitmap(old_target);
    
    //draws visible bitmap to older target bitmap (normally the display backbuffer)
    al_draw_bitmap(visible_bitmap, x, y, 0);
    al_draw_rectangle(x, y, x + visiblewidth, y + visibleheight, al_map_rgb(255,255,255), 0);
}

void mapgrid::set_map_bitmap(ALLEGRO_BITMAP *bmp) {
    if (!bmp) return;
    
    if (map_bitmap) {
        al_destroy_bitmap(map_bitmap);
    }

    map_bitmap = bmp;
    width = al_get_bitmap_width(map_bitmap);
    height = al_get_bitmap_height(map_bitmap);
}

bool mapgrid::is_over(int px, int py) {
    if (px < x)      return false;
    if (px > x + width)  return false;
    if (py < y)      return false;
    if (py > y + height)  return false;
    
    return true;
}

void mapgrid::scroll(int dx, int dy) {
    if (width > visiblewidth) {
        mapx -= dx;
        if (mapx < 0) {
            mapx = 0;
        }
        if (mapx + visiblewidth > width) {
            mapx = width - visiblewidth;
        }
    }
    
    if (height > visibleheight) {
        mapy -= dy;
        if (mapy < 0) {
            mapy = 0;
        }
        if (mapy + visibleheight > height) {
            mapy = height - visibleheight;
        }
    }
}

void mapgrid::handle_input(const ALLEGRO_EVENT& ev) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
        if (is_over(ev.mouse.x, ev.mouse.y)) {
            ALLEGRO_MOUSE_STATE state;
            al_get_mouse_state(&state);

            if (al_mouse_button_down(&state, 1)) {
                scroll(ev.mouse.dx, ev.mouse.dy);
                std::cout << "mapx = " << mapx << std::endl;
                std::cout << "width = " << width << std::endl;
                std::cout << "visiblewidth = " << visiblewidth << std::endl;
            }
        }
    }
}