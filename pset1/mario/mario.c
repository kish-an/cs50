#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // initialise height 
    int height;
    
    // prompt user for input 
    do 
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    
    for (int i = 0; i < height; i++)
    {
        // print co
        for (int j = 0; j < (height - i) - 1; j++) 
        {
            printf(" ");
        }
        for (int k = 0; k < (i + 1); k++) 
        {
            printf("#");
        }
        // print new line after both inner loops finish
        printf("\n");
    }
    
}
