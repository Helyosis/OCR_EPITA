#ifndef _test_app_h
#define _test_app_h

typedef struct {
    uint8_t* rgb_image;
    int height;
    int width;
    int bpp;
} image;

image* load_image(string filename);

#endif
