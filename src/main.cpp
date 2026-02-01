#include "corruptImages.hpp"
#include "ordering.hpp"
#include "ssim.hpp"
#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;

vector<vector<double>> filterAdjMat(const vector<vector<double>> &adjMat, const vector<int> &keepIdx){
    
    if (keepIdx.empty()) throw runtime_error("keepIdx vector is not populated");

    if (adjMat.empty()) throw runtime_error("No adjMat is populated");

    const int rows = adjMat.size();
    const int cols = adjMat[0].size();

    if (rows != cols) throw runtime_error("adjMat does not have same rows and cols");

    const int M = keepIdx.size();
    
    vector<vector<double>> adjMatF(M, vector<double>(M,0.0));

    for (int a = 0; a < M; a++){
        for (int b = 0; b < M; b++){
            adjMatF[a][b] = adjMat[keepIdx[a]][keepIdx[b]];
        }
    }

    return adjMatF;
}

vector<int> getImageOrderBack(const vector<int>& imageOrder, const vector<int> &keepIdx){
    const int M = (int)imageOrder.size();

    vector<int> globalOrder;
    globalOrder.reserve(M);

    for (const int node: imageOrder){
        globalOrder.push_back(keepIdx[node]+1);
    }
    return globalOrder;
}

bool writeVideoFromOrder(const vector<int>& globalOrder,const string& rawImagesFolder, const string& outVideoPath, double fps){
    if(globalOrder.empty()){
        cerr<< "Order of images are empty: verify it..." << endl;
        return false;
    }

    const int M = (int)globalOrder.size();
    
    const string startImage = rawImagesFolder + "/" +to_string(globalOrder[0]) + ".png";
    cv::Mat first = cv::imread(startImage, cv::IMREAD_COLOR);

    if (first.empty()){
        cerr << " Cant read first image: " << startImage << endl;
        return false;
    }


    int fourcc = cv::VideoWriter::fourcc('m','p','4','v');
    cv::VideoWriter writer(outVideoPath, fourcc, fps, cv::Size(first.cols, first.rows), true);

    if (!writer.isOpened()){
        cerr << "Cant open VideWritter: " << outVideoPath << endl;
        return false;
    }

    for (int i = 0; i < M; i++){
        string imgPath = rawImagesFolder + "/" + to_string(globalOrder[i]) + ".png";
        
        cv::Mat img = cv::imread(imgPath, cv::IMREAD_COLOR);

        if(img.empty()){
            cerr<< " Image cant read: " << imgPath <<endl;
            continue;
        }

        writer.write(img);
    }

    writer.release();
    cout << "Wrote video: " << outVideoPath << "\n";
    return true;

}



int main(){

    string rawImagesFolder = "../data/rawImages";
    
    int N = 114;

    vector<string> paths;

    getImagePaths(paths, rawImagesFolder, N); // o(N)

    vector<vector<double>> adjMat= buildSSIMDistanceMatrix(paths);

    string fileName = "../data/distanceMatDistance.txt";

    writeDistanceMatrix(adjMat, fileName);

    // string fileName = "../data/distanceMatSSIM_G.txt"; // make sure you change the KNNscore calculation...

    // vector<vector<double>> adjMat = readDistanceMatrix(fileName, N);

    int K = 5; // Top scores:

    vector<double> scores = KNNscore(adjMat,K);

    float factor = 1.0f; // instead of 3 I am taking 1.0f

    vector<int> keepIdx = inlierImages(scores, factor);

    vector<vector<double>> adjMatF = filterAdjMat(adjMat, keepIdx);

    vector<int> orderFirstPass = twoEndNN(adjMatF);

    // for (const auto &a : orderFirstPass) cout << a << endl;

    double costB = costImageOrder(orderFirstPass, adjMatF);

    // local search...
    twoOpt(orderFirstPass, adjMatF, 50);

    double costA = costImageOrder(orderFirstPass, adjMatF);

    // for (const auto &a : orderFirstPass) cout << a << endl;

    vector<int> globalOrder = getImageOrderBack(orderFirstPass, keepIdx);


    string outVideo = "../data/reconstructed.mp4";

    reverse(globalOrder.begin(), globalOrder.end());
    bool status = writeVideoFromOrder( globalOrder,rawImagesFolder , outVideo, 25.0);

    cout << "Reconstruction of Images are done: " << status << endl;
    cout<< "Total size of images: "<<adjMat.size() << " Images kept after filtered: " << adjMatF.size()
     << " Cost of first pass: " << costB <<  " Cost after two-opt: " << costA<<"\n";

    return 0;
}
