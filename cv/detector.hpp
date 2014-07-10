#include <vector>

#include <opencv2/core/core.hpp>

using namespace cv;

struct FaceSet {
    Rect location;
    Rect leftEye, rightEye;
};

class Detector {
public:
    static void detectFaceSets(const Mat& img, std::vector<FaceSet>& faceSets);
    static void convertFaceSetsToJson(const std::vector<FaceSet>& faceSets, std::string& json);
};

