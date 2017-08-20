#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void){

    long long cardnum;

    do {
        printf("Number:");
        cardnum = get_long_long();
        } while (cardnum < 1);

    // work out how long the card number is

    int length;
    length = 1; // initialise length

    long long numtocount;
    numtocount = cardnum; // copy the of card number


    while(numtocount > 9){length++; numtocount = numtocount/10;}{
        // increment a length counter every time the number being counted can be  divided by 10
    }


    // initialise counters for the odd digits and the even digits in the number
    int oddchecksum;
    oddchecksum = 0;
    int evenchecksum;
    evenchecksum =0;

    long long n;

    // if the number is a valid length
    if (length == 13 || length == 15 || length == 16){


        int index;
        // loop for the length of the number, index starts at 1 which represents digit far right.
        for(index = 1; index < length + 1; index ++ ){


            // generate index to power of 10, used to get the digit at each index.
            n = pow(10, index);

            long long digit;
            digit = cardnum % n; // digit now starts with the digit we need

            n = n /10; // drop a zero from n
            digit = digit / n; // and divide digit by this to get the first digit



            if (index %2 == 0) {

                // if the index was even, multiply the digit by 2
                digit = digit *2;


                // and if digit greater than 9 add the tens and units together
                if (digit > 9) {

                    int units;
                    units = digit % 10;

                    int tens;
                    tens = (digit - units) /10;

                    // add the units to tens
                    digit = tens + units;


                }

                // add to even checksum;

                evenchecksum += digit;




            }else{
                // if the index was add add the digit to the odd checksum

               oddchecksum += digit;

            }

        } // have now looped through all digits

        // add the odd and even checksums together

        int finalsum;

        finalsum = evenchecksum + oddchecksum;

        // debugging can print the final sum
        // printf("final sum is: %d\n", finalsum);

        // get final digit of the sum
        int lastdigit;
        lastdigit = finalsum % 10;

        if (lastdigit == 0){

            // reduce n to length -1 digits by dividing by 10
            n = n /10;

            int firsttwo;
            // find first to digits of the card number
            firsttwo = cardnum / n;

            int checknum;
            checknum = firsttwo / 10;

            if (checknum == 4){
               printf("VISA\n");
            return 0;
            }

            if (checknum == 3){
                //might be amex, need to check second number
                checknum = firsttwo % 10;
                if (checknum == 4 || checknum == 7){
                   printf("AMEX\n");
                } else{
                   printf("INVALID\n");
                   return 0;
                }
            }

            if (checknum == 5){
                //might be mastercard, need to check second number
                checknum = firsttwo % 10;
                if (checknum == 1 || checknum == 2 || checknum == 3|| checknum == 4 || checknum == 5){
                   printf("MASTERCARD\n");
                } else{
                   printf("INVALID\n");
                   return 0;
                }
            }

        } else{ // last digit of check sum isn't 0
            printf("INVALID\n");
            return 0;
        }

    } else {
        printf("INVALID\n");
        return 0;
    }


}
