#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Set definitions.
#define SET_SIZE 256

typedef struct p_node {
        bool isOp;
        char c;
        struct p_node* left;
        struct p_node* right;
} p_node_t; //Parse node

bool
isOp(char c) {
        switch(c) {
                case '.':
                case '+':
                case '*':
                return true;
        }
        return false;
}

bool
isPara(char c) {
        switch(c) {
                case '(':
                case ')':
                return true;
        }
        return false;
}

bool
isKeyChar(char c) {
        return isOp(c) || isPara(c);
}

// int
// linearize(char* regex, char** linear_regex, char** mapping)
// {
//         int i, j;
//         int n = 0; //Number of characters to linearize.

//         i = 0;
//         while (regex[i] != '\0') {
//                 n += isKeyChar(regex[i]) ? 0 : 1;
//                 i++;
//         }

//         *linear_regex = malloc(n*sizeof(int));
//         *mapping = malloc(n*sizeof(char));

//         i = 0;
//         j = 0;
//         while (j < n) {
//                 char c = regex[i];
//                 (*linear_regex)[i] = isKeyChar(c) ? c : j;
//                 if (!isKeyChar(c)) {
//                         (*mapping)[j] = c;
//                         j++;
//                 }
//                 i++;
//         }
//         return n;
// }

p_node_t*
makeOpNode(char op, p_node_t *left_p, p_node_t *right_p)
{
        p_node_t* node = (p_node_t*) malloc(sizeof(p_node_t));
        node->isOp = true;
        node->c = op;
        node->left = left_p;
        node->right = right_p;
        return node;
}

p_node_t*
makeLeafNode(char c)
{
        p_node_t* node = (p_node_t*) malloc(sizeof(p_node_t));
        node->isOp = false;
        node->c = c;
        node->left = NULL;
        node->right = NULL;
        return node;
}

char*
substring(char* str, int i, int j)
{
        int n = j - i;
        assert (0 <= n);
        char* sub = malloc(n*sizeof(char));
        sub[n+1] = '\0'; 
        memcpy(sub, str, n);
        return sub;
}

/* 
 * BNF Parser
 */
p_node_t*
parse(char* expression)
{
        int n = strlen(expression);
        int l = 0;
        int r = 0;

        if (n <= 1) {
                char c = expression[0];
                return makeLeafNode(c);
        }

        for (int i = 0; i < n; i++) {
                char c = expression[i];
                if (c == '(') {
                        l++;
                } else if (c == ')')  {
                        r++;
                } else if (l - r == 1) {
                        bool isUnaryOp = c == '*';
                        bool isBinaryOp = isOp(c) && !isUnaryOp;

                        if (isBinaryOp) {
                                p_node_t *left_p = parse(substring(expression, 1, i));
                                p_node_t *right_p = parse(substring(expression, i+1, (int) strlen(expression) - 1));
                                return makeOpNode(c, left_p, right_p);
                        } else if (isUnaryOp) {
                                p_node_t *subformula = parse(substring(expression, 2, (int) strlen(expression) - 1));
                                return makeOpNode(c, subformula, NULL); //Assume unary op only uses left.
                        }
                }            
        }
        return NULL;
}

// void
// compute_sets(p_node_t* root_p, char** P_p, char** D_p, char** F_p)
// {
//         if (!root_p->isOp) {
//                 char** P_p
//         }
// }

void
glushkov(char* regex)
{
        // int* lregex = NULL;
        // char* lmapping = NULL;

        //Step 1: Linearize
        //int n = linearize(regex, &lregex, &lmapping);

        //Parse regex
        p_node_t* root_p = parse(regex);

        root_p 
        (void) root_p;

        //Step 2: Compute sets P, D and F
        // int* P = calloc(n, sizeof(bool)); //Set
        // int* D = calloc(n, sizeof(char)); //End characters.
        // int* F = calloc(n, sizeof(char)); //Pair characters.

        // compute_sets(root_p, &P, &D, &F);
}

int
main(void)
{
        char* regex = "(b.(a*))";
        glushkov(regex);

        printf("Done!");
	return (0);
}