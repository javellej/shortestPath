#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <string>
#include <vector>

using namespace std;

class Tools{
public:
    static int hammingDistance( const string word1, const string word2);
    static bool isConnected( const string word1, const string word2);
    static vector<string> readWordsFromFile( const string& fileName);
};

#endif // _TOOLS_HPP_
