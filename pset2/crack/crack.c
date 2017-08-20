#include <stdio.h>
#include <cs50.h>
#define _XOPEN_SOURCE
#include <unistd.h>

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

    string hash = argv[1]; // get hashed password

    printf("hash is %s\n", hash);


    // crack password
    // password hashed with C's DES Crypt function
    // Check man page for crypt
    // The salt is the first two characters of the hashed password
    // The crypt function takes two arguments, key and salt

    // hashing a word with the same function always returns the same hash
    // these passwords are no longer than four characters long
    // they are composed entirely of alphabetical characters

    // think of a way to iterate over and generate plain text to be hashed

    // generate all possible passwords of a to z and A to Z chars

    for (int i = 0; i < 4; i++ ){ // for 1, 2, 3, and 4 letter passwords

        for(char c = 'a'; c <= 'z'; ++c){

            printf("Char is %c\n", c);
        }
        for c

    }

    for(char c = 'a'; c <= 'z'; ++c){

        printf("Char is %c\n", c);
    }

    // passwords are case sensitive
    // consider an array of up to 5 bytes where last byte is \0
    // iterate over 1 character, 2 character, 3 character then 4 characters lon to generate all the plain texts to be hashed
    // once hash of guess matches the hash user has given you, you have found their password.

    // compile with clang -ggdb3 -O0 -std=c11 -Wall -Werror -Wshadow crack.c -lcrypt -lcs50 -lm -o crack
}
    
