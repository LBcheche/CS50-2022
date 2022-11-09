#include <cs50.h>
#include <stdio.h>

void printPyramid(int h);

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
    
    printPyramid(height);
    
}

void printPyramid(int h)
{
    for (int i = 0; i < h; i++) //loop of rows
    {
        for (int j = 0; j < h - i - 1; j++) //loop of spaces in a row
        {
            printf(" "); //print spaces in a row
        }
        
        for (int j = 0; j <= i; j++) //loop of # in a row
        {
            printf("#"); //print # in a row
        }
        
        printf("\n"); //go next row
    }   

}
