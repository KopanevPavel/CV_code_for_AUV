#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include "include/mat/MatDetector.h"
#include "include/mat/MatDescriptor.h"
#include "include/buckets/BucketDetector.h"
#include "include/buckets/BucketDescriptor.h"
#include "include/util/ImgprocPipeline.h"
#include "include/util/ImgprocUtil.h"
#include "include/util/ColorPicker.h"


int main() {

    MatDetector detector;
    MatDetectorFrontCamera detector_frontCamera;
    MatDetectorBottomCamera detector_bottomCamera;
    BucketDetector bucket_detector;

    for (;;) {

    //std::string imagefile = "Images/blue.jpg";
    std::string imagefile = "/home/pavel/CLionProjects/Vision/Images/red.jpg";
    cv::Mat src = cv::imread(imagefile);

    //cv::VideoCapture cap(0); /// Open the default camera
    //if(!cap.isOpened())  /// Check if we succeeded
     //   return -1;


        //cv::Mat src;
        //cap >> src; // get a new frame from camera
        //cv::fastNlMeansDenoising(image, image, 3, 7, 21);

        cv::Mat image_copy, image_copy_FF, image_copy_BF, drawing;
        image_copy = src;

        std::vector<std::vector<cv::Point>> contours;

        cv::Mat image_copy_descriptor_front = src;
        cv::Mat image_copy_descriptor_bottom = src;


        if (!src.empty()) detector.detectContours(src, image_copy, contours, true);

        image_copy_FF = image_copy;
        image_copy_BF = image_copy;
        

        MatDescriptorFrontCamera front_image = detector_frontCamera.detect(image_copy, image_copy_FF, contours);
        MatDescriptorBottomCamera bottom_image = detector_bottomCamera.detect(image_copy, image_copy_BF);

        drawing = cv::Mat::zeros(src.size(), CV_8UC3); /// Black


        if (front_image.hasMat()) {
            std::vector<std::vector<cv::Point>> contour = front_image.getContour();

            cv::RNG rng;
            cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255)); /// Random colors
            cv::drawContours(drawing, contour, 0, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());

            cv::namedWindow("Biggest Contour IN MAIN");
            if (!drawing.empty()) cv::imshow("Biggest Contour IN MAIN", drawing);

            drawing = cv::Mat::zeros(src.size(), CV_8UC3); /// Black

            cv::Point2f center = front_image.getCenter();
            cv::circle(image_copy_descriptor_front, center, 3, cv::Scalar(0, 0, 255));
            center = convertToCentralCoordinates(center, src.cols, src.rows);
            std::cout<<center<<" THIS IS MAIN"<<std::endl;

            color = cv::Scalar(255, 255, 0);
            cv::rectangle(image_copy_descriptor_front, front_image.getBoundingRect().tl(), front_image.getBoundingRect().br(), color, 2, 8, 0);

            cv::namedWindow("Bounding Rectangle");
            if (!image_copy_descriptor_front.empty()) cv::imshow("Bounding Rectangle", image_copy_descriptor_front);

        }

        if (bottom_image.hasHorizontalLines()) {

            /// Draw the lines
            for (int i = 0; i < bottom_image.getHorizontalLines().size(); i++) {
                cv::Vec4f l = bottom_image.getHorizontalLines()[i];
                cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
                cv::line(drawing, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), color, 2, CV_AA);
            }

            cv::namedWindow("Horizontal Lines");
            if (!drawing.empty()) cv::imshow("Horizontal Lines", drawing);

            drawing = cv::Mat::zeros(src.size(), CV_8UC3); /// Black

            /// Expand the lines
            for (int i = 0; i < bottom_image.getHorizontalLines().size(); i++) {
                cv::Vec4i v = bottom_image.getHorizontalLines()[i];
                bottom_image.getHorizontalLines()[i][0] = 0;
                bottom_image.getHorizontalLines()[i][1] = ((float) v[1] - v[3]) / (v[0] - v[2]) * -v[0] + v[1];
                bottom_image.getHorizontalLines()[i][2] = src.cols;
                bottom_image.getHorizontalLines()[i][3] =
                        ((float) v[1] - v[3]) / (v[0] - v[2]) * (src.cols - v[2]) + v[3];
                cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
                cv::line(drawing,
                         cv::Point(bottom_image.getHorizontalLines()[i][0], bottom_image.getHorizontalLines()[i][1]),
                         cv::Point(bottom_image.getHorizontalLines()[i][2], bottom_image.getHorizontalLines()[i][3]),
                         color, 2, CV_AA);
                cv::line(image_copy_descriptor_bottom,
                         cv::Point(bottom_image.getHorizontalLines()[i][0], bottom_image.getHorizontalLines()[i][1]),
                         cv::Point(bottom_image.getHorizontalLines()[i][2], bottom_image.getHorizontalLines()[i][3]),
                         color, 2, CV_AA);

                std::cout<<bottom_image.getIntersectionWithHorizontal(src, bottom_image.getHorizontalLines()[i])<<" Intersection Y"<<std::endl;
            }

            cv::namedWindow("Horizontal Lines EXP");
            if (!drawing.empty()) cv::imshow("Horizontal Lines EXP", drawing);

            drawing = cv::Mat::zeros(src.size(), CV_8UC3); /// Black

        }
        if (bottom_image.hasVerticalLines()) {

            /// Draw the lines
            for (int i = 0; i < bottom_image.getVerticalLines().size(); i++) {
                cv::Vec4f l = bottom_image.getVerticalLines()[i];
                cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
                cv::line(drawing, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), color, 2, CV_AA);
            }

            cv::namedWindow("Vertical Lines");
            if (!drawing.empty()) cv::imshow("Vertical Lines", drawing);

            drawing = cv::Mat::zeros(src.size(), CV_8UC3); /// Black

            /// Expand the lines
            for (int i = 0; i < bottom_image.getVerticalLines().size(); i++) {
                cv::Vec4i v = bottom_image.getVerticalLines()[i];
                bottom_image.getVerticalLines()[i][0] = 0;
                bottom_image.getVerticalLines()[i][1] = ((float) v[1] - v[3]) / (v[0] - v[2]) * -v[0] + v[1];
                bottom_image.getVerticalLines()[i][2] = src.cols;
                bottom_image.getVerticalLines()[i][3] =
                        ((float) v[1] - v[3]) / (v[0] - v[2]) * (src.cols - v[2]) + v[3];
                cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
                cv::line(drawing,
                         cv::Point(bottom_image.getVerticalLines()[i][0], bottom_image.getVerticalLines()[i][1]),
                         cv::Point(bottom_image.getVerticalLines()[i][2], bottom_image.getVerticalLines()[i][3]),
                         color, 2, CV_AA);
                cv::line(image_copy_descriptor_bottom,
                         cv::Point(bottom_image.getVerticalLines()[i][0], bottom_image.getVerticalLines()[i][1]),
                         cv::Point(bottom_image.getVerticalLines()[i][2], bottom_image.getVerticalLines()[i][3]),
                         color, 2, CV_AA);
                std::cout<<bottom_image.getIntersectionWithVertical(src, bottom_image.getVerticalLines()[i])<<" Intersection X"<<std::endl;
            }

            cv::namedWindow("Vertical Lines EXP");
            if (!drawing.empty()) cv::imshow("Vertical Lines EXP", drawing);

            cv::namedWindow("Lines EXP");
            if (!image_copy_descriptor_bottom.empty()) cv::imshow("Lines EXP", image_copy_descriptor_bottom);

            drawing = cv::Mat::zeros(src.size(), CV_8UC3); /// Black

        }






        BucketDescriptor image_with_buckets = bucket_detector.detect(src, false);

        cv::namedWindow("SRC");
        if (!src.empty()) cv::imshow("SRC", src);


        cv::Mat image_copy_descriptor_buckets = src;

        if (image_with_buckets.hasBucketCenter()) {
            if ((image_with_buckets.hasBlueBucketCenter()) && (image_with_buckets.hasRedBucketCenter())) {
                std::vector<cv::Vec3f> RedBuckets = image_with_buckets.getAllRedBuckets();
                std::vector<cv::Vec3f> BlueBuckets = image_with_buckets.getAllBlueBuckets();

                for (int i = 0; i < RedBuckets.size(); i++) {
                    cv::Point2f RedBucketCenter = image_with_buckets.getRedBucketCenter(i);
                    if ((RedBucketCenter.x != 0) && (RedBucketCenter.y != 0)) {
                        /// Draw
                        cv::circle (image_copy_descriptor_buckets, RedBucketCenter, 3, cv::Scalar(0, 255, 0));
                        cv::circle (image_copy_descriptor_buckets, RedBucketCenter, RedBuckets[i][2], cv::Scalar(255, 0, 0));

                        RedBucketCenter = convertToCentralCoordinates(RedBucketCenter, src.cols, src.rows);
                        std::cout<<RedBucketCenter<<" THIS IS MAIN RED"<<std::endl;
                    }
                }

                for (int i = 0; i < BlueBuckets.size(); i++) {
                    cv::Point2f BlueBucketCenter = image_with_buckets.getBlueBucketCenter(i);

                    if ((BlueBucketCenter.x != 0) && (BlueBucketCenter.y != 0)) {
                        /// Draw
                        cv::circle (image_copy_descriptor_buckets, BlueBucketCenter, 3, cv::Scalar(0, 255, 0));
                        cv::circle (image_copy_descriptor_buckets, BlueBucketCenter, BlueBuckets[i][2], cv::Scalar(0, 0, 255));

                        BlueBucketCenter = convertToCentralCoordinates(BlueBucketCenter, src.cols, src.rows);

                        std::cout<<BlueBucketCenter<<" THIS IS MAIN BlUE"<<std::endl;
                    }
                }
            }
            else {
                if (image_with_buckets.hasBlueBucketCenter()) {
                    std::vector<cv::Vec3f> BlueBuckets = image_with_buckets.getAllBlueBuckets();

                    for (int i = 0; i < BlueBuckets.size(); i++) {
                        cv::Point2f BlueBucketCenter = image_with_buckets.getBlueBucketCenter(i);

                        if ((BlueBucketCenter.x != 0) && (BlueBucketCenter.y != 0)) {
                            /// Draw
                            cv::circle(image_copy_descriptor_buckets, BlueBucketCenter, 3, cv::Scalar(0, 255, 0));
                            cv::circle(image_copy_descriptor_buckets, BlueBucketCenter, BlueBuckets[i][2], cv::Scalar(0, 0, 255));

                            BlueBucketCenter = convertToCentralCoordinates(BlueBucketCenter, src.cols, src.rows);

                            std::cout << BlueBucketCenter << " THIS IS MAIN BlUE 1" << std::endl;
                        }
                    }

                }
                if (image_with_buckets.hasRedBucketCenter()) {
                    std::vector<cv::Vec3f> RedBuckets = image_with_buckets.getAllRedBuckets();

                    for (int i = 0; i < RedBuckets.size(); i++) {
                        cv::Point2f RedBucketCenter = image_with_buckets.getRedBucketCenter(i);

                        if ((RedBucketCenter.x != 0) && (RedBucketCenter.y != 0)) {
                            /// Draw
                            cv::circle (image_copy_descriptor_buckets, RedBucketCenter, 3, cv::Scalar(0, 255, 0));
                            cv::circle (image_copy_descriptor_buckets, RedBucketCenter, RedBuckets[i][2], cv::Scalar(255, 0, 0));

                            RedBucketCenter = convertToCentralCoordinates(RedBucketCenter, src.cols, src.rows);
                            std::cout<<RedBucketCenter<<" THIS IS MAIN RED 1"<<std::endl;
                        }
                    }
                }
            }
            cv::namedWindow("Buckets");
            if (!image_copy_descriptor_buckets.empty()) cv::imshow("Buckets", image_copy_descriptor_buckets);
        }

        else {
            std::cerr<<"NO Buckets"<<std::endl;
            ///STUFF
        }

        if(cv::waitKey(30) >= 0) break;
        //cv::waitKey(33);   //Exit on any key pressed
    }
    //cv::waitKey(30);
    return 0;
}