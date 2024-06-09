/*
File: CustomDatasetReader.h
*/

#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "CustomLinkedList.h"

// Returns true if a character represents a number (0-9) and false otherwise
bool isDigit(const char& c) {
    return (c >= '0' && c <= '9');
}

// Converts a line of a .txt file into an integer and returns that integer
// Works with lines of format 'x y' such as '2039 9283' and '3203 181728'
// Converts one segment at a time, must be passed the starting index of the first segment (0)
// Automatically stores the starting index of the second segment in the same variable after the first call
ll parseLine(const std::string& line, ll& index) {
    std::string numberStr = "";
    ll length = line.length();
    while (index < length && isDigit(line[index])) numberStr += line[index++];
    if (index != length) {
        while (index < length && !isDigit(line[index])) index++;
    }
    return std::stoll(numberStr);
}

// Removes duplicate elements in a linked list
void removeDuplicates(LinkedList<ll>& list) {
    if (list.getLength() == 0) return;
    std::set<ll> uniqueElements;
    LinkedList<ll> tempList;
    Iterator<ll> itr(list.getHead());
    while (itr.hasNext()) {
        if (uniqueElements.find(itr.getValue()) == uniqueElements.end()) {
            uniqueElements.insert(itr.getValue());
            tempList.addFront(itr.getValue());
        }
        itr.advance();
    }
    if (uniqueElements.find(itr.getValue()) == uniqueElements.end()) {
        uniqueElements.insert(itr.getValue());
        tempList.addFront(itr.getValue());
    }
    itr = tempList.getHead();
    list = LinkedList<ll>();
    while (tempList.getLength()) {
        list.addFront(tempList.removeFront());
    }
}

// Removes junk edges from the graph (duplicates and self-loops)
void cleanGraph(LinkedList<ll>*& adjList, const ll& vertices) {
    for (ll i = 0; i < vertices; i++) {
        adjList[i].removeAll(i);
        removeDuplicates(adjList[i]);
    }
}

// Reads a .txt dataset file and extracts the graph and the number of vertices from it
// Junk edges such as duplicate edges and self-loops will be discarded
// Returns true if file is read successfully and false otherwise
// Vertices' numbering will differ in the result graph to avoid cases of unordered vertices
bool loadGraph(const std::string& filePath, LinkedList<ll>*& adjList, ll& numVertices) {
    std::ifstream datasetFile;
    std::map<ll, ll> vertexIndexer; // Maps original vertex numbers to new indices
    ll currentIndex = 1; // Start indexing from 1 to allow 0 to be the default 'unindexed' state
    LinkedList<std::pair<ll, ll>> edgeTracker; // Tracks the edges of the graph while reading and indexing vertices

    datasetFile.open(filePath, std::ios::in);
    if (datasetFile.is_open()) {
        std::string line;
        // Skip the first 4 lines
        for (ll i = 0; i < 4; i++) std::getline(datasetFile, line);

        while (std::getline(datasetFile, line)) {
            ll index = 0;
            ll fromVertex = parseLine(line, index);
            ll toVertex = parseLine(line, index);
            if (vertexIndexer.find(fromVertex) == vertexIndexer.end()) vertexIndexer[fromVertex] = currentIndex++;
            if (vertexIndexer.find(toVertex) == vertexIndexer.end()) vertexIndexer[toVertex] = currentIndex++;
            edgeTracker.addFront({ vertexIndexer[fromVertex] - 1, vertexIndexer[toVertex] - 1 }); // -1 to let numbering start from 0
        }
        datasetFile.close();
    } else {
        return false;
    }

    numVertices = currentIndex - 1;
    adjList = new LinkedList<ll>[numVertices];
    while (edgeTracker.getLength()) {
        std::pair<ll, ll> edge = edgeTracker.removeFront();
        adjList[edge.first].addFront(edge.second);
    }
    cleanGraph(adjList, numVertices);
    return true;
}
