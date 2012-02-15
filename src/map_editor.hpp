#ifndef MAP_EDITOR_HPP
#define MAP_EDITOR_HPP

#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "mapgrid.hpp"

const int SCREEN_W = 1024;
const int SCREEN_H = 768;

class map_editor {
    public:
        map_editor();
        ~map_editor();
        
        //essential functions - you only need to call these.
        void init();
        void run();

        //callback functions
        static void new_map_cb(map_editor* const m);
        static void load_map_cb(map_editor* const m);
        static void save_map_cb(map_editor* const m);
        static void load_tileset_cb(map_editor* const m);
        static void exit_cb(map_editor* const m);
        
        //getters
        ALLEGRO_DISPLAY* get_display()   const { return display;          }
        bool             is_map_saved()  const { return map_saved;        }
        bool             get_quit()      const { return quit;             }
        
        //setters
        void set_map_saved(const bool saved) { map_saved        = saved; }
        void set_quit(const bool q)          { quit             = q;     }

        bool new_map();
        bool save_map(const char *name);
        bool load_map(const char *name);
        
    private:
        //allegro handlers
        ALLEGRO_DISPLAY *display;
        ALLEGRO_FONT *font;
        ALLEGRO_EVENT_QUEUE *eq;
        ALLEGRO_TIMER *timer;

        bool map_saved;
        bool quit;
        
        mapgrid mg;

        static void ask_to_save(map_editor* const m);
        
        void destroy();
        
        map_editor(const map_editor&);
        map_editor& operator=(const map_editor&);
};

#endif //MAP_EDITOR_HPP