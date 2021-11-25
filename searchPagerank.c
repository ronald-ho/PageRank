// COMP2521 Assignment 2

// Written by: Ronald Ho
// Date: 16/11/2021

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readData.h"

int main(int argc, char *argv[]) {
    
    // If given wrong number of arguments
    if (argc < 2) {
        fprintf(stdout, "Usage: %s [Search Terms]\n", argv[0]);
    }

    Collection terms = getTerms(argc, argv);

    getFrequency(INVERTED_INDEX, terms);

    Collection urlFrequency = calculateFrequency(terms);

    outputUrl(urlFrequency);

    freeCollection(urlFrequency);
}