

#include "audio_backend.hpp"
#include <cstdio>

std::string filename("src/bullet.mp3");

int main(int argc, char const *argv[])
{
    audio::init();

    audio::play(filename);

    audio::start();

    getchar();

    audio::stop();

    audio::uninit();


    return 0;
}

