#ifndef SCROLLBAR_HPP
#define SCROLLBAR_HPP

#include "../../sglib/src/vector2d.hpp"

class scrollbar {
    public:
        scrollbar(float x, float y, int width, int height);
        ~scrollbar();
        
        void scroll(float dx, float dy);
        void draw();
        bool is_over(float x, float y);
        
        void set_pos(float x, float y) { set_x(x); set_y(y); }
        void set_x(float x)            { pos.x = x; }
        void set_y(float y)            { pos.y = y; }
        void set_width(int w)          { width = w; }
        void set_height(int h)         { height = h; }
        
        float get_x()    { return pos.x; }
        float get_y()    { return pos.y; }
        int get_width()  { return width; }
        int get_height() { return height; }
        
    private:
        vector2d pos;
        int width;
        int height;
        
        scrollbar(const scrollbar& other);
        scrollbar& operator=(const scrollbar& other);
};

#endif //SCROLLBAR_HPP