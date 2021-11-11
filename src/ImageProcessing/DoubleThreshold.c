#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#pragma GCC diagnostic pop

#define highThresholdRatio 0.09
#define lowThresholdRatio  0.05

#define WeakValue   25
#define StrongValue 255

double* DoubleThreshold(double* img, int nb_pixels) {
    double maxi = 0;
    for (int i = 0; i < nb_pixels; ++i)
        if (img[i] > maxi) maxi = img[i];

    printf("maxi = %f\n", maxi);
    double highThreshold = maxi * highThresholdRatio;
    double lowThreshold  = highThreshold * lowThresholdRatio;

    double* res = calloc(nb_pixels, sizeof(*res));
    for (int i = 0; i < nb_pixels; ++i) {
        if (img[i] > highThreshold)
            res[i] = StrongValue;
        else if (img[i] > lowThreshold)
            res[i] = WeakValue;
        // The zeros are already set with calloc()
    }

    return res;
}

double* Hysteresis(double* img, int h, int w) {
    double* result = malloc(h * w * sizeof(double));
    memcpy(result, img, h * w * sizeof(double));

    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            if (img[y * w +x] == WeakValue) {
                if (img[(y - 1) * w + x - 1] == StrongValue ||
                    img[(y - 1) * w + x] == StrongValue ||
                    img[(y - 1) * w + x + 1] == StrongValue ||
                    img[y * w + x - 1] == StrongValue ||
                    img[y * w + x] == StrongValue ||
                    img[y * w + x + 1] == StrongValue ||
                    img[(y + 1) * w + x - 1] == StrongValue ||
                    img[(y + 1) * w + x] == StrongValue ||
                    img[(y + 1) * w + x + 1] == StrongValue) {
                    // The pixel is near at least one strong one, so it is strong too
                    result[y * w + x] = StrongValue;
                } else {
                    result[y * w + x] = 0;
                }
            }
        }
    }

    return result;
}
