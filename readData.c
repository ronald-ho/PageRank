#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "readData.h"
#include "graph.h"

// Create and initalise pointers for collection
Collection newCollection() {
    Collection collection = malloc(sizeof(*collection));
    collection->urlHead = NULL;
    collection->termHead = NULL;
    collection->textHead = NULL;

    collection->urlCount = 0;
    collection->termCount = 0;
    collection->textCount = 0;

    return collection;
}

// Create and initalise pointers for url
Url newUrl() {
    Url url = malloc(sizeof(*url));
    // Initalise urlName
    strcpy(url->urlName, "");
    // Initalise urlFileName
    strcpy(url->urlFileName, "");
    //Initalise next pointer
    url->next = NULL;
    //Initalise index
    url->index = 0;
    // Initalise weight
    url->weight = 0;

    return url;
}

// Create and initalise pointers for newTerm
Term newTerm() {
    Term term = malloc(sizeof(*term));
    // Initalise termName
    strcpy(term->termName, "");
    // Initalise next pointer for term
    term->next = NULL;
    // Initalise head pointer for url
    term->urlHead = NULL;
    // Initalise urlCount
    term->urlCount = 0;

    return term;
}

// Create and initalise pointers for newtext
Text newText() {
    Text text = malloc(sizeof(*text));
    // Initalise next pointer 
    text->next = NULL;
    // Initalise head pointer for url
    text->urlHead = NULL;
    // Initalise urlCount
    text->urlCount = 0;

    return text;
}

// Given a filename, create and return a collection of urls
Collection getCollection(char *fileName) {
    
    // Creating a new collection
    Collection collection = newCollection();
    // Getting the head
    Url currUrl = collection->urlHead;

    // Open the collection file and store the urls
    FILE *urlFile = fopen(fileName, "r");

    int index = 0;

    char url[MAX_URL];

    while (fscanf(urlFile, "%s ", url) != EOF) {

        Url urlNew = newUrl();

        // save the url name into urlName
        strcpy(urlNew->urlName, url);
        // strcat the ".txt" into the urlName
        strcpy(urlNew->urlFileName, url);
        strcat(urlNew->urlFileName, TXT);
        // set the url's index
        urlNew->index = index;
        // Insert the node
        currUrl = insertUrlCollection(currUrl, urlNew, collection);

        // move the loop forward
        index++;
        collection->urlCount++;
    }

    fclose(urlFile);
    return collection;
}

// Given the url filename, return a collection of urls 
Collection getUrlList(char *fileName) {

    // Create a new collection
    Collection collection = newCollection();
    // Getting the head 
    Url currUrl = collection->urlHead;

    // Open the collection file and store the urls
    FILE *urlFile = fopen(fileName, "r");

    char temp[MAX_URL];
    
    while (fscanf(urlFile, "%s", temp) != EOF) {
        if (strcmp(temp, END_TAG) == 0) {
            break;
        } else if (tagCheck1(temp)) {
            // Create a new url
            Url urlNew = newUrl();
            // Save the urlname into urlName
            strcpy(urlNew->urlName, temp);
            // Insert the node
            currUrl = insertUrlCollection(currUrl, urlNew, collection);
            // Increase the urlcount by 1
            collection->urlCount++;
        }
    }

    fclose(urlFile);
    return collection;
}

// Function to get terms from argument
Collection getTerms(int argc, char *argv[]) {
    // Create a new collection of terms
    Collection collection = newCollection();
    // Getting the head
    Term currTerm = collection->termHead;

    for (int i = 1; i < argc; i++) {
        // Create a new term
        Term termNew = newTerm();
        // Copy the terms from command line to the node
        strcpy(termNew->termName, argv[i]);
        // Insert the node
        currTerm = insertTermCollection(currTerm, termNew, collection);

        collection->termCount++;
    }

    return collection;
}

