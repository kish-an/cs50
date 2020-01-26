/* Hash Table Implementation */

// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table (150% amount of words in dictionary)
const unsigned int N = 250000;

// Number of words in dictionary
unsigned int words = 0;

// Hash table
node *table[N];

// Gets a node for a singly linked list
node *getNode(const char *key)
{
    node *pNode = malloc(sizeof(node));
    if (pNode == NULL)
    {
        printf("Could not allocate memory for linked list node.\n");
        return pNode;
    }
    strcpy(pNode->word, key);
    // Set next to NULL pointer
    pNode->next = NULL;

    return pNode;
}

// Pass pointer to pointer for head node
void insertNode(node **head, const char *key)
{
    // Create node
    node *pNode = getNode(key);

    // Insert node into linked list
    if (*head != NULL)
    {
        // Make new node point to first item in linked list (a.k.a head)
        pNode->next = *head;

    }
    // Now point head to new node
    *head = pNode;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Set all next pointers to NULL in hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char word[LENGTH + 1];
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Get key from hash function
        unsigned int key = hash(word);

        // Get pointer address of head node (which is a pointer itself thus node**)
        node **head = &table[key];
        // Add value to Hash table
        insertNode(head, word);

        words++;
    }
    fclose(dict);
    return true;
}

// Hashes word to a number
// DJB2 Hashing Algorithim adapted from http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % N;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Convert word to lowercase to get same key from hash function
    char copy[strlen(word) + 1];
    strcpy(copy, word);
    char *p = copy;
    for (; *p; ++p)
    {
        *p = tolower(*p);
    }

    unsigned int key = hash(copy);

    // Temp traversal pointer (points to head)
    node *trav = table[key];

    // Traverse through linked list (linear search)
    while (trav != NULL)
    {
        if (strcmp(copy, trav->word) == 0)
        {
            return true;
        }
        trav = trav->next;
    }
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

void unloader(node *node)
{
    if (node->next != NULL)
    {
        unloader(node->next);
    }
    free(node);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Skip if no linked list exists in element
        if (table[i] == NULL)
        {
            continue;
        }
        unloader(table[i]);
    }
    return true;
}
