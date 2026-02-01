#pragma once 
#include<cassert>
#include<iostream>
#include<opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>
#include <vector>


// I am implimating the golbal SSIM

struct  MomentSXY
{
    double muX = 0.0, muY = 0.0;

    double varX = 0.0, varY = 0.0;
    
    double covXY = 0.0;
};

MomentSXY computeMomentSXY(const cv::Mat &X, const cv::Mat &Y);

void getImagePaths(std::vector<std::string> &paths, const std::string &basePath, int N);

std::vector<std::vector<double>> buildSSIMDistanceMatrix(const std::vector<std::string>& paths);

void writeDistanceMatrix(const std::vector<std::vector<double>>& D, const std::string& filename);
                         


