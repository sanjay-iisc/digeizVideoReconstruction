#include "corruptImages.hpp"
#include <fstream>
#include <iostream>

using namespace std;


vector<vector<double>> readDistanceMatrix(const string& fileName,  int N){

    ifstream ifs(fileName);

    if (!ifs.is_open()){
        throw runtime_error("File is not open: "+ fileName);
    }
    
    if (N <= 0) throw invalid_argument("Number of elements N is zero....");

    vector<vector<double>> DMat(N, vector<double>(N, -1.0)); 
    
    for (int i = 0; i< N; i++){
        for (int j = 0; j < N; j++){
            if (!(ifs >> DMat[i][j])) throw runtime_error("Cannot read the file: " + to_string(i) + "-to-"+ to_string(j)+"....");
        }
    }

    return DMat;
}


//filterning frames:

vector<int> inlierImages(const vector<double> &scores, float factor){
    
    const int N = static_cast<int>(scores.size());

    if (N == 0) throw runtime_error(" The scorese matrix is empty...");
    
    if (N == 1) return {true}; // since the N=1 means all are true;

    double sum = 0.0;
    double sumX2 = 0.0;

    for (double x : scores){
        sum += x;
        sumX2 += x * x;
    }

    double Ninv = 1.0/N;

    double mu = sum*Ninv;

    double NinvVar = 1.0/(N-1);

    double var = (sumX2 - mu * mu * N)* NinvVar;

    if (var < 0.0) var = 0.0;
    
    double sigma = std::sqrt(var);

    double thrU = mu + factor*sigma;
    double thrL = mu - factor*sigma;
    // keeping vector
    vector<int> keep;
    keep.reserve(N);

    for (int i = 0; i<N; i++){
        if( scores[i] <= thrU ) keep.push_back(i); //&& scores[i] <= thrUscores[i] >= thrL
        else cout<< "Rejected image = " << i+1 << endl;
    }

    return keep;
}

// k nearest images to avoid the global infulances of distance matrix...

vector<double> KNNscore(const vector<vector<double>> &adjMat, int K){

    int N = static_cast<int>(adjMat.size());

    vector<double> scores(N, 0.0);

    if (N == 0 || K <= 0 || K >= N) throw runtime_error("Check number of top K argument or adjMat ");;
    
    for (int i = 0; i< N; i++){
        vector<double> dists;
        dists.reserve(N-1);
        
        for (int j = 0; j<N; j++){
            if (i==j) continue;
            dists.push_back(adjMat[i][j]);
        }
        
        sort(dists.begin(), dists.end());

        // reverse(dists.begin(), dists.end()); // top K elment if I use SSIM
        double sum = 0.0;

        for (int k = 0; k < K; k++){
            sum += dists[k];
        }

        scores[i] = sum/K; 
    }

    return scores;
}


