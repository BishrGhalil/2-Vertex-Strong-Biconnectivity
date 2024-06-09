#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include "CustomLinkedList.h"

void convertToUndirected(MyLinkedList<int>*& adjList, const int& vertices) {
	for (int i = 0; i < vertices; i++) {
		ListIterator<int> itr(adjList[i].get_clone());
		if (!itr.reference()) continue;
		while (itr.hasNext()) {
			adjList[itr.value()].push_front(i);
			itr.next();
		}
		adjList[itr.value()].push_front(i);
	}
}

MyLinkedList<int> depthFirstSearch(MyLinkedList<int>*& graph, const int& vertex, bool*& visited) {
	MyLinkedList<int> result;
	std::stack<int> search;
	search.push(vertex);
	visited[vertex] = true;
	while (!search.empty()) {
		int current = search.top();
		search.pop();
		result.push_front(current);
		MyLinkedList<int> outdegrees = graph[current];
		ListIterator<int> itr(outdegrees.get_clone());
		for (int i = 0; i < outdegrees.get_len(); i++) {
			if (!visited[itr.value()]) {
				search.push(itr.value());
				visited[itr.value()] = true;
			}
			itr.next();
		}
	}
	return result;
}

void dfsForSCC(MyLinkedList<int>*& graph, const int& vertex, const int& n, bool*& visited, bool*& inStack, 
	int& dfsCounter, int*& dfsNumbers, std::stack<int>& stack, std::stack<int>& representativeStack, int*& components) {
	visited[vertex] = true;
	dfsNumbers[vertex] = dfsCounter++;
	stack.push(vertex); representativeStack.push(vertex);
	inStack[vertex] = true;
	MyLinkedList<int> outdegrees = graph[vertex];
	ListIterator<int> itr(outdegrees.get_clone());
	for (int i = 0; i < outdegrees.get_len(); i++) {
		if (!visited[itr.value()])
			dfsForSCC(graph, itr.value(), n, visited, inStack, dfsCounter, dfsNumbers, stack, representativeStack, components);
		else
			if (inStack[itr.value()]) while (dfsNumbers[itr.value()] < dfsNumbers[representativeStack.top()]) representativeStack.pop();
		itr.next();
	}
	if (vertex == representativeStack.top()) {
		representativeStack.pop();
		while (!stack.empty()) {
			int w = stack.top(); stack.pop();
			inStack[w] = false;
			components[w] = vertex;
			if (w == vertex) break;
		}
	}
}

int* findStronglyConnectedComponents(MyLinkedList<int>*& adjList, const int& vertices) {
	int dfsCounter = 0;
	bool* visited = new bool[vertices] {}, *inStack = new bool[vertices] {};
	int* dfsNumbers = new int[vertices];
	int* components = new int[vertices];
	std::stack<int> stack, representativeStack;
	for (int i = 0; i < vertices; i++) {
		if (visited[i]) continue;
		dfsForSCC(adjList, i, vertices, visited, inStack, dfsCounter, dfsNumbers, stack, representativeStack, components);
	}
	return components;
}

void printSCCReport(int*& components, const int& vertices) {
	std::map<int, int> componentTracker;
	for (int i = 0; i < vertices; i++) {
		componentTracker[components[i]]++;
	}
	std::cout << "Number of strongly connected components: " << componentTracker.size() << '\n';
	int min = vertices + 1, max = 0;
	int minIndex = -1, maxIndex = -1;
	for (auto& i : componentTracker) {
		if (i.second < min) {
			min = i.second; minIndex = i.first;
		}
		if (i.second > max) {
			max = i.second; maxIndex = i.first;
		}
	}
	std::cout << "Largest strongly connected component size: " << max << " (Representative vertex: " << maxIndex << ")\n";
	std::cout << "Smallest strongly connected component size: " << min << " (Representative vertex: " << minIndex << ")\n";
}

void edgeSwap(MyLinkedList<int>*& adjList, const int& vertices) {
	MyLinkedList<int>* swapped = new MyLinkedList<int>[vertices];
	for (int i = 0; i < vertices; i++) {
		ListIterator<int> itr(adjList[i].get_clone());
		for (int j = 0; j < adjList[i].get_len(); j++) {
			swapped[itr.value()].push_front(i);
			itr.next();
		}
	}
	adjList = swapped;
}

bool validateSCC(int*& components, MyLinkedList<int>*& adjList, const int& vertices) {
	std::cout << "-> mapping components...\n";
	std::map<int, int> componentSizeTracker;
	for (int i = 0; i < vertices; i++) {
		componentSizeTracker[components[i]]++;
	}
	int* indexes = new int[vertices]{};
	std::cout << "-> encoding vertices component-wise...\n";
	int* encoder = new int[vertices];
	for (int i = 0; i < vertices; i++) {
		encoder[i] = indexes[components[i]]++;
	}
	std::cout << "-> checking each component's connectivity...\n";
	bool valid = true;
	for (auto i : componentSizeTracker) {
		MyLinkedList<int>* adj = new MyLinkedList<int>[i.second];
		for (int j = 0; j < vertices; j++) {
			if (components[j] != i.first) continue;
			int vertexIndex = encoder[j];
			ListIterator<int> itr(adjList[j].get_clone());
			for (int k = 0; k < adjList[j].get_len(); k++) {
				if (components[itr.value()] == i.first) adj[vertexIndex].push_front(encoder[itr.value()]);
				itr.next();
			}
		}
		bool* visited = new bool[i.second] {};
		MyLinkedList<int> res1 = depthFirstSearch(adj, 0, visited);
		edgeSwap(adj, i.second);
		visited = new bool[i.second] {};
		MyLinkedList<int> res2 = depthFirstSearch(adj, 0, visited);
		if (res1.get_len() != res2.get_len() || res1.get_len() != i.second) {
			std::cout << "mismatch for component " << i.first << " (DFS1: " << res1.get_len() << ", DFS2: " << res2.get_len() << ", Size: " << i.second << ")\n";
			valid = false;
		}
	}
	if (valid) std::cout << "VERDICT: Successful\n";
	else std::cout << "VERDICT: Failed
