//
// Created by freezlex on 12/28/23.
//

#ifndef IMAGEREADER_H
#define IMAGEREADER_H
#include <utility>
#include <vector>
#include <opencv2/opencv.hpp>

#include "../PixelSet/PixelSet.h"

using namespace std;

class PixelDataset {
private:
    int lbpPass=1;

public:
    PixelDataset(int lbpPass) {
        this->lbpPass = lbpPass;
    }

    bool addDirToDataset(const string& dir, pSet::DataType* dataType, cv::ImreadModes imreadMode);
    void compareDataset(PixelDataset* dataset,pSet::DistanceType type);

    vector<pSet::PixelSet*> pSetDataset;
};

#endif //IMAGEREADER_H
