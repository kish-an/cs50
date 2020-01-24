#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int validate_key(int argc, string argv[]);

int main(int argc, string argv[])
{
    int key;

    if (validate_key(argc, argv) == -1)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        key = validate_key(argc, argv);
    }

    string plain_txt = get_string("plaintext: ");

    printf("ciphertext: ");

    for (int i = 0, n = strlen(plain_txt); i < n; i++)
    {
        if (isalpha(plain_txt[i]))
        {
            int cipher = (plain_txt[i] + key);
            int shift;

            if (isupper(plain_txt[i]))
            {
                shift = 65;
            }
            else
            {
                shift = 97;
            }

            // calculates new char position (A=0, B=1..), accounts for wrapping back from Z to A
            cipher = (cipher - shift) % 26;
            // add shift back to print ASCII value as char
            printf("%c", cipher + shift);
        }
        else
        {
            printf("%c", plain_txt[i]);
        }
    }

    printf("\n");
    return 0;
}

int validate_key(int argc, string argv[])
{
    if (argc != 2)
    {
        return -1;
    }

    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            return -1;
        }
    }

    int key = atoi(argv[1]);
    return key;
}