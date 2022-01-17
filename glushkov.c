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
linearize(char* regex, int** linear_regex, char** mapping)
{
        int n = (int) strlen(regex);
        *linear_regex = malloc(n*sizeof(int));
        *mapping = malloc(n*sizeof(char));

        for(int i = 0; i < n; i++){
                char c = regex[i];
                if (!isKeyChar(c)) {
                        *mapping[i] = regex[i];
                        *linear_regex[i] = i;
                }
        }
}

int
glushkov(char* regex)
{
        int* lregex = NULL;
        char* lmapping = NULL;

        linearize(regex, &lregex, &lmapping);
	return (0);
}

int
main(void)
{
        glushkov("baa");
	return (0);
}