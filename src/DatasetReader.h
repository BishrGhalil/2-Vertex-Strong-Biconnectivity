#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "LinkedList.h"

// Check if a character is a numeric digit (0-9)
bool isDigit(const char& c) {
    return (c >= '0' && c <= '9');
}

// Extract an integer from a string, starting at a given index
// Format expected: 'x y' where 'x' and 'y' are integers
// Updates the index to point to the start of the next integer
ll extractInteger(const std::string& line, ll& index) {
    std::string result = "";
    ll length = line.length();
    while (index < length && isDigit(line[index])) {
        result += line[index++];
    }
    while (index < length && !isDigit(line[index])) {
        index++;
    }
    return std::stoll(result);
}

// Remove duplicate elements from a linked list
void removeDuplicates(LinkedList<ll>& list) {
    if (list.get_len() == 0) return;

    std::set<ll> uniqueElements;
    LinkedList<ll> tempList;
    ListIterator<ll> iterator(list.get_clone());

    while (iterator.hasNext()) {
        ll value = iterator.value();
        if (uniqueElements.insert(value).second) {
            tempList.push_front(value);
        }
        iterator.next();
    }

    // Insert the last value if unique
    ll value = iterator.value();
    if (uniqueElements.insert(value).second) {
        tempList.push_front(value);
    }

    // Reconstruct the original list without duplicates
    list = LinkedList<ll>();
    while (tempList.get_len() > 0) {
        list.push_front(tempList.pop_front());
    }
}

// Clean up graph by removing self-loops and duplicate edges
void cleanGraph(LinkedList<ll>*& adjList, const ll& vertices) {
    for (ll i = 0; i < vertices; i++) {
        adjList[i].removeValue(i); // Remove self-loops
        removeDuplicates(adjList[i]); // Remove duplicate edges
    }
}

// Read and process a SNAP dataset file
// Extracts the graph and the number of vertices, removing junk edges
bool parseDatasetFile(const std::string& filePath, LinkedList<ll>*& adjList, ll& vertexCount) {
    std::fstream datasetFile;
    std::map<ll, ll> vertexIndexer;
    ll currentIndex = 1; // Start from 1 to allow 0 as the default unindexed state
    LinkedList<std::pair<ll, ll>> edgeTracker;

    datasetFile.open(filePath, std::ios::in);
    if (!datasetFile.is_open()) return false;

    std::string line;
    // Skip header lines
    for (ll i = 0; i < 4; i++) std::getline(datasetFile, line);

    while (std::getline(datasetFile, line)) {
        ll index = 0;
        ll fromVertex = extractInteger(line, index);
        ll toVertex = extractInteger(line, index);

        if (!vertexIndexer[fromVertex]) vertexIndexer[fromVertex] = currentIndex++;
        if (!vertexIndexer[toVertex]) vertexIndexer[toVertex] = currentIndex++;

        edgeTracker.push_front({ vertexIndexer[fromVertex] - 1, vertexIndexer[toVertex] - 1 });
    }
    datasetFile.close();

    vertexCount = currentIndex - 1;
    adjList = new LinkedList<ll>[vertexCount];

    while (edgeTracker.get_len() > 0) {
        std::pair<ll, ll> edge = edgeTracker.pop_front();
        adjList[edge.first].push_front(edge.second);
    }

    cleanGraph(adjList, vertexCount);
    return true;
}