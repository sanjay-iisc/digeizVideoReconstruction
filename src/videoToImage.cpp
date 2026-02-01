#include<opencv2/opencv.hpp>
#include<iostream>
#include<filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(){

    string path = "../data/corrupted_video.mp4";

    string rawImageSave = "../data/rawImages/";

    if (!fs::exists(rawImageSave)){
        fs::create_directory(rawImageSave);
    }

    cv::VideoCapture cap(path);

    if (!cap.isOpened()){
        cerr<<"the video is not available....."<< endl;
        return 0;
    }

    int totalCount = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    float fps = static_cast<float>(cap.get(cv::CAP_PROP_FPS));

    std::cout << "Total frames: " << totalCount << ", FPS: " << fps << std::endl;
    
    int frameId = 0;

    while(true){
        cv::Mat frame;
        
        bool ret = cap.read(frame);
        
        if (!ret){
            cout<< "The video is ended" << endl;
            break;
        }

        frameId++;
        string fileName = rawImageSave + to_string(frameId)+".png";
        cv::imwrite(fileName, frame);
    }

    cap.release();
    //cv::destroyAllWindows();

    return 0;
}


 // cv::imshow("Video", frame);
        // char key =cv::waitKey(1);
        // if (key == 'q'){
        //     break;
        // }

//cout<<"Frame size row: " << frame.rows << "col: " << frame.cols << endl;