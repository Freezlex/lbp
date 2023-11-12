#include <iostream>
#include "opencv2/opencv.hpp"
#include <numeric>
#include <fstream>
#include <vector>

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
double compareImageData(FileOutputData imageData, vector<FileOutputData> dataset);
vector<FileOutputData> getAllImages(const string& dirPath, DataType dataType);
array<int, 256> getImageLBP(cv::Mat image);
void writeDataToFile(vector<int> data, bool type);
FileOutputData getImageOutputData(string imgPath, DataType dataType);

int main(int argc, char *argv[]) {

    vector<FileOutputData> dataset = getAllImages(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\EmptyParkingSpots\*.jpg)", DataType::empty);
    dataset.append_range(getAllImages(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\FullParkingSpots\*.jpg)", DataType::full));

    vector<FileOutputData> testDataset = getAllImages(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\Test\EmptyParkingSpots\*.jpg)", DataType::empty);
    testDataset.append_range(getAllImages(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\Test\FullParkingSpots\*.jpg)", DataType::full));

    map<bool, int> test {
    {false, 0},
    {true, 0}};
    int testDstLength = testDataset.size();
    for(int i=0;i < testDstLength; i++) {
        DataType determined = DataType::undefined;
        double dst = 999999;
        double clctDst = compareImageData(testDataset[i], dataset);
        if(dst == 999999) {
            dst = clctDst;
            determined = testDataset[i].dataType;
        } else {
            if(dst > clctDst) {
                dst = clctDst;
                determined = testDataset[i].dataType;
            }
        }
        test[(testDataset[i].dataType == determined)] += 1;
        std::cout << (testDataset[i].dataType == determined) << endl;
    }
    return 0;
}

double compareImageData(FileOutputData imageData, vector<FileOutputData> dataset) {
    double sum = 0;
    int dtLength = dataset.size();
    for(int i=0; i< dtLength;i++) {
        sum = 0;
        for(int n=0;n<256;n++) {
            sum += (imageData.data[n] - dataset[i].data[n]);
        }
        sum = sqrt(sum);
    }
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