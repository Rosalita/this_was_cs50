#include <stdio.h>

int main(int argc, char *argv[])
{
     // accept one command line arg
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file.raw\n");
        return 1;
    }


    // remember filename
    char *filename = argv[1];

    // open file in reading mode
    FILE* inptr = fopen(filename, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s\n", filename);
        return 2;
    }
    // make an array called buffer to read into
    unsigned char buffer[512];

    // also need a flag to know when first jpg found
    int firstjpgfound = 0;

    // also need a counter to count the jpgs for jpg filenames
    int jpgcount = 0;

    // also need an empty file to write jpgs into
     FILE* jpg = NULL;

    // read in blocs of 512 bytes
    // while not end of file
    while (fread(buffer, 1, 512, inptr)){

    // check to see if this block is a jpg
    // its a jpg if first three bytes are 0xff 0xd8 0xff

    // if first few bytes match the pattern found a jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0){


            // if this isn't the first jpg thats been found then we just hit the end of previous jpg
            if (firstjpgfound == 1){
                // which means need to close writing to previous jpg
                fclose(jpg);
            } else {
            // found the first jpg so set flag
            firstjpgfound = 1;
            }

            // generate the jpgfilename with the jpgcount
            char jpgfilename[8];
            sprintf(jpgfilename, "%03d.jpg", jpgcount);
            // open the jpg in append mode
            jpg = fopen(jpgfilename, "a");
            jpgcount++;


        }

        //next block of 512 if in the middle of jpg section
        if (firstjpgfound == 1)
        {
            // write whats in the buffer into the jpg
            fwrite(&buffer, 512, 1, jpg);
        }


    }

    // once end of file reached
    fclose(inptr); //close the input file
    fclose(jpg); // close the jpg

    return 0;


}
