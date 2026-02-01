#include "ordering.hpp"
#include <deque>
#include <limits>
#include <iostream>
#include <algorithm>
using namespace std;

std::vector<int> twoEndNN(const std::vector<std::vector<double>> &adjMat){
    const int N = adjMat.size();

    if(N == 0) return {};

    if(N == 1) return {0};

    // selecting a starting point could be random.

    int a0 = 0, b0 = 5;

    double best = adjMat[a0][b0];

    // finding best starting point

    for (int i = 0; i < N; i++){
        for (int j= i+1; j < N; j++){
            double value = adjMat[i][j];
            if (value < best){
                best = value;
                a0 = i;
                b0 = j;
            }
        }
    }

    deque<int> dq;

    vector<char>visited(N, 0);

    dq.push_back(a0);
    dq.push_back(b0);

    visited[a0] = 1;
    visited[b0] = 1;

    // growing path 
    while ((int)dq.size()< N){
        int L = dq.front();
        int R = dq.back();
        
        int bestNode = -1;
        bool attachLeft = false;
        double bestCost = numeric_limits<double>::infinity();//1e18;

        for (int node = 0; node < N; node++){
            if(visited[node]) continue;
            
            double cL = adjMat[node][L];
            double cR = adjMat[R][node];

            if (cL < bestCost){
                bestCost = cL;
                bestNode = node;
                attachLeft = true;
            }

            if (cR < bestCost){
                bestCost = cR;
                bestNode = node;
                attachLeft = false;
            }
        
        }
        
        if (bestNode < 0) break;

        visited[bestNode] = 1;
        if (attachLeft) dq.push_front(bestNode);
        else dq.push_back(bestNode);
    }

    return vector<int>(dq.begin(), dq.end());
}

double costImageOrder( const std::vector<int>& imageOrder, const std::vector<std::vector<double>>& adjMat){
    double cost = 0.0;

    if (imageOrder.size() <= 1) return 0.0;

    const int M = (int)imageOrder.size();

    for (int i =0; i+1 < M; i++){
        cost += adjMat[imageOrder[i]][imageOrder[i+1]];
    }
    
    return cost;
}


void twoOpt(std::vector<int>& imageOrder, const std::vector<std::vector<double>>& adjMat, int maxItr){
    const int M = (int)imageOrder.size();

    if (M < 4) return;

    bool improved = true;

    int it = 0;

    while (improved && it < maxItr){
        
        improved = false;
        it++;

        for (int i = 0; i < M-3; i++){
            for (int j = i + 2; j < M-1; j++){
                int a = imageOrder[i], b = imageOrder[i+1];
                int c = imageOrder[j], d = imageOrder[j+1];

                double costBefore = adjMat[a][b] + adjMat[c][d];
                double costAfter =  adjMat[a][c] + adjMat[b][d];
                //cout << "costBefore : " << costBefore << " costAfter : "<<costAfter << endl;
                if (costAfter + 1e-12 < costBefore){
                    reverse(imageOrder.begin()+(i+1), imageOrder.begin()+(j+1));
                    improved = true;
                }
            }
        }

    }
}
