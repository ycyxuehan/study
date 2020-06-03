#include <opencv.hpp>
#include <highgui.hpp>
using namespace cv;
int main()
{
    Mat img = imread("/root/bao.jpg", cv::IMREAD_COLOR);
    cv::namedWindow("image", cv::WINDOW_NORMAL);

    cv::resizeWindow("image", cv::Size(640, 480));
    cv::imshow("image", img);

    cv::waitKey();
    return 0;
}
