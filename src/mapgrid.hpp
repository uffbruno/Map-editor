#ifndef MAPGRID_HPP
#define MAPGRID_HPP

#include <string>

#include <allegro5/allegro.h>

#include "scrollbar.hpp"

#include "../../sglib/src/map2d.hpp"

class mapgrid {
    public:
        mapgrid(int x, int y, int visiblewidth, int visibleheight);
        ~mapgrid();
        
        bool new_map();
        bool load_map(const std::string& mapfile);
        bool save_map(const std::string& mapfile);
        
        void set_tile_id(unsigned int x, unsigned int y, unsigned int id);
        
        void draw();
        bool handle_input(const ALLEGRO_EVENT& ev);
        void toggle_grid();
        
    private:
        vector2d pos;
        vector2d map;
        vector2d tileset;
        vector2d highlight;
        vector2d old_highlight;

        int width;
        int height;
        int visiblewidth;
        int visibleheight;
        int selected_id;
        
        map2d m;
        
        scrollbar hsb; //horizontal scrollbar
        scrollbar vsb; //vertical scrollbar
        
        ALLEGRO_BITMAP *map_bitmap;
        ALLEGRO_BITMAP *visible_bitmap;
        ALLEGRO_BITMAP *tileset_bitmap;
        
        bool is_over(const int px, const int py, 
            const int mx, const int my, ALLEGRO_BITMAP *bmp) const;
        void scroll(int dx, int dy);
        
        void init_visual_parameters();
        
        int get_tile_id(const int x, const int y) const;
        void highlight_tile(const int x, const int y);

        
        mapgrid(const mapgrid& other);
        mapgrid& operator=(const mapgrid& other);
};

#endif //MAP_GRID_HPP