
#include <iostream>
#include <stdexcept>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

#if !defined(AUD_BACKEND)
#define AUD_BACKEND

#include "macros.hpp"

namespace audio
{

    ma_device device;
    ma_decoder decoder;
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    ma_device_data_proc vis_callback = nullptr;

    inline int uninit(){
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return 0;
    }


    inline void cb(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
        //std::cout << "Callback Called " << frameCount <<"\n";
        if(vis_callback){ /*if sum future shit exist, call it*/
            vis_callback(pDevice, pOutput, pInput, frameCount);
        }

        ma_decoder_read_pcm_frames(&decoder, pOutput, frameCount, NULL);
    }


    inline int start(){
        check_cb(ma_device_start(&device), "Starting playback device failed", uninit();, std::runtime_error);
        return 0;
    }

    inline ma_result stop(){
        return ma_device_stop(&device);
    }

    

    inline int init(std::string filename){
        ma_device_config deviceConfig;

        check(ma_decoder_init_file(filename.c_str(), NULL, &decoder), "Could not load file: " + filename)

        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.format   = decoder.outputFormat;
        deviceConfig.playback.channels = decoder.outputChannels;
        deviceConfig.sampleRate        = decoder.outputSampleRate;
        deviceConfig.dataCallback      = cb;
        deviceConfig.pUserData         = &decoder;

        check_cb(ma_device_init(NULL, &deviceConfig, &device), "Failed to open playback device", ma_decoder_uninit(&decoder);, std::runtime_error)
        return 0;
    }

} // namespace audio



#endif // AUD_BACKEND
