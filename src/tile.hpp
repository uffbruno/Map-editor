#ifndef TILE_HPP
#define TILE_HPP

#include "game_object.hpp"

class tile: public game_object {
    public:
        tile(ALLEGRO_BITMAP *image, int x, int y, int w, int h):  game_object(x,y,w,h), image(image) {}
        ~tile();
        
        void is_passable(bool passable) { this->bb.set_ignore_collision(passable); }
        bool is_passable() { return this->bb.get_ignore_collision(); }
        
        void draw() const;
    
    private:
        ALLEGRO_BITMAP *image;
        
        tile(const tile& other);
        tile& operator=(const tile& other);
};

#endif //TILE_HPP