#include <cs50.h>
#include <stdio.h>
#include <math.h>

void verifyCoin(int coin);
void countCoins();
int cents;
int counter = 0;

int main(void)
{
    float dollars;

    do
    {
        dollars = get_float("Change owed:"); //ask user for change value.
    } 
    while (dollars < 0);
    
    cents = round(dollars * 100); //round float to int for greater precision.

    countCoins(); // function that count minimun coins for change.

    printf("%i\n", counter); // print number of coins.

}

void countCoins()
{
    while (cents > 0)
    {
        verifyCoin(25); // verify how many $0,25 coins need for change.
        verifyCoin(10); // verify how many $0,10 coins need for change.
        verifyCoin(5);  // verify how many $0,05 coins need for change.
        verifyCoin(1);  // verify how many $0,01 coins need for change.
    }
}

void verifyCoin(int coin) // function that add amount of coins to counter. 
{
    if (cents >= coin)
    {
        do
        {
            cents -= coin;
            counter ++;

        } 
        while (cents >= coin); 
    }
}
