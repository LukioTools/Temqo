


#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <vector>



std::ofstream different_terminal("/dev/pts/3");


#define LOG_STREAM different_terminal
#include "macros.hpp"
#define LOG_WIN log_scr
#include "nc_macros.hpp"

#include "ascii_img.hpp"
#include <libpng16/png.h>


void writePNG(const char* filename, const unsigned char* data, int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file for writing: %s\n", filename);
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        fprintf(stderr, "Error: Failed to create PNG write structure\n");
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        fprintf(stderr, "Error: Failed to create PNG info structure\n");
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        fprintf(stderr, "Error: libpng error during PNG creation\n");
        return;
    }

    png_init_io(png, fp);

    png_set_IHDR(
        png, info, width, height,
        8,           // Bit depth (8 bits per channel)
        PNG_COLOR_TYPE_RGB, // RGB color
        PNG_INTERLACE_NONE, // Non-interlaced
        PNG_COMPRESSION_TYPE_DEFAULT, // Default compression
        PNG_FILTER_TYPE_DEFAULT // Default filter
    );

    png_write_info(png, info);

    // Allocate memory for each row
    png_bytep row_pointers[height];
    for (int i = 0; i < height; i++) {
        row_pointers[i] = (unsigned char*) &data[i * width * 3]; // Assuming RGB format (3 channels)
    }

    // Write the image data
    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    // Cleanup
    png_destroy_write_struct(&png, &info);
    fclose(fp);

    printf("PNG image saved to %s\n", filename);
}


int main(int argc, char const *argv[])
{
    constexpr int x=100, y=100;
    auto values = ascii_img::load_image("test.png", x, y);
    for (size_t i = 0; i < x*y; i++)
    {
        logv( "Values: " << (int) values[i].r<< " "<<(int) values[i].g<< " " << (int) values[i].b)
        break;
    }

    std::vector<uint8_t> imageData(x * y * 3, 255);

    writePNG("out.png", (const u_char*) values, x, y);    

    delete[] values;
    
    return 0;
}
