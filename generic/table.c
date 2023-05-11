// Matthew Tran
// Project 3 Week 2
// Utilizing Hash Tables, but generic

//Included libraries 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "set.h"

// Setting up structure set. Calling it SET
typedef struct set{
    int length;
    int count;
    void **data;
    char *flags;
    int (*compare)();
    unsigned(*hash)();
}SET;


// Search function
// Starts by finding the intended location of the hash key
// Checks the flags and iterates through the array to probe and 
// see if there is somewhere to isert. The first 'd' location
// is flagged.
// Avg: O(1) Worst: O(n)
int search(SET *sp,void *elt,bool *found )
{
    

    bool firstDelFlag = false;
    int deleteFlag = -1;

    int i = 0;
    int locn = (*sp->hash)(elt) % sp->length;

    assert(sp!= NULL);
    assert(sp->flags != NULL);
    assert(sp->data != NULL);
    *found = false;
    while(i< sp->length)
    {
        if(sp->flags[locn]=='f')
        {
            if((*sp->compare)(sp->data[locn],elt)==0)
            {
                *found = true;
                return locn;
            }
        }
        if(sp->flags[locn]=='d')
        {
            if(firstDelFlag==false)
            {
                deleteFlag = locn;
                firstDelFlag = true;
            }
        }
        if(sp->flags[locn]=='e')
        {
            if(firstDelFlag == true)
            {
                return deleteFlag;
            }
            return locn;
        }
        i++;
        locn = (locn +1) % sp->length;
    }
    return deleteFlag;
}

// Creates the set and initializes the values
// Allocates memory for flags and data
// initializes the new functions that we are given
// O(n)
SET *createSet (int maxElts, int (*compare)(), unsigned(*hash)())
{
    SET *sp;
    sp = malloc(sizeof(SET));
    assert(sp!= NULL);
    sp->length = maxElts;
    sp->count = 0;
    sp->data = malloc(sizeof(void*)*maxElts);
    sp->flags = malloc(sizeof(char)*maxElts);
    assert(compare!= NULL);
    assert(hash!= NULL);
    sp->compare = compare;
    sp->hash = hash;
    assert(sp->data != NULL);
    assert(sp->flags != NULL);
    int i;
    for(i =0; i<maxElts;i++)
    {
        sp->flags[i]='e';
    }
    return sp;
}

// Function to destroy the set
// Frees strings that are flagged with 'f'
// Deallocates from the inside out
// O(1)
void destroySet(SET *sp)
{
    
    assert(sp!=NULL);
    assert(sp->data != NULL);
    assert(sp->flags != NULL);
    
    free(sp->flags);
    free(sp->data);
    free(sp);
}

// Returns the number of words in the data
// This is kept track of with count
// O(1)
int numElements(SET *sp)
{
    assert(sp != NULL);
    return sp->count;
}

// Add element function
// This is adding an element to the array where it should go
// The search function will do the probing for you
// It will only add if it was not found
// It also it increments count
// avg: O(1) worst O(n)
void addElement(SET *sp, void *elt) 
{
    bool found;
    assert(sp!=NULL);
    assert(elt!=NULL);

    int locn = search(sp,elt,&found);

    if (found == true)
    {
        return;
    }
    assert(sp->count < sp->length);
    assert(sp->data!=NULL);
    assert(sp->flags!=NULL);
    sp->data[locn] = elt;
    sp->flags[locn] = 'f';
    sp->count++;
}

// Remove element function
// This removes the specified element and decrements the count
// searches for the location of the element and if it is there and flag it as 'd'
// avg: O(1) worst: O(n)
void removeElement(SET *sp, void *elt)
{
    bool found;
    assert(sp!=NULL);
    assert(sp->data != NULL);
    assert(sp->flags != NULL);
    assert(sp->count > 0);
    int locn = search(sp,elt,&found);
    if(found)
    {
        sp->flags[locn] = 'd';
        sp->count--;
    }
}

// Finds the string and returns it
// Uses search function to get the location of the element 
// If it is found, then it will return the string, but return NULL if not
// avg: O(1) worst: O(n)
void *findElement(SET *sp, void *elt)
{
    assert(sp!=NULL);
    assert(sp->data != NULL);
    bool found;
    int locn = search(sp,elt,&found);

    
    if (found)
    {
        
        return sp->data[locn];
    }
    return NULL;
}

// Gets all of the strings, puts it into an array, and returns it
// You cannot just mem copy, so I had to check each 'f' flag and get that string
// returns the array of string pointers
// O(n)
void *getElements(SET *sp)
{
    assert(sp!=NULL);
    assert(sp->data != NULL);
    assert(sp->flags != NULL);
    void **elts = malloc(sizeof(void*)*sp->count);
    assert(elts != NULL);
    int i;
    int j = 0;
    for (i=0;i<sp->length;i++)
    {
        if(sp->flags[i]=='f')
        {
            elts[j]=sp->data[i];
            j++;
        }
        
    }
    return elts;
}
