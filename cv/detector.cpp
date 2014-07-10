#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "detector.hpp"

using namespace cv;

std::string face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
std::string eyes_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

void Detector::detectFaceSets(const Mat& img, std::vector<FaceSet>& faceSets) {
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;
    if (!face_cascade.load(face_cascade_name)) {
        fprintf(stderr, "--(!)Error loading\n");
        return;
    }
    if (!eyes_cascade.load(eyes_cascade_name)) {
        fprintf(stderr, "--(!)Error loading\n");
        return;
    }

    Mat grayImg;
    cvtColor(img, grayImg, CV_RGB2GRAY);
    equalizeHist(grayImg, grayImg);
    int flags = CASCADE_SCALE_IMAGE;
    Size minFeatureSize(20, 20);
    float searchScaleFactor = 1.1f;
    int minNeighbors = 2;
    std::vector<Rect> faces;

    face_cascade.detectMultiScale(grayImg, faces, searchScaleFactor,
                                  minNeighbors, flags, minFeatureSize);
    std::vector<Rect>::const_iterator fit;
    for (fit = faces.begin(); fit != faces.end(); fit++) {
        FaceSet fs;
        memset(&fs, 0, sizeof(fs));
        fs.location = *fit;

        Mat face = grayImg(*fit);
        std::vector<Rect> eyes;

        eyes_cascade.detectMultiScale(face, eyes, searchScaleFactor,
            minNeighbors, flags, minFeatureSize);

        std::vector<Rect>::const_iterator eit;
        if (eyes.size() < 2) {
            fs.leftEye = Rect(-1, -1, -1, -1);
            fs.rightEye = Rect(-1, -1, -1, -1);
        } else {
            fs.leftEye = eyes[0];
            fs.rightEye = eyes[1];
        }
        faceSets.push_back(fs);
    }
}

void convertRectToJson(const Rect& r, std::string& json) {
    std::stringstream ss;
    ss << "'x' : " << r.x << ",\n"
       << "'y' : " << r.y << ",\n"
       << "'width'" << r.width << ",\n"
       << "'height'" << r.height << ",\n";
    json = ss.str();
}

void Detector::convertFaceSetsToJson(const std::vector<FaceSet>& faceSets, std::string& json) {
    std::stringstream ss;
    ss << "[\n";
    std::vector<FaceSet>::const_iterator it;
    for (it = faceSets.begin(); it != faceSets.end(); it++) {
        std::string ls, les, res;
        convertRectToJson(it->location, ls);
        convertRectToJson(it->leftEye, les);
        convertRectToJson(it->rightEye, res);
        ss << "{\n" << ls
               /*<< "{\n" << les << "},\n"
               << "{\n" << res << "},\n"*/
           << "},\n";
        
    }
    ss << "]\n";
    json = ss.str();
    std::cout << json << std::endl;
}

