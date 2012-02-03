#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <allegro5/allegro.h>

#include "vector2d.hpp" 

class bounding_box {
    public:
        bounding_box(float x, float y, float w, float h, bool ic = false);

        ~bounding_box();
        
        float left() const   { return center.x - w/2; }
        float top() const    { return center.y - h/2; }
        float right() const  { return center.x + w/2; }
        float bottom() const { return center.y + h/2; }
        
        void set_x(float x) { center.x = x; }
        void set_y(float y) { center.y = y; }
        void set_w(int w)   { this->w = w; }
        void set_h(int h)   { this->h = h; }
        
        float get_x() const { return center.x; }
        float get_y() const { return center.y; }
		float get_w() const { return w; }
		float get_h() const { return h; }
        
        bool get_ignore_collision() { return ignore_collision; }
        void set_ignore_collision(bool ignore_collision) 
            { this->ignore_collision = ignore_collision; }

        void draw() const;
        void update();
        void set_color(ALLEGRO_COLOR color) { this->color = color; }
        
        bool collide(const bounding_box& other, vector2d& normal) const;

    private:
        vector2d center;
		
        float w;
        float h;
        
        bool ignore_collision;
      
        ALLEGRO_COLOR color;
        bounding_box(const bounding_box& other);
        bounding_box& operator=(const bounding_box& other);

};

#endif //BOUNDING_BOX_HPP