#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

// Set definitions.
#define MAX 256
#define set_chunk_t long 
#define set_t set_chunk_t*

set_t
makeset() 
{
      long* set = calloc(MAX/sizeof(set_chunk_t), sizeof(set_chunk_t));  
      return set;
}

void
set_add(set_t set, int entry)
{
        int offset = entry/sizeof(set_chunk_t);
        short bit = entry % sizeof(set_chunk_t);
        set_chunk_t* chunk = set + offset;

        *chunk = *chunk && 1 >> bit; //Set bit.
}

void
set_remove(set_t set, int entry)
{
        int offset = entry/sizeof(set_chunk_t);
        short bit = entry % sizeof(set_chunk_t);
        set_chunk_t* chunk = set + offset;

        *chunk = *chunk ^ 1 >> bit; //Unset bit.
}

bool
set_contains(set_t set, int entry)
{
        int offset = entry/sizeof(set_chunk_t);
        short bit = entry % sizeof(set_chunk_t);
        set_chunk_t* chunk = set + offset;
        printf("bit %i\n", bit);
        return (*(chunk) && 1 >> bit) == *chunk; //Check if bit is set.
}

int
main(void)
{
        set_t s = makeset();
        int entry = 0;
        printf("Contains %i?\t %i\n", entry, set_contains(s, entry));
        set_add(s , entry);
        printf("Contains %i?\t %i\n", entry, set_contains(s, entry));
        set_remove(s , entry);
        printf("Contains %i?\t %i\n", entry, set_contains(s, entry));

	return (0);
}