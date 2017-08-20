#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    //ask for name
   // printf("What is your name?\n");
    string s = get_string();

    // make sure got a string
    if (s != NULL){

        int initialindex; // represents a state: while this var is 0, looking for an initial. While 1, not looking for an initial
        initialindex = 0; // start in the state of looking for an initial

        //go through the input string one char at a time
        for(int i = 0, n = strlen(s); i < n; i++){

            if (initialindex == 0){ // if looking for an initial

                // if first character is a space, ignore it
                if (s[i] == ' '){
                    continue; // skip to next loop iteration
                }
                else{
                    //print the initial in upper case
                    printf("%c", toupper(s[i]));
                    initialindex = 1; //increment initial index as no longer looking until reach the end of a word
                }
            }

           // if we come across a space, we need to go back to looking for an initial
           if (s[i] == ' '){
               initialindex = 0;
               continue;
           }
        }
        // reached end of string, print a new line
        printf("\n");

    }
}
