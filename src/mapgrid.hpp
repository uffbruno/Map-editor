#ifndef MAPGRID_HPP
#define MAPGRID_HPP

#include <string>

#include <allegro5/allegro.h>

#include "map2d.hpp"

class mapgrid {
    public:
        mapgrid(int x, int y, int visiblewidth, int visibleheight);
        ~mapgrid();
        
        bool load_map(const std::string& mapfile);
        bool save_map(const std::string& mapfile);
        
        void set_tile_id(unsigned int x, unsigned int y, unsigned int id);
        
        void draw();
        void handle_input(const ALLEGRO_EVENT& ev);
        
        //temporary event to test widget; remove later.
        void set_map_bitmap(ALLEGRO_BITMAP *map_bitmap);
        
    private:
        int x;
        int y;
        int visiblewidth;
        int visibleheight;
        int mapx;
        int mapy;
        int width;
        int height;
        
        map2d m;
        
        ALLEGRO_BITMAP *map_bitmap;
        ALLEGRO_BITMAP *visible_bitmap;
        bool is_over(int px, int py);
        void scroll(int dx, int dy);
        
        mapgrid(const mapgrid& other);
        mapgrid& operator=(const mapgrid& other);
};

#endif //MAP_GRID_HPP