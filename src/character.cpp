#include <sstream>
#include <fstream>
#include <iostream>

#include "character.hpp"

character::character(float pos_x, float pos_y, float speed_x, float speed_y, 
    const std::string& directory): game_object(pos_x, pos_y, 0, 0), 
                                   speed(speed_x, speed_y), 
                                   spr(),
                                   loaded(false), 
                                   state(0), 
                                   directory(directory.c_str()) {
    keys[KEY_LEFT]  = false; 
    keys[KEY_RIGHT] = false;
    keys[KEY_UP]    = false;
    keys[KEY_DOWN]  = false;
}

character::~character() {
}

void character::init() {
    if (!loaded) {
        loaded = load_frames();
    }
}

bool character::load_frames() {
    std::ostringstream oss;
    oss << directory << "//anim_info.txt";

    std::ifstream file(oss.str().c_str());
  
    if (!file) {
        std::cout << "error opening " << oss.str().c_str() << std::endl;
        return false;
    }
    
    std::string line;
    animation *anim = NULL;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#' || line[0] == '\n') {
            continue;
        } else if (line[0] == '.') {
            if (anim != NULL) {
                spr.add_animation(anim);
            }
            
            anim = NULL;
        } else {
            std::ostringstream oss_frame;        
            oss_frame << directory << "//" << line;

            if (anim == NULL) {
                anim = new animation(8.0);
            }

            std::cout << "opening " << oss_frame.str().c_str() << std::endl;
            ALLEGRO_BITMAP *image = al_load_bitmap(oss_frame.str().c_str());
            if (image == NULL) {
                std::cout << "error loading frame" << std::endl;
            } else {
                frame *f = new frame(image);
                anim->add_frame(f);
            }
        }
    }
    
    file.close();
    return true;
}

vector2d character::handle_collision(const game_object* other) {
    if (other == NULL) return vector2d(0,0);
    
    vector2d normal(0,0);

    if (bb.collide(other->get_bounding_box(), normal)) {
        //push this box out of the other one
        bb.set_x(this->bb.get_x() + normal.x);
        bb.set_y(this->bb.get_y() + normal.y);
    }
    
    return normal;
}

void character::set_state(const unsigned int state) {
    unsigned int num_animations = spr.get_num_animations();
    if (state > num_animations) {
        this->state = 0;
    }
    else {
        this->state = state;
    }
    
    spr.set_animation(state);
    update_bounding_box();
}

void character::handle_input(const ALLEGRO_EVENT& ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                keys[KEY_UP] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_DOWN:
                keys[KEY_DOWN] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                keys[KEY_LEFT] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[KEY_RIGHT] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
        }
    }
}

void character::update_bounding_box() {
    animation *anim = spr.get_animation();
    frame *f = anim->get_frame();
    
    bb.set_w(al_get_bitmap_width(f->image));
    bb.set_h(al_get_bitmap_height(f->image));
}

void character::update() {
    if (loaded) {
        state = impl_update();
        
        //animates the character
        spr.set_animation(state);
        spr.run_animation();
    }
}

void character::print() {
    std::cout << "center.x      = " << bb.get_x() << std::endl;
    std::cout << "center.y      = " << bb.get_y() << std::endl;
}

void character::draw() {
    if (loaded) {
        impl_draw();
    }
}