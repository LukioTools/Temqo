
#include <iostream>

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


    inline void cb(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
        std::cout << "Callback Called " << frameCount <<"\n";


        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        if (pDecoder == NULL) {
            return;
        }

        ma_decoder_read_pcm_frames(&decoder, pOutput, frameCount, NULL);

        (void)pInput;
    }


    inline int play(std::string filename){
        check(ma_decoder_init_file(filename.c_str(), nullptr, &decoder) != MA_SUCCESS, "Decoder initialisation failed for file: " + filename);
        return 0;
    }

    inline int start(){
        check_cb(
            ma_device_start(&device) != MA_SUCCESS, 
            "Starting playback device failed", 
            ma_decoder_uninit(&decoder);, 
            std::runtime_error
        );
        return 0;
    }

    inline ma_result stop(){
        return ma_device_stop(&device);
    }

    inline int uninit(){
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return 0;
    }

    inline int init(){
        config.playback.format   = ma_format_unknown;   // Set to ma_format_unknown to use the device's native format.
        config.playback.channels = 0;               // Set to 0 to use the device's native channel count.
        config.sampleRate        = 0;           // Set to 0 to use the device's native sample rate.
        config.dataCallback      = cb;   // This function will be called when miniaudio needs more data.
        /*idk if it is necessary */ // config.pUserData         = pMyCustomData;   // Can be accessed from the device object (device.pUserData).
        // Initialize the playback device with the format from the decoder
        ma_device_init(NULL, &config, &device);
        device.pUserData = &decoder;
        return 0;
    }

} // namespace audio



#endif // AUD_BACKEND
