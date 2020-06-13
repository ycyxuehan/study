#include <iostream>
#include <vector>
#include "core.hpp"
#include <imgcodecs.hpp>
#include <highgui.hpp>
#include <imgproc.hpp>


using namespace cv;
using namespace std;

void LocalColorCorrectionUsingNonLinearMasking()
{
    Mat img = imread("/git/study/opencv/test2.jpg", IMREAD_COLOR);
    imshow("img", img);

    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    
    Mat img_gray_invert;
    Mat gaussian_mask;
    bitwise_not(img_gray, img_gray_invert);
    GaussianBlur(img_gray_invert, gaussian_mask, Size(5, 5), 3, 3);
    
    Mat const_128 = Mat(img.size(), CV_64FC1, Scalar(128));
    Mat const_255 = Mat(img.size(), CV_64FC1, Scalar(255));
    
    Mat gaussian_mask_double;
    gaussian_mask.convertTo(gaussian_mask_double, CV_64FC1);
    Mat tmp = const_128 - gaussian_mask_double;

    tmp = tmp / const_128;

    imshow("tmp", tmp);

    imshow("mask", gaussian_mask);

    img.convertTo(img, CV_64FC3);

    // Mat output = img.clone();

    for (int i = 0; i < img.rows; ++i)
    {
        for (int j = 0; j < img.cols; ++j)
        {
            Vec3d& output_pix = img.at<Vec3d>(i, j);
            double pow_value = pow(2.0, tmp.at<double>(i, j));
            output_pix[0] = (255.0 * pow(output_pix[0] / 255.0, pow_value));
            output_pix[1] = (255.0 * pow(output_pix[1] / 255.0, pow_value));
            output_pix[2] = int(255.0 * pow(output_pix[2] / 255.0, pow_value));
            
        }
    }

    img.convertTo(img, CV_8UC3);
    imshow("lcc", img);
    // CalculateHistogram(img);
    
    waitKey();

}

int main()
{
    // AdaptiveImageContrastCorrection();

    LocalColorCorrectionUsingNonLinearMasking();
    
    return 0;
}