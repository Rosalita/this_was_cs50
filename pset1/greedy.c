#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void){
    float n;
    do{
        printf("O hai! How much change is owed?\n");
        n = get_float();
    }
    while(n < 0);


    float cents;
    cents = n * 100;
    cents = roundf(cents);

    int coincount;
    coincount = 0;

    int howmany;
    howmany = 0;


    howmany = (cents / 25);
    coincount = coincount + howmany;
    cents = (cents - (25 * howmany));

    if (cents > 0){
        howmany = (cents /10);
        coincount = coincount + howmany;
        cents = (cents - (10 * howmany));
    }

    if (cents > 0){
        howmany = (cents /5);
        coincount = coincount + howmany;
        cents = (cents - (5 * howmany));
    }

    if (cents > 0){
        howmany = (cents /1);
        coincount = coincount + howmany;
    }

    printf("%i\n", coincount);

}
