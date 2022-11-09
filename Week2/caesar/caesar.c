#include <stdlib.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

bool verifyUsage(int c, char *v[]);
void setKey(char *v[]);
void setPlaintext();
void encipher(string text, int k);
void printCiphertext();

int key;
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
    bool isArgvDigits = true;

    if (c != 2)
    {
        printf("Usage: ./caesar key\n");    // print usage if number of arguments is incorrect and return false
        isArgvDigits = false;
    }

    else
    {
        for (int i = 0, n = strlen(v[1]); i < n; i ++)  // print usage if type of second argument is incorrect and return false
        {
            if (isdigit(v[1][i]) == 0)
            {
                printf("Usage: ./caesar key\n");
                isArgvDigits = false;
            }
        }
    }

    return isArgvDigits;
}

void setKey(char *v[])  // set key value based on argv[1]
{
    key = atoi(v[1]);
}

void setPlaintext()     // prompt to user and set plaintext value
{
    plaintext = get_string("Type plaintext: ");
}

void encipher(string text, int k)   // set ciphertext after encipher plaintext 
{
    string aux = text;

    for (int i = 0, n = strlen(text); i < n; i ++)
    {
        if (isalpha(text[i]))
        {
            if (isupper(text[i]))
            {
                aux[i] = 65 + ((text[i] - 65) + k) % 26; // shift formula using uppercase 
            }
            else
            {
                aux[i] = 97 + ((text[i] - 97) + k) % 26; // shift formula using lowercase
            }
        }
        else
        {
            aux[i] = text[i];
        }

    }

    ciphertext = aux;
}
void printCiphertext()  // print enciphered text
{
    printf("ciphertext: %s\n", ciphertext);

}