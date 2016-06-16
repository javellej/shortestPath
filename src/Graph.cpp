#include <Graph.hpp>
#include <Tools.hpp>
#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

Node::Node( std::string _name) {
    this->name = _name;
}

void Node::printNode( ) {
    cout << this->name + " : ";
    for ( set<int>::iterator it = this->neighbors.begin(); it != this->neighbors.end(); ++it ) {
        cout << *it << " ";
    }
    cout << endl;
}

Graph::Graph() {
    this->isSorted = false;
}

Graph::Graph( bool isSorted) {
    this->isSorted = isSorted;
}

void Graph::printGraph( ) {
    for ( vector<Node>::iterator it = globalGraph.begin(); it != globalGraph.end(); ++it ) {
        it->printNode();
    }
}

void Graph::addWord( string word) {
    // create new node associated with input word
    Node wordNode( word);
    int wordNodePosition = globalGraph.size();
    int currPosition = 0;
    // connect node to its neighbors
    for ( vector<Node>::iterator it = globalGraph.begin(); it != globalGraph.end(); ++it ) {
        if ( it->name == word ) {
            cout << "Error - Duplicate word : \"" << word << "\"" << endl;
            throw 6;
        }
        if ( Tools::isConnected( it->name, wordNode.name) ) {
            wordNode.neighbors.insert( currPosition);
            it->neighbors.insert( wordNodePosition);
        }
        currPosition++;
    }
    // add node to global graph
    globalGraph.push_back( wordNode);
}

void Graph::addWordsFromFile( string fileName) {
    vector<string> wordVector = Tools::readWordsFromFile( fileName);
    for ( vector<string>::iterator it = wordVector.begin(); it != wordVector.end(); ++it ) {
        this->addWord( *it);
    }
}

vector<string> Graph::shortestPath( std::string start, std::string end) {
    if ( start.size() != end.size() ) {
        cout << "Error - Word lengths do not match" << endl;
        throw 1;
    }
    int graphOrder = globalGraph.size();
    if ( graphOrder == 0 ) {
        cout << "Error - Empty graph" << endl;
        throw 5;
    }
    // Step 1 : find index of starting word
    int startIndex = 0;
    int endIndex;
    bool isStartFound = false;
    if ( this->isSorted ) { // dichotomic search (sorted dict)
        int lowerIndex = 0;
        int upperIndex = graphOrder;
        while ( lowerIndex != upperIndex - 1 ) {
            int midIndex = ( upperIndex + lowerIndex ) >> 1;
            if ( globalGraph[midIndex].name > start ) {
                upperIndex = midIndex;
            } else {
                lowerIndex = midIndex;
            }
        }
        if ( globalGraph[lowerIndex].name == start ) {
            isStartFound = true;
            startIndex = lowerIndex;
        }
    } else { // linear search (non-sorted dict)
        for ( vector<Node>::iterator it = globalGraph.begin(); it != globalGraph.end(); ++it ) {
            if ( it->name == start ) {
                isStartFound = true;
                break;
            }
            startIndex++;
        }
    }
    // throw exception if start word not found
    if ( !isStartFound ) {
        cout << "Error - Word \"" << start << "\" not found in dictionnary" << endl;
        throw 2;
    }
    // Step 2 : breadth-first search on global graph
    set<int> visitedNodes;
    set<int> currentNodes;
    set<int> nextNodes;
    set<int> hasPrev;
    int wordDistance = 0;
    bool isEndFound = false;
    currentNodes.insert( startIndex);
    while ( currentNodes.size() > 0 ) {
        for ( set<int>::iterator it_curr = currentNodes.begin(); it_curr != currentNodes.end(); ++it_curr ) {
            Node currNode = globalGraph[*it_curr];
            if ( currNode.name == end ) {
                isEndFound = true;
                endIndex = *it_curr;
                break;
            }
            // add neighbors of current node to "next nodes" set if not visited already
            for ( set<int>::iterator it_next = currNode.neighbors.begin() ; it_next != currNode.neighbors.end() ; ++it_next ) {
                if ( visitedNodes.find( *it_next) == visitedNodes.end() ) {
                    if ( hasPrev.find( *it_next) == hasPrev.end() ) {
                        globalGraph[*it_next].prevNode = *it_curr;
                        hasPrev.insert( *it_next);
                    }
                    nextNodes.insert( *it_next);
                }
            }
            // mark current node as visited
            visitedNodes.insert( *it_curr);
        }
        if ( isEndFound ) { break; }
        // next nodes become current nodes
        currentNodes = nextNodes;
        nextNodes.clear();
        wordDistance++;
    }
    if ( isEndFound ) {
        // build chain of words by following predecessors backwards
        vector<string> wordChain;
        int currIndex = endIndex;
        while ( currIndex != startIndex ) {
            wordChain.push_back( globalGraph[currIndex].name);
            currIndex = globalGraph[currIndex].prevNode;
        }
        wordChain.push_back( globalGraph[startIndex].name);
        return wordChain;
    } else {
        cout << "Error - Word \"" + end + "\" not reachable from \"" << start << "\"" << endl;
        throw 3;
    }
}

void Graph::clear( ) {
    globalGraph.clear();
    isSorted = false;
}

void Graph::saveToFile( string dstFileName) {
    ofstream outFile( dstFileName.c_str(), ofstream::binary);
    outFile.write( "digraph {\n", 10);
    for ( int i=0; i<globalGraph.size(); i++ ) {
        ostringstream nodeStream;
        nodeStream << i;
        string nodeStr = nodeStream.str() + " [label=\" " + globalGraph[i].name + " \"];\n";
        outFile.write( nodeStr.c_str(), nodeStr.size());
        for ( set<int>::iterator it_neighbor = globalGraph[i].neighbors.begin(); it_neighbor != globalGraph[i].neighbors.end(); ++it_neighbor ) {
            ostringstream neighborStream;
            neighborStream << *it_neighbor;
            string edgeStr = nodeStream.str() + " -> " + neighborStream.str() + "\n";
            outFile.write( edgeStr.c_str(), edgeStr.size());
        }
    }
    outFile.write( "}\n", 2);
    outFile.close();
}

void Graph::loadFromFile( string srcFileName) {
    // clear graph contents
    this->clear();
    bool isBadFormat = false;
    vector<string> fileContents = Tools::readWordsFromFile( srcFileName);
    int fileContentsSize = fileContents.size();
    int index = 2;
    while ( index < fileContentsSize - 1 ) {
        if ( fileContents[index+1] == "[label=\"" ) { // read new node entry
            if ( fileContents.size() - index < 4 ) {
                isBadFormat = true;
                break;
            }
            globalGraph.push_back( Node( fileContents[index+2]));
            index += 4;
        } else { // read edge
            if ( fileContents.size() - index < 3 ) {
                isBadFormat = true;
                break;
            }
            int srcNodeIndex = atoi( fileContents[index].c_str());
            int dstNodeIndex = atoi( fileContents[index+2].c_str());
            if ( srcNodeIndex >= globalGraph.size() ) {
                isBadFormat = true;
                break;
            }
            globalGraph[srcNodeIndex].neighbors.insert( dstNodeIndex);
            index += 3;
        }
    }
    if ( isBadFormat ) {
        cout << "Error - Bad file format" << endl;
        throw 8;
    }
}
