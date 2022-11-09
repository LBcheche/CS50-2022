#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int countLetters(string t);
int countWords(string t);
int countSenteces(string t);
float countAverage(float n1, float n2);
void printColemanLiauIndex(float L, float S);

int main(void)
{
    string text;
    int nLetters;       //number of letters in text
    int nWords;         //number of words in text
    int nSentences;     //number of sentences in text
    float avSentences;  //average number of senteces per 100 words
    float avLetters;    //average number of letters per 100 words
   

    text = get_string("Text: ");

    nLetters = countLetters(text);
    nWords = countWords(text);
    nSentences = countSenteces(text);
    avLetters = countAverage(nLetters, nWords);
    avSentences = countAverage(nSentences, nWords);
    
    printColemanLiauIndex(avLetters, avSentences);

    // printf("Number of Letters: %i \n", nLetters);
    // printf("Number of Words: %i \n", nWords);
    // printf("Number of Sentences: %i \n", nSentences);
    
}

int countLetters(string t) // count number of letters in text
{
    int counter = 0;

    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (isalnum(t[i]))
        {
            counter += 1 ;
        }
    }

    return counter;
}

int countWords(string t) // count number of words in text
{
    int counter = 0;
    bool isPreviousAlphaNum = false;
    bool isCurrentAlphaNum = false;
    
    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (isalnum(t[i]) || t[i] == 39 || t[i] == 45)
        {
            isCurrentAlphaNum = true;
        }
        else
        {
            isCurrentAlphaNum = false;
        }
        
        if (isPreviousAlphaNum == false && isCurrentAlphaNum == true)
        {
            counter += 1;
        }
        
        isPreviousAlphaNum = isCurrentAlphaNum;
    }
    
    return counter;

}

int countSenteces(string t) // count number of senteces in text
{
    int counter = 0;
    bool isPreviousPunctuation = true;
    bool isCurrentPunctuation = true;
    
    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (t[i] == 33 || t[i] == 46 || t[i] == 63)
        {
            isCurrentPunctuation = false;
        }
        else
        {
            isCurrentPunctuation = true;
        }
        
        if (isPreviousPunctuation == true && isCurrentPunctuation == false)
        {
            counter += 1;
        }
        
        isPreviousPunctuation = isCurrentPunctuation;
    }
    
    return counter;

}

float countAverage(float n1, float n2) // count average per 100
{
    return 100 * n1 / n2;
    
}

void printColemanLiauIndex(float L, float S) // print Coleman Liau Index result
{
    float f = 0.0588 * L - 0.296 * S - 15.8;
    int i = 0;
    
    if (f < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (f > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        i = (int) round(f);
        printf("Grade %i\n", i);
        
    }
}