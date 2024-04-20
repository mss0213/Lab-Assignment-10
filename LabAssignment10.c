#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* This program was written by Michael Schlosbon on 4/19/2024. The purpose was to construct a program that would take a string and print out the amount of occurrences through a trie format. */

// Trie structure
struct Trie
{
    struct Trie *children[26]; // Assuming only lowercase alphabets
    int isEndOfWord;
};

struct Trie *createTrie();

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            current->children[index] = createTrie();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    int counter = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            break;
        }
        current = current->children[index];
        counter++;
    }
    return counter;
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *pTrie)
{
    if (!pTrie)
    {
        return;
    }
    for (int i = 0; i < 26; i++)
    {
        deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *newNode = (struct Trie *)malloc(sizeof(struct Trie));
    if (newNode)
    {
        newNode->isEndOfWord = 0;
        for (int i = 0; i < 26; i++)
        {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

int readDictionary(char *filename, char ***pInWords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int wordCount = 0;
    char word[100]; // Assuming maximum word length is 100 characters
    while (fscanf(file, "%s", word) == 1) {
        wordCount++;
    }

    // Allocate memory for words
    *pInWords = (char **)malloc(wordCount * sizeof(char *));
    rewind(file);

    int i = 0;
    while (fscanf(file, "%s", word) == 1) {
        (*pInWords)[i] = strdup(word); // Assuming strdup is available
        i++;
    }

    fclose(file);
    return wordCount;
}

int main(void)
{
  char *inWords[256];
  for (int i = 0; i < 256; i++) {
      inWords[i] = malloc(100 * sizeof(char)); // Allocating memory for each word
  }
  // Read the number of words in the dictionary
  int numWords = readDictionary("dictionary.txt", &inWords);
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Words to check for occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate Trie
    deallocateTrie(pTrie);

    return 0;
}