#include "../../include/buckets/BucketDescriptor.h"
#include "../../include/util/ImgprocUtil.h"

BucketDescriptor BucketDescriptor::noBuckets() {return BucketDescriptor(false, false, std::vector<cv::Vec3f>(), std::vector<cv::Vec3f>());}

BucketDescriptor BucketDescriptor::create(const std::vector<cv::Vec3f>& redBuckets, const std::vector<cv::Vec3f>& blueBuckets) {
    if ((!redBuckets.empty()) && (!blueBuckets.empty())) return BucketDescriptor(true, true, redBuckets, blueBuckets);
    else if (!redBuckets.empty()) return BucketDescriptor(true, false, redBuckets, std::vector<cv::Vec3f>());
    else return BucketDescriptor(false, true,  std::vector<cv::Vec3f>(), blueBuckets);
}

BucketDescriptor::BucketDescriptor(bool redBucket, bool blueBucket, const std::vector<cv::Vec3f>& redBuckets, const std::vector<cv::Vec3f>& blueBuckets) {
    this->redBucket = redBucket;
    this->blueBucket = blueBucket;
    if (redBucket)
        this->redBuckets = redBuckets;
    if (blueBucket)
        this->blueBuckets = blueBuckets;
}

BucketDescriptor::BucketDescriptor(const BucketDescriptor &other) {
    this->redBucket = other.redBucket;
    this->blueBucket = other.blueBucket;
    this->redBuckets = other.redBuckets;
    this->blueBuckets = other.blueBuckets;
}

BucketDescriptor& BucketDescriptor::operator=(const BucketDescriptor &other) {
    if (this != &other) {
        this->redBucket = other.redBucket;
        this->blueBucket = other.blueBucket;
        this->redBuckets = other.redBuckets;
        this->blueBuckets = other.blueBuckets;
    }
    return *this;
}

bool BucketDescriptor::hasBucketCenter() {return redBucket || blueBucket;}
bool BucketDescriptor::hasRedBucketCenter() {return redBucket;}
bool BucketDescriptor::hasBlueBucketCenter() {return blueBucket;}

cv::Point2f BucketDescriptor::getRedBucketCenter(const int i) {return cv::Point2f(redBuckets[i][0], redBuckets[i][1]);}
cv::Point2f BucketDescriptor::getBlueBucketCenter(const int i) {return cv::Point2f(blueBuckets[i][0], blueBuckets[i][1]);}

std::vector<cv::Vec3f> BucketDescriptor::getAllRedBuckets() {
    return redBuckets;

}
std::vector<cv::Vec3f> BucketDescriptor::getAllBlueBuckets() {
    return blueBuckets;
}

