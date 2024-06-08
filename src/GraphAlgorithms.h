#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include "LinkedList.h"

// Converts a directed graph to an undirected graph by discarding duplicate and self-loop edges
// Parameters:
// - adjList: the graph represented by an adjacency list
// - vertices: the number of vertices in the graph
void makeUndirected(LinkedList<ll>*& adjList, const ll& vertices) {
    for (ll i = 0; i < vertices; i++) {
        ListIterator<ll> itr(adjList[i].get_clone());
        if (!itr.reference()) continue;
        while (itr.hasNext()) {
            adjList[itr.value()].push_front(i);
            itr.next();
        }
        adjList[itr.value()].push_front(i);
    }
    tune(adjList, vertices);
}

// Performs depth-first search on the graph from a given vertex
// Parameters:
// - graph: the graph represented by an adjacency list
// - vertex: the vertex to start the search from
// - visited: a boolean array tracking discovered vertices
LinkedList<ll> dfs(LinkedList<ll>*& graph, const ll& vertex, bool*& visited) {
    LinkedList<ll> result; 
    std::stack<ll> search;
    search.push(vertex);
    visited[vertex] = true;
    while (!search.empty()) {
        ll current = search.top();
        search.pop();
        result.push_front(current);
        LinkedList<ll> outdegrees = graph[current];
        ListIterator<ll> itr(outdegrees.get_clone());
        for (ll i = 0; i < outdegrees.get_len(); i++) {
            if (!visited[itr.value()]) {
                search.push(itr.value());
                visited[itr.value()] = true;
            }
            itr.next();
        }
    }
    return result;
}

// Runs depth-first search on a segment of the graph and discovers local strongly connected components
// Parameters:
// (Refer to comments in the original code for detailed parameter descriptions)
void gabow_dfs(LinkedList<ll>*& graph, const ll& vertex, const ll& n, bool*& visited, bool*& inOstack, 
    ll& dfs_counter, ll*& dfs_numbers, std::stack<ll>& ostack, std::stack<ll>& rstack, ll*& components) {
    visited[vertex] = true;
    dfs_numbers[vertex] = dfs_counter++;
    ostack.push(vertex); 
    rstack.push(vertex);
    inOstack[vertex] = true;
    LinkedList<ll> outdegrees = graph[vertex];
    ListIterator<ll> itr(outdegrees.get_clone());
    for (ll i = 0; i < outdegrees.get_len(); i++) {
        if (!visited[itr.value()]) {
            gabow_dfs(graph, itr.value(), n, visited, inOstack, dfs_counter, dfs_numbers, ostack, rstack, components);
        } else {
            if (inOstack[itr.value()]) {
                while (dfs_numbers[itr.value()] < dfs_numbers[rstack.top()]) {
                    rstack.pop();
                }
            }
        }
        itr.next();
    }
    if (vertex == rstack.top()) {
        rstack.pop();
        while (!ostack.empty()) {
            ll w = ostack.top();
            ostack.pop();
            inOstack[w] = false;
            components[w] = vertex;
            if (w == vertex) break;
        }
    }
}

// Discovers strongly connected components in a directed graph using the Cheriyan-Mehlhorn-Gabow algorithm
// Parameters:
// - adjList: the graph represented by an adjacency list
// - vertices: the number of vertices in the graph
ll* gabow(LinkedList<ll>*& adjList, const ll& vertices) {
    ll dfs_counter = 0;
    bool* visited = new bool[vertices] {};
    bool* inOstack = new bool[vertices] {};
    ll* dfs_numbers = new ll[vertices];
    ll* components = new ll[vertices];
    std::stack<ll> ostack, rstack;
    for (ll i = 0; i < vertices; i++) {
        if (visited[i]) continue;
        gabow_dfs(adjList, i, vertices, visited, inOstack, dfs_counter, dfs_numbers, ostack, rstack, components);
    }
    return components;
}

// Outputs a report about strongly connected components
// Parameters:
// - components: an array representing vertices' components
// - vertices: the number of vertices in the graph
void SCCReport(ll*& components, const ll& vertices) {
    std::map<ll, ll> tracker;
    for (ll i = 0; i < vertices; i++) {
        tracker[components[i]]++;
    }
    std::cout << "Number of strongly connected components: " << tracker.size() << '\n';
    ll min = vertices + 1, max = 0;
    ll min_i = -1, max_i = -1;
    for (auto& i : tracker) {
        if (i.second < min) {
            min = i.second; 
            min_i = i.first;
        }
        if (i.second > max) {
            max = i.second; 
            max_i = i.first;
        }
    }
    std::cout << "Largest strongly connected component size: " << max << " (Representative vertex: " << max_i << ")\n";
    std::cout << "Smallest strongly connected component size: " << min << " (Representative vertex: " << min_i << ")\n";
}

