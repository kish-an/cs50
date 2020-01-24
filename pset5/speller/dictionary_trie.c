/* Trie Implementation */

// Implements a dictionary's functionality

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

#define ALPHABET_SIZE 27

// Represents a node in a trie
typedef struct trieNode
{
    struct trieNode *next[ALPHABET_SIZE];
    bool endOfWord;
}
trieNode;

// Gets a trie node
trieNode *getNode(void)
{
    // Create a null pointer
    trieNode *pNode = NULL;
    // Set to size of a trie node
    pNode = calloc(1, sizeof(trieNode));
    if (pNode == NULL)
    {
        printf("Could not allocate memory for trie node\n.");
        pNode = NULL;
        return pNode;
    }
    else
    {
        pNode->endOfWord = false;
        // Set every pointer in node array to NULL
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            pNode->next[i] = NULL;
        }
    }
    return pNode;
}

int words = 0;

// Inserts key into trie
void insertNode(trieNode *rootNode, char *key)
{
    int length = strlen(key);
    int index;

    // Create temp traversal pointer
    trieNode *trav = rootNode;

    for (int i = 0; i < length; i++)
    {
        // Get index from character (a = 0, b = 1...)
        if (key[i] == '\'')
        {
            index = ALPHABET_SIZE - 1;
        }
        else
        {
            // All dictionary words are lower case by default
            index = key[i] - 97;
        }

        // Point to another node for next character (if it doesn't already exist)
        if (trav->next[index] == NULL)
        {
            trav->next[index] = getNode();
        }
        // Update traversal pointer
        trav = trav->next[index];
    }
    // Mark last node (leaf) after loop as end of the word
    trav->endOfWord = true;
}

// Set up root node of trie (can not set root to getNode() in global scope);
struct trieNode *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    root = getNode();

    // Open dictionary and all all words to trie
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(dict, "%s", word) != EOF)
    {
        insertNode(root, word);
        words++;
    }

    fclose(dict);
    return true;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int length = strlen(word);
    int index;

    // Create temp traversal pointer
    trieNode *trav = root;

    for (int i = 0; i < length; i++)
    {
        // Get index from character (a = 0, b = 1...)
        if (word[i] == '\'')
        {
            index = ALPHABET_SIZE - 1;
        }
        else
        {
            index = tolower(word[i]) - 97;
        }

        // Check if next node is null (a.k.a word doesn't exist)
        if (trav->next[index] == NULL)
        {
            return false;
        }
        // Update traversal pointer
        trav = trav->next[index];
    }

    return (trav != NULL && trav->endOfWord);
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Recursive function to destroy trie
bool unloader(trieNode *node)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->next[i] != NULL)
        {
            unloader(node->next[i]);
        }

    }
    free(node);
    return true;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
   return unloader(root);
}
