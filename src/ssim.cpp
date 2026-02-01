#include "ssim.hpp"
#include <omp.h>
using namespace std;



// I am implimating the golbal SSIM

// simple calculation of stats Matries
MomentSXY computeMomentSXY(const cv::Mat &X, const cv::Mat &Y){ 

    if ( X.empty() || Y.empty()){
        throw invalid_argument("Images are empty");
    }

    if (X.type() != CV_8UC1 || Y.type()!= CV_8UC1){
        throw invalid_argument("Images must be in cv_8UC1");
    }

    if (X.rows != Y.rows || X.cols != Y.cols){
        cerr<< "X rows: " << X.rows << "X cols: " << X.cols << "\n" << "Y rows: " << Y.rows << "Y cols: " << Y.cols <<endl;    
        throw invalid_argument(" Images sizes are not same...");
    }
    

    double sumX = 0.0, sumY = 0.0; // sum of pixels

    double sumX2 = 0.0, sumY2 = 0.0; // sum of squares 

    double sumXY = 0.0;

    for (int i = 0; i < X.rows; i++){ 
        for (int j = 0; j < X.cols; j++){
            
            double x = X.at<uchar>(i,j);
            double y = Y.at<uchar>(i,j);

            sumX += x;
            sumY += y;
            
            sumX2 += x * x;
            sumY2 += y * y;

            sumXY += x * y;

        }
    }

    int N = X.rows * X.cols;

    double Ninv = 1.0/N; 

    MomentSXY m;

    m.muX = sumX * Ninv;
    m.muY = sumY * Ninv;

    double NinvVar = 1.0/(N-1);

    m.varX = (sumX2 - sumX * sumX * Ninv)* NinvVar;

    m.varY = (sumY2 - sumY * sumY * Ninv)* NinvVar;

    m.covXY = (sumXY - sumX * sumY * Ninv)* NinvVar;

    return m;
}

double computeSSIM(const cv::Mat &X, const cv::Mat &Y){
    MomentSXY m = computeMomentSXY(X, Y);
    
    constexpr double K1 = 0.01, K2 = 0.03, L = 255.0; // wiki

    const double C1 = (K1 * L) * (K1 * L);
    const double C2 = (K2 * L) * (K2 * L);

    const double num = (2 * m.muX * m.muY + C1) * (2 * m.covXY + C2);

    const double den = (m.muX * m.muX + m.muY * m.muY + C1) * (m.varX  + m.varY + C2);

    if( den > 0.0) return num/den;

    else return 0.0;
}

vector<vector<double>> buildSSIMDistanceMatrix(const vector<string>& paths) {  
    omp_set_num_threads(omp_get_max_threads()); // use all cores
    cv::setNumThreads(1);
    if (paths.empty()) throw runtime_error(" Image paths are not givin..");
    //computing all combinattion:: cant aviod but best on the global SSIM;
    const int N = static_cast<int>(paths.size());

    vector<cv::Mat> imgs(N);
    for (int i = 0; i < N; i++){    
        string p = paths[i];
        imgs[i] = cv::imread(p, cv::IMREAD_GRAYSCALE);
        if (imgs[i].empty()) 
            throw runtime_error("Failed to read: " + p);
    }
   

    vector<vector<double>> adjMat(N, vector<double>(N, 0.0)); // keeping diagonal already zero;

    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < N; i++){
        adjMat[i][i] = 0.0;
        for(int j = i+1; j < N; j++){
            double ssim = computeSSIM(imgs[i], imgs[j]);
            if (ssim < 0.0) ssim = 0.0;
            if (ssim > 1.0) ssim = 1.0;
    
            double dist = 1.0 - ssim;

            adjMat[i][j] = adjMat[j][i] = dist;
            
            // if ((i*N + j) % 200 == 0){
            //     #pragma omp critical
            //     cout << "Progress: i = " << i << " j=" << j << endl;
            // } 
        }

        if (i % 10 == 0) 
        {
        #pragma omp critical
        std::cout << "Progress: i=" << i << "/" << N << "\n";
        }

    }
    
    return adjMat;
}

void writeDistanceMatrix(const std::vector<std::vector<double>>& adjMat, const std::string& filename)
{
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw runtime_error("Could not open file: " + filename);
    }

    const int N = adjMat.size();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            ofs << adjMat[i][j];
            if (j + 1 < N)
                ofs << " ";
        }
        ofs << "\n";
    }

    ofs.close();
}



void getImagePaths(vector<string> &paths, const string &basePath, int N){
    paths.reserve(N);
    for (int i = 1; i <= N; ++i) {
        paths.push_back(basePath + "/" + to_string(i) + ".png");
    }
}

