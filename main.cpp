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

    const auto dataset = new PixelDataset(3);
    dataset->addDirToDataset(R"(/home/freezlex/Development/m-5/lbp/.data/EmptyParkingSpots/*.jpg)", emptyLot, cv::IMREAD_COLOR);
    dataset->addDirToDataset(R"(/home/freezlex/Development/m-5/lbp/.data/FullParkingSpots/*.jpg)", fullLot, cv::IMREAD_COLOR);

    const auto test_dataset = new PixelDataset(3);
    test_dataset->addDirToDataset(R"(/home/freezlex/Development/m-5/lbp/.data/Test/EmptyParkingSpots/*.jpg)", emptyLot, cv::IMREAD_COLOR);
    test_dataset->addDirToDataset(R"(/home/freezlex/Development/m-5/lbp/.data/Test/FullParkingSpots/*.jpg)", fullLot, cv::IMREAD_COLOR);

    test_dataset->compareDataset(dataset,pSet::Euclidian);
    return 0;
}