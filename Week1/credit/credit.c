#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

bool checkCardLenght(long cL);
int countCardLenght(long cN);
bool checkSum(long cN, int cL);
void checkAndPrintFlag(long cN, int cL);
void checkAndPrintFlagLenght13(long cN, int cL);
void checkAndPrintFlagLenght15(long cN, int cL);
void checkAndPrintFlagLenght16(long cN, int cL);  

int main(void)
{
    long cardNumber;
    int cardLenght;
    
    do 
    {
        cardNumber = get_long("Type credit card number:");    
    }
    while (cardNumber < 0); // prompt an non-negative number input for user

    cardLenght = countCardLenght(cardNumber);

    if (checkCardLenght(cardLenght) == true) 
    {
        if (checkSum(cardNumber, cardLenght) == true)
        {
            checkAndPrintFlag(cardNumber, cardLenght);
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}
int countCardLenght(long cN) // counts card number's lenght
{
    long powerX;
    long rest;
    
    for (int i = 13 ; i <= 16 ; i++)
    {
        powerX = pow(10, i);
        rest = cN / powerX;
        
        if (rest == 0)
        {    
            return i;
        }
    }
    return 0;
}

bool checkCardLenght(long cL) // checks if card number's lenght iguals 13, 15 ou 16
{
    if (cL == 13 || cL == 15 || cL == 16)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool checkSum(long cN, int cL) //calculates and verify checkSum Luhn's Algorithm
{
    long firstSum = 0;      // first Sum for checkSum Luhn's Algorithm
    long secondSum = 0;     // second Sum for checkSum Luhn's Algorithm
    int digit;              // each single digit from card Number
    int secondDigit;        // second digit if first digit > 9 after times 2
    bool toggle = false;    // boolean to alternate between sums.
    
    for (int i = 0 ; i < cL ; i++)
    {
        digit = cN % 10;
        cN = cN / 10;
    
        if (toggle == true) // calculate first sum 
        {
            digit = digit * 2;
            
            if (digit >= 10)
            {
                secondDigit = digit;
                secondDigit = secondDigit / 10;
                digit = digit % 10;
                digit +=  secondDigit;
            }
                
            firstSum += digit ;
            toggle = false;
        }
        else // calculate second sum
        {
            secondSum += digit ;
            toggle = true;
        }
    
    }
   
    secondSum += firstSum; // final sum for second sum

    if (secondSum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void checkAndPrintFlag(long cN, int cL) // checks first digits and prints flag for AMEX, MASTERCARD and VISA
{
    checkAndPrintFlagLenght13(cN, cL);
    checkAndPrintFlagLenght15(cN, cL);
    checkAndPrintFlagLenght16(cN, cL);  
}
void checkAndPrintFlagLenght13(long cN, int cL) // checks first digits and prints flag for leght 13 card's size
{
    long fD; // first digits

    if (cL == 13)
    {
        fD = cN / pow(10, 12);
        if (fD == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    } 
}
void checkAndPrintFlagLenght15(long cN, int cL) // checks first digits and prints flag for leght 15 card's size
{
    long fD; // first digits

    if (cL == 15)
    {
        fD = cN / pow(10, 13);
        if (fD == 34 || fD == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
}

void checkAndPrintFlagLenght16(long cN, int cL) // checks first digits and prints flag for leght 16 card's size
{
    long fD; // first digits

    if (cL == 16)
    {
        fD = cN / pow(10, 14);
        
        if (fD == 51 || fD == 52 || fD == 53 || fD == 54 || fD == 55)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            fD = cN / pow(10, 15);
            if (fD == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
}
    

