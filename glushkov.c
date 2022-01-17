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
        int n = (int) strlen(regex);
        *linear_regex = malloc(n*sizeof(int));
        *mapping = malloc(n*sizeof(char));

        for(int i = 0; i < n; i++){
                char c = regex[i];
                if (!isKeyChar(c)) {
                        *mapping[i] = c;
                }
                *linear_regex[i] = isKeyChar(c) ? c : (char) i;
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
        char* regex = "baa";
        glushkov(regex);
	return (0);
}