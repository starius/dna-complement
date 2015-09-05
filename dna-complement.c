#include <stdio.h>
#include <errno.h>
#include <string.h>

#define NCHARS 256

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

char translate[NCHARS];
short short_translate[NCHARS * NCHARS];

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
#if USE_SHORT
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
#endif
}

int main(int argc, char** argv) {
    initializeTranslate();
    if (argc < 1 + 2) {
        fprintf(stderr, "Usage: %s input output\n", argv[0]);
        return 1;
    }
    const char* input_fname = argv[1];
    const char* output_fname = argv[2];
    FILE* input = fopen(input_fname, "rb");
    if (!input) {
        fprintf(stderr, "Can't open %s: %s\n",
                input_fname, strerror(errno));
        return 1;
    }
    FILE* output = fopen(output_fname, "wb");
    if (!output) {
        fprintf(stderr, "Can't open %s: %s\n",
                output_fname, strerror(errno));
        return 1;
    }
    char input_buffer[BUFSIZE];
#ifdef SAME_BUFFER
# define output_buffer input_buffer
#else
    char output_buffer[BUFSIZE];
#endif
#if USE_SHORT
    short* input_buffer_end = (short*)(input_buffer + BUFSIZE);
#else
    char* input_buffer_end = input_buffer + BUFSIZE;
#endif
    size_t nread;
    while (nread = fread(input_buffer, 1, BUFSIZE, input)) {
#if USE_SHORT
        short* input_it;
# define table short_translate
# define input_buffer1 (short*)input_buffer
# define output_buffer1 (short*)output_buffer
# ifndef SAME_BUFFER
        short* output_it;
# endif
#else
        char* input_it;
# define table translate
# define input_buffer1 input_buffer
# define output_buffer1 output_buffer
# ifndef SAME_BUFFER
        char* output_it;
# endif
#endif
#ifdef SAME_BUFFER
# define output_it input_it
#endif
        for (input_it = input_buffer1
#ifndef SAME_BUFFER
             , output_it = output_buffer1
#endif
             ; input_it < input_buffer_end; input_it++
#ifndef SAME_BUFFER
             , output_it++
#endif
             )
        {
            *output_it = table[*input_it];
        }
        fwrite(output_buffer, 1, nread, output);
    }
    fclose(input);
    fclose(output);
    return 0;
}
