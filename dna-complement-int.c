#include <stdio.h>
#include <errno.h>
#include <string.h>

// open
#include <sys/stat.h>
#include <fcntl.h>

// mmap
#include <sys/mman.h>

#define NCHARS 256

#ifndef BUFSIZE
#define BUFSIZE (1024*1024)
#endif

int main(int argc, char** argv) {
    if (argc < 1 + 3) {
        fprintf(stderr, "Usage: %s table16g input output\n", argv[0]);
        return 1;
    }
    const char* table16g_fname = argv[1];
    int table16g_fd = open(table16g_fname, O_RDONLY);
    size_t table16g_size =
        (size_t)(256*256) * (size_t)(256*256) * sizeof(int);
    unsigned int* int_translate = mmap(
            0, // target address
            table16g_size, // size
            PROT_READ, // protection
            MAP_PRIVATE, // flags
            table16g_fd, // file descriptor
            0 // offset
            );
    if (!int_translate) {
        fprintf(stderr, "Can't mmap table16g\n");
        return 1;
    }
    const char* input_fname = argv[2];
    const char* output_fname = argv[3];
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
    int* input_it;
    int* input_buffer_end = (int*)(input_buffer + BUFSIZE);
#ifdef SAME_BUFFER
# define output_buffer input_buffer
# define output_it input_it
#else
    char output_buffer[BUFSIZE];
    int* output_it;
#endif
    size_t nread;
    while (nread = fread(input_buffer, 1, BUFSIZE, input)) {
        for (input_it = (unsigned int*)input_buffer
#ifndef SAME_BUFFER
             , output_it = (unsigned int*)output_buffer
#endif
             ; input_it < input_buffer_end; input_it++
#ifndef SAME_BUFFER
             , output_it++
#endif
             )
        {
            *output_it = int_translate[*input_it];
        }
        fwrite(output_buffer, 1, nread, output);
    }
    fclose(input);
    fclose(output);
    munmap(int_translate, table16g_size);
    return 0;
}
