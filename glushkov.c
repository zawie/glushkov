#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "parse/parse.h"
#include "set/src/set.h"
#include "set_helpers/set_helpers.h"
#include "parse/parse.c"
#include "set/src/set.c"
#include "set_helpers/set_helpers.c"

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

SimpleSet*
compute_F(p_node_t* root_p)
{
        if (root_p == NULL || !root_p->isOp) { //F(0) = F(emptystr) = F(a) = \emptyset
                SimpleSet* F = (SimpleSet*) malloc(sizeof(SimpleSet));
                set_init(F);
                return F;
        } 
              
        char op = root_p-> c;

        SimpleSet* F0;
        SimpleSet* L = compute_F(root_p->left);

        if (op == '+' || op == '.') {
                F0 = (SimpleSet*) malloc(sizeof(SimpleSet));
                set_init(F0);
                SimpleSet* R = compute_F(root_p->right);
                set_union(F0, L, R); //Let F0 = F(e) union F(f)
                free(R);
                free(L);     
        }
        if (op == '*')
                F0 = L; //Let F0 = L = F(e)
        if(op == '+')
                return F0;
        
        //Note, op == '+' or op == '*'

        SimpleSet* F1;
        F1 = (SimpleSet*) malloc(sizeof(SimpleSet));
        set_init(F1);

        SimpleSet* De = compute_D(root_p->left);
        if (op == '.') {
                SimpleSet* Pf = compute_P(root_p->right);
                SimpleSet* DePf = set_concat(De, Pf);
                set_union(F1, F0, DePf);
                free(DePf);
                free(Pf);
        } else { //op == '*'
                SimpleSet* Pe = compute_P(root_p->left);
                SimpleSet* DePe = set_concat(De, Pe);
                set_union(F1, F0, DePe);
                free(DePe); 
                free(Pe);   
        }
        free(De);
        free(F0); //NOT THE FINAL F

        return F1;
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
        SimpleSet* F = compute_F(root_p);

        printf("P: ");
        set_print(P);
        printf("D: ");
        set_print(D);
        printf("F: ");
        set_print(F);
                    
}

int
main(void)
{
        char* regex = "(((a.((a.b)*))*)+((b.a)*))"; //BNAF of (a(ab)*)* + (ba)*
        //char* regex = "((a.b).a)";
        glushkov(regex);

        printf("Done!");
	return (0);
}
