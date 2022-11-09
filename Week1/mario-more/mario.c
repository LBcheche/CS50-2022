#include <cs50.h>
#include <stdio.h>

void printPyramids(int h);

int main(void)
{
    int height;
    int minHeight = 1;
    int maxHeight = 8;
    
    do
    {
        height = get_int("Type pyramid height:\n");
    }
    while (height < minHeight || height > maxHeight); // rejeita entradas indevidas e solicita novamente.
    
    printPyramids(height);
    
}

void printPyramids(int h)
{
    for (int i = 0; i < h; i++) //loop of rows
    {
        for (int j = 0; j < h - i - 1; j++) //loop of spaces in a row (first pyramid)
        {
            printf(" "); //print spaces in a row
        }
        
        for (int j = 0; j <= i; j++) //loop of # in a row (first pyramid)
        {
            printf("#"); //print # in a row
        }

        printf("  "); // print space between pyramids
        
        for (int j = 0; j <= i; j++) //loop of # in a row (second pyramid)
        {
            printf("#"); //print # in a row 
        }
        
        printf("\n"); //go next row
    }   

}
