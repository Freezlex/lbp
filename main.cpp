#include <iostream>
#include "opencv2/opencv.hpp"
#include <numeric>
#include <fstream>
#include <vector>

#include "Objetcs/PixelSet/PixelSet.h"

using namespace std;


// Utility def
enum DataType {
    empty,
    full,
    undefined
};

struct FileOutputData {
    string filename;
    array<int, 256> data;
    DataType dataType;
};

list<pair<int, int>> matrix = {
    {-1, -1}, // 1
    {0, -1}, // 2
    {1, -1}, // 4
    {-1, 0}, // 8
    {1, 0}, // 16
    {-1, 1}, // 32
    {0, 1}, // 64
    {1, 1}}; // 128

// Class def
double compareImageData(FileOutputData img_src, FileOutputData img_dts);
vector<FileOutputData> getAllImages(const string& dirPath, DataType dataType);
array<int, 256> getImageLBP(cv::Mat image);
void writeDataToFile(vector<int> data, bool type);
FileOutputData getImageOutputData(string imgPath, DataType dataType);

int main(int argc, char *argv[]) {
    const cv::Mat image = cv::imread(R"(/home/freezlex/Development/m-5/lbp/.data/EmptyParkingSpots/empty409.jpg)", cv::IMREAD_GRAYSCALE);
    const auto test = new pSet::PixelSet(image,2);
    return 0;
}

void temp() {
    vector<FileOutputData> dataset = getAllImages(R"(/home/freezlex/development/m-5/lbp/.data/EmptyParkingSpots/*.jpg)", DataType::empty);
    auto p = getAllImages(R"(/home/freezlex/development/m-5/lbp/.data/FullParkingSpots/*.jpg)", DataType::full);
    for(const auto & i : p) {
        dataset.push_back(i);
    }

    vector<FileOutputData> testDataset = getAllImages(R"(/home/freezlex/development/m-5/lbp/.data/Test/EmptyParkingSpots/*.jpg)", DataType::empty);
    p = getAllImages(R"(/home/freezlex/development/m-5/lbp/.data/Test/FullParkingSpots/*.jpg)", DataType::full);
    for(const auto & i : p) {
        testDataset.push_back(i);
    }

    int error_ctn = 0;
    for(FileOutputData src_img : testDataset) {
        auto distance = DBL_MAX;
        DataType predicted = DataType::undefined;
        string filename;
        for(auto dst_image : dataset) {
            double computed = compareImageData(src_img, dst_image);
            if(computed < distance) {
                distance = computed;
                predicted = dst_image.dataType;
                filename = dst_image.filename;
            }
        }

        if(predicted != src_img.dataType) {
            string error_debug_msg = std::format("File predicted to {} and was supposed to be {}.\n   Source image : {}\n   Dataset image : {}", (predicted == DataType::empty ? "empty" : "full"), (src_img.dataType == DataType::empty ? "empty" : "full"), src_img.filename, filename);
            std::cout << error_debug_msg << endl;
            error_ctn+=1;
        }
    }
    double percent = ((double)100/testDataset.size())*error_ctn;
    string result_data = std::format("Process finished with {} errors on {} images. Sucess rate {}%", error_ctn, testDataset.size(), ((double)100-percent));
    std::cout << result_data << endl;
}

double compareImageData(FileOutputData img_src, FileOutputData img_dts) {
    double sum = 0;
    for(int i=0; i < img_dts.data.size();i++) {
        sum += (img_dts.data[i] - img_src.data[i])*(img_dts.data[i] - img_src.data[i]);
    }
    sum = sqrt(sum);
    return sum;
}

vector<FileOutputData> getAllImages(const string& dirPath, const DataType dataType) {
    vector<cv::String> fn;
    cv::glob(dirPath, fn, false);

    vector<FileOutputData> images;
    size_t count = fn.size(); //number of png files in images folder
    for (size_t i=0; i<count; i++)
        images.push_back(getImageOutputData(fn[i], dataType));

    return images;
}

FileOutputData getImageOutputData(const string imgPath, const DataType dataType) {
    const cv::Mat image = cv::imread(imgPath,cv::IMREAD_GRAYSCALE);
    const array<int, 256> lbp = getImageLBP(image);

    FileOutputData fileOutputData = {imgPath, lbp, dataType};
    return fileOutputData;
}

array<int, 256> getImageLBP(cv::Mat image) {
    array<int, 256> lbp{0};
    for(int y=1;y<image.cols-1;y++){
        for(int x=1;x<image.rows-1;x++){
            const int current = image.at<uchar>(x, y);

            int weight = 1;
            int score = 0;
            for (auto [fx, fy] : matrix) {
                if(current <= image.at<uchar>((x + fx), (y + fy))){
                    score+=weight;
                }
                weight+=weight;
            }
            lbp[score] += 1;
        }
    }
    return lbp;
}