#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdlib>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Map.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    //namedWindow("Map");
    //imshow("Map", image);
    
    Mat blurred;
    blur(image, blurred, Size(3, 3));
//    namedWindow("Blurred Map");
//    imshow("Blurred Map", blurred);
    imwrite("Blurred Map.bmp", blurred);
    
    Mat thresh;
    //adaptiveThreshold(blurred, thresh, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 0);
    threshold(blurred, thresh, 250, 255, THRESH_BINARY);
//    namedWindow("Threshold Map");
//    imshow("Threshold Map", thresh);
    imwrite("Threshold Map.bmp", thresh);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    Mat contoured(thresh.size(), CV_8UC1, Scalar(255));
    drawContours(contoured, contours, -1, Scalar(0), 1);
//    namedWindow("Contoured Map");
//    imshow("Contoured Map", contoured);
    imwrite("Contoured Map.bmp", contoured);
    cout << "Done" << endl;
//    waitKey(0);
    return 0;
}
