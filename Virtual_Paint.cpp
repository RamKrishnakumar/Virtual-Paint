#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// <summary>                VIRTUAL PAINTER
/// 
/// Developing of Virtual Painter using OpenCV
/// we impliment Contours detection, color detection, webcam etc..
///                       CONCEPT 
/// We will first detect the color using HSV Color space, once we have color detected
/// then,we are going to pass it to contour method to find the countours, after finding countour we are goint to take the X& Y values of those contours
/// then, we will create a circle at that point.
/// 
/// Idea to detect color is that we will have an array for all the color having (hmin, smin, vmin & hmax, smax, vmax) individually
/// and we will loop through this arrayor vector or matrix and one by one we will find an image for that
/// So, Will have multiple mask for diff-diff colors to do that we will create a Vector and 
/// if inside the vector if there is 3 vector then mask if 4 vector inside the vector then 4 maskt
/// </summary>

Mat img;
VideoCapture cap(0);
vector<vector<int>> newPoints;//{{x(value of x point), y(value of y point), value of color},{}}

//////////////////////////// Color Values////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//{hmin,smin,vmin,hmax,smax,vmax}
vector<vector<int>> myColors{ {35,40,251,91,162,255},    //Green               //this sturcture here contains color value to get the values
                                {95,98,225,126,255,255},//Ultramine Blue       //color picker script or code to find color
                                  {146,0,208,174,127,255}//
                             };
   //for all of the above color we have to find the mask


//now we have to provide that what color will these upper value show
vector<Scalar> myColorValues{ {0,255,0},      //Green
                               {255,0,0}, //Ultramarine Blue
                                {255,90,172} };
//----------------------------------------------------------------------------------------------------------------------------------------------------------


Point getContours(Mat mask) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //drawContours(img,contours, -1, Scalar(255,0,255),2);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    Point myPoint(0, 0);

    for (int i = 0; i < contours.size(); i++) {

        int area = contourArea(contours[i]);
       // cout << area << endl;

       
        string objectType;

        if (area > 50) {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            //cout << conPoly[i].size() << endl;
            boundRect[i] = boundingRect(conPoly[i]);
            myPoint.x = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y = boundRect[i].y;

            //drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 3);
        }
    }
    return myPoint;
}



///Here in below function we get the mask values from mask we need to find the exact points where we want to draw
//We need contors

vector<vector<int>> findColor(Mat img) 
{
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    for (int i = 0; i < myColors.size(); i++) {
        
        Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);//to get the all hsv min values form above myColors vector holding value that we find out using color-picker
        Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);//to get the all hsv max values from above myColors vector holding value that we find out using color-picker
        Mat mask;
        inRange(imgHSV, lower, upper, mask);
        // imshow(to_string(i), mask); //this will show the diff diff mask window for diff diff color that we have in myColors vector
        Point myPoint = getContours(mask);

        if (myPoint.x != 0 && myPoint.y != 0) {
            newPoints.push_back({ myPoint.x,myPoint.y,i });
        }
    
    }
    return newPoints;

}

//funciont to draw on screen
void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues) {
    for (int i = 0; i < newPoints.size(); i++) {
        circle(img, Point(newPoints[i][0],newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
        //circle(img, Point(newPoints[i][0]), Point(newPoints[i][1])),10,myColorValues);
  }
}

void main() {
    //VideoCapture cap(0);
    //Mat img;
    while (true) {
        cap.read(img);
        newPoints = findColor(img);
        drawOnCanvas(newPoints,myColorValues);

        imshow("Webcam", img);
        waitKey(1);
    }
}