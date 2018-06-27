// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

typedef struct node
    {
        char word[LENGTH +1];
        struct node *next;
    }
node;

// find a hash function
// found at stackoverflow.com/questions/14409466/simple-hash-functions
unsigned int hash(const char* word)
{
    unsigned int count;
    unsigned int hashValue = 0;
    for(count = 0; word[count] != '\0'; count++)
        hashValue = word[count] + (hashValue << 6) + (hashValue << 16) - hashValue;
 
    return (hashValue % HASHTABLESIZE);
}

// need to declare a global hashtable
node* hashtable[HASHTABLESIZE];

// global word counter
int wordcounter = 0;


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // needs to be a case insensitive function so convert the word to lowercase
    // but word is a read only variable so need to make a copy of it to do this
    // need to know how long it is to copy it to a new array of chars
    
    int length = strlen(word);
    char lowercaseword[length + 1];
    
    // convert word to lowercase
    for (int i = 0; i < length; i++)
    {
       lowercaseword[i] = tolower(word[i]);
    }
    
    // the lowercase word needs to end in \0
    lowercaseword[length] = '\0';
    
    
    // get the hash of the word that is being checked so know where in the hashtable to start
    int hashed;
    hashed = hash(lowercaseword);
    //printf("Checking %s\n", lowercaseword);
    //printf("its hash is %d\n", hashed);
  
    // if the word exists, find it in the dictionary datastructure
    // traversing linked lists
    // declare node pointer called head that points to the first node in linked list
  
    node* headpointer = hashtable[hashed];
    node* cursor = headpointer;

    //printf("%p", cursor);
    while (cursor != NULL)
    {
        //printf("something in hashtable");
        return true;        
        // check if the word is in the node
        // use strcasecmp to do this

        if (strcmp(cursor->word, lowercaseword) == 0)
        {
            // word is in dictionary
            return true;
        }
        else
        {
            // check next node
            cursor = cursor->next;
        }
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
     //initialise the hash table
    for (int i = 0; i < HASHTABLESIZE; i++)
    {
        hashtable[i] = NULL;
    }

    // read the words from the dictionary file first
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open dictionary.\n");
        unload();
        return false;
    }
    
    // create an empty array to hold the word being read from the dictionary
    char word[LENGTH + 1];
    
    //scan dictionary word by word till EOF 
    while (fscanf(fp, "%s", word) !=EOF)  
    {
        //printf("%s\n", word);
        //hash the word
        // count the word
        wordcounter++;
        
        int hashed;
        hashed = hash(word);
   
        // start by allocating enough space in memory to store the node
        // creating a node is just mallocing a node pointer
        node *newnode = malloc(sizeof(node));
        
        // check that malloc was ok
        if (newnode == NULL)
        {
            unload();
            return false;
        }
         
        // to set a word value in a node
        // use arrow notation to access the word value, then set the nodes value
        strcpy(newnode->word, word);
        
        //create a new pointer and point it at the first node at hash index
        node* hashtableheadpointer = hashtable[hashed];

        // if there is nothing at hashtable[hashed] yet then this new pointer will be null 
        if (hashtableheadpointer == NULL) 
        {
            // which means can put the pointer to the new node into the first element of the hashtable
            hashtable[hashed] = newnode;
        } else { //the first element in the hashtable already contains a pointer to a node
            // point the new nodes next at the existing head node
            newnode->next = hashtable[hashed]; 
            // make the first element in the hash table now the new node
            hashtable[hashed] = newnode;
        }
    }
    
    // close dictionary
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcounter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // loop through the hashtable
    for (int i = 0; i < HASHTABLESIZE; i++)
    {
    // free all the nodes
    // point to the head node
        node* head = hashtable[i];
        // if head is null then theres nothing to do
        while (head != NULL) // if head is not null need to traverse the list freeing nodes
        {
            //make a pointer that points at the first node to delete, the head node
            node* todelete = head;
            // change the head node to point at the next value
            head = head -> next;
            // then delete the node which is no longer the head
            free(todelete);
        }
    }
    return true;
}

