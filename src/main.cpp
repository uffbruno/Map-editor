#include <iostream>
#include <cstdlib>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "map2d.hpp"

const int SCREEN_W = 800;
const int SCREEN_H = 600;

double calc_fps() {
    static double old_time = 0.0;
    double new_time = al_get_time();
    double delta = new_time - old_time;
    double fps = 1.0/(delta);
    old_time = new_time;
    return fps;
}

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        std::cout << "usage: mapeditor filename" << std::endl;
        return 1;
    }
    
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    
    //acquire allegro resources
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_FONT *fpsfont = al_load_ttf_font("resources//fonts//pirulen.ttf", 12, 0);
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    
    bool exit = false; //exit game
    bool redraw = false; //redraw screen

    al_start_timer(timer);
    
    double fps = 0.0;
    map2d m;
    
    //TODO: these
//    int map_width = m.get_rows() * m.get_cell_size();
//    int map_height = m.get_cols() * m.get_cell_size();
  
    if (!m.load_map(std::string(argv[1]))) {
        std::cout << "error loading map" << argv[1] << std::endl;
        return 1;
    };

    //TODO: these
    //ALLEGRO_BITMAP *map_bmp = al_create_bitmap(map_width, map_height);
    //ALLEGRO_BITMAP *tileset_bmp = al_create_bitmap(m.get_tileset_width(), m.get_tileset_height());
    
    while (!exit) {
        do {
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);
        
            //if user closes window
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                exit = true;
            }
        
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                //pressing ESC also exits the loop.
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    exit = true;
                }
            }
            
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            }

            if (event.type == ALLEGRO_EVENT_TIMER) {
                redraw = true;
                fps = calc_fps();
            }
            
        } while (!al_is_event_queue_empty(event_queue));
        
        if (redraw) {
            al_clear_to_color(al_map_rgb(0,0,0));
            
            al_draw_textf(fpsfont, al_map_rgb(255,255,255), 0, SCREEN_H - 12, 0, "FPS = %.0f", fps);
            m.draw();
            
            al_flip_display();
            redraw = false;
        }
    }
    
    //release allegro resources
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_font(fpsfont);

    return 0;
}