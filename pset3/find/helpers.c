/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    // linear search can search any list, binary search requires list is sorted first
    //binary faster than linear

    // binary search

    if(n < 1){
        printf("error: list is smaller than 1 item");
        return 1;
    }

    // for debuging uncomment to set values to some known values
    //values[0] = 39;
    //values[1] = 40;
    //values[2] = 41;
    //values[3] = 4;
    //values[4] = 5;

    // check the list of values is sorted correctly, if it isn't then can't search
    // Should return an error if can't sort the list

    // for each item in values starting at n = 1, check its greater than value n - 1;
    for (int v = 1; v < n; v++ ){ //loop will break when v = n

        if (values[v] < values[v-1]){ //if a value is smaller than the one before it then not sorted
            printf("error can't search because values are not sorted");
            return 1;
        }

    }


    int startSearchPoint = 0;
    int endSearchPoint = n;
    int indexToCheck = (startSearchPoint + endSearchPoint) /2; // find the middle of the array


    while(startSearchPoint <= endSearchPoint){   //loop while the start point is less than or equal to end point

        if (values[indexToCheck] == value){ // if value being checked is the value searching for, then found it
         return true; // so return true
        } else if (values[indexToCheck] < value){ // if value being checked is less than value searching for
             startSearchPoint = indexToCheck + 1;  //cut the array in half by making the startpoint of next search one more than index checked
        } else {  // if neither less than value or found value, then its greater than the value searched for, so cut array in half and keep other side
             endSearchPoint = indexToCheck -1; // make the end point one less than the index that was searched
        }


        // then calculate the next mid point to check
        indexToCheck = (startSearchPoint + endSearchPoint) / 2;

    }
        // when the start search point is greater than end point then have looked everywhere
           return false; // so return false
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    // implement selection sort



   // printf("n is %i", n);

    //initialise a variable to hold a value while swapping
    int toSwap = 0;



    // loop through i for each value in array
    for (int i = 0; i <(n-1); i++){ //for each value in the array

        int toBeSorted = i; // this is the index of the value trying to sort.
                        // for values array of size 4 this will be 0.1.2.3

        int smallestValueFound; // need a var to remember the smallest value found while searching

        // loop for each value that isn't the value to be sorted, and compare it to the value being sorted.
        for (int min = i + 1 ; min < n ; min++ ){ // for values array of size 4, the min will be index 1.2.3 ignoring 0

           //check if the value to be sorted is greater than each of the values being checked, eg. is the value being checked smaller than value being sorted
           if ( values[toBeSorted] > values[min] ){ // if value being checked is smaller than value to be sorted
            smallestValueFound = min; // Keep track of the smallest value found
           }
           if ( smallestValueFound != toBeSorted ){ // the smallest value does not equal the value to be sorted
           // then need to swap
             toSwap = values[toBeSorted];
             values[toBeSorted] = values[smallestValueFound];
             values[smallestValueFound] = toSwap;
           }
        }

    }

}
