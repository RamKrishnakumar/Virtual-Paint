#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
    Mat imgHSV, outputMask;
    int hmin=0, smin=0, vmin=0;
    int hmax=179, smax=255, vmax=255;
    

    VideoCapture cap(0);
    Mat img;

    //---------------------------------------TrackBar-----------------------
    namedWindow("TrackBars", (640, 400));
    createTrackbar("Hue Min", "TrackBars", &hmin, 179);
    createTrackbar("Hue Max", "TrackBars", &hmax, 179);
    createTrackbar("Sat Min", "TrackBars", &smin, 255);
    createTrackbar("Sat Max", "TrackBars", &smax, 255);
    createTrackbar("Value Min", "TrackBars", &vmin, 255);
    createTrackbar("Value Max", "TrackBars", &vmax, 255);
    //----------------------------------------------------------------------

    while (true) {
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);

        inRange(imgHSV, lower, upper, outputMask);
        cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;
        imshow("Webcam", img);
        imshow("Color mask detection", outputMask);
        waitKey(1);
    }
}