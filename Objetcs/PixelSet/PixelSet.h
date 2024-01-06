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

    enum DistanceType {
        Euclidian,
        Chisqrt,
        Manathan,
        SSD

    };

    class DataType {

    public:
        string label;

    public:
        explicit DataType(const string& label) {
            this->label = label;
        }

        [[nodiscard]] string label1() const {
            return label;
        }
    };

    class PixelSet {
    private:
        //var
        vector<vector<int>> calculatedLbp;
        DataType* realDataType;
        DataType* inferedDataType{};

        //fn
        void processImageToLbp(cv::Mat image, int pass_amnt);
        vector<vector<vector<int>>> extractImageCube(cv::Mat image);
        vector<vector<int>> dataCubeToLbpArray(const vector<vector<vector<int>>>& cube, int pass_amnt);
        vector<int> dataArrayToLbp(const vector<vector<int>>& data, int pass_amnt);
        vector<int> computeLbp(vector<vector<int>> data);
        double calcDistance(const PixelSet* compareData, DistanceType type);


    public:
        PixelSet(cv::Mat image, DataType* dataType, int pass_amnt) {
            this->realDataType = dataType;
            processImageToLbp(std::move(image), pass_amnt);
        }

        DataType* inferType(const vector<PixelSet*>& dataset, const DistanceType& processType);

        string getRealTypeLabel() {
            return this->realDataType->label;
        }

        string getInferedTypeLabel() {
            return this->inferedDataType == nullptr ? "UNDEFINED" : this->inferedDataType->label ;
        }

        DataType* getRealDataType() {
            return this->realDataType;
        }
    };
}



#endif //PIXELSET_H
