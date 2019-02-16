#ifndef VISION_BUCKETDESCRIPTOR_H
#define VISION_BUCKETDESCRIPTOR_H

#include <opencv2/opencv.hpp>

class BucketDescriptor {

private:

    bool redBucket, blueBucket;

    std::vector<cv::Vec3f> redBuckets;
    std::vector<cv::Vec3f> blueBuckets;

    BucketDescriptor(bool redBucket, bool blueBucket, const std::vector<cv::Vec3f>& redBuckets, const std::vector<cv::Vec3f>& blueBuckets); /// Constructor

public:

    static BucketDescriptor noBuckets();
    static BucketDescriptor create(const std::vector<cv::Vec3f>& redBuckets, const std::vector<cv::Vec3f>& blueBuckets);  /// Functions to initialise class

    BucketDescriptor(const BucketDescriptor& other);
    ~BucketDescriptor() = default;
    BucketDescriptor& operator=(const BucketDescriptor& other);

    bool hasBucketCenter();
    bool hasBlueBucketCenter();
    bool hasRedBucketCenter();

    std::vector<cv::Vec3f> getAllRedBuckets();
    std::vector<cv::Vec3f> getAllBlueBuckets();

    cv::Point2f getBlueBucketCenter(const int i);
    cv::Point2f getRedBucketCenter(const int i);

};


#endif //VISION_BUCKETDESCRIPTOR_H
