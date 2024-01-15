#include <iostream>
#include "opencv2/opencv.hpp"
#include <numeric>
#include <fstream>
#include <vector>

#include "Objetcs/ImageReader/PixelDataset.h"
#include "Objetcs/PixelSet/PixelSet.h"

using namespace std;

int main(int argc, char *argv[]) {
    const auto emptyLot = new pSet::DataType("Empty Parking Lot");
    const auto fullLot = new pSet::DataType("Full Parking Lot");

    const int pass = 3;
    const auto imreadMode = cv::IMREAD_COLOR;

    std::cout << "OPT PASS : " << pass << endl;

    const auto dataset = new PixelDataset(pass);
    dataset->addDirToDataset(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\EmptyParkingSpots\*.jpg)", emptyLot, imreadMode);
    dataset->addDirToDataset(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\FullParkingSpots\*.jpg)", fullLot, imreadMode);

    const auto test_dataset = new PixelDataset(pass);
    test_dataset->addDirToDataset(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\Test\EmptyParkingSpots\*.jpg)", emptyLot, imreadMode);
    test_dataset->addDirToDataset(R"(C:\Users\Freezlex\Documents\Work\Bin\UHA\M_405\lbp\.data\Test\FullParkingSpots\*.jpg)", fullLot, imreadMode);

    test_dataset->compareDataset(dataset,pSet::Euclidian);
    test_dataset->compareDataset(dataset,pSet::Bhattacharya);
    test_dataset->compareDataset(dataset,pSet::Chisqrt);
    test_dataset->compareDataset(dataset,pSet::Manathan);
    test_dataset->compareDataset(dataset,pSet::SSD);

    std::cin.get();
    return 0;
}