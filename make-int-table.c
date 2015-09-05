#include <stdio.h>

#define NCHARS 256

char translate[NCHARS];
unsigned short short_translate[NCHARS * NCHARS];

void initializeTranslate() {
    int i;
    for (i = 0; i < NCHARS; i++) {
        translate[i] = i;
    }
    translate['A'] = 'T';
    translate['T'] = 'A';
    translate['G'] = 'C';
    translate['C'] = 'G';
    translate['a'] = 't';
    translate['t'] = 'a';
    translate['g'] = 'c';
    translate['c'] = 'g';
    int x, y;
    for (x = 0; x < NCHARS; x++) {
        for (y = 0; y < NCHARS; y++) {
            unsigned short in_xy = (x << 8) | y;
            unsigned short out_xy =
                ((unsigned short)(translate[x]) << 8) |
                (unsigned short)(translate[y]);
            short_translate[in_xy] = out_xy;
        }
    }
}

int main() {
    initializeTranslate();
    unsigned x, y;
    for (x = 0; x < NCHARS * NCHARS; x++) {
        unsigned int buffer[NCHARS * NCHARS];
        for (y = 0; y < NCHARS * NCHARS; y++) {
            unsigned int in_xy = (x << 16) | y;
            unsigned int out_xy =
                ((unsigned int)(short_translate[x]) << 16) |
                (unsigned int)(short_translate[y]);
            buffer[y] = out_xy;
        }
        fwrite((char*)buffer, 1, sizeof(buffer), stdout);
    }
    return 0;
}
