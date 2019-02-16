cmake_minimum_required(VERSION 3.13)

project(Vision)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "    OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
    # Add OpenCV headers location to your include paths
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable  (Vision main.cpp src/mat/MatDetector.cpp include/mat/MatDetector.h
                        src/util/ImgprocPipeline.cpp include/util/ImgprocPipeline.h include/util/ImgprocUtil.h
                        src/util/ImgprocUtil.cpp src/mat/MatDescriptor.cpp include/mat/MatDescriptor.h
                        include/buckets/BucketDescriptor.h src/buckets/BucketDescriptor.cpp include/buckets/BucketDetector.h src/buckets/BucketDetector.cpp src/util/ColorPicker.cpp include/util/ColorPicker.h)

target_link_libraries(Vision ${OpenCV_LIBS})