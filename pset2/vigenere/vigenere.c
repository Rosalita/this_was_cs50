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

    string keyword = argv[1];


  // check each char inkeyword is alphabetical

  for(int i = 0, n=strlen(keyword); i< n; i++){
      if ( isalpha(keyword[i]) < 1){
          printf("Keyword contains non-alphabetical characters\n");
          return 1;
      }

  }

    printf("plaintext:");
    string s = get_string();

    int keywordindex = 0;
    int plaintextindex = 0;


     // make sure got a string
        if (s != NULL){
             //print the ciphertext
             printf("ciphertext:");
             //go through the input string one char at a time

            for(int i = 0, n = strlen(s); i < n; i++){

                // check if plain text is not alphabetical
                 if ( isalpha(s[i]) < 1){ // if not alpha numerical
                       printf("%c", s[i]); //just print it and continue
                       continue;

                 } else { //if plain text is alphabetical
                     // use plain index to calculated the keyword index
                     keywordindex = plaintextindex%strlen(keyword);
                 //    printf("keyword index is %i\n", keywordindex);
                     plaintextindex ++; // then increment the plain text index
                     // this makes sure that keyword letters aren't skipped when plain text contains spaces
                 }


                if isupper(s[i]){

                    char alphakey = keyword[keywordindex];

                    if isupper(alphakey){ // if the alphakey is upper case, subtract 65 to get numerical index
                        alphakey = alphakey - 65;
                    } else{
                        alphakey = alphakey - 97; // convert alphakey to numerical value.  a is 97, to convert to alphabetical index, subtract 97
                    }

                    char plaintextchar = s[i] - 65;
                    char cipherchar = ((plaintextchar + alphakey)%26); // get alphabetical index of ciphered char
                    cipherchar = cipherchar + 65; // convert back to ascii value

                    printf("%c", cipherchar);
                }else{

                    char alphakey = keyword[keywordindex];

                    if isupper(alphakey){ // if the alphakey is upper case, subtract 65 to get numerical index
                        alphakey = alphakey - 65;
                    } else{
                        alphakey = alphakey - 97; // convert alphakey to numerical value.  a is 97, to convert to alphabetical index, subtract 97
                    }

                    char plaintextchar = s[i] - 97;
                    char cipherchar = ((plaintextchar + alphakey)%26); // get alphabetical index of ciphered char
                    cipherchar = cipherchar + 97; // convert back to ascii value

                    printf("%c", cipherchar);
                }

            }
        }

        printf("\n");
        return 0;
}
    
