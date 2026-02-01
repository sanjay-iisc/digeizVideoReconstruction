#pragma once

#include<vector>


// using two end NN-heuristic algorithem with multistart then 2-opt local search refinement 

std::vector<int> twoEndNN(const std::vector<std::vector<double>>& adjMat);
double costImageOrder( const std::vector<int>& imageOrder, const std::vector<std::vector<double>>& adjMat);

// two-opt after NN-heuristic algo local search...

void twoOpt(std::vector<int>& imageOrder, const std::vector<std::vector<double>>& adjMat, int maxItr);