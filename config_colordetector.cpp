#include "config_colordetector.h"

using namespace CONFIG;

colorDetector::colorDetector()
{
    sesX = new sesgo[3];
    selected = false;

    //estos 5 valores fueron sacados despues de varias pruebas en el laboratorio de prototipos y fueron los que mejores resultado arrojaron
    low_diff = 20;
    high_diff = 10;
    conn = 4;
    val = 255;
    flags = conn + (val << 8) + CV_FLOODFILL_MASK_ONLY;

}

void colorDetector::calibrar(Mat m,int Nsesgo)
{
    //codigo sacado de la página 101 del libre Practical Opencv
    frame = m;

    if(!selected)
        mask.create(frame.rows+2, frame.cols+2, CV_8UC1);

    Mat frame_hsv;
    cvtColor(frame, frame_hsv, CV_BGR2HSV);

    // extract the hue and saturation channels
    int from_to[] = {0,0, 1,1};
    Mat hs(frame.size(), CV_8UC2);
    mixChannels(&frame_hsv, 1, &hs, 1, from_to, 2);

    // check for the range of H and S obtained from floodFill
    inRange(hs, Scalar(sesX[Nsesgo].get_l_h(), sesX[Nsesgo].get_l_s()),
            Scalar(sesX[Nsesgo].get_h_h(), sesX[Nsesgo].get_h_s()),
            frame_thresholded);

    // open and close to remove noise
    Mat str_el = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(frame_thresholded, frame_thresholded, MORPH_CLOSE, str_el);
    morphologyEx(frame_thresholded, frame_thresholded, MORPH_OPEN, str_el);

    morphologyEx(frame_thresholded, frame_thresholded, MORPH_OPEN, str_el);
    morphologyEx(frame_thresholded, frame_thresholded, MORPH_CLOSE, str_el);

    str_el = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));
    morphologyEx(frame_thresholded, frame_thresholded, MORPH_CLOSE, str_el);

}

void colorDetector::set_seedPoint(int Nsesgo, Point p)
{
/*    selected = true;

    this->seedPoint = p;

    //todo este código salió del libro Practical OPENCV p101
    // make mask using floodFill
    mask = Scalar::all(0);
    floodFill(frame, mask, p, Scalar(255, 255, 255), 0, Scalar(low_diff, low_diff, low_diff),
              Scalar(high_diff, high_diff, high_diff), flags);

    // find the H and S range of piexels selected by floodFill
    Mat channels[3];
    split(frame_hsv, channels);

    minMaxLoc(channels[0], sesX[Nsesgo].get_l_h_DIRMEM(), sesX[Nsesgo].get_h_h_DIRMEM(), NULL, NULL, mask.rowRange(1, mask.rows-1).colRange(1, mask.cols-1));
    minMaxLoc(channels[1], sesX[Nsesgo].get_l_s_DIRMEM(), sesX[Nsesgo].get_h_s_DIRMEM(), NULL, NULL, mask.rowRange(1, mask.rows-1).colRange(1, mask.cols-1));
*/
    //***************************************
    selected = true;

    //seed point
    //cv::Point p(x, y);

    // make mask using floodFill
    mask = Scalar::all(0);
    floodFill(frame, mask, p, Scalar(255, 255, 255), 0, Scalar(low_diff, low_diff, low_diff),
    Scalar(high_diff, high_diff, high_diff), flags);

    // find the H and S range of piexels selected by floodFill
    Mat channels[3];
    split(frame_hsv, channels);
    int l_h = 0, h_h = 0, l_s = 0, h_s = 0;
    minMaxLoc(channels[0], &l_h, &h_h, NULL, NULL, mask.rowRange(1, mask.rows-1).colRange(1, mask.cols-1) );
    minMaxLoc(channels[1], &l_s, &h_s, NULL, NULL, mask.rowRange(1, mask.rows-1).colRange(1, mask.cols-1) );


}

void colorDetector::setSesgos(int Nsesgo, double h_h, double l_h, double h_s, double l_s)
{
    sesX[Nsesgo].setValues(h_h, l_h, h_s, l_s);
}



//------------------------------------------------
sesgo::sesgo()
{
    h_h = l_h = h_s = l_s = 0;
}

void sesgo::setValues(double h_h, double l_h, double h_s, double l_s)
{
    this-> h_h = h_h;
    this-> l_h = l_h;
    this-> h_s = h_s;
    this-> l_s = l_s;
}
