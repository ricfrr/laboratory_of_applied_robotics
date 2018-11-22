//
//  Character_Recognition_Algorithm.cpp
//  LAR_1_0
//
//  Created by Air Marvin on 10.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#include "../Headers/Character_Recognition_Algorithm.hpp"

Character_Recognition_Algorithm::Character_Recognition_Algorithm(){
        this->filter = HSVFilterRange("bad");
}

cv::Mat Character_Recognition_Algorithm::loadImage(const std::string& filename){
    // Load image from file
    cv::Mat img = cv::imread(filename.c_str());
    if(img.empty()) {
        throw std::runtime_error("Failed to open the file " + filename);
    }
    return img;
}

void Character_Recognition_Algorithm::displayImage(cv::Mat & image,std::string windowTitle){
    //cv::imshow(windowTitle, image);
}

void Character_Recognition_Algorithm::convert_bgr_to_hsv(cv::Mat &original, cv::Mat &converted){
    // Convert color space from BGR to HSV
    cv::cvtColor(original, converted, cv::COLOR_BGR2HSV);
}

void Character_Recognition_Algorithm::apply_mask(cv::Mat &original, cv::Mat &converted, cv::Scalar lowerbound, cv::Scalar upperbound){
    cv::inRange(original,lowerbound,upperbound, converted);
}

cv::Mat Character_Recognition_Algorithm::apply_some_filtering(cv::Mat &img){
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((1*2) + 1, (1*2)+1));
    cv::dilate(img, img, kernel);
    cv::erode(img, img, kernel);

    return kernel;
}

std::vector<cv::Rect> Character_Recognition_Algorithm::extract_regions_of_interest(cv::Mat &original_img,cv::Mat & filtered_img,cv::Mat &returnedImg){
    
    std::vector<std::vector<cv::Point>> contours, contours_approx;
    std::vector<cv::Point> approx_curve;
    
    returnedImg = original_img.clone();
    cv::findContours(filtered_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    displayImage(filtered_img, "contours");
    //cv::waitKey(0);
    
    std::vector<cv::Rect> boundRect(contours.size());
    for (int i=0; i<contours.size(); ++i)
    {
        double area = cv::contourArea(contours[i]);
        if (area < this->MIN_AREA_SIZE) continue; // filter too small contours to remove false positives
        approxPolyDP(contours[i], approx_curve, 2, true);
        contours_approx = {approx_curve};
        drawContours(returnedImg, contours_approx, -1, cv::Scalar(0,170,220), 3, cv::LINE_AA);
        boundRect[i] = boundingRect(cv::Mat(approx_curve)); // find bounding box for each green blob
    }
    
    return boundRect;
}

std::tuple<cv::Mat,cv::Mat> Character_Recognition_Algorithm::invert_masked_image(cv::Mat &original, cv::Mat &masked_image){
    
    //filtered cv::Scalar values are the ones that will be overlayed in the original image
    cv::Mat mask_inv, filtered(original.rows, original.cols, CV_8UC3, cv::Scalar(255,255,255));
    
    //only get the shapes in black color - so we need to invert because only shapes are white and the rest is black
    cv::bitwise_not(masked_image, mask_inv); // generate binary mask with inverted pixels w.r.t. green mask -> black numbers are part of this mask
    
    original.copyTo(filtered, mask_inv);   // create copy of image without green shapes
    
    return std::tuple<cv::Mat,cv::Mat>(mask_inv, filtered);
}

void Character_Recognition_Algorithm::rotate_image(cv::Mat &src, double angle, cv::Mat &result){
    
    // get rotation matrix for rotating the image around its center in pixel coordinates
    cv::Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0,2) += bbox.width/2.0 - src.cols/2.0;
    rot.at<double>(1,2) += bbox.height/2.0 - src.rows/2.0;
    
    cv::warpAffine(src, result, rot, bbox.size(),cv::INTER_LINEAR,
                   cv::BORDER_CONSTANT,
                   cv::Scalar(0, 0, 0));
                   //cv::Scalar(255, 255, 255));
}


