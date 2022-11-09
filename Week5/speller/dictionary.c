// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash hashtable
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

FILE *in_dict_pointer = NULL;
int words_counter = 0;

// Number of buckets in hash hashtable
const unsigned int N = 100000;

// Hash hashtable
node *hashtable[N];
node *new_node_pointer = NULL;

void read_file_and_copy_word_to(char w[]);
bool add_word_to_hashtable(char w_a[]);
void add_node_to_hashtable(unsigned int h_code);
void to_lower_case(const char *w, char *l_c_w);
bool initialize_new_node_pointer();
void clean_array (char array[LENGTH + 1]);
void clean_hashtable();
bool points_to_dictionary (const char *dictionary);
bool compare_word_with_hashtable(int h_code, char* l_c_w);


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char word_buffer[LENGTH + 1];                                //data_buffer to be written from dictionary

    clean_array(word_buffer);                                    //to warrant new word array has no trash.

    if(points_to_dictionary(dictionary) == false)               // points in_dict_pointer to dictionary and check if the pointer is not NULL
    {
        printf("Could not load file.\n");
        return false;
    }

    if(words_counter == 0)
    {
        clean_hashtable();
    }

    while (true)
    {
        read_file_and_copy_word_to(word_buffer);                 // read file and store data in data_buffer

        if (feof(in_dict_pointer))                              // if data file's pointer is at end, stop loop
        {
            break;
        }

        if(add_word_to_hashtable(word_buffer) == false)          // add word to hashtable and if its false, return false
        {
            printf("Could not load file.\n");
            return false;
        }
    }

    fclose(in_dict_pointer);

    return true;
}

bool points_to_dictionary (const char *dictionary)
{
    in_dict_pointer = fopen(dictionary, "r");                   //points the pointer to dictionary

    if (!in_dict_pointer)                                       //check if pointer is not NULL
    {
        return false;
    }

    return true;
}

void clean_array (char array[LENGTH + 1])                       //clears an entire array of characters with / 0.
{
    for (int i = 0; i < (LENGTH + 1); i++)
    {
        array[i] = '\0';
    }

}

void clean_hashtable()                                          //clears the hashtable
{
    for(int i = 0; i < N; i++)
    {
       hashtable[i] = NULL;
    }
}

void read_file_and_copy_word_to(char w_a[])                     //read file and store at word_buffer
{
    fscanf(in_dict_pointer, "%s", w_a);
}

bool add_word_to_hashtable(char w_a[])                          //add an word from a word_buffer to hashtable
{
    unsigned int hash_code = hash(w_a);                         // position at hash-hashtable

    if (!initialize_new_node_pointer())
    {
        return false;
    }

    strcpy(new_node_pointer->word, w_a);                        // copy word from word_buffer to node

    add_node_to_hashtable(hash_code);

    words_counter++;

    return true;
}

bool initialize_new_node_pointer()                              // initialize hashtables's global new_node_pointer
{
    new_node_pointer = malloc(sizeof(node));                    // alocate space for a new node pointer

    if (!new_node_pointer)
    {
        unload();
        printf("No memory for node pointer.\n");
        return false;
    }

    new_node_pointer->next = NULL;

    return true;
}

// Hashes word to a number
// https://www.youtube.com/watch?v=nvzVHwrrub0
unsigned int hash(const char *word)
{
    int sum = 0;
    int n = strlen(word);

    for (int j = 0; j < n; j++ )
    {
        sum += word[j];
    }

    return sum % N;
}

void to_lower_case(const char *w, char *l_c_w)                  //copy a word to its lower case word
{
    for(int i = 0; i < strlen(w); i++)
    {
        l_c_w[i] = tolower(w[i]);
    }
}

void add_node_to_hashtable(unsigned int h_code)                 // add a node to the global hashtable
{
    new_node_pointer->next = hashtable[h_code];
    hashtable[h_code] = new_node_pointer;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words_counter;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lower_case_word[LENGTH + 1];

    clean_array(lower_case_word);

    if(word != NULL)
    {
        to_lower_case(word, lower_case_word);
    }
    else
    {
        return false;
    }

    unsigned int hash_code = hash(lower_case_word);

    if(compare_word_with_hashtable(hash_code, lower_case_word) == true)
    {
        return true;
    }
    return false;
}

bool compare_word_with_hashtable(int h_code, char* l_c_w)                       // compare a word with its repective word in hashtable
{
    for(node *np = hashtable[h_code]; np != NULL; np = np->next)
    {
        if(strcmp(np->word, l_c_w) == 0)
        {
            return true;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp = NULL;
    node *np = NULL;

    for(int i = 0; i < N; i++)
    {
        if(hashtable[i] != NULL)
        {
            np = hashtable[i];

            while(np != NULL)
            {
                tmp = np;
                np = np->next;
                free(tmp);
            }
        }
    }
    return true;
}