// Given a url collection, create a graph
Graph getGraph(Collection urlList) {

    // Get the number of urls in urlList
    int urlCount = urlList->urlCount;

    Graph graph = newGraph(urlCount);

    // For each one of the urls, get the url.txt file name
    // Open each file, and get each of the outLinks of the read URL
    for (Url curr = urlList->urlHead; curr != NULL; curr = curr->next) {

        Collection outLinkList = getUrlList(curr->urlFileName);

        // Goes through each of the outlink URLs and insert into the correct edge
        // with the weight of 1
        // Checks if there is self-outlink and inlink and does ignores it
        for (Url currUrl = outLinkList->urlHead; currUrl != NULL; currUrl = currUrl->next) {

            // If there is node url that exists
            // And the url does not self link
            if (outlinkExists(currUrl->urlName, urlList) && 
                strcmp(curr->urlName, currUrl->urlName) != 0) {
                
                // Find the index of the url in file in urlList
                int position = outlinkPos(currUrl->urlName, urlList);

                // Insert the edge into the graph
                insertEdge(graph, curr->index, position, 1);
            }
        }
        freeCollection(outLinkList); 
    }

    return graph;
}

// Given arguments from command line, create and return a collection of text files
Collection getText(int argc, char *argv[]) {
    // Create a new collection of text
    Collection collection = newCollection();
    // Getting the head
    Text currText = collection->textHead;

    for (int i = 1; i < argc; i++) {
        // Create a new text
        Text textNew = newText();
        // Copy the text from command line to the node
        textNew->textName = malloc(sizeof(char) * (strlen(argv[i]) + 1));
        strcpy(textNew->textName, argv[i]);
        // Insert the node
        currText = insertTextCollection(currText, textNew, collection);

        collection->textCount++;
    }

    return collection;
}

// Save the pagerank into the collection of urls
void savePR(Collection urlList, double currPageRank[]) {
    for (Url curr = urlList->urlHead; curr != NULL; curr = curr->next) {
        curr->weight = currPageRank[curr->index];
    }
}

// Given a collection of texts, open each text file and link the urls to the 
// specific texts
void getUrlFromText(Collection texts) {
    // For each text file given in command open the file and save the contents 
    // into the texts collection
    for (Text currText = texts->textHead; currText != NULL; currText = currText->next) {

        // Open the file
        FILE *urlFile = fopen(currText->textName, "r");

        char tempUrl[MAX_URL];
        int index = 1;

        Url currUrl = currText->urlHead;

        while (fscanf(urlFile, "%s", tempUrl) != EOF) {
            // Create a new url node to save it
            Url urlNew = newUrl();
            // Copy the url name into the url node
            strcpy(urlNew->urlName, tempUrl);
            // Set the index of the url
            urlNew->index = index;
            // Insert the url
            if (currUrl == NULL) {
                currText->urlHead = urlNew;
            } else {
                currUrl->next = urlNew;
            }
            currUrl = urlNew;
            // Increase the number of urls in the specific text file
            currText->urlCount++;
            index++;
        }
        fclose(urlFile);
    }
}

// Given a collection of texts, create a new collection and only stores unique urls
Collection getUniqueUrls(Collection texts) {
    // Create a new Collection to store the unique urls
    Collection uniqueUrls = newCollection();

    Url uniqueCurrUrl = uniqueUrls->urlHead;

    int index = 1;

    for (Text currText = texts->textHead; currText != NULL; currText = currText->next) {
        for (Url currUrl = currText->urlHead; currUrl != NULL; currUrl = currUrl->next) {
            if (!urlExists(currUrl->urlName, uniqueUrls)) {
                Url urlNew = newUrl();
                // Copy the url name into the new collection
                strcpy(urlNew->urlName, currUrl->urlName);
                // Insert the node
                uniqueCurrUrl = insertUrlCollection(uniqueCurrUrl, urlNew, uniqueUrls);

                uniqueUrls->urlCount++;
                urlNew->index = index;
                index++;
            }
        }
    }
    return uniqueUrls;
}



// From given terms in argument
// Go through invertedIndex.txt to find the url frequencies
void getFrequency(char *fileName, Collection collection) {

    for (Term currTerm = collection->termHead; currTerm != NULL; currTerm = currTerm->next) {

        char temp[MAX_LINE];

        // Open the collection file, find and store the frequencies
        FILE *urlFile = fopen(fileName, "r");

        while (fscanf(urlFile, "%s", temp) != EOF) {
            if (strcmp(temp, currTerm->termName) == 0) {
                fgets(temp, MAX_LINE, urlFile);
                getUrl(currTerm, temp);
            }
        }

        fclose(urlFile);
    }
}

