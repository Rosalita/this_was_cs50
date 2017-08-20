#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]){

    // if no command line args passed
    if (argc == 1){
        printf("No command line arguments passed\n");
        return 1;
    }

    if (argc > 2){
        printf("Too many command line arguments passed\n");
        return 1;
    }

    // convert argv[1] from string to int
    int key = atoi(argv[1]); // returns 0 if not convertable


    printf("plaintext:");
    string s = get_string();

    printf("ciphertext:");

        // make sure got a string
        if (s != NULL){
             //go through the input string one char at a time
            for(int i = 0, n = strlen(s); i < n; i++){

                 if (isalpha(s[i])){
                     // check if character is upper case

                     if (isupper(s[i])){
                         // A is 65, to convert to alphabetical index, subtract 65
                         char cipherchar = s[i] - 65;
                         // then add key and mod by 26
                         cipherchar = (cipherchar + key)%26;
                         // convert back from alpha index to ascii
                          cipherchar = cipherchar + 65;
                          printf("%c", cipherchar);

                     } else { // dealing with lower case character
                         // a is 97, to convert to alphabetical index, subtract 97
                         char cipherchar = s[i] - 97;
                         // then add key and mod by 26
                         cipherchar = (cipherchar + key)%26;
                         // convert back from alpha index to ascii
                          cipherchar = cipherchar + 97;
                          printf("%c", cipherchar);
                     }

                 } else { // not an alphabetical character
                     // so print it without ciphering it
                     printf("%c", s[i]);
                 }
            }
        }
        //print the final newline
        printf("\n");
        return 0;
}
