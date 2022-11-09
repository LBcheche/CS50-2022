#include <stdlib.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

bool verifyUsage(int c, char *v[]);
bool verifyKeyLenght(char *v[]);
bool verifyKeyAlpha(char *v[]);
bool verifyKeyNonRepetition(char *v[]);
void setKey(char *v[]);
void setPlaintext();
void encipher(string text, string k);
void printCiphertext();


char key[26];
string plaintext;
string ciphertext;

int main(int argc, string argv[])
{
    bool isUsageOk;

    isUsageOk = verifyUsage(argc, argv);

    if (isUsageOk)                          // if usage ok, continue algorithm
    {
        setKey(argv);

        setPlaintext();

        encipher(plaintext, key);

        printCiphertext();
    }
    else                                    // else, stop and return 1
    {
        return 1;
    }
}

bool verifyUsage(int c, char *v[])          // verify correct usage of arguments
{

    if (c != 2)
    {
        printf("Usage: ./substitution key\n");    // print usage if number of arguments is incorrect and return false
        return false;
    }
    if (verifyKeyLenght(v) == false)
    {
        printf("Key must contain 26 characters\n");
        return false;
    }
    if (verifyKeyAlpha(v) == false)
    {
        printf("Usage: ./substitution key\n");
        return false;
    }
    if (verifyKeyNonRepetition(v) == false)
    {
        printf("Usage: ./substitution key\n");
        return false;
    }

        return true;
}

bool verifyKeyLenght(char *v[])
{
    if(strlen(v[1]) != 26)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool verifyKeyAlpha(char *v[])
{

    for (int i = 0, n = strlen(v[1]); i < n; i ++)
    {
        if (isalpha(v[1][i]) == 0)
        {
            return false;
        }
    }

    return true;
}

bool verifyKeyNonRepetition(char *v[])
{
    for (int i = 0, n = strlen(v[1]); i < n; i ++)
    {
        for (int j = 0; j < n; j ++)
        {
            if(i != j)
            {
                if (v[1][i] == v[1][j])
                {
                     return false;
                }
            }
        }
    }

    return true;

}
void setKey(char *v[])  // set key value based on argv[1]
{

    for (int i = 0, n = strlen(v[1]); i < n; i ++)
    {
        key[i] = tolower(v[1][i]);
    }

}

void setPlaintext()     // prompt to user and set plaintext value
{
    plaintext = get_string("plaintext: ");
}

void encipher(string text, string k)   // set ciphertext after encipher plaintext
{
    int n = strlen(text);
    char aux[n];
    char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";
    bool upper = false;


    for (int i = 0; i < n; i ++)
    {
        if (isalpha(text[i]))
        {
            upper = isupper(text[i]);

            text[i] = tolower(text[i]);

            for (int j = 0; j < 26; j ++)
            {
                if(text[i] == alphabet[j])
                {
                    if(upper)
                    {
                        aux[i] = toupper(key[j]);
                    }
                    else
                    {
                        aux[i] = key[j];
                    }
                }
            }
        }
        else
        {
            aux[i] = plaintext[i];
        }
    }

   ciphertext = aux;
}


void printCiphertext()  // print enciphered text
{
    printf("ciphertext: %s\n", ciphertext);

}