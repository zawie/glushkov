#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Set definitions.
#define SET_SIZE 256

struct p_node_t {
        char*
}
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

int
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
        return n;
}

void
glushkov(char* regex)
{
        char* lregex = NULL;
        char* lmapping = NULL;

        //Step 1: Linearize
        int n = linearize(regex, &lregex, &lmapping);

        //Parse regex
        p_node_t parse_node = parse(lregex);
        //Step 2: Compute sets P, D and F
        //Start characters.
        char* P = calloc(n*sizeof(bool)); //Set

        char* D = malloc(n*sizeof(char)); //End characters.
        char* F = malloc(n*sizeof(char)); //Pair characters.
}

int
main(void)
{
        char* regex = "baa\0";
        glushkov(regex);

        printf("Done!");
	return (0);
}