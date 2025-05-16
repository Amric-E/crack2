#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        
const int HASH_LEN = 33;        

// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
// Get this function working first!

#define HASH_LINE_LEN 256
#define MAX_WORD_LEN 100

char *tryWord(char *plaintext, char *hashFilename) {
    // Hash the plaintext
    char *computedHash = md5(plaintext, strlen(plaintext));
    if (!computedHash) {
        return NULL;
    }

    // Open the hash file
    FILE *file = fopen(hashFilename, "r");
    if (!file) {
        perror("Could not open hash file");
        free(computedHash);
        return NULL;
    }

    // Loop through the hash file, one line at a time.
    char line[HASH_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Comparing file hash to computed hash
        if (strcmp(computedHash, line) == 0) {
            fclose(file);
            char *result = strdup(computedHash);
            free(computedHash);
            return result;
        }
    }

    // Cleanup or return NULL if no match
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
    
    // New add below
    //Erorr propmt if unable to open
    FILE *dictFile = fopen(argv[2], "r");
    if (!dictFile) {
        perror("ERORR** can not open the dictionary file please fix");
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


