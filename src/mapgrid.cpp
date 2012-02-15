#include <iostream>
#include <cmath>
#include <allegro5/allegro_primitives.h>

#include "mapgrid.hpp"

mapgrid::mapgrid(int x, int y, int visiblewidth, int visibleheight): pos(x, y),
                                                                     map(0,0),
                                                                     tileset(0,0),
                                                                     highlight(0,0),
                                                                     old_highlight(0,0),
                                                                     width(0),
                                                                     height(0),
                                                                     visiblewidth(visiblewidth),
                                                                     visibleheight(visibleheight),
                                                                     selected_id(0),
                                                                     m(),
                                                                     hsb(0,0,0,0),
                                                                     vsb(0,0,0,0),
                                                                     map_bitmap(0),
                                                                     visible_bitmap(0),
                                                                     tileset_bitmap(0) {
}

mapgrid::~mapgrid() {
    if (tileset_bitmap) {
        al_destroy_bitmap(tileset_bitmap);
    }
}

void mapgrid::init_visual_parameters() {
    width = m.get_width();
    height = m.get_height();

    m.set_map_visible_size(width, height);
    
    if (map_bitmap) {
        al_destroy_bitmap(map_bitmap);
    }

    if (tileset_bitmap) {
        al_destroy_bitmap(tileset_bitmap);
    }
    
    map_bitmap = al_create_bitmap(width, height);
    visible_bitmap = al_create_bitmap(visiblewidth, visibleheight);
    tileset_bitmap = m.create_tileset_bitmap(visiblewidth);
        
    tileset.x = pos.x;
    tileset.y = pos.y + visibleheight + 20;
    
    if (width > visiblewidth) {
        float sbwidth = static_cast<float>(visiblewidth) / width;
        sbwidth *= visiblewidth;
        
        hsb.set_pos(pos.x, pos.y + visibleheight + 1);
        hsb.set_width(sbwidth);
        hsb.set_height(20);
    }

    if (height > visibleheight) {
        float sbheight = static_cast<float>(visibleheight) / height;
        sbheight *= visibleheight;

        vsb.set_pos(pos.x + visiblewidth + 1, pos.y);
        vsb.set_height(sbheight);
        vsb.set_width(20);
    }
}

bool mapgrid::new_map() {
    if (!m.new_map()) {
        return false;
    }
    
    init_visual_parameters();
    
    return true;
}

bool mapgrid::load_map(const std::string& mapfile) {
    if (!m.load_map(mapfile)) {
        return false;
    }
    
    init_visual_parameters();
    
    return true;
}

bool mapgrid::save_map(const std::string& mapfile) {
    return m.save_map(mapfile);
}

void mapgrid::set_tile_id(unsigned int x, unsigned int y, unsigned int id) {
    m.set_tile_id(x, y, id);
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
    al_draw_bitmap_region(map_bitmap, map.x, map.y, visiblewidth, visibleheight, 0, 0, 0);
    
    //draws tileset
    al_set_target_bitmap(tileset_bitmap);
    al_draw_rectangle(tileset.x, tileset.y,
        al_get_bitmap_width(tileset_bitmap),
        al_get_bitmap_height(tileset_bitmap),
        al_map_rgb(255,255,255), 
        0);
    
    //draws highlighted tile rectangle    
    al_draw_rectangle(old_highlight.x, old_highlight.y, 
        old_highlight.x + m.get_cell_size(), 
        old_highlight.y + m.get_cell_size(),
        al_map_rgb(0,0,0), 
        0);
    al_draw_rectangle(highlight.x, highlight.y, 
        highlight.x + m.get_cell_size(), 
        highlight.y + m.get_cell_size(),
        al_map_rgb(255,0,0),
        0);

    
    //restore older target bitmap
    al_set_target_bitmap(old_target);

    //draws scrollbars 
    hsb.draw();
    vsb.draw();
    
    //draws visible bitmap to older target bitmap (normally the display backbuffer)
    al_draw_bitmap(visible_bitmap, pos.x, pos.y, 0);
    al_draw_rectangle(pos.x, pos.y, pos.x + visiblewidth, pos.y + visibleheight, al_map_rgb(255,255,255), 0);
    
    //draws tileset bitmap
    al_draw_bitmap(tileset_bitmap, tileset.x, tileset.y, 0);
    
}

bool mapgrid::is_over(const int px, const int py, 
    const int mx, const int my, ALLEGRO_BITMAP *bmp) const {
    
    if (!bmp) return false;
    
    const int width = al_get_bitmap_width(bmp);
    const int height = al_get_bitmap_height(bmp);

    if (px < mx)          return false;
    if (px > mx + width)  return false;
    if (py < my)          return false;
    if (py > my + height) return false;
    
    return true;
}

