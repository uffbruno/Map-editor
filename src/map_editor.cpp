#include <iostream>
#include <cstdlib>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "map_editor.hpp"
#include "button.hpp"

map_editor::map_editor(): display(0),
                          font(0),
                          eq(0),
                          timer(0),
                          mg(20, 20, SCREEN_W - 40, SCREEN_H / 2)
{}

map_editor::~map_editor() {
    destroy();
}

void map_editor::init() {
    if (!al_init()) {
        std::cout << "error initializing allegro" << std::endl;
        std::exit(1);
    }
    
    if (!al_install_keyboard()) {
        std::cout << "error installing keyboard" << std::endl;
        std::exit(1);
    }
    
    if (!al_install_mouse()) {
        std::cout << "error installing mouse" << std::endl;
        std::exit(1);
    }
    
    if (!al_init_image_addon()) {
        std::cout << "error initializing image addon" << std::endl;
        std::exit(1);
    }

    al_init_font_addon();
    
    if (!al_init_ttf_addon()) {
        std::cout << "error initializing ttf addon" << std::endl;
        std::exit(1);
    }
    
    if (!al_init_primitives_addon()) {
        std::cout << "error initializing primitives addon" << std::endl;
        std::exit(1);
    }
    
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        std::cout << "error creating display" << std::endl;
        std::exit(1);
    }
    
    font    = al_load_font("resources//fonts//pirulen.ttf", 10, 0);
    if (!font) {
        std::cout << "error loading font" << std::endl;
        std::exit(1);
    }
    
    eq      = al_create_event_queue();
    if (!eq) {
        std::cout << "error creating event queue" << std::endl;
        std::exit(1);
    }
    
    timer   = al_create_timer(1.0/60.0);
    if (!timer) {
        std::cout << "error creating timer" << std::endl;
        std::exit(1);
    }
    
    al_register_event_source(eq, al_get_display_event_source(display));
    al_register_event_source(eq, al_get_keyboard_event_source());
    al_register_event_source(eq, al_get_mouse_event_source());
    al_register_event_source(eq, al_get_timer_event_source(timer));
}

void map_editor::destroy() {
    if (display) {
        al_destroy_display(display);
        display = NULL;
    }
    
    if (font) {
        al_destroy_font(font);
        font = NULL;
    }
    
    if (eq) {
        al_destroy_event_queue(eq);
        eq = NULL;
    }
    
    if (timer) {
        al_destroy_timer(timer);
        timer = NULL;
    }
}

void map_editor::run() {
    button new_btn         ("New"         , font,  50, SCREEN_H - 40, 120, 20);
    button load_btn        ("Load"        , font, 170, SCREEN_H - 40, 120, 20);
    button save_btn        ("Save"        , font, 290, SCREEN_H - 40, 120, 20);
    button load_tileset_btn("Load Tileset", font, 410, SCREEN_H - 40, 120, 20);
    button exit_btn        ("Exit"        , font, 530, SCREEN_H - 40, 120, 20);
    
    if (!mg.load_map("resources//maps//map.ini")) {
        std::cout << "error loading map" << std::endl;
        std::exit(1);
    }
    
    bool exit = false;
    bool redraw = false;
    al_start_timer(timer);

    while (!exit) {
        if (redraw && al_is_event_queue_empty(eq)) {
            new_btn.draw();
            load_btn.draw();
            save_btn.draw();
            load_tileset_btn.draw();
            exit_btn.draw();
        
            mg.draw();
        
            al_flip_display();
            redraw = false;
        }
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(eq, &ev);
        
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            exit = true;
        }

        new_btn.handle_input(ev);
        load_btn.handle_input(ev);
        save_btn.handle_input(ev);
        load_tileset_btn.handle_input(ev);
        exit_btn.handle_input(ev);
        
        mg.handle_input(ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }
    }
    
    if (!mg.save_map("resources//maps//map.ini")) {
        std::cout << "error saving mapfile" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}