std::vector<cv::Mat> Character_Recognition_Algorithm::preprocessing(cv::Mat &img, cv::Mat &filtered, std::vector<cv::Rect> &boundRect){
    
    // Convert color space from BGR to HSV
    cv::Mat hsv_img;
    convert_bgr_to_hsv(img, hsv_img);
    
    //find a filter
    Color_Processing color;
    std::string filename = "../data/calib/filter_+++.png"; //the link might has to be adapted
    color.calibrate_color(filename);
    HSVFilterRange filter = color.getFilter();
    this->filter = filter;
    
    
    // Find green regions
    cv::Mat green_mask;
    apply_mask(hsv_img,
               green_mask,
               cv::Scalar(filter.lb[0], filter.lb[1], filter.lb[2]),
               cv::Scalar(filter.ub[0], filter.ub[1], filter.ub[2]));
    
    
    // Apply some filtering
    cv::Mat kernel = apply_some_filtering(green_mask);
    
    // Find contours
    cv::Mat contours_img;
    boundRect = extract_regions_of_interest(img, green_mask,contours_img);
    
    //invert the pixels black white
    std::tuple<cv::Mat,cv::Mat> inversionResult = invert_masked_image(img, green_mask);

    cv::Mat green_mask_inv  = std::get<0>(inversionResult); //only for displaying purposes
    filtered        = std::get<1>(inversionResult); // needed to detect digit
    

    //cutting out the areas of interest and return them
    std::vector<cv::Mat> cut_images;
    std::vector<cv::Rect> rects;
    for(int i = 0;i<boundRect.size();i++){
        cv::Mat cut_img = cv::Mat(filtered, boundRect[i]);
        if (!cut_img.empty()){
            cut_images.push_back(cut_img);
            rects.push_back(boundRect[i]);
        }
    }
    
    boundRect = rects;
    
    return cut_images;
}

void Character_Recognition_Algorithm::turn_image(cv::Mat input, cv::Mat & output, double angle){
    rotate_image(input, angle, output);
}

double Character_Recognition_Algorithm::determine_orientation(cv::Mat image){
    
    cv::Vec4f line;
    
    cv::Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY); //perform gray scale conversion.
    
    threshold( gray, gray, 100,255,cv::THRESH_BINARY_INV );
    
    std::vector<std::vector<cv::Point>> contours;
    
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(gray, contours, hierarchy,
                     cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE );
    
    //cv::drawContours(gray, contours, -1, cv::Scalar(0, 255, 255), cv::FILLED, 8, hierarchy );
    
    std::vector<cv::Point> numbers;
    for(int i = 0;i<contours.size();i++)
        for (int j = 0;j<contours[i].size();j++)
            numbers.push_back(contours[i][j]);

    
    cv::fitLine(numbers, line, CV_DIST_L1, 0,  0.01,  0.01);

    cv::line(gray, cv::Point(line[2],line[3]), cv::Point(line[2]+100*line[0],line[3]+100*line[1]), cv::Scalar(0,100,200));
    
    std::cout << "fitline result " << line << std::endl;
    
    double rho = std::sqrt(std::pow(line[3],2) + std::pow(line[2],2));
    double angle = std::atan2(line[3]/rho, line[2]/rho);
    
    
    //convert rad to deg
    return (angle) * 57.2958;
    
}

void Character_Recognition_Algorithm::prepare_uniform_window(cv::Mat &img){
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((2*2) + 1, (2*2)+1));
    
    cv::resize(img, img, cv::Size(200, 200)); // resize the ROI
   // cv::imshow("after resizing", img);
    
    //cv::adaptiveThreshold(img, img, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 0);
    cv::threshold( img, img, 80, 255, 1 ); // threshold and binarize the image, to suppress some noise
   // cv::imshow("after threshold", img);
    
    // Apply some additional smoothing and filtering
    cv::erode(img, img, kernel);
  //  cv::imshow("after erosion", img);
    cv::GaussianBlur(img, img, cv::Size(3, 3), 2, 2);
 //   cv::imshow("after blur", img);
    cv::erode(img, img, kernel);
 //   cv::imshow("after erosion2", img);
    cv::erode(img, img, kernel);
 //   cv::imshow("after erosion3", img);

    
//    cv::dilate(img, img, kernel);
//    cv::imshow("after dilation", img);
//    cv::erode(img, img, kernel);
//    cv::imshow("eroding", img);
}

void Character_Recognition_Algorithm::set_lower_bound_filter(double hue, double saturation, double value){
    if(hue > 180 || saturation > 255 || value > 255){
        std::cout << "wrong values set for lower bound filter hsv values" << std::endl;
        return;
    }
    if(hue < 0 || saturation < 0 || value < 0){
        std::cout << "wrong values set for lower bound filter hsv values" << std::endl;
        return;
    }
    this->filter.lb[0] = hue;
    this->filter.lb[1] = saturation;
    this->filter.lb[2] = value;
}

void Character_Recognition_Algorithm::set_upper_bound_filter(double hue, double saturation, double value){
    if(hue > 180 || saturation > 255 || value > 255){
        std::cout << "wrong values set for upper bound filter hsv values" << std::endl;
        return;
    }
    if(hue < 0 || saturation < 0 || value < 0){
        std::cout << "wrong values set for upper bound filter hsv values" << std::endl;
        return;
    }
    this->filter.ub[0] = hue;
    this->filter.ub[1] = saturation;
    this->filter.ub[2] = value;
}
