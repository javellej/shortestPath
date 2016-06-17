#include <Graph.hpp>
#include <Tools.hpp>
#include <string>
#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST( BadUsage, load_wrong_file) {
    Graph g;
    EXPECT_ANY_THROW( g.loadFromFile( "qlkjsdh"));
}

TEST( FailedQuery, non_existant_word) {
    Graph g;
    EXPECT_ANY_THROW( g.shortestPath( "foo", "bar"));
    g.addWord( "foo");
    EXPECT_ANY_THROW( g.shortestPath( "foo", "bar"));
    EXPECT_ANY_THROW( g.shortestPath( "bar", "foo"));
    EXPECT_ANY_THROW( g.addWord( "foo"));
}

TEST( FailedQuery, unconnected_graph) {
    Graph g;
    // first connex component
    g.addWord( "aa");
    g.addWord( "ab");
    g.addWord( "ba");
    g.addWord( "bb");
    // second connex component
    g.addWord( "cc");
    g.addWord( "cd");
    g.addWord( "dc");
    g.addWord( "dd");
    EXPECT_ANY_THROW( g.shortestPath( "ab", "cd"));
}

TEST( NormalUsage, edge_cases) {
    Graph g;
    g.addWordsFromFile( "data/dict1");
    EXPECT_EQ( 1, g.shortestPath( "a", "a").size());
    EXPECT_EQ( 2, g.shortestPath( "a", "b").size());
}

TEST( NormalUsage, task_example) {
    Graph g;
    g.addWord( "hit");
    g.addWord( "dot");
    g.addWord( "dog");
    g.addWord( "cog");
    g.addWord( "hot");
    g.addWord( "log");
    g.saveToFile( "test/exampleGraph.dot");
    EXPECT_EQ( 4, g.shortestPath( "hit", "cog").size() - 1);
}

TEST( NormalUsage, save_load) {
    Graph g;
    g.addWordsFromFile( "data/dict3");
    EXPECT_NO_THROW( g.saveToFile( "test/graph3.dot"));
    g.clear();
    EXPECT_NO_THROW( g.loadFromFile( "test/graph3.dot"));
    EXPECT_NO_THROW( g.saveToFile( "test/graph3_2.dot"));
    EXPECT_EQ( 5, g.shortestPath( "new", "run").size());
}

TEST( NormalUsage, randomQUery) {
    // initialize PRNG
    int seed = time( NULL);
    cout << "random seed : " << seed << endl;
    srand( seed);
    Graph g;
    // read words from dictionnary and pick random start and end
    vector<string> words = Tools::readWordsFromFile( "data/connectedDict");
    int dictSize = words.size();;
    // add words that form a connected graph (so that random queries never fail)
    g.addWordsFromFile( "data/connectedDict");
    for ( int i=0; i<100; i++ ) {
        int startIndex = rand( ) % dictSize;
        int endIndex = rand( ) % dictSize;
        vector<string> path1 = g.shortestPath( words[startIndex], words[endIndex]);
        int size1 = path1.size();
        vector<string> path2 = g.shortestPath( words[endIndex], words[startIndex]);
        int size2 = path2.size();
        // check that the paths are valid
        for ( int i=0; i<size1-1; i++ ) {
            EXPECT_EQ( 1, Tools::hammingDistance( path1[i], path1[i+1]));
        }
        for ( int i=0; i<size2-1; i++ ) {
            EXPECT_EQ( 1, Tools::hammingDistance( path2[i], path2[i+1]));
        }
        // check that distance between query words is the same in both directions
        EXPECT_EQ( size1, size2);
    }
}

TEST( NormalUsage, sorted_dictionary) {
    Graph g( true);
    g.addWordsFromFile( "data/dict4");
    EXPECT_EQ( 6, g.shortestPath( "blue", "dash").size() - 1);
}

int main( int argc, char **argv) {
    cout << "Begin Improbable tests" << endl;
    ::testing::InitGoogleTest( &argc, argv);
    return RUN_ALL_TESTS();
}
