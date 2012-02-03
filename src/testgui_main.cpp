#include <iostream>
#include <cstdlib>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "button.hpp"
#include "mapgrid.hpp"

void my_function() {
    std::cout << "callback function 1" << std::endl;
}

void my_function2() {
    std::cout << "callback function 2" << std::endl;
}

void my_function3() {
    int x = 3;
    int y = 2;
    std::cout << "sum of " << x << " + " << y << " = " << x + y << std::endl;
}

int main() {
    al_init();
    
    al_install_keyboard();
    al_install_mouse();
    
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    
    ALLEGRO_DISPLAY *display = al_create_display(640, 480);
    ALLEGRO_FONT *font = al_load_font("resources//fonts//pirulen.ttf", 10, 0);
    ALLEGRO_EVENT_QUEUE *eq = al_create_event_queue();
    
    al_register_event_source(eq, al_get_display_event_source(display));
    al_register_event_source(eq, al_get_keyboard_event_source());
    al_register_event_source(eq, al_get_mouse_event_source());
    
    button b1("New", font, 50, 440, 120, 20);
    button b2("Load", font, 170, 440, 120, 20);
    button b3("Save", font, 290, 440, 120, 20);
    button b4("Load Tilemap", font, 410, 440, 120, 20);
    button b5("Exit", font, 530, 440, 120, 20);
    
    b1.set_onclick_callback(my_function);
    b2.set_onclick_callback(my_function2);
    b3.set_onclick_callback(my_function3);
    b4.set_onclick_callback(my_function);
    b5.set_onclick_callback(my_function2);
    
    mapgrid mg(20, 20, 600, 200);
    ALLEGRO_BITMAP *map_bitmap = al_load_bitmap("resources//images//output1.png");
    
    //TODO: implement handler for std::atexit
    if (!map_bitmap) {
        std::cout << "error loading map bitmap" << std::endl;
        std::exit(1);
    }
    
    if (!mg.load_map("resources//maps//map.ini")) {
        std::cout << "error loading map.ini" << std::endl;
        std::exit(1);
    }
    
    bool exit = false;
    
    while (!exit) {
        b1.draw();
        b2.draw();
        b3.draw();
        b4.draw();
        b5.draw();
        
        mg.draw();
        
        al_flip_display();
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(eq, &ev);
        
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            exit = true;
        }
        
        b1.handle_input(ev);
        b2.handle_input(ev);
        b3.handle_input(ev);
        b4.handle_input(ev);
        b5.handle_input(ev);
        
        mg.handle_input(ev);
    }
    
    al_destroy_display(display);
    al_destroy_event_queue(eq);
    
    return 0;
}