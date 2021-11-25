#ifndef READDATA_H
#define READDATA_H

#include "graph.h"

#define TXT ".txt"
#define START_TAG "#start"
#define END_TAG "#end"
#define SECTION_1 "Section-1"
#define COLLECTION_FILE "collection.txt"
#define INVERTED_INDEX "invertedIndex.txt"
#define PAGERANK_LIST "pagerankList.txt"

#define POSITION 0
#define EXISTS 1
#define MAX_URL 105
#define MAX_LINE 1001

typedef struct Collection *Collection;
typedef struct Url *Url;
typedef struct Term *Term;
typedef struct Text *Text;

struct Collection {
    Url urlHead;
    Term termHead;
    Text textHead;

    int urlCount;
    int termCount;
    int textCount;
};

struct Url {
    char urlName[MAX_URL];
    char urlFileName[MAX_URL];

    Url next;

    double weight;
    int index;
};

struct Term {
    char termName[MAX_LINE];

    Url urlHead;
    Term next;

    int urlCount;
};

struct Text {
    char *textName;

    Url urlHead;
    Text next;

    int urlCount;
};


Collection newCollection();
Url newUrl();
Term newTerm();
Text newText();

Graph getGraph(Collection urlList);
Collection getCollection(char *fileName);
Collection getUrlList(char *filename);
Collection getTerms(int argc, char *argv[]);
Collection calculateFrequency(Collection terms);
Collection getText(int argc, char *argv[]);
Collection getUniqueUrls(Collection texts);
Url getUrlIndex(Collection urlList, int index);
Url getUrlName(Collection urlFreq, int index);

void getUrlFromText(Collection texts);
void getFrequency(char *fileName, Collection collection);
void getUrl(Term term, char *temp);
double getPR(char *url, char *fileName);

void savePR(Collection urlList, double currPageRank[]);
bool outlinkExists(char *url, Collection urlList);
int outlinkPos(char *url, Collection urlList);
bool tagCheck1(char *word);
bool urlExists(char *token, Collection terms);
void urlSearchIncrease(char *token, Collection collection);
void outputUrl(Collection urlFreq);

Url insertUrlCollection(Url currUrl, Url urlNew, Collection collection);
Term insertTermCollection(Term currTerm, Term termNew, Collection collection);
Text insertTextCollection(Text currText, Text textNew, Collection collection);

void freeCollection(Collection urlList);
void freeList(char **list);
void freeTerms(Collection terms);
void freeUrls(Term term);

#endif