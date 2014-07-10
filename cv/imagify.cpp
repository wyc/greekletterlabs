#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "detector.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    if( argc != 2) {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    if(!image.data) {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    std::vector<FaceSet> faceSets;
    Detector::detectFaceSets(image, faceSets);
    std::string json;
    Detector::convertFaceSetsToJson(faceSets, json);

    //imwrite("./gray.jpg", gray_image );

    return 0;
}

