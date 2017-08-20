#include <stdio.h>
#include <cs50.h>

int main (void){
     int n;
    do{
        printf("Height:");
        n = get_int();
    }
    while((n < 0) || (n > 23));

 int row;
    for( row = 1; row < n + 1; row ++ ){
       int space;
       int hash;

       space = n - row;
       hash = n - space;

       int i;
       for (i = space; i > 0; i--){
           printf(" ");
       }
       for (i = hash; i > 0; i--){
           printf("#");
       }
       printf("  ");
       for (i = row; i > 0; i--){
           printf("#");
       }
       printf("\n");
   }
}
