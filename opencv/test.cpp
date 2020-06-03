#include <opencv.hpp>
#include <highgui.hpp>
using namespace cv;
int main()
{
    //以IMREAD_COLOR(彩色)模式读取图片
    Mat img = imread("/root/bao.jpg", cv::IMREAD_COLOR);
    //创建一个窗口
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    //设置窗口大小
    cv::resizeWindow("image", cv::Size(640, 480));
    //将图片显示到窗口
    cv::imshow("image", img);
    //等待窗口退出
    cv::waitKey();
    return 0;
}
