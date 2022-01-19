#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "set/src/set.h"
#include "set/src/set.c"

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

int
set_add_char(SimpleSet* set, char c) {
        char* str = (char*) malloc(2*sizeof(char));
        str[0] = c;
        str[1] = '\0';
        return set_add(set, str);
}

void
set_print(SimpleSet* set) {
        uint64_t size;
        char** arr = set_to_array(set, &size);
        int n = (int) size;
        printf("(Size: %i) ", n);
        for(int i = 0; i < n; i++) 
                printf("%c, ", (*arr)[i]);
        printf("\n");
        return;
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
        char* sub = malloc((n+1)*sizeof(char));
        sub[n+1] = '\0'; 
        memcpy(sub, str+i, n);
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
 
        printf("Expression: \"%s\" (n=%i)\n", expression, (int) strlen(expression));
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

/*
*Return false if empty, return true if has empty string
*/
bool
compute_A(p_node_t* root_p)
{
        if (root_p == NULL) //A(emptyset) = emptyset
                return false; 

        char c = root_p->c;
        if (!root_p->isOp)
                return c == '\0'; //A(emptystr) = {emptystr}, A(a) = emptyset

        bool L = compute_A(root_p->left);
        bool R = compute_A(root_p->right);

        switch(c) {
                case '+':
                        return L || R; //A(e+f) = A(e) union A(f)
                case '.':
                        return L && R; //A(e.f) = A(e).A(f)
                case '*':
                        return true;
        }
        
        assert(true);
        return false;
}

SimpleSet*
compute_P(p_node_t* root_p)
{
        SimpleSet* P = (SimpleSet*) malloc(sizeof(SimpleSet));
        set_init(P);

        if (root_p == NULL)
                return P;

        char c = root_p->c;
        if (!root_p->isOp) {
                set_add_char(P, c);
                return P;
        } 

        SimpleSet* L = compute_P(root_p->left);
        SimpleSet* R = compute_P(root_p->right);

        switch(c) {
                case '+': //P(e) union P(f)
                        set_union(P, L, R);
                        break;
                case '.': //P(e) union A(e)P(f)
                        if (compute_A(root_p->left)) {
                                set_union(P, L, R);
                        } else {
                                *P = *L;  
                        }
                        break;
                case '*': //P(e*) = P(e)
                        *P = *L;
                        break;
        }

        free(L);
        free(R);

        return P;
}

SimpleSet*
compute_D(p_node_t* root_p)
{
        SimpleSet* D = (SimpleSet*) malloc(sizeof(SimpleSet));
        set_init(D);

        if (root_p == NULL) {
                return D;
        }

        char c = root_p->c;
        if (!root_p->isOp) {
                set_add_char(D, c);
                return D;
        } 

        SimpleSet* L = compute_D(root_p->left);
        SimpleSet* R = compute_D(root_p->right);

        switch(c) {
                case '+': //D(e) union D(f)
                        set_union(D, L, R);
                        break;
                case '.': //D(e) union D(e)A(f)
                        if (compute_A(root_p->right)) {
                                set_union(D, L, R);
                        } else {
                                *D = *R;
                        }
                        break;
                case '*': //D(e*) = D(e)
                        *D = *L;
                        break;
        }

        free(L);
        free(R);

        return D;
}

void
glushkov(char* regex)
{
        // int* lregex = NULL;
        // char* lmapping = NULL;

        //Step 1: Linearize
        //int n = linearize(regex, &lregex, &lmapping);

        //Parse regex
        p_node_t* root_p = parse(regex);

        //Step 2: Compute sets P, D and F
        SimpleSet* P = compute_P(root_p);
        SimpleSet* D = compute_P(root_p);
        printf("P: ");
        set_print(P);
        printf("D: ");
        set_print(D);

        // SimpleSet F;
}

int
main(void)
{
        //char* regex = "(((a.((a.b)*))*)+((b.a)*))"; //BNAF of (a(ab)*)* + (ba)*
        char* regex = "(a.b)";
        glushkov(regex);

        printf("Done!");
	return (0);
}
