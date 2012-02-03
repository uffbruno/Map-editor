#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>

#include <allegro5/allegro_font.h>

typedef void (*callback_type)();

class button {
    public:
        button(const std::string& label, ALLEGRO_FONT *font, int x, int y, int w, int h);
        ~button();
        
        void set_x(int x) { this->x = x; }
        void set_y(int y) { this->y = y; }
        void set_w(int w) { this->w = w; }
        void set_h(int h) { this->h = h; }
        
        int get_x() const { return x; }
        int get_y() const { return y; }
        int get_w() const { return w; }
        int get_h() const { return h; }
        
        void set_onclick_callback(callback_type ct) { this->ct = ct; }
        void handle_input(const ALLEGRO_EVENT& ev);
        void draw();
    
    private:
        std::string label;
        ALLEGRO_FONT *font;
        int x, y, w, h;
        callback_type ct;
        ALLEGRO_COLOR color;
        ALLEGRO_COLOR mouseover_color;
        
        bool is_over(int x, int y) const;
        button(const button& other);
        button& operator=(const button& other);
};

#endif //BUTTON_HPP