// Function to tokenize line and add urls into the terms struct
void getUrl(Term term, char *temp) {

    Url currUrl = term->urlHead;

    char *token = strtok(temp, " ");

    while (token != NULL) {
        // Skip the end character
        if (strcmp(token, "\n") != 0) {
            Url urlNew = newUrl();
            // Save the url name into urlName
            strcpy(urlNew->urlName, token);
            // Insert the node
            if (currUrl == NULL) {
                term->urlHead = urlNew;
            } else {
                currUrl->next = urlNew;
            }
            currUrl = urlNew;
            // Increase the count of urls stored in collection
            term->urlCount++;
        }
        token = strtok(NULL, " ");
    }
}

// Calculate the frequency from terms from argument
// Returns a collection of url with index of highest frequency
Collection calculateFrequency(Collection terms) {
    // Create a new collection
    Collection urlFrequncy = newCollection();

    Url freqencyHead = urlFrequncy->urlHead;

    // For each term 
    for (Term currTerm = terms->termHead; currTerm != NULL; currTerm = currTerm->next) {

        for (Url currUrl = currTerm->urlHead; currUrl != NULL; currUrl = currUrl->next) {

            if (urlExists(currUrl->urlName, urlFrequncy)) {
                // Increases the weight of the url by 1
                urlSearchIncrease(currUrl->urlName, urlFrequncy);
            
            } else {
                // Create a new url
                Url urlNew = newUrl();
                // Save the urlname into the new urlName
                strcpy(urlNew->urlName, currUrl->urlName);
                urlNew->weight++;
                // Insert the node
                freqencyHead = insertUrlCollection(freqencyHead, urlNew, urlFrequncy);
                // Increase the urlCount of collection
                urlFrequncy->urlCount++;
                // Add the PR to its frequency to get final ranking
                double PR = getPR(currUrl->urlName, PAGERANK_LIST);
                urlNew->weight += PR;
            }
        }
    }
    // Free the terms collection
    freeTerms(terms);

    return urlFrequncy;
}

// Function to get the PR of urls
double getPR(char *url, char *fileName) {

    // Open file with fileName
    FILE *PRFile = fopen(fileName, "r");

    char tempUrl[MAX_URL];
    char tempOutLink[MAX_URL];
    double pageRank = 0;

    while (fscanf(PRFile, "%s %s %lf\n", tempUrl, tempOutLink, &pageRank) != EOF) {
        char *token = strtok(tempUrl, ",");

        if (strcmp(token, url) == 0) {
            return pageRank;
        }
    }

    fclose(PRFile);
    return 0;
}

// Function to convert a Collection into an array
// void *collectionToArray(Collection uniqueUrls) {
//     // Initalising an int array
//     int unionArray = malloc(sizeof(int) * (uniqueUrls->urlCount + 1));

//     for (Url curr = uniqueUrls->urlHead; curr != NULL; curr = curr->next) {
//         unionArray[curr->index - 1] = curr->index;
//         unionArray[curr->index] = NULL;
//     }
//     return unionArray;
// }

// Function to output urls to stdout
void outputUrl(Collection urlFreq) {

    int maxOutput = 30;
    int out = 0;

    for (Url curr = urlFreq->urlHead; curr != NULL && out < maxOutput; curr = curr->next) {

        int index = 0;
        double max = 0;
        // A temp url to save the previous max
        char tempMaxUrl[MAX_URL];
        strcpy(tempMaxUrl, "");
        
        Url count = urlFreq->urlHead;

        for (int i = 0; i < urlFreq->urlCount; i++) {
            if (count->weight > max) {
                strcpy(tempMaxUrl, count->urlName);
                max = count->weight;
                index = i;
            } else if (count->weight == max) {
                if (strcmp(count->urlName, tempMaxUrl) < 0) {
                    strcpy(tempMaxUrl, count->urlName);
                    max = count->weight;
                    index = i;
                }
            }
            count = count->next;
        }

        Url urlToPrint = getUrlName(urlFreq, index);

        printf("%s\n", urlToPrint->urlName);

        urlToPrint->weight = -1;
        out++;
    }
}


/// HELPER FUNCTIONS !!!!!!!!!!!


