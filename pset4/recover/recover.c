#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Correct usage: /.recover <forensic_image>\n");
        return 1;
    }

    // forensic image filename
    char *img = argv[1];
    // open input file
    FILE *inptr = fopen(img, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", img);
        return 1;
    }

    // counter
    int jpg_count = 0;
    // name of output files (e.g. 000.jpg\0)
    char recovered_jpg[8];

    // buffer to hold 512 bytes (1 block)
    BYTE *buffer = malloc(BLOCK * sizeof(BYTE));
    if (buffer == NULL)
    {
        fclose(inptr);
        printf("Not enough memory to store image.\n");
        return 2;
    }

    // output file (set pointer to NULL)
    FILE *jpg = NULL;

    // Read through entire card until last 512 byte BLOCK
    // fread returns number of items of size 512 (sizeof buffer) that were read (end of file may not have 512 bytes so fread won't return 1)
    while (fread(buffer, sizeof(buffer), 1, inptr) == 1)
    {
        // jpg header signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpg_count > 0)
            {
                // close current output file
                fclose(jpg);
            }

            // set jpg ouput filename
            sprintf(recovered_jpg, "%03i.jpg", jpg_count);

            // open new output file
            jpg = fopen(recovered_jpg, "w");
            // write new block to output file
            fwrite(buffer, sizeof(buffer), 1, jpg);

            jpg_count++;

        }
        // check if stil writing jpg (jpgs stored back to back)
        else if (jpg_count > 0)
        {
            fwrite(buffer, sizeof(buffer), 1, jpg);
        }
    }

    // free memory
    free(buffer);
    // close input file
    fclose(inptr);
    // close output file
    fclose(jpg);
}
