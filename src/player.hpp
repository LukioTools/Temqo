

#include <cstddef>
#include <cstring>
#include <curses.h>
#include <ncurses.h>

#include <set>
#if !defined(PLAYER)
#define PLAYER
#include "macros.hpp"
#include <dirent.h>
#include <filesystem>
#include <string>
#include "audio_backend.hpp"

#include "rgb.hpp"

//README värit jostai syystä paskana ja ei oo mitään hajua


struct ncurses_color {
    short r = 0; //0-1000
    short g = 0; //0-1000
    short b = 0; //0-1000

    operator RGB<>(){
        return {
            static_cast<unsigned char>(r*255/1000), 
            static_cast<unsigned char>(g*255/1000), 
            static_cast<unsigned char>(b*255/1000),
        };
    }

    ncurses_color(RGB<> rgb){
        r = rgb.r/255 * 1000;
        g = rgb.g/255 * 1000;
        b = rgb.b/255 * 1000;
    };

    ncurses_color(short _R, short _G, short _B){
        r = _R;
        g = _G;
        b = _B;
    };
};




#define PLAYER_MOD_RECURSIVE 0x00000001


//ncurses_color player_normal_foreground(RGB<>{160,160,160});
//ncurses_color player_normal_background(RGB<>{10,10,10});

#define COLOR_PLAYER_NORMAL_FOREGROUND 21
#define COLOR_PLAYER_NORMAL_BACKGROUND 23


//ncurses_color player_hilight_foreground(RGB<>{200,200, 200});
//ncurses_color player_hilight_background(RGB<>{75,75,75});

#define COLOR_PLAYER_HILIGHT_FOREGROUND 26
#define COLOR_PLAYER_HILIGHT_BACKGROUND 29

#define COLOR_PAIR_PLAYER_NORMAL 100
#define COLOR_PAIR_PLAYER_HILIGHT 101

/**
 * @brief regex for the supported formats
 */


class Player
{
private:
    
public:
    std::set<std::string> files;
    size_t index = 0;
    u_int mod = 0;
    u_int state = 0;


    void out_ncurses(WINDOW* window, unsigned int x_offset = 2, unsigned int y_offset = 1, unsigned int read_offset = 0, unsigned int max_lines = 0, const char* prefix = "", const char* postfix = ""){
        auto sz = files.size();
        auto begin = files.begin();
        auto end = files.end();
        size_t idx = 0;
        if(read_offset < sz){
            std::advance(begin, read_offset);
        }

        for (auto str = begin; str != end; str++)
        {
            if( idx > max_lines && max_lines != 0){
                break;
            }
            auto i = idx + read_offset;
            //if(i == index){
            //    wattron(window, COLOR_PAIR(COLOR_PAIR_PLAYER_HILIGHT));
            //    wattroff(window, COLOR_PAIR(COLOR_PAIR_PLAYER_NORMAL));
            //}
            if(i==index){
                wattron(window, COLOR_PAIR(COLOR_PAIR_PLAYER_HILIGHT));
            }
            else{
                wattron(window, COLOR_PAIR(COLOR_PAIR_PLAYER_NORMAL));
            }

            mvwprintw(window, y_offset+idx, x_offset, "%s%s%s", prefix, (*str).c_str(), postfix);
            //if(i == index){
            //    wattroff(window, COLOR_PAIR(COLOR_PAIR_PLAYER_HILIGHT));
            //    wattron(window, COLOR_PAIR(COLOR_PAIR_PLAYER_NORMAL));
            //}
            idx++;
        }

        wattroff(window, COLOR_PAIR(COLOR_PAIR_PLAYER_HILIGHT));
        wattroff(window, COLOR_PAIR(COLOR_PAIR_PLAYER_NORMAL));
        
        return;
    }


    std::string to_string(const char* sep = "\n "){
        std::string out;
        for(auto str : files){
            out += str + sep;
        }
        return out;
    }


    void play(){

    }

    void add_file(std::string file){
        if(!std::regex_match(file, audio::supported_formats)){
            #ifdef VERBOSE
                err("Unsupported filetype: " + file);
            #endif
            //***//
            return;
        }
        files.insert(file);
    }

    void add_dir(std::string file){
        /*it shall be recursive now*/
        constexpr bool recursive = true;

        struct dirent **namelist;
        int numEntries;
        numEntries = scandir(file.c_str(), &namelist, NULL,  NULL);
        check(numEntries == -1, "Scandir Failed");

        while (numEntries--) {
            std::string folder_file = file + namelist[numEntries]->d_name; //create safe copy
            if(strcmp(namelist[numEntries]->d_name , ".") == 0 || strcmp(namelist[numEntries]->d_name , "..") == 0){
                free(namelist[numEntries]);
                continue;
            }
            free(namelist[numEntries]);

            //printf("%s\n", folder_file.c_str());

            //not recursive and is_direcotry -> skip
            if(!recursive && std::filesystem::is_directory(folder_file)){
                continue;
            }
            add(folder_file);
        }

        free(namelist); //free that bad boi
        return;
    }

    Player& add(std::string file){
        check(!std::filesystem::exists(file), "Player::add: Doesnt exist : '" + file + "'");
        std::filesystem::is_directory(file) ? add_dir(file) : add_file(file);
        return *this;
    }


    void refresh_colors();

    void init_colors();
    
    Player() {}
    ~Player() {}
};

ncurses_color player_normal_foreground(1000,1000 ,0);
ncurses_color player_normal_background(0,0,1000);

ncurses_color player_hilight_foreground(0,0,1000);
ncurses_color player_hilight_background(1000,0,0);

/*
void refresh_colors() {
    init_color(COLOR_PLAYER_NORMAL_BACKGROUND, player_normal_background.r, player_normal_background.g, player_normal_background.b);
    init_color(COLOR_PLAYER_NORMAL_FOREGROUND, player_normal_foreground.r, player_normal_foreground.g, player_normal_foreground.b);

    init_color(COLOR_PLAYER_HILIGHT_BACKGROUND, player_hilight_background.r, player_hilight_background.g, player_hilight_background.b);
    init_color(COLOR_PLAYER_HILIGHT_FOREGROUND, player_hilight_foreground.r, player_hilight_foreground.g, player_hilight_foreground.b);
    
}
*/

void init_colors(){
     // Enable color support
    logvar(start_color())
    logvar(can_change_color());

    logvar(COLORS);

    

    init_color(COLOR_PLAYER_NORMAL_FOREGROUND, player_normal_foreground.r, player_normal_foreground.g, player_normal_foreground.b);
    init_color(COLOR_PLAYER_NORMAL_BACKGROUND, player_normal_background.r, player_normal_background.g, player_normal_background.b);

    init_color(COLOR_PLAYER_HILIGHT_BACKGROUND, player_hilight_background.r, player_hilight_background.g, player_hilight_background.b);
    init_color(COLOR_PLAYER_HILIGHT_FOREGROUND, player_hilight_foreground.r, player_hilight_foreground.g, player_hilight_foreground.b);

    logvar(init_pair(COLOR_PAIR_PLAYER_NORMAL, COLOR_PLAYER_NORMAL_FOREGROUND, COLOR_PLAYER_NORMAL_BACKGROUND)); 
    logvar(init_pair(COLOR_PAIR_PLAYER_HILIGHT, COLOR_PLAYER_NORMAL_FOREGROUND, COLOR_PLAYER_NORMAL_BACKGROUND)); 
}
#endif // PLAYER


