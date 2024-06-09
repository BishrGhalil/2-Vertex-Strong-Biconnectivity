/*
File 5: Main.cpp
*/

#include <iostream>
#include <iomanip>
#include <chrono>
#include "CustomLinkedList.h"
#include "DataStorage.h"
#include "GraphAlgorithms.h"
using namespace std::chrono;

int main() {
    ll n;
    CustomLinkedList<ll>* adjList;
    for (ll i = 0; i < CustomDatasets::datasets.size(); i++) {
        std::cout << "Loading dataset " << CustomDatasets::datasets[i] << "...\n";
        if (!CustomDatasets::readFile(CustomDatasets::directory + CustomDatasets::datasets[i], adjList, n)) {
            std::cout << "Failed to load dataset: Incorrect path.\n";
            continue;
        }
        std::cout << "Checking directed graph's strong connectivity...\n";
        auto start = high_resolution_clock::now();
        ll* components = customGabow(adjList, n);
        if (!customIsStronglyConnected(components, n)) {
            std::cout << "-> Considering maximum strongly connected component...\n";
            auto largestSCC = customExtractLargestSCC(adjList, n, components);
            adjList = largestSCC.first; n = largestSCC.second;
        }
        auto end = high_resolution_clock::now();
        std::cout << "Testing two-connectivity of underlying graph...\n";
        auto start2 = high_resolution_clock::now();
        customMakeUndirected(adjList, n);
        if (!customIsBiconnected(adjList, n)) {
            auto end2 = high_resolution_clock::now();
            duration<double, std::milli> d1 = end - start;
            duration<double, std::milli> d2 = end2 - start2;
            std::cout << "Result: Graph is NOT 2-vertex strongly biconnected!\n";
            std::cout << "Gabow Time: " << d1.count() << "ms\n";
            std::cout << "Jens Time: " << d2.count() << "ms\n\n";
            continue;
        }
        auto end2 = high_resolution_clock::now();
        std::cout << "Bruteforcing... (2-vertex strong biconnectivity candidate)\n";
        bool success = true;
        for (ll j = 0; j < n; j++) {
            CustomLinkedList<ll>* test = adjList;
            ll vertices = n;
            customRemoveVertex(test, vertices, j);
            if (!customIsBiconnected(test, vertices)) { success = false; break; }
            if (j % 1000 == 0) std::cout << std::setprecision(2) << (double(j) * 100) / n << "% Complete\n";
        }
        if (success) std::cout << "100% Complete: Graph is 2-vertex strongly biconnected!\n";
        else std::cout << "100% Complete: Graph is NOT 2-vertex strongly biconnected!\n";
        duration<double, std::milli> d1 = end - start;
        duration<double, std::milli> d2 = end2 - start2;
        std::cout << "Gabow Time: " << d1.count() << "ms\n";
        std::cout << "Jens Time: " << d2.count() << "ms\n\n";
    }
    return 0;
}
