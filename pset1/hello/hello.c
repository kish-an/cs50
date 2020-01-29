#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // store input in variable name of string type
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}
