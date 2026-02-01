# digeizVideoReconstruction

Reconstructs the correct frame order of a shuffled video using SSIM similarity and graph-based ordering.

---

## Input video

- Resolution: **1080 × 1920**
- Frames: **114**
- FPS: **25**

Place the corrupted video at:

```data/corrupted_video.mp4```

---

## Requirements

- C++17
- OpenCV
- OpenMP
- CMake

You can control the number of threads with:
```export OMP_NUM_THREADS=$(nproc)```


or set a fixed number of threads manually.

---

## Build & Run

Run everything with:
```./run.sh```


This script:

1. Runs `config.sh` → creates the build folder and runs CMake
2. Runs `build.sh` → compiles the project
3. Executes:
```cd build```
```./videoToImage``` # optional: extract frames
```./videoReorder``` # reconstruct ordering


---

## Pipeline Overview

### Step 1 - Video -> Images

**File:** `src/videoToImage.cpp`

Extracts frames from the corrupted video and saves them to: ```data/rawImages/```



If your video is stored elsewhere, update the path inside `videoToImage.cpp` (lines 10–12) and rebuild.

---

### Step 2 - Pairwise SSIM distance

**File:** `src/ssim.cpp`

Computes dist = 1-global SSIM between all image pairs (full NxN matrix).  
This forms an adjacency matrix representing similarity between frames.

The computation is parallelized using OpenMP to accelerate all-pairs comparison.

---

### Step 3 - Outlier removal

**File:** `src/corruptImages.cpp`

Uses a KNN-based score to detect corrupted frames.

For this dataset:

- k = 5 nearest neighbors
- threshold factor = 1.0f

These parameters are passed from `main.cpp`.  
For other videos, you may need to tune them.

---

### Step 4 - Frame ordering

**File:** `src/ordering.cpp`

Ordering is solved using:

- Two-end nearest-neighbor heuristic
- 2-opt local search refinement

In my case, the NN heuristic already produces an optimal path and 2-opt rarely improves it, but it is kept as a safety refinement.

---

### Step 5 - Reconstruction

The final ordered frames are written back to a video.

Both forward and reversed sequences are generated.  
The visual result looks correct, although the original motion direction (forward/backward) cannot be guaranteed.

---

## Results

## Results

### Before

[▶ Watch corrupted video](data/corrupted_video.mp4)

### After

[▶ Watch reconstructed video](data/reconstructed.mp4)



<!-- Make sure you have openMP and OpenCV. I used c++ 17. 

Also make sure your data/corrupted_video.mp4 should be in the data folder. 

To run this code, see the run.sh file. This file execute two files build.sh and config.sh. The run.sh file first run config.sh file which make build folder and execute cmake file. Then it runs build.sh to build the files. After does cd build ; export OMP_NUM_THREADS=24; ./videoToImage [if you want to convert the video data/corrupted_video.mp4 into images] ; ./videoReorder

you can also set number of theards using nproc or you can use export OMP_NUM_THREADS=$(nproc) in run.sh file.


Step1: first src/videoToImage.cpp ./videoToImage create video to image, make sure the path is written inside the code src/videoToImage.cpp. If you want to place the video on different place just change  the line 10 and 12 and then run. 

Step2: The src/main.cpp is the main file after converting video to images and data/rawImages. First I use global SSIM from wikipidea calculation [src/ssim.cpp] with all combination pair of images; To speed up the calculation of SSIM, the parallel threads I used using openMP. This creates a adjMat of all combination of nodes.

Step3: Then I use KNNscores [src/corruptImages.cpp] to remove outliers. For the task video I used First 5 top candidates of each node connetion and set the float factor to 1.0f. If you use for someother video please play with this factor. This is passed in [src/main.cpp].

Step4: After outliers detection. Used two end NN-heuristic algorithem with 2-opt local search refinement [src/ordering.cpp]. However, two end NN-heuristic is giving the optimal path, and local search algorithm does not improve; You can also visulize on printing.

Step5: The reconstracted the image order and also reverse. The video looks good , however I am not sure whether the person was moving back or forward during recording.

These are the video before [data/corrupted_video.mp4] and after [data/reconstructed.mp4]








Then in main 

 -->
