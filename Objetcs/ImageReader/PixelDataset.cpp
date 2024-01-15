//
// Created by freezlex on 12/28/23.
//

#include "PixelDataset.h"

#include <utility>

#include "../LbpResult/LbpResult.h"

bool PixelDataset::addDirToDataset(const string& dir, pSet::DataType* dataType, cv::ImreadModes imreadMode) {
    vector<cv::String> fn;
    cv::glob(dir, fn, false);

    vector<pSet::PixelSet> pixelSets;
    size_t count = fn.size();
    std::cout << format("Start processing {} image from {} datasets", fn.size(), dataType->label) << endl;
    for(size_t i=0;i<count;i++) {
        const cv::Mat image = cv::imread(fn[i], imreadMode);
        this->pSetDataset.push_back(new pSet::PixelSet(image, dataType, this->lbpPass));
    }
    return true;
}

void PixelDataset::compareDataset(PixelDataset* dataset, pSet::DistanceType type) {
    auto results = LbpResult();
    const auto start = std::chrono::system_clock::now();
    std::cout << "Start comparing dataset." << endl;
    for(const auto pixelSet : this->pSetDataset) {
        results.addResult(pixelSet->getRealDataType(), pixelSet->inferType(dataset->pSetDataset, type));
    }

    // Calculate the difference (end - start) in milliseconds
    const std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
    std::cout << "-- Dataset processed in " << elapsed.count() << "s with " << type << " distance." << endl;
    results.getStats();
}