void mapgrid::scroll(int dx, int dy) {
    if (width > visiblewidth) {
        //calculate how much the scroll bar will move for one pixel scroll of the map.
        float mapx_scroll_interval = width - visiblewidth;
        float scrollbar_scroll_interval = visiblewidth - hsb.get_width();
        float scroll_proportion = mapx_scroll_interval / scrollbar_scroll_interval;

        map.x += scroll_proportion * dx;
        if (map.x < 0) {
            map.x = 0;
        }
        if (map.x + visiblewidth > width) {
            map.x = width - visiblewidth;
        }
        
        float sbx = hsb.get_x() + dx;
        if (sbx < pos.x) {
            sbx = pos.x;
        }
        else if (sbx > pos.x + visiblewidth - hsb.get_width()) {
            sbx = pos.x + visiblewidth - hsb.get_width();
        }
        hsb.set_x(sbx);
    }
    
    if (height > visibleheight) {
        //calculate how much the map will scroll for one pixel of the scrollbar.
        float mapy_scroll_interval = height - visibleheight;
        float scrollbar_scroll_interval = visibleheight - vsb.get_height();
        float scroll_proportion = mapy_scroll_interval / scrollbar_scroll_interval;

        map.y += scroll_proportion * dy;
        if (map.y < 0) {
            map.y = 0;
        }
        if (map.y + visibleheight > height) {
            map.y = height - visibleheight;
        }

        float sby = vsb.get_y() + dy;
        if (sby < pos.y) {
            sby = pos.y;
        }
        else if (sby > pos.y + visibleheight - vsb.get_height()) {
            sby = pos.y + visibleheight - vsb.get_height();
        }
        vsb.set_y(sby);
    }
}

int mapgrid::get_tile_id(const int x, const int y) const {
    int tileset_width  = al_get_bitmap_width(tileset_bitmap);
    int tileset_height = al_get_bitmap_height(tileset_bitmap);
    
    if (x > tileset_width || y > tileset_height) return 0;
    
    unsigned int row = (y / m.get_cell_size());
    unsigned int col = (x / m.get_cell_size());

    std::cout << "row = " << row << std::endl;
    std::cout << "col = " << col << std::endl;
    
    unsigned int cols = std::ceil(static_cast<double>(tileset_width) / m.get_cell_size());
    std::cout << "cols = " << cols << std::endl;
    
    return row * cols + col;
}

void mapgrid::highlight_tile(const int x, const int y) {
    int tileset_width  = al_get_bitmap_width(tileset_bitmap);
    int tileset_height = al_get_bitmap_height(tileset_bitmap);
    
    if (x > tileset_width || y > tileset_height) return;
    
    unsigned int row = (y / m.get_cell_size());
    unsigned int col = (x / m.get_cell_size());
    
    old_highlight.x = highlight.x;
    old_highlight.y = highlight.y;
    
    highlight.x = col * m.get_cell_size();
    highlight.y = row * m.get_cell_size();
}

bool mapgrid::handle_input(const ALLEGRO_EVENT& ev) {
    bool changes_made = false;
    
    if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
        bool over_hsb = hsb.is_over(ev.mouse.x, ev.mouse.y);
        bool over_vsb = vsb.is_over(ev.mouse.x, ev.mouse.y);
        if (over_hsb || over_vsb) {
            ALLEGRO_MOUSE_STATE state;
            al_get_mouse_state(&state);

            if (al_mouse_button_down(&state, 1)) {
                int dx = over_hsb ? ev.mouse.dx : 0;
                int dy = over_vsb ? ev.mouse.dy : 0;
                scroll(dx, dy); 
            }
        }
    }
    
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        ALLEGRO_MOUSE_STATE state;
        al_get_mouse_state(&state);

        if (is_over(ev.mouse.x, ev.mouse.y, pos.x, pos.y, visible_bitmap)) {
             
             //set selected tile to map.
            if (al_mouse_button_down(&state, 1)) {
                m.set_tile_id(ev.mouse.x + map.x - this->pos.x, ev.mouse.y + map.y - this->pos.y, selected_id);
                changes_made = true;
            } 
            
            //set a tile as passable or not passable.
            else if (al_mouse_button_down(&state, 2)) { 
                int posx = ev.mouse.x - pos.x + map.x;
                int posy = ev.mouse.y - pos.y + map.y;
                
                //toggle tile's passable flag.
                m.set_tile_passable(posx, posy, !m.get_tile_passable(posx, posy));
                changes_made = true;
            }
        }
        
        //select tile in tileset.
        else if (is_over(ev.mouse.x, ev.mouse.y, tileset.x, tileset.y, tileset_bitmap)) { 
            int posx = ev.mouse.x - tileset.x;
            int posy = ev.mouse.y - tileset.y;

            highlight_tile(posx, posy);

            selected_id = get_tile_id(posx, posy);
        }
    }
    
    return changes_made;
}

void mapgrid::toggle_grid() {
    m.toggle_grid();
}