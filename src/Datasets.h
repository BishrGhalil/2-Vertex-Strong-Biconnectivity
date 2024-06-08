#pragma once
#include <iostream>
#include <string>
#include <vector>

typedef long long int ll;

// Namespace to handle SNAP datasets information
namespace SNAP {
    // List of available SNAP dataset files
    const std::vector<std::string> datasets = {
        "Email-EuAll.txt", 
        "Cit-HepTh.txt", 
        "Cit-HepPh.txt", 
        "Wiki-Vote.txt", 
        "p2p-Gnutella06.txt", 
        "p2p-Gnutella04.txt"
    };

    // Directory path for the datasets
    const std::string directory = "datasets\\";
}
