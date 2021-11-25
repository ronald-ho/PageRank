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
#include "graph.h"

void calculatePageRank(Collection urlList, Graph graph, double dampF, double diffPR, int maxIt);
void outputText(Graph graph, Collection urlList);

int main(int argc, char *argv[]) {

    // If given wrong number of arguments
    if (argc != 4) {
        fprintf(stdout, "Usage: %s [damping factor] [diffPR] [maxIterations]\n", argv[0]);
    }

    // Get the damping factor diffpR and maxIterations from argv
    double dampF = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIt = atoi(argv[3]);
    
    // Create a set of urls to 
    Collection urlList = getCollection(COLLECTION_FILE);

    // Create a graph using the stored urls
    Graph graph = getGraph(urlList);

    // Calculate the Pagerank of each URL for maxIterations
    calculatePageRank(urlList, graph, dampF, diffPR, maxIt);

    // Output the list of URLs, one per line, to a file named pagerankList.txt
    // URL, outdegree, Weighted PageRank
    // Sorted in descending order
    // URLs with the same Weighted PageRank is ordered alphabetically and ascending by the URL
    outputText(graph, urlList);

    freeCollection(urlList);

    freeGraph(graph);
}

void calculatePageRank(Collection urlList, Graph graph, double dampF, double diffPR, int maxIt) {

    // N = number of URLs in the collection
    int N = getnV(graph);
    double floatN = (double)N;
    // Create an array for the pageRank, and another array for the prev pageRank
    double currPageRank[N];
    double prevPageRank[N];

    // For each URL pi in the collection PR(pi, 0) = 1/N
    // Set both arrays to 1/N each
    for (int i = 0; i < N; i++) {
        currPageRank[i] = 1/floatN;
        prevPageRank[i] = 1/floatN;
    }

    // diff = diffPR
    double diff = diffPR;

    for (int iteration = 0; iteration < maxIt && diff >= diffPR; iteration++) {

        double diffSum = 0;

        // For each url in the graph, loop through to find its respective inLinks
        for (int url = 0; url < N; url++) {

            double weightedPageRank = 0;
            // Loop to find all URLs that can have an outLink to url
            for (int j = 0; j < N; j++) {
                // If there is a link from url to the specific url
                if (adjacent(graph, j, url)) {
                    // wIn = total inLinks of vertex / sum of inLinks from reference Vertexs
                    // wOut = total outLinks of vertex / sum of outLinks from reference Vertexs
                    double wIn = (wInCount(graph, url) / sumInLinks(graph, j));
                    double wOut = (wOutCount(graph, url) / sumOutLinks(graph, j));
                    // PR(v) * wIn * wOut
                    // On the first run, every pageRank is 1
                    // prevPageRank[j] * wIn * wOut
                    weightedPageRank += (prevPageRank[j] * wIn * wOut);
                }
            }
            // Saves the final true pageRank of the url into pageRank array
            // PR(pi, t + 1) = (1 - dampF)/floatN + dampF * weightedPageRank
            currPageRank[url] = ((1 - dampF) / floatN) + (dampF * weightedPageRank);
        }

        for (int i = 0; i < N; i++) {
            // Save the pageRank into the prevPageRank array and update diff
            diffSum += fabs(currPageRank[i] - prevPageRank[i]);
            prevPageRank[i] = currPageRank[i];
        }

        diff = diffSum;
    }

    // Save the final PR into the urlList
    savePR(urlList, currPageRank);
}

void outputText(Graph graph, Collection urlList) {

    // Open the pagerankList file for writing
    FILE *URL_output = fopen("pagerankList.txt", "w");

    for (Url curr = urlList->urlHead; curr != NULL; curr = curr->next) {

        int index = 0;
        double max = 0;
        // A temp url to save the previous max
        char tempMaxUrl[MAX_URL];
        strcpy(tempMaxUrl, "");

        for (Url count = urlList->urlHead; count != NULL; count = count->next) {
            if (count->weight > max) {
                strcpy(tempMaxUrl, count->urlName);
                max = count->weight;
                index = count->index;
                
            } else if (count->weight == max) {
                if (strcmp(count->urlName, tempMaxUrl) < 0) {
                    strcpy(tempMaxUrl, count->urlName);
                    max = count->weight;
                    index = count->index;
                }
            }
        }

        Url urlToPrint = getUrlIndex(urlList, index);
        int outLink = (int)wOutCount(graph, index);

        fprintf(URL_output, "%s, %d, %.7f\n", urlToPrint->urlName, outLink, max);

        urlToPrint->weight = -1;
    }

    // Close the file
    fclose(URL_output); 
} 