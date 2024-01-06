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
    for(size_t i=0;i<count;i++) {
        const cv::Mat image = cv::imread(fn[i], imreadMode);
        this->pSetDataset.push_back(new pSet::PixelSet(image, dataType, this->lbpPass));
    }
    return true;
}

void PixelDataset::compareDataset(PixelDataset* dataset, pSet::DistanceType type) {
    LbpResult results = LbpResult();
    for(const auto pixelSet : this->pSetDataset) {
        results.addResult(pixelSet->getRealDataType(), pixelSet->inferType(dataset->pSetDataset, type));
    }
    results.getStats();
}

