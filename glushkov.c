#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "set/src/set.h"
#include "set/src/set.c"
#include <math.h>

// Set definitions.
#define SET_SIZE 256

typedef struct p_node {
        bool isOp;
        char c;
        int leaf_id; //Unique ID for leaves
        struct p_node* left;
        struct p_node* right;
        char* str;
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
set_add_node(SimpleSet* set, p_node_t* node) {
        int id = node->leaf_id; //Unique ID for linerization. 
        char c = node->c; //Actual character being repsented.

        char* str = (char*) malloc((int)((ceil(log10(id))+2)*sizeof(char)));
        str[0] = c; 
        sprintf(str+1, "%i", node->leaf_id); 

        return set_add(set, str);
}

void
set_print(SimpleSet* set) {
        uint64_t size;
        char** arr = set_to_array(set, &size);

        int n = (int) size;
        printf("(Size: %i) ", n);
        for(int i = 0; i < n; i++) 
                printf("%s, ", arr[i]);
        printf("\n");
        free(arr);
        return;
}

p_node_t*
makeOpNode(char op, p_node_t *left_p, p_node_t *right_p, char* exp)
{
        p_node_t* node = (p_node_t*) malloc(sizeof(p_node_t));
        node->isOp = true;
        node->c = op;
        node->left = left_p; 
        node->right = right_p;
        node->str = exp;
        return node;
}

p_node_t*
makeLeafNode(char c, char* exp)
{
        static int id = 0;
        p_node_t* node = (p_node_t*) malloc(sizeof(p_node_t));
        node->isOp = false;
        node->c = c;
        node->left = NULL;
        node->right = NULL;
        node->str = exp;
        node->leaf_id = id++;
        return node;
}

char*
substring(char* str, int i, int j)
{
        int n = j - i;
        assert (0 <= n);
        char* sub = (char*) malloc((n+1)*sizeof(char));
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
        if (n <= 1)  
                return makeLeafNode(expression[0], expression);

        for (int i = 0; i < n; i++) {
                char c = expression[i];
                if (c == '(') {
                        l++;
                } else if (c == ')')  {
                        r++;
                } else if (l - r == 1) {
                        bool isUnaryOp = c == '*';
                        bool isBinaryOp = isOp(c) && !isUnaryOp;

                        p_node_t *left_p = parse(substring(expression, 1, i));

                        if (isBinaryOp) {
                                p_node_t *right_p = parse(substring(expression, i+1, (int) strlen(expression) - 1));
                                return makeOpNode(c, left_p, right_p, expression);
                        } else if (isUnaryOp) {
                                return makeOpNode(c, left_p, NULL, expression); //Assuming unary op only uses left.
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
                if (c != '\0')
                        set_add_node(P, root_p);
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

        printf("P(\"%s\"): ", root_p->str);
        set_print(P);
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
                if (c != '\0')
                        set_add_node(D, root_p);
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

        printf("D(\"%s\"): ", root_p->str);
        set_print(D);
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
        SimpleSet* D = compute_D(root_p);
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
        char* regex = "((a.b).a)";
        glushkov(regex);

        printf("Done!");
	return (0);
}
