#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Set definitions.
#define SET_SIZE 256

bool
isKeyChar(char c) {
        switch(c) {
                case '(':
                case ')':
                case '+':
                case '*':
                return true;
        }
        return false;
}

void
linearize(char* regex, char** linear_regex, char** mapping)
{
        int i, j;
        int n = 0; //Number of characters to linearize.

        i = 0;
        while (regex[i] != '\0') {
                n += isKeyChar(regex[i]) ? 0 : 1;
                i++;
        }

        *linear_regex = malloc(n*sizeof(int));
        *mapping = malloc(n*sizeof(char));

        i = 0;
        j = 0;
        while (j < n) {
                char c = regex[i];
                (*linear_regex)[i] = isKeyChar(c) ? c : j;
                if (!isKeyChar(c)) {
                        (*mapping)[j] = c;
                        j++;
                }
                i++;
        }
}

void
glushkov(char* regex)
{
        char* lregex = NULL;
        char* lmapping = NULL;

        linearize(regex, &lregex, &lmapping);
}

int
main(void)
{
        char* regex = "baa\0";
        glushkov(regex);

        
        printf("Done!");
	return (0);
}