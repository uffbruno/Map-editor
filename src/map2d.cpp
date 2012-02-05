#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "map2d.hpp"

map2d::map2d(): cell_size(0), 
                rows(0), 
                cols(0), 
                width(0), 
                height(0), 
                tile_horizontal_gap(0), 
                tile_vertical_gap(0), 
                offset(0), 
                tileset_filename(), 
                default_tile_id(0),  
                cells(), 
                tiles()
 {

}

map2d::~map2d() {
    cells.clear();
    for (unsigned int i = 0; i < tiles.size(); ++i) {
        if (tiles[i]) al_destroy_bitmap(tiles[i]);
        tiles.clear();
    }
}

bool map2d::load_map(const std::string& mapname) {
    ALLEGRO_CONFIG *map_config = al_load_config_file(mapname.c_str());
    if (!map_config) {
        std::cout << "error loading config file for map" << std::endl;
        return false;
    }
    
    std::cout << "loading " << mapname << std::endl;
    
    if (!read_tileset_information(map_config)) {
        std::cout << "error reading tileset information" << std::endl;
        return false;
    };
    
    const char *number_of_rows = al_get_config_value(map_config, "map", "number_of_rows");
    if (!number_of_rows) {
        std::cout << "number_of_rows key doesn't exist" << std::endl;
        return false;
    }
    
    const char *number_of_cols = al_get_config_value(map_config, "map", "number_of_cols");
    if (!number_of_cols) {
        std::cout << "number_of_cols key doesn't exist" << std::endl;
        return false;
    }
    
    const char *default_tileid = al_get_config_value(map_config, "map", "default_tile_id");
    
    rows            = std::atoi(number_of_rows);
    cols            = std::atoi(number_of_cols);
    default_tile_id = std::atoi(default_tileid);
    
    if (!rows || !cols) {
        std::cout << "invalid row / column value" << std::endl;
        return false;
    }
    
    int pos_y = 0;
    for (unsigned int i = 0; i < rows; ++i) {
        std::ostringstream oss_row;
        oss_row << "row " << i;

        int pos_x = 0;
        for (unsigned int j = 0; j < cols; ++j) {
            std::ostringstream oss_col;
            oss_col << "col_" << j << "_id";
            const char *sid = al_get_config_value(map_config, oss_row.str().c_str(), 
                                                       oss_col.str().c_str());

            int id = default_tile_id;
            if (sid) {
                id = std::atoi(sid);
            }
            
            tile *t = new tile(pos_x, pos_y, cell_size, cell_size, id);
            cells.push_back(t);
            pos_x += cell_size;
        }
        pos_y += cell_size;
    }
    
    al_destroy_config(map_config);
    
    width = calc_width();
    height = calc_height();
    
    return true;
}

bool map2d::save_map(const std::string& mapname) {
    ALLEGRO_CONFIG* config = al_create_config();
    
    const int max_chars = 100;
    char *values = new char[max_chars];
    
    //tileset information
    al_add_config_section(config, "tileset");
    al_set_config_value(config, "tileset", "tileset_file"       , tileset_filename.c_str());
    
    std::sprintf(values, "%d", cell_size);
    al_set_config_value(config, "tileset", "tile_dimension"     , values);
    
    std::sprintf(values, "%d", tile_horizontal_gap);
    al_set_config_value(config, "tileset", "tile_horizontal_gap", values);
    
    std::sprintf(values, "%d", tile_vertical_gap);
    al_set_config_value(config, "tileset", "tile_vertical_gap"  , values);
    
    std::sprintf(values, "%d", offset);
    al_set_config_value(config, "tileset", "offset"             , values);
    
    //map metadata
    al_add_config_section(config, "map");
    
    std::sprintf(values, "%d", rows);
    al_set_config_value(config, "map", "number_of_rows", values);
    
    std::sprintf(values, "%d", cols);
    al_set_config_value(config, "map", "number_of_cols", values);
    
    std::sprintf(values, "%d", default_tile_id);
    al_set_config_value(config, "map", "default_tile_id", values);
    
    //map data
    char *rowcat = new char[max_chars];
    char *colkeys = new char[max_chars];
    for (unsigned int i = 0; i < rows; ++i) {
        std::sprintf(rowcat, "row %d", i);
        al_add_config_section(config, rowcat);
        
        for (unsigned int j = 0; j < cols; ++j) {
            unsigned int tile_id = cells[i * cols + j]->get_id();
            
            if (tile_id != default_tile_id) {
                std::sprintf(colkeys, "col_%d_id", j);
                std::sprintf(values, "%d", cells[i * cols + j]->get_id());
                al_set_config_value(config, rowcat, colkeys, values);
            }
        }
    }
    
    delete [] values;
    delete [] colkeys;
    delete [] rowcat;
    
    if (!al_save_config_file(mapname.c_str(), config)) {
        std::cout << "error saving " << mapname << std::endl;
        return false;
    }
    
    al_destroy_config(config);
    return true;
}

