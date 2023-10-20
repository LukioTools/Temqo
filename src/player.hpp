

#include <cstring>
#include <set>
#if !defined(PLAYER)
#define PLAYER
#include "macros.hpp"
#include <dirent.h>
#include <filesystem>
#include <string>
#include "audio_backend.hpp"

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

    
    Player() {}
    ~Player() {}
};


#endif // PLAYER


