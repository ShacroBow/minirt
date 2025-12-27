#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp = fopen("output.ppm", "wb");
    if (!fp) {
        perror("Cannot open file");
        return 1;
    }

    // --- PPM P6 header ---
    fprintf(fp, "P6\n4 4\n255\n"); // 4x4 image, max color 255

    // --- Pixel data ---
    // Each pixel is 3 bytes: R G B
    unsigned char pixels[4][4][3] = {
        {{255,0,0}, {0,255,0}, {0,0,255}, {255,255,0}},   // row 0
        {{255,0,255}, {0,255,255}, {192,192,192}, {128,128,128}}, // row 1
        {{128,0,0}, {0,128,0}, {0,0,128}, {128,128,0}},   // row 2
        {{128,0,128}, {0,128,128}, {64,64,64}, {255,255,255}}     // row 3
    };

    // Write pixel data row by row
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            fwrite(pixels[y][x], 1, 3, fp); // write 3 bytes per pixel
        }
    }

    fclose(fp);
    printf("4x4 PPM P6 image generated as output.ppm\n");
    return 0;
}
