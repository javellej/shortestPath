#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <vector>
#include <set>
#include <string>

using namespace std;

class Node {
friend class Graph;
public:
    Node( string _name);
    void printNode();
protected:
    string name;
    set<int> neighbors;
    int prevNode; // used for shortest path recovery
};

class Graph {
public:
    Graph();
    Graph( bool isSorted);
    void printGraph( );
    void addWord( string word);
    void addWordsFromFile( string fileName);
    vector<string> shortestPath( string start, string end);
    void clear( );
    void saveToFile( string dstFileName);
    void loadFromFile( string srcFileName);
private:
    bool isSorted;
    vector<Node> globalGraph;
};

#endif
