#ifndef MAP2D_HPP
#define MAP2D_HPP

#include <vector>
#include <allegro5/allegro_image.h>

struct cell {
    int x;
    int y;
    int w;
    int h;
    
    unsigned int id;
};

typedef struct cell cell;
typedef std::vector<cell> cell_collection;
typedef std::vector<ALLEGRO_BITMAP*> tileset;

class map2d {
    public:
        map2d();
        ~map2d();
        
        bool load_map(const std::string& mapname);
        bool load_tileset(const std::string& mapname, int dimension, int gap_x = 1, int gap_y = 1);
        void make_default();
        void print();
        void set_id(int x, int y, int id);
        void draw() const;
    
    private:
        int cell_size;
        int rows;
        int cols;
        cell_collection cells;
        tileset tiles;
        
        bool read_tileset_information(const ALLEGRO_CONFIG *config);
        void draw_tileset() const;
        
        map2d(const map2d& other);
        map2d& operator=(const map2d& other);
        
        
};

#endif //MAP2D_HPP