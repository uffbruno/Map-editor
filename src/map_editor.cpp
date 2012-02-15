#include <iostream>
#include <cstdlib>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "map_editor.hpp"
#include "button.hpp"

map_editor::map_editor(): display(0),
                          font(0),
                          eq(0),
                          timer(0),
                          map_saved(true),
                          quit(false),
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

bool map_editor::new_map() {
    return this->mg.new_map();
}

bool map_editor::save_map(const char* name) {
    return this->mg.save_map(name);
}

bool map_editor::load_map(const char* name) {
    return this->mg.load_map(name);
}

void map_editor::run() {
    //create, label and position buttons 
    button new_btn         ("New"         , font,  50, SCREEN_H - 40, 120, 20);
    button load_btn        ("Load"        , font, 170, SCREEN_H - 40, 120, 20);
    button save_btn        ("Save"        , font, 290, SCREEN_H - 40, 120, 20);
    button load_tileset_btn("Load Tileset", font, 410, SCREEN_H - 40, 120, 20);
    button exit_btn        ("Exit"        , font, 530, SCREEN_H - 40, 120, 20);
    
    //set button callbacks
    new_btn.         set_onclick_callback(&map_editor::new_map_cb,      this);
    load_btn.        set_onclick_callback(&map_editor::load_map_cb,     this);
    save_btn.        set_onclick_callback(&map_editor::save_map_cb,     this);
    load_tileset_btn.set_onclick_callback(&map_editor::load_tileset_cb, this);
    exit_btn.        set_onclick_callback(&map_editor::exit_cb,         this);

    //load map.
    if (!mg.new_map()) {
        std::cout << "error creating new map" << std::endl;
        std::exit(1);
    }
    
    bool redraw = false;
    al_start_timer(timer);

    while (!this->quit) {
        if (redraw && al_is_event_queue_empty(eq)) {
            al_clear_to_color(al_map_rgb(0,0,0));
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
        
        //if window is closed, call exit function (prompts for save, etc.)
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            this->exit_cb(this);
        } 
        
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            
            //if ESC is pressed, call exit function (prompts for save, etc.)
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                this->exit_cb(this);
            } 
            
            //show / hide grid for map
            else if (ev.keyboard.keycode == ALLEGRO_KEY_G) {
                mg.toggle_grid();
            }
        }

        new_btn.handle_input(ev);
        load_btn.handle_input(ev);
        save_btn.handle_input(ev);
        load_tileset_btn.handle_input(ev);
        exit_btn.handle_input(ev);

        //if any changes were made to the map, mark it as not saved.
        if (mg.handle_input(ev)) {
            map_saved = false;
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }
    }
}

//---------------------------------------------------------//
// static functions from here on - to be used as callbacks //
//---------------------------------------------------------//

void map_editor::ask_to_save(map_editor* const m) {
    //if current map is not saved, let's ask if the user wants to save it first.
    if (!m->is_map_saved()) {
        int rc = al_show_native_message_box(m->get_display(), 
                                            "Warning", 
                                            "There are unsaved changes to the current map", 
                                            "Do you wish to save them?",
                                            NULL,
                                            ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO);
        if (rc) {
            save_map_cb(m);
        }
    }
}

//---------------------------------------------------------//
// starts a new map                                         //
//---------------------------------------------------------//
void map_editor::new_map_cb(map_editor* const m) {
    if (!m) return;
    
    ask_to_save(m);
    
    if (!m->new_map()) {
        std::cout << "error creating new map" << std::endl;
    }
}

//---------------------------------------------------------//
// loads existing map                                      //
//---------------------------------------------------------//
void map_editor::load_map_cb(map_editor* const m ) {
    if (!m) return;
    
    //if current map is not saved, let's ask if the user wants to save it first.
    ask_to_save(m);
    
    ALLEGRO_FILECHOOSER *fc = al_create_native_file_dialog(".", "Load map", "*.ini", 
                                                           ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
    if (!fc) {
        std::cout << "error creating load map filechooser" << std::endl;
        return;
    }
    
    if (al_show_native_file_dialog(m->get_display(), fc)) {
        const char* name = al_get_native_file_dialog_path(fc, 0);
        if (!name) {
            std::cout << "error getting file name" << std::endl;
            return;
        }
        
        m->load_map(name);
    }
    
    al_destroy_native_file_dialog(fc);
}

//---------------------------------------------------------//
// saves current map                                       //
//---------------------------------------------------------//
void map_editor::save_map_cb(map_editor* const m) {
    ALLEGRO_FILECHOOSER *fc = al_create_native_file_dialog(".", "Load map", "*.ini", 
                                                           ALLEGRO_FILECHOOSER_SAVE);
    
    if (!fc) {
        std::cout << "error creating save map filechooser" << std::endl;
        return;
    }
    
    if (al_show_native_file_dialog(m->get_display(), fc)) {
        const char *name = al_get_native_file_dialog_path(fc, 0);
        if (!name) {
            std::cout << "error getting save file name" << std::endl;
            return;
        }
        m->save_map(name);
    }
}

//---------------------------------------------------------//
// loads current tileset                                   //
//---------------------------------------------------------//
void map_editor::load_tileset_cb(map_editor* const m) {
    std::cout << "load tileset - yet to be implemented" << m << std::endl;
}

//---------------------------------------------------------//
// exits application                                       //
//---------------------------------------------------------//
void map_editor::exit_cb(map_editor* const m) {
    if (!m) return;
    
    ask_to_save(m);
    
    m->set_quit(true);
}