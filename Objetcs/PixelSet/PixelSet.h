//
// Created by freezlex on 12/28/23.
//

#ifndef PIXELSET_H
#define PIXELSET_H
#include <utility>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

namespace pSet {
    const vector<pair<int, int>> _lbpMatrix = {
        {-1, -1}, // 1
        {0, -1}, // 2
        {1, -1}, // 4
        {-1, 0}, // 8
        {1, 0}, // 16
        {-1, 1}, // 32
        {0, 1}, // 64
        {1, 1}}; // 128

    class PixelSet {
    private:
        vector<array<int, 256>> calculatedLbp;
        void processImageToLbp(cv::Mat image, int pass_amnt);
        vector<vector<vector<int>>> extractImageCube(cv::Mat image);
        vector<array<int, 256>> dataCubeToLbpArray(const vector<vector<vector<int>>>& cube);
        array<int, 256> dataArrayToLbp(const vector<vector<int>>& data);


    public:
        PixelSet(cv::Mat image, int pass_amnt) {
            processImageToLbp(std::move(image), pass_amnt);
        }
    };
}



#endif //PIXELSET_H
