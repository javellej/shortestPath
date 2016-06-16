#include <Tools.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

int Tools::hammingDistance( const string word1, const string word2) {
    int wordLength = word1.length();
    if ( wordLength != word2.length() ) {
        return -1;
    }
    int hammingDist = 0;
    for ( int i=0; i<wordLength; i++ ) {
        if ( word1[i] != word2[i] ) {
            hammingDist++;
        }
    }
    return hammingDist;
}

bool Tools::isConnected( const string word1, const string word2) {
    int wordLength = word1.length();
    if ( wordLength != word2.length() ) {
        return false;
    }
    int hammingDist = 0;
    for ( int i=0; i<wordLength; i++ ) {
        if ( word1[i] != word2[i] ) {
            hammingDist++;
        }
        if ( hammingDist > 1 ) { // no need to compute the actual value of the Hamming distance
            return false;
        }
    }
    return hammingDist == 1;
}

vector<string> Tools::readWordsFromFile( const string& fileName) {
    ifstream inputFile( fileName.c_str(), ifstream::in);
    if ( inputFile.good() == 0 ) {
        cout << "Error - Bad file name" << endl;
        throw 7;
    }
    vector<string> wordVector;
    string word;
    while ( inputFile >> word ) {
        wordVector.push_back( word);
    }
    return wordVector;
}
