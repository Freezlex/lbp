//
// Created by freezlex on 1/6/24.
//

#ifndef LBPRESULT_H
#define LBPRESULT_H
#include <map>
#include <numeric>
#include <vector>

#include "../PixelSet/PixelSet.h"

using namespace std;

class LbpResult {
private:
    map<pSet::DataType*, vector<pSet::DataType*>> resultSet;

public:
    void addResult(pSet::DataType* realType, pSet::DataType* inferedType) {
        if(!resultSet.contains(realType)) {
            resultSet[realType] = vector{inferedType};
        } else {
            resultSet[realType].push_back(inferedType);
        }
    }

    void getStats() {
        std::cout << "Processing result for display" << endl;

        for(auto rSet : resultSet) {
            map<pSet::DataType*, double> ordered;
            for(auto type : rSet.second) {
                if(ordered.contains(type)) {
                    ordered[type]++;
                } else {
                    ordered[type] = 1;
                }
            }

            vector<string> strings;
            for(auto type: ordered) {
                double percent = type.second / static_cast<double>(rSet.second.size()) *100;
                strings.push_back(format("\n    - Predicted as {}: {} | {}%", type.first->label, type.second, percent));
            }
            std::cout <<
                format("Dataset {}:\n   - Total count: {} {}", rSet.first->label, rSet.second.size(), std::accumulate(strings.begin(), strings.end(), std::string(""))) << endl;
        }
    }
};



#endif //LBPRESULT_H
