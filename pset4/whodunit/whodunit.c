#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
     // accept two commandline args, the first the name of a file the second the name of an output file to open for writing
    if (argc != 3)
    {
        // if executed with less than two command line args, remind the user of the correct usage as with fprintf() to stderr and main should return 1
        fprintf(stderr, "Usage: ./whodunnit cluefile verdict\n");
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];


    // open the file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        // if the input file cannot be opened for reading, inform the user with printf to stderr and main should return 2
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }



    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        // if the output file cannot be opened for writing, inform the user with fprintf to stderr and main should return 3
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
         // if the input file is not a 24 bit uncompressed BMP 4.0 you program should inform the user with fprintf to stdeer and main should return 4
        return 4;
    }

    // update headers info for outfile
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    // read the clue's scanline, pixel by pixel
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {

            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // change the pixel's colour as necessary
            // pixels are represented by 3 bytes, the amount of blue green and red
            // ff0000 is blue and ffffff is white
            // rgb triple struct called triple. to change colour set the blue green and red values
            // a green pixel looks like
            // triple.rgbtBlue = 0x00;
            // triple.rgbtGreen = 0xff;
            // triple.rgbtRed = 0x00;

            // if a pixel is a certain colour
            // if (triple.rgbtBlue == 0xff){
            //}

            // might want to remove red from pixels or only change pure red pixels to white

            // trying remove red from pixels
            //triple.rgbtRed = 0x00;  // don't see anything

            //trying to change pure red pixels to white
            if ((triple.rgbtBlue == 0x00)&&(triple.rgbtGreen == 0x00)&&(triple.rgbtRed == 0xff)){
                triple.rgbtBlue = 0xff;
                triple.rgbtGreen = 0xff;
            }



            // write RGB triple to outfile
            // write the verdict's scanline pixel by pixel
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
