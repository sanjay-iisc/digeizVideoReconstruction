import os 
import numpy as np
import matplotlib.pyplot as plt

def getHeatMap(D):
    plt.figure()
    plt.imshow(D)                  
    plt.colorbar(label="distance (1-SSIM)")
    plt.xlabel("frame index")
    plt.ylabel("frame index")

def getHistogram(D):
    upper = D[np.triu_indices_from(D, k=1)]  
    plt.figure()
    plt.hist(upper, bins=50)
    plt.xlabel("distance")
    plt.ylabel("freq")

def getLinePlot(D, i = 0):
    di = D#D[i, :]
    mu = np.mean(di)
    sigma = np.std(di)

    plt.figure()
    plt.plot(D, '-o')
    plt.axhline(mu, linestyle='--', linewidth=2, label='mean')
    plt.axhline(mu + 3*sigma, linestyle='--', linewidth=2)
    plt.axhline(mu - 3*sigma, linestyle='--', linewidth=2)
    plt.title(f"Distances from frame {i}")
    plt.xlabel("frame index j")
    plt.ylabel("distance")
    plt.grid(True)


if __name__ == "__main__":
    fileName = "data/distanceMatDistance.txt";
    D = np.loadtxt(fileName)  
    getLinePlot(D, i = 100)
    getHeatMap(D)
    getHistogram(D)
    ### Heat map
    



    plt.show()