#include <iostream>
#include "opencv2/opencv.hpp"
#include <numeric>
#include <fstream>
#include <vector>

using namespace cv;
using namespace std;

list<pair<int, int>> matrix = {
        {-1, -1}, // 1
        {0, -1}, // 2
        {1, -1}, // 4
        {1, 0}, // 8
        {1, 1}, // 16
        {0, 1}, // 32
        {-1, 1}, // 64
        {-1, 0}}; // 128

vector<int> getImageLBP(Mat image);
void writeDataToFile(vector<int> data, bool type);

int main(int argc, char *argv[]) {
    Mat image = imread("",IMREAD_GRAYSCALE);

    vector<int> lbp = getImageLBP(image);
    auto score = accumulate(lbp.begin(), lbp.end(), 0);
    return 0;
}

void writeDataToFile(vector<int> data, bool type){
    std::ofstream outfile("data.lbp");
    /*if(outfile.is_open()){
        for()
    }*/
}

vector<int> getImageLBP(Mat image) {
    vector<int> lbp = {};
    for(int y=1;y<image.cols-1;y++){
        for(int x=1;x<image.rows-1;x++){
            int current = image.at<uchar>(x, y);
            int score = 0;
            int weight=1;
            for (auto [fx, fy] : matrix) {
                if(current <= image.at<uchar>((x + fx), (y + fy))){
                    score+=weight;
                    weight+=weight;
                }
            }
            lbp.push_back(score);
        }
    }
    return lbp;
}