// Swaps every pair of edges (x,y) to (y,x)
// Parameters:
// - adjList: the graph represented by an adjacency list
// - vertices: the number of vertices in the graph
void swap(LinkedList<ll>*& adjList, const ll& vertices) {
    LinkedList<ll>* swapped = new LinkedList<ll>[vertices];
    for (ll i = 0; i < vertices; i++) {
        ListIterator<ll> itr(adjList[i].get_clone());
        for (ll j = 0; j < adjList[i].get_len(); j++) {
            swapped[itr.value()].push_front(i);
            itr.next();
        }
    }
    adjList = swapped;
}

// Checks the correctness of a components array
// Parameters:
// - components: an array representing vertices' components
// - adjList: the graph represented by an adjacency list
// - vertices: the number of vertices in the graph
bool validSCC(ll*& components, LinkedList<ll>*& adjList, const ll& vertices) {
    std::cout << "Mapping components...\n";
    std::map<ll, ll> tracker;
    for (ll i = 0; i < vertices; i++) {
        tracker[components[i]]++;
    }
    ll* indexes = new ll[vertices] {};
    std::cout << "Encoding vertices component-wise...\n";
    ll* encoder = new ll[vertices];
    for (ll i = 0; i < vertices; i++) {
        encoder[i] = indexes[components[i]]++;
    }
    std::cout << "Checking each component's connectivity...\n";
    bool valid = true;
    for (auto i : tracker) {
        LinkedList<ll>* adj = new LinkedList<ll>[i.second];
        for (ll j = 0; j < vertices; j++) {
            if (components[j] != i.first) continue;
            ll vertex_index = encoder[j];
            ListIterator<ll> itr(adjList[j].get_clone());
            for (ll k = 0; k < adjList[j].get_len(); k++) {
                if (components[itr.value()] == i.first) {
                    adj[vertex_index].push_front(encoder[itr.value()]);
                }
                itr.next();
            }
        }
        bool* visited = new bool[i.second] {};
        LinkedList<ll> res1 = dfs(adj, 0, visited);
        swap(adj, i.second);
        visited = new bool[i.second] {};
        LinkedList<ll> res2 = dfs(adj, 0, visited);
        if (res1.get_len() != res2.get_len() || res1.get_len() != i.second) {
            std::cout << "Mismatch for component " << i.first << " (DFS1: " << res1.get_len() << ", DFS2: " << res2.get_len() << ", Size: " << i.second << ")\n";
            valid = false;
        }
    }
    std::cout << (valid ? "VERDICT: Successful\n" : "VERDICT: Failed\n");
    return valid;
}

// Checks the strong connectivity of a graph
// Parameters:
// - components: an array representing vertices' components
// - vertices: the number of vertices in the graph
bool isStronglyConnected(ll*& components, const ll& vertices) {
    for (ll i = 1; i < vertices; i++) {
        if (components[0] != components[i]) return false;
    }
    return true;
}

// Extracts the largest strongly connected component as an individual subgraph
// Parameters:
// - adjList: the graph represented by an adjacency list
// - vertices: the number of vertices in the graph
// - components: an array representing vertices' components
std::pair<LinkedList<ll>*, ll> extractLargestSCC(LinkedList<ll>*& adjList, const ll& vertices, ll*& components) {
    std::map<ll, ll> tracker, encoder;
    for (ll i = 0; i < vertices; i++) {

componentSizes[components[i]]++;
    }
    ll max = 0, largestComponent = -1;
    for (auto& i : componentSizes) {
        if (i.second > max) {
            max = i.second;
            largestComponent = i.first;
        }
    }
    ll index = 0;
    for (ll i = 0; i < numVertices; i++) {
        if (components[i] == largestComponent) encoder[i] = index++;
    }
    LinkedList<ll>* scc = new LinkedList<ll>[index];
    for (ll i = 0; i < numVertices; i++) {
        if (components[i] != largestComponent) continue;
        ll vertex = encoder[i];
        ListIterator<ll> itr(adjList[i].get_clone());
        for (ll j = 0; j < adjList[i].get_len(); j++) {
            if (components[itr.value()] == largestComponent) scc[vertex].push_front(encoder[itr.value()]);
            itr.next();
        }
    }
    return std::pair<LinkedList<ll>*, ll> { scc, index };
}

void depthFirstSearchWithClassification(LinkedList<ll>*& adjList, const ll& vertex, std::set<std::pair<ll, ll>>*& treeEdges, 
	 bool*& visited, ll& dfsCounter, ll*& dfsNumbers, ll*& backEdgesCount) {
    visited[vertex] = true;
    dfsNumbers[vertex] =