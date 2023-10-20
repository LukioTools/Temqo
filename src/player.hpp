

#include <cstring>
#ifdef __NCURSES_H
#include <ncurses.h>
#endif

#include <set>
#if !defined(PLAYER)
#define PLAYER
#include "macros.hpp"
#include <dirent.h>
#include <filesystem>
#include <string>
#include "audio_backend.hpp"

#include "rgb.hpp"


RGB<> hilight_color = {50,50,50};

#define PLAYER_MOD_RECURSIVE 0x00000001
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

#ifdef __NCURSES_H

    void out_ncurses(WINDOW* window, unsigned int x_offset = 2, unsigned int y_offset = 1, unsigned int read_offset = 0, unsigned int max_lines = 0, const char* prefix = "", const char* postfix = ""){
        auto sz = files.size();
        auto begin = files.begin();
        auto end = files.end();
        size_t index = 0;
        if(read_offset < sz){
            std::advance(begin, read_offset);
        }
        for (auto str = begin; str != end; str++)
        {
            auto i = index + read_offset;
            if( index > max_lines && max_lines != 0){
                break;
            }
            mvwprintw(window, x_offset, y_offset+index, "%s%s%s", prefix, files., postfix);
            index++;
        }
        return;
        
    }

#endif

    std::string to_string(const char* sep = "\n "){
        std::string out;
        for(auto str : files){
            out += str + sep;
        }
        return out;
    }

    std::string to_string_ansi_hilighted(const char* sep = "\n "){
        std::string out;
        size_t i = 0;
        logvar(index)
        for(auto str : files){
            logvar(i)
            if(i==index){
                out += rgb_bgstr(hilight_color) + str + bg_reset + sep;
            }
            else{
                out += str + sep;
            }
            i++;
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

    
    Player() {}
    ~Player() {}
};


#endif // PLAYER


