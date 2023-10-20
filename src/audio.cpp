#include "audio_backend.hpp"
#include <iostream>
std::string filename("src/bullet.mp3");
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
    printf("filename: %s\n", filename.c_str());

    audio::init(filename);
    audio::start();
    bool playing = true;
    while (char c = getchar())
    {
        if(c == 's'){
            playing = !playing;
            playing ? audio::start() : audio::stop();
        }
        if(c == 'q'){
            if(playing){
                audio::stop();
                playing = false;
            }
            audio::uninit();
            break;
        }
    }
    
    
    return 0;
}

