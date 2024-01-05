//
// Created by freezlex on 12/28/23.
//

#include "PixelDataset.h"

#include <utility>

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
    for(const auto pixelSet : this->pSetDataset) {
        if(!pixelSet->inferType(dataset->pSetDataset, type))
            std::cout << format("-- OUTPUT : Determinated to {} | is {}", pixelSet->getInferedTypeLabel(), pixelSet->getRealTypeLabel()) << endl;
    }
}

