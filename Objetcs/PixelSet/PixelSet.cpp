//
// Created by freezlex on 12/28/23.
//

#include "PixelSet.h"

#include <utility>

using namespace std;
using namespace pSet;

void PixelSet::processImageToLbp(cv::Mat image, int pass_amnt) {
    const auto cube = extractImageCube(move(image));
    this->calculatedLbp = dataCubeToLbpArray(cube);
    cout << cube.size() << endl;
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

vector<array<int, 256>> PixelSet::dataCubeToLbpArray(const vector<vector<vector<int>>>& cube) {
    auto cumputedLbp = vector(cube.size(), array<int, 256>{0});
    for(int i=0;i<cube.size();i++) {
        cumputedLbp[i] = dataArrayToLbp(cube[i]);
    }
    return cumputedLbp;
}

// Skipping first and last rows/cols because cannot compute empty cells
array<int, 256> PixelSet::dataArrayToLbp(const vector<vector<int>>& data) {
    array<int, 256> lbp{0};
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

