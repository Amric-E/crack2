#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings
//const int HASH_LINE_LEN = 256;  // Length of the hash line

// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
// Get this function working first!

#define HASH_LINE_LEN 256
#define MAX_WORD_LEN 100

char *tryWord(char *plaintext, char *hashFilename) {
    // Step 1: Hash the plaintext
    char *computedHash = md5(plaintext, strlen(plaintext));
    if (!computedHash) {
        return NULL;
    }

    // Step 2: Open the hash file
    FILE *file = fopen(hashFilename, "r");
    if (!file) {
        perror("Could not open hash file");
        free(computedHash);
        return NULL;
    }

    // Step 3: Loop through the file line by line
    char line[HASH_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Step 4: Compare the file hash to computed hash
        if (strcmp(computedHash, line) == 0) {
            fclose(file);
            char *result = strdup(computedHash); // copy so caller can free
            free(computedHash);
            return result;
        }
    }

    // Step 5: Cleanup and return NULL if no match
    fclose(file);
    free(computedHash);
    return NULL;
}

   int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    FILE *dictFile = fopen(argv[2], "r");
    if (!dictFile) {
        perror("Could not open dictionary file");
        exit(1);
    }

    char word[MAX_WORD_LEN];
    int crackedCount = 0;

    // For each word in dictionary
    while (fgets(word, sizeof(word), dictFile)) {
        // Remove newline character
        size_t len = strlen(word);
        if (len > 0 && word[len - 1] == '\n') {
            word[len - 1] = '\0';
        }

        // Try the word
        char *hash = tryWord(word, argv[1]);
        if (hash) {
            printf("%s %s\n", hash, word);
            crackedCount++;
            free(hash);
        }
    }

    fclose(dictFile);

    // Prints number of hashes cracked
    printf("Cracked %d hashes!\n", crackedCount);

    return 0;
}


