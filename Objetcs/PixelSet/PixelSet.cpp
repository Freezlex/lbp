//
// Created by freezlex on 12/28/23.
//

#include "PixelSet.h"

#include <utility>

using namespace std;
using namespace pSet;

void PixelSet::processImageToLbp(cv::Mat image, int pass_amnt) {
    const auto cube = extractImageCube(move(image));
    this->calculatedLbp = dataCubeToLbpArray(cube, pass_amnt);
}

/*
 * Here we're just copying the image array to a Cube of data.
 * It will be easier later to just process splitted arrays of RGB or Grayscale
 * channels instead of reading each time "is the image colored or grayscaled?".
 */
vector<vector<vector<int>>> PixelSet::extractImageCube(cv::Mat image) {
    // Cube must be [I][X][Y] = val
    vector<vector<vector<int>>> cube =
        vector(image.channels(),vector(image.cols,vector(image.rows,0)));

    for(int i = 0;i<image.channels();i++) {
        for(int x = 0; x<image.rows;x++) {
            for(int y = 0; y<image.cols;y++) {
                if(image.channels() > 1) {
                    const cv::Vec3b intensity = image.at<cv::Vec3b>(x, y);
                    cube[i][x][y] = intensity.val[i];
                } else {
                    const int intensity = image.at<uchar>(x, y);
                    cube[i][x][y] = intensity;
                }
            }
        }
    }

    return cube;
}

vector<vector<int>> PixelSet::dataCubeToLbpArray(const vector<vector<vector<int>>>& cube, int pass_amnt) {
    vector<vector<int>> cumputedLbp;
    for(int i=0;i<cube.size();i++) {
        cumputedLbp.push_back(dataArrayToLbp(cube[i], pass_amnt));
    }
    return cumputedLbp;
}

// Skipping first and last rows/cols because cannot compute empty cells
vector<int> PixelSet::dataArrayToLbp(const vector<vector<int>>& data, int pass_amnt) {

    vector<int> aggregatedLbp;
    for(int pass=1;pass<=pass_amnt;pass++) {
        const int fragSize = data.size()/pass;
        for(int frag=1;frag<=pass*pass;frag++) {
            // Copy array
            auto fragment = vector(fragSize, vector(fragSize, 0));
            for(int x=0;x<fragSize;x++) {
                for(int y=0;y<fragSize;y++) {
                    if(data.size() > ((frag-1) * fragSize + x) && data[x].size() >((frag-1) * fragSize + y)) {
                        fragment[x][y] = data[(frag-1) * fragSize + x][(frag-1) * fragSize + y];
                    }
                }
            }
            auto lbp = computeLbp(fragment);
            aggregatedLbp.insert(aggregatedLbp.end(), lbp.begin(), lbp.end());
        }
    }
    return aggregatedLbp;
}

vector<int> PixelSet::computeLbp(vector<vector<int>> data) {
    auto lbp = vector(256, 0);
    for(int x = 1; x<data.size()-1;x++) {
        for(int y = 1; y<data[x].size()-1;y++) {
            int weight = 1;
            int score = 0;
            for(auto [fx, fy]: _lbpMatrix) {
                if(data[x][y] <= data[x + fx][y + fy]) {
                    score+= weight;
                }
                weight+=weight;
            }
            lbp[score] += 1;
        }
    }
    return lbp;
}

bool PixelSet::inferType(const vector<PixelSet*>& dataset, const DistanceType& processType) {
    auto distance = DBL_MAX;
    for (PixelSet* pixelSet : dataset) {
        if(const double computed = calcDistance(pixelSet, processType); computed < distance) {
            distance = computed;
            this->inferedDataType = pixelSet->realDataType;
        }
    }
    return this->inferedDataType == this->getRealDataType();
}

double PixelSet::calcDistance(const PixelSet* compareData, DistanceType type) {
    if(this->calculatedLbp.size() != compareData->calculatedLbp.size())
        throw std::invalid_argument("Data and dataset data does not complete requirement. Not same size.");
    switch (type) {
        case Euclidian: {
            double sum = 0;
            for(int o=0;o<this->calculatedLbp.size();o++) {
                auto step = 0;
                for(int i=0;i<this->calculatedLbp[o].size();i++) {
                    step += (calculatedLbp[o][i] - compareData->calculatedLbp[o][i])*(calculatedLbp[o][i] - compareData->calculatedLbp[o][i]);
                }
                sum += sqrt(step);
            }
            return sum;
        }
        default: {return DBL_MAX;}
    }
}