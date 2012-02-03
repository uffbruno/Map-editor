#include "tile.hpp"

tile::~tile() {
    if (image) {
        al_destroy_bitmap(image);
        image = NULL;
    }
}

void tile::draw() const {
    if (image) {
        al_draw_bitmap(image, bb.get_x(), bb.get_y(), 0);
    }
}