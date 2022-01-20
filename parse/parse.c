#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "parse.h"

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