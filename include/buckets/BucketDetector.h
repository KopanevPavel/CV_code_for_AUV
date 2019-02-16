#ifndef VISION_BUCKETDETECTOR_H
#define VISION_BUCKETDETECTOR_H

#include <opencv2/opencv.hpp>
#include "../../include/buckets/BucketDescriptor.h"

class BucketDetector {

private:

    void defaultPreprocess(const cv::Mat& src, cv::Mat& dst); // Image filter
    void extractValueChannel(const cv::Mat& src, cv::Mat& dst);
    void detectContours(const cv::Mat& src, cv::Mat& dst, std::vector<std::vector<cv::Point>>& contours);
    std::vector<cv::Vec3f> findCircles(const cv::Mat& src, cv::Mat& dst);

    void morphology(const cv::Mat& src, cv::Mat& dst);
    void meanShift(const cv::Mat& src, cv::Mat& dst);

public:

    bool isRed(const cv::Mat& src, const cv::Vec3f circle);
    bool isBlue(const cv::Mat& src, const cv::Vec3f circle);

    BucketDetector() = default;
    ~BucketDetector() = default;
    BucketDetector& operator=(const BucketDetector& other) = default;

    BucketDescriptor detect(const cv::Mat src, bool withPreprocess);

};

#endif //VISION_BUCKETDETECTOR_H
