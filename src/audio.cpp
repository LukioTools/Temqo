#include "player.hpp"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <vector>
#include <dirent.h>

std::string filename("src/bullet.mp3");
std::string filename2("src/aleph.mp3");

int main(int argc, char const *argv[])
{
    Player p;
    p.add(filename);

    p.add("src/");

    for (auto str : p.files)
    {
        printf("file: %s\n", str.c_str());
    }
    
    return 0;
}


/*
int main(int argc, char** argv)
{
    int counter = 0;
    while (auto arg = *argv) {
        printf("%i: %s\n", counter, arg);
        argv++;
        counter++;
    }

    if(argc > 2){
        filename = argv[2];
    }
    if(argc > 2){
        filename2 = argv[3];
    }


    printf("filename: %s\n", filename.c_str());

    audio::init(filename);
    audio::start();
    bool playing = true;
    bool stop = false;

    while (stop == false)
    {
        char c = getchar();
        switch (c)
        {
        case 's':
            playing = !playing;
            playing ? audio::start() : audio::stop();
            break;
        case 'n':
            if(playing){
                audio::stop();
            }
            audio::init(filename2);
            audio::start();
            playing = true;
            break;
        case 'q':
            std::cout << "Quitiing\n";
            if(playing){
                audio::stop();
                playing = false;
            }
            audio::uninit();
            stop = true;
            break;

        default:
            break;
        }

    }

    return 0;
}
*/
