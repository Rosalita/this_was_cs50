/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // accept exactly three command line args
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n input.bmp output.bmp\n");
        return 1;
    }

    // reading in char but want to use it like an into so convert it to int with atoi
    int n = atoi(argv[1]);

    //debug print n
    //printf("n is : %i\n", n);

    // n must be a positive integer <= 100
    if (n > 100){
        fprintf(stderr, "n is too large\n");
    }

    if (n < 1){
         fprintf(stderr, "n must be a positive integer\n");
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // declare a bitmap info header to store output values in
    BITMAPINFOHEADER biout;

    // start by copying bi to biout
    biout = bi;

    // biout's biWidth is itself multiplied by n
    biout.biWidth *= n;

    // biout's biHeight is itself multiplied by n
    //commented out for now as want to try get a width resize working first
    biout.biHeight *= n;

    // need to workout how much padding needed after updating biout's width
    int outputpadding =  (4 - (biout.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // biout's sizeImage is the space used by ((RGBTRIPLE * width) + padding * height))
    biout.biSizeImage = ((sizeof(RGBTRIPLE)* biout.biWidth) + outputpadding) * abs(biout.biHeight);

    // declaring a bitmap header to store the output files values in
    BITMAPFILEHEADER bfout;

    //copying everything from the input header to the output header
    bfout = bf;

    // need to change the file size - bfSize = total size of the file (stored in bitmapinfoheader) in bytes includes pixels padding and headers
    bfout.bfSize = biout.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfout, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biout, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // loop for y times to increase height by n
        for(int y =0; y <n; y++){

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                 for(int x = 0; x < n; x++){
                    // printf("writing pixel x = %i\n", x);
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                 }
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < outputpadding; k++)
            {
                fputc(0x00, outptr);
            }

            // move the read point in input file back to start of the scan line
            // the number of byte to move is the width of the original * 3 as each pixel has RGB value which is 3 bytes (triple) + any padding
            fseek(inptr, -((bi.biWidth * 3) + padding ), SEEK_CUR);


        }
        // after have increased the height n times need to put the read point in input file back to the end of the line again
        // so it can continue for the next scanline without getting stuck
        fseek(inptr, (bi.biWidth * 3) + padding, SEEK_CUR);


    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
