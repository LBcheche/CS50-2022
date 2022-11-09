#include <stdio.h>
#include <cs50.h> // include this to use get_string function.

int main(void) // main function where program starts.
{
    string name = get_string("What is your name?\n"); // ask user´s name and assign to variable called "name".

    printf("hello, %s\n", name); // print "hello" + content at variable name.
}
