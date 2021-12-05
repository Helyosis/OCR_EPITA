#ifndef LOADDATASET
#define LOADDATASET
struct tImage{
    int label;
    double* pixVect;
};
struct tImage** imageVect(size_t nbImages);
void freeImVect(struct tImage** v, size_t nbImages);
#endif