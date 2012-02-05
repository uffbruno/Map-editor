#ifndef MAP_EDITOR_HPP
#define MAP_EDITOR_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "mapgrid.hpp"

const int SCREEN_W = 1024;
const int SCREEN_H = 768;

class map_editor {
    public:
        map_editor();
        ~map_editor();
        
        void init();        
        void run();
        
    private:
        ALLEGRO_DISPLAY *display;
        ALLEGRO_FONT *font;
        ALLEGRO_EVENT_QUEUE *eq;
        ALLEGRO_TIMER *timer;
        
        mapgrid mg;


        void destroy();
        
        map_editor(const map_editor&);
        map_editor& operator=(const map_editor&);
};

#endif //MAP_EDITOR_HPP