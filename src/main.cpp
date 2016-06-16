#include <Graph.hpp>
#include <string>
#include <iostream>

using namespace std;

int usage( ) {
    cout << "Usage : ./shortestPath start end <path/to/dictionnary> [-sorted]" << endl;
    return 1;
}

int main( int argc, char *argv[]) {
    cout << "eee" << endl;
    return 0;
    if ( argc < 4 || argc > 5 ) {
        return usage();
    }
    bool isSorted = false;
    string start = argv[1];
    string end = argv[2];
    string dictFileName = argv[3];
    if ( argc == 5 ) {
        string sortedStr = argv[4];
        if ( sortedStr == "-sorted" ) {
            isSorted = true;
        } else {
            return usage();
        }
    }
    try {
        Graph g( isSorted);
        g.addWordsFromFile( dictFileName);
        g.saveToFile( "graph.dot");
        vector<string> path = g.shortestPath( start, end);
        cout << "Minimum distance from \"" << start << "\" to \"" << end << "\" is " << path.size() - 1 << endl;
        cout << "Example of a minimal sequence : " << endl;
        for ( int i=path.size()-1; i>=0; i-- ) {
            cout << path[i];
            if ( i > 0 ) { cout << " -> "; }
        }
        cout << endl;
    }
    catch ( int e )
    {
        cout << "Exception " << e << endl;
    }
    return 0;
}