bool map2d::read_tileset_information(const ALLEGRO_CONFIG *config) {
    const char* tileset_file = al_get_config_value(config, "tileset", "tileset_file");
    if (!tileset_file) {
        std::cout << "tileset_file key doesn't exist" << std::endl;
        return false;
    }
    
    const char *dimension = al_get_config_value(config, "tileset", "tile_dimension");
    if (!dimension) {
        std::cout << "tile_dimension key doesn't exist" << std::endl;
        return false;
    }
    
    //gap_x and gap_y are optional keys.
    const char *gap_x = al_get_config_value(config, "tileset", "tile_horizontal_gap");
    if (!gap_x) {
        std::cout << "tile_horizontal_gap key doesn't exist" << std::endl;
        gap_x = new char[2];
        gap_x = "1";
    }

    const char *gap_y = al_get_config_value(config, "tileset", "tile_vertical_gap");
    if (!gap_y) {
        std::cout << "tile_vertical_gap key doesn't exist" << std::endl;
        gap_y = new char[2];
        gap_y = "1";
    }
    
    const char *offset = al_get_config_value(config, "tileset", "offset");
    if (!offset) {
        std::cout << "offset key doesn't exist" << std::endl;
        offset = new char[2];
        offset = 0;
    }

    return this->load_tileset(tileset_file, 
                              std::atoi(dimension), 
                              std::atoi(gap_x), 
                              std::atoi(gap_y), 
                              std::atoi(offset));
}

bool map2d::load_tileset(const std::string& imgname, int cell_dimension, int gap_x, int gap_y, int offset) {
                         
    ALLEGRO_BITMAP *tileimg = al_load_bitmap(imgname.c_str());
    if (!tileimg) {
        std::cout << "tileset file not found: " << imgname << std::endl;
        return false;
    }
    
    this->tile_horizontal_gap = gap_x;
    this->tile_vertical_gap = gap_y;
    this->offset = offset;
    this->cell_size = cell_dimension;
    this->tileset_filename.assign(imgname.c_str());
    
    std::cout << "loading " << imgname << std::endl;
    
    unsigned int pos_x         = this->offset;
    unsigned int pos_y         = this->offset;
    unsigned int bitmap_width  = al_get_bitmap_width(tileimg);
    unsigned int bitmap_height = al_get_bitmap_height(tileimg);
    
    while (pos_y < bitmap_height) {
        if (pos_x + cell_size < bitmap_width) {
            ALLEGRO_BITMAP *tile = al_clone_bitmap(al_create_sub_bitmap(tileimg, 
                                                                        pos_x, 
                                                                        pos_y, 
                                                                        cell_size,
                                                                        cell_size));
        
            std::cout << "(x,y) = (" << pos_x << "," << pos_y << ")" << std::endl;
            std::cout << "x + width = " << pos_x + cell_size 
                      << " bitmap width = " << bitmap_width << std::endl;
            if (!tile) {
                std::cout << "error cloning tile" << std::endl;
                return false;
            }
        
            tiles.push_back(tile);
            std::cout << "tile loaded sucessfully" << std::endl;
        
            pos_x += cell_size + tile_horizontal_gap;
        } else {
            pos_x = this->offset;
            pos_y += cell_size + tile_vertical_gap;
        }
    }
    
    al_destroy_bitmap(tileimg);
    return true;
}

unsigned int map2d::calc_width() const {
    unsigned int width = 0;
    for (unsigned int i = 0; i < cols && i < cells.size(); ++i) {
        width += cells[i]->get_w();
    }
    
    return width;
}

unsigned int map2d::calc_height() const {
    unsigned int height = 0;
    for (unsigned int i = 0; i < rows; ++i) {
        height += cells[i]->get_h();
    }
    
    return height;
}

void map2d::draw() const {
    cell_collection::const_iterator i;
    for (i = cells.begin(); i != cells.end(); ++i) {
        tile *t = (*i);
        al_draw_bitmap(tiles.at(t->get_id()), t->get_x(), t->get_y(), 0);
    }
}

void map2d::set_tile_id(unsigned int x, unsigned int y, unsigned int id) {
    if (x > width || y > height || id > tiles.size()) return;

    
    unsigned int row = (y / cell_size);
    unsigned int col = (x / cell_size);

    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "row = " << row << std::endl;
    std::cout << "col = " << col << std::endl;
    
    tile *t = cells.at(row * cols + col);
    
    std::cout << "row * cols + col = " << row * cols + col << std::endl;
    t->set_id(id);
}

