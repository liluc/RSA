#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int rsaEncreption(int n){
    if (n < 253){
        int q = 11, p = 23;
        int e = 3;
        double power = pow(n, e);
        return (int)power % (q * p);
    }else {
        return n;
    }
}

int rsaDecreption(int c){
    int q = 11, p = 23, d = 147;
    int m = q * p;

    //using square and multiple to find congruence.
    if (c < 253){
        double power2 = pow(c, 2);
        int c2 = (int)power2 % m;

        double power4 = pow(c2, 2);
        int c4 = (int)power4 % m;

        double power8 = pow(c4, 2);
        int c8 = (int)power8 % m;

        double power16 = pow(c8, 2);
        int c16 = (int)power16 % m;

        double power32 = pow(c16, 2);
        int c32 = (int)power32 % m;

        double power64 = pow(c32, 2);
        int c64 = (int)power64 % m;

        double power128 = pow(c64, 2);
        int c128 = (int)power128 % m;

        return (c128 * c16 * c2 * c) % m;
    }else{
        return c;
    }
}

void imgEncreption(char name[], char encName[]){
    int width, height, channels;
    //original picture loaded, stored at the memory address of img.
    //because using char, img points to the address of the first byte of the picture, which is the red channel of the first pixel.
    //if we want to examine every pixel, we need to go by channels.
    unsigned char *img = stbi_load(name, &width, &height, &channels, 0);

    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }

    size_t img_size = width * height * channels;

    //a new image created, stored at the memory address of encImg.
    unsigned char *encImg = malloc(img_size);
    if (encImg == NULL){
        printf("Unable to allocate memory for the new image.");
        exit(1);
    }

    //changing rgb values of each pixel;
    //since img only store one byte of information
    for (unsigned char *p = img, *pg = encImg; p != img + img_size; p++, pg ++){
        //transforming information from the original address (one channel each time)
        *pg       = (*p);
    }
    stbi_write_jpg(encName, width, height, channels, encImg, width * channels);
    stbi_image_free(img);
    free(encImg);
}


void imgDecreption(char name[], char encName[]){
    int width, height, channels;
    unsigned char *img = stbi_load(name, &width, &height, &channels, 0);

    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }

    size_t img_size = width * height * channels;

    //a new image created, stored at the memory address of decImg.
    unsigned char *decImg = malloc(img_size);
    if (decImg == NULL){
        printf("Unable to allocate memory for the new image.");
        exit(1);
    }

    for (unsigned char *p = img, *pg = decImg; p != img + img_size; p ++, pg ++){
        //transforming information from the original address
        *pg       = (*p);
    }
    stbi_write_jpg(encName, width, height, channels, decImg, width * channels);
    stbi_image_free(img);
    free(decImg);
}

int compare(char name1[], char name2[]){
    int width1, height1, channels1;
    unsigned char * img1= stbi_load(name1, &width1, &height1, &channels1, 0);

    int width2, height2, channels2;
    unsigned char *img2 = stbi_load(name2, &width2, &height2, &channels2, 0);

    size_t img_size = width1 * height1 * channels1;

    int counter = 0;

    for (unsigned char *p1 = img1, *p2 = img2; p1 != img1 + img_size; p1++, p2++){
        counter += ((*p1 * *p1) - (*p2 * *p2));
    }

    return counter;
}


int main(void) {
    imgEncreption("sky.jpg", "sky-en.jpg");
    imgDecreption("sky-en.jpg", "sky-afDe.jpg");
    printf("%d\n", compare("sky-en.jpg", "sky-afDe.jpg"));
    printf("%d", compare("sky.jpg", "sky-en.jpg"));

}