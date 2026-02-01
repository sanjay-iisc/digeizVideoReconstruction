#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cmath>

std::vector<std::vector<double>> readDistanceMatrix(const std::string& filename,  int N);

std::vector<double> KNNscore(const std::vector<std::vector<double>> &DMat, int K); // compute the k nearest images to avoid noises due to global calculation

std::vector<int> inlierImages(const std::vector<double> &scores, float factor); // 