#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");

    int letters = 0, words = 0, sentences = 0;

    for (int i = 0; i <= strlen(text); i++)
    {
        if (text[i] == ' ' || text[i] == '\0')
        {
            words++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
        else if (isalpha(text[i]))
        {
            letters++;
        }
    }
    // printf("letters: %i\nwords: %i\nsentences: %i\n", letters, words, sentences);

    const float AVERAGE = 100.0 / words;
    float index = (0.0588 * AVERAGE * letters) - (0.296 * AVERAGE * sentences) - 15.8;
    // printf("index: %f\n", index);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}