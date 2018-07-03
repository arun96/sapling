#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include "sapling_api.h"

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " <genome file> " << endl;
        return 0;
    }

    Sapling sap(argv[1]);
    
    cout << "Testing Sapling" << endl;
    int numQueries = 5000000;
    vector<string> queries(numQueries, "");
    vector<long long> kmers = vector<long long>(numQueries, 0);
    for(int i = 0; i<numQueries; i++)
    {
    	size_t idx = rand() % (sap.n - sap.k);
    	queries[i] = sap.reference.substr(idx, sap.k);
    	kmers[i] = sap.kmerize(queries[i]);
    }
    
    FILE *outfile = fopen ("queries.out", "w");
    for(int i = 0; i < numQueries; i++)
    {
        fprintf(outfile, "@read%d\n", i+1);
        fprintf(outfile, "%s\n", queries[i].c_str());
        fprintf(outfile, "+\n");
        
        for(int j = 0; j<sap.k; j++) fprintf(outfile, "9");
        fprintf(outfile, "\n");
    }
    
    cout << "Constructed queries" << endl;
    // Run piece-wise linear test
    vector<size_t> plAnswers(numQueries, 0);
    for(int i = 0; i<numQueries; i++)
    {
        plAnswers[i] = sap.plQuery(queries[i], kmers[i], queries[i].length());
    }
    
    // Check the answers
    int countCorrect = 0;
    for(int i = 0; i<numQueries; i++)
    {
        if(plAnswers[i] + sap.k <= sap.n && queries[i] == sap.reference.substr(plAnswers[i], sap.k))
        {
            countCorrect++;
        }
    }
    cout <<"Piecewise linear correctness: " << countCorrect << " out of " << numQueries << endl;
}