// Function to find if the outlink exists in a Collection
bool outlinkExists(char *url, Collection urlList) {

    for (Url currUrl = urlList->urlHead; currUrl != NULL; currUrl = currUrl->next) {
        if (strcmp(currUrl->urlName, url) == 0) {
            return true;
        }
    }
    return false;
}

// Function to find the outlink position in a collection
int outlinkPos(char *url, Collection urlList) {

    for (Url currUrl = urlList->urlHead; currUrl != NULL; currUrl = currUrl->next) {
        if (strcmp(currUrl->urlName, url) == 0) {
            return currUrl->index;
        }
    }
    return 0;
}

// Gets the index of the url in a collection
Url getUrlIndex(Collection urlList, int index) {
    for (Url curr = urlList->urlHead; curr != NULL; curr = curr->next) {
        if (curr->index == index) {
            return curr;
        }
    }
    return NULL;
}

// Gets the url name from a collection
Url getUrlName(Collection urlFreq, int index) {

    Url curr = urlFreq->urlHead;
    int i = 0;

    while (i < index) {
        curr = curr->next;
        i++;
    }
    return curr;
}

// Checks for the tag in section 1
bool tagCheck1(char *word) {
    if (strcmp(word, START_TAG) != 0 && 
        strcmp(word, SECTION_1) != 0 &&
        strcmp(word, END_TAG) != 0) {

        return true;
    }
    return false;
}

// Checks if a url exists in a collection with given url name
bool urlExists(char *token, Collection collection) {
    for (Url curr = collection->urlHead; curr != NULL; curr = curr->next) {
        if (strcmp(curr->urlName, token) == 0) {
            return true;
        }
    }
    return false;
}

// Search for the url in a collection with given url name, and add 1 to weight
void urlSearchIncrease(char *token, Collection collection) {
    for (Url curr = collection->urlHead; curr != NULL; curr = curr->next) {
        if (strcmp(curr->urlName, token) == 0) {
            curr->weight += 1;
        }
    }
}

// Count and return the total number of urls across terms in a collection of terms
int totalUrlCount(Collection terms) {
    int totalUrl = 0;

    for (Term currTerm = terms->termHead; currTerm != NULL; currTerm = currTerm->next) {
        totalUrl += currTerm->urlCount;
    }

    return totalUrl;
}

// Insert Url in a Collection
Url insertUrlCollection(Url currUrl, Url urlNew, Collection collection) {
    if (currUrl == NULL) {
        collection->urlHead = urlNew;
    } else {
        currUrl->next = urlNew;
    }
    return urlNew;
}

// Insert Term in a Collection
Term insertTermCollection(Term currTerm, Term termNew, Collection collection) {
    if (currTerm == NULL) {
        collection->termHead = termNew;
    } else {
        currTerm->next = termNew;
    }
    return termNew;
}

// Insert Text in a Collection
Text insertTextCollection(Text currText, Text textNew, Collection collection) {
    if (currText == NULL) {
        collection->textHead = textNew;
    } else {
        currText->next = textNew;
    }
    return textNew;
}

// Create and return an int 

// // Swapping the pointers of x and y
// void swap(int *x, int *y) {
//     int temp;
//     temp = *x;
//     *x = *y;
//     *y = temp;
// }

// // Recursive function to show all permutations of a set
// // Implemented with inspiration from 
// // https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
// void recursivePermute(int set[], int startIndex, int endIndex) {

//     // base case
//     if (startIndex == endIndex) {

//     } else {
//         for (int i = startIndex; i <= endIndex; i++) {
//             swap(set[i], set[])
//         }
//     }
// }



/// HELPER FUNCTIONS TO FREE STRUCTS!!!

// Free an collection with only singlely linked list
void freeCollection(Collection urlList) {
    Url curr = urlList->urlHead;

    while (curr != NULL) {
        Url temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(urlList);
}

// Free a doublely linked collection with terms and urls
void freeTerms(Collection terms) {
    Term curr = terms->termHead;

    while (curr != NULL) {
        Term temp = curr;
        curr = curr->next;
        freeUrls(temp);
    }
    free(terms);
}

// Free urls for the freeTerm function
void freeUrls(Term term) {
    Url curr = term->urlHead;

    while (curr != NULL) {
        Url temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(term);
}

