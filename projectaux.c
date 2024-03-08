#include "project.h"
#include "constants.h"
#include "projectaux.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidRegistration(char *reg) {
    // Check if the length of the registration is valid
    if (strlen(reg) != REGISTRATION_LENGTH) {
        return -1;
    }

    // Initialize counts for letters and digits
    int letterCount = 0;
    int digitCount = 0;

    // Iterate through each character in the registration
    for (int i = 0; i < REGISTRATION_LENGTH; i++) {
        char cur = reg[i];

        // Check if the current character is a letter (uppercase)
        if (cur >= 'A' && cur <= 'Z') {
            letterCount++;

            // Check if the next character is also a letter (a pair cannot contain a mix of letters and digits)
            if (i < REGISTRATION_LENGTH - 1 && reg[i + 1] >= 'A' && reg[i + 1] <= 'Z') {
                return -1;
            }
        }
        // Check if the current character is a digit
        else if (cur >= '0' && cur <= '9') {
            digitCount++;

            // Check if the next character is also a digit (a pair cannot contain a mix of letters and digits)
            if (i < REGISTRATION_LENGTH - 1 && reg[i + 1] >= '0' && reg[i + 1] <= '9') {
                return -1;
            }
        }
        // Check if the current character is a hyphen
        else if (cur != '-') {
            // If the character is not a hyphen, it must be either a letter or a digit
            return -1;
        }
    }

    // Check if the registration contains at least one pair of letters and one pair of digits
    if (letterCount < 2 || digitCount < 2) {
        return -1;
    }

    // If all checks pass, the registration is valid
    return 0;
}

/*
 * Reads the input from the user and inserts it into the buffer.
 */
Buffer *getBuffer(Buffer *buffer) {
    char c;
    int count;

    memset(buffer->buffer, 0, BUFSIZE);
    buffer->index = 0;

    for (count = 0; (c = getchar()) != '\n'; count++) {
        buffer->buffer[count] = c;
    }

    return buffer;
}


char *nextWord(Buffer *buffer) {
    int i = 0;
    char *next_word = (char *)malloc(strlen(buffer->buffer) * sizeof(char));

    if (next_word == NULL) {
        exit(1);
    }

    while (buffer->buffer[buffer->index] == ' ' || buffer->buffer[buffer->index] == '\n') {
        buffer->index++;
    }

    if (buffer->buffer[buffer->index] == '"') {
        buffer->index++;
        while (buffer->buffer[buffer->index] != '"') {
            next_word[i] = buffer->buffer[buffer->index];
            i++, buffer->index++;
        }
        buffer->index++;
    } else {
        while (buffer->buffer[buffer->index] != ' ' && buffer->buffer[buffer->index] != '\n' &&
               buffer->buffer[buffer->index] != '\0') {
            next_word[i] = buffer->buffer[buffer->index];
            i++, buffer->index++;
        }
    }
    next_word[i] = '\0';

    if (i == 0) {
        free(next_word);
        return NULL;
    } else {
        return next_word;
    }
}