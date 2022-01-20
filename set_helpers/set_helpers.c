#include "../set/src/set.h"
#include "../parse/parse.h"
#include <math.h>

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

SimpleSet*
set_concat(SimpleSet* set0, SimpleSet* set1) {
        SimpleSet* set2 = (SimpleSet*) malloc(sizeof(SimpleSet));
        set_init(set2);

        uint64_t size0, size1;
        char** arr0 = set_to_array(set0, &size0);
        char** arr1 = set_to_array(set1, &size1);
        for (int i = 0; i < (int) size0; i++) {
                for (int j = 0; j < (int) size1; j++) {
                        char* str0 = arr0[i];
                        char* str1 = arr1[j];
                        int l0 = (int) strlen(str0);
                        int l1 = (int) strlen(str1);
                        char* concat_str = (char *) malloc((l0+l1+1)*sizeof(char));
                        memcpy(concat_str, str0, l0);
                        memcpy(concat_str + l0, str1, l1 + 1); // +1 adds '\0' as well.
                        set_add(set2, concat_str);
                }          
        }

        free(arr0);
        free(arr1);
        return set2;
}