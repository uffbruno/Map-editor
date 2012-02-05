#ifndef MAP2D_HPP
#define MAP2D_HPP

#include <vector>
#include <allegro5/allegro_image.h>

#include "tile.hpp"

typedef std::vector<tile*> cell_collection;
typedef std::vector<ALLEGRO_BITMAP*> tileset;

class map2d {
    public:
        map2d();
        ~map2d();
        
        bool load_map(const std::string& mapname);
        bool save_map(const std::string& mapname);
        bool load_tileset(const std::string& mapname, 
                          int dimension, 
                          int gap_x = 1, 
                          int gap_y = 1, 
                          int offset = 0);
                          
        void draw() const;
        
        void set_tile_id(unsigned int x, unsigned int y, unsigned int id);
        
        unsigned int get_width() const { return width; }
        unsigned int get_height() const { return height; }
    
    private:
        unsigned int cell_size;
        unsigned int rows;
        unsigned int cols;
        unsigned int width;
        unsigned int height;
        unsigned int tile_horizontal_gap;
        unsigned int tile_vertical_gap;
        unsigned int offset;
        std::string tileset_filename;
        unsigned int default_tile_id;
        
        cell_collection cells;
        tileset tiles;
        
        unsigned int calc_width() const;
        unsigned int calc_height() const;
    
        bool read_tileset_information(const ALLEGRO_CONFIG *config);
        void draw_tileset() const;
       
        map2d(const map2d& other);
        map2d& operator=(const map2d& other);
        
        
};

#endif //MAP2D_HPP