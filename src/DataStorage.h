/*
File: DataStorage.h
*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#define DATASETS_DIRECTORY "datasets\\"

namespace fs = std::filesystem;

// Manages data for datasets
namespace DatasetManager {
    std::vector<std::string> getDatasetFiles(const std::string& directory) {
        std::vector<std::string> datasetFiles;
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.path().extension() == ".txt") {
                datasetFiles.push_back(entry.path().filename().string());
            }
        }
        return datasetFiles;
    }

    std::string getDirectory() {
        return DATASETS_DIRECTORY;
    }
}
