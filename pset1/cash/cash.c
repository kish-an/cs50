#include <stdio.h>
#include <cs50.h>
#include <math.h>

float get_positive_float(string question);

int main(void)
{
    float dollars = get_positive_float("How much change are you owed? ");
    int coins = round(dollars * 100);
    
    int count = 0;
    while (coins > 0)
    {
        if (coins >= 25) 
        {
            coins -= 25;
            count++;
        }
        else if (coins >= 10)
        {
            coins -= 10;
            count++;
        }
        else if (coins >= 5)
        {
            coins -= 5;
            count++;
        }
        else 
        {
            coins--;
            count++;
        }
    }
    
    printf("%i\n", count);
}


float get_positive_float(string question)
{
    float n;
    do 
    {
        n = get_float("%s", question);
    }
    while (n < 0);
    
    return n;
}
