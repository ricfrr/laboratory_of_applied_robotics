//
//  Digit_Recognition.hpp
//  LAR_1_0
//
//  Created by Air Marvin on 09.10.18.
//  Copyright © 2018 Air Marvin. All rights reserved.
//

#ifndef Digit_Recognition_hpp
#define Digit_Recognition_hpp

#include <stdio.h>
#include <iostream>

//tesseract doesn't work when using namespace std
//delete every using namespace std line in the previous code to compile this code
#include "Optical_Character_Recognition.hpp"
#include "Template_Character_Recognition.hpp"
#include "People.hpp"

namespace ImageProcessing
{

/// \brief different algorithms for recognizing digits
enum DigitRecognitionAlgo {
    templateMatching,
    tesseractOCP
};

/**
 \brief The Digit_Recognition class is used to detect digits in the arena and export People objects.
 
 \discussion The Digit_Recognition class is used to detect digits in the arena and export People objects.
 The class contains a Character_Recognition_Algorithm member that is used to detect digits. Changing the DigitRecognitionAlgo type results in the use of another implementation of Character_Recognition_Algorithm derived classes.
 New types of Character_Recognition_Algorithm subclasses can be added to improve digit recognition performance over time.
 The class contains HSVFilterRange object that is able to automatically construct a color filter from a given input image.
 When detect_digit_for_map is called the class will identify circles, use the filter to extract the digits, perform the digit recognition and export a People object that can be used to create the map.
 \see People
 \see Map
 */
class Digit_Recognition {
    
public:
    
    /// \brief constructing the class with a set DigitRecognitionAlgo type
    /// \param algorithm a DigitRecognitionAlgo enum type that specifies the type of Character_Recognition_Algorithm used to perform the recognition of the digit
    /// \see Optical_Recognition_Algorithm, Template_Character_Recognition
    Digit_Recognition(DigitRecognitionAlgo algorithm);
    
    /**
     \brief constructing the class with a set DigitRecognitionAlgo type
     \param algorithm a DigitRecognitionAlgo enum type that specifies the type of Character_Recognition_Algorithm used to perform the recognition of the digit
     \param suff_confidence confidence level [0-100] the recognized digit is accepted
     \param search_angle region +/- detected angle the algorithm will try to find the digit
     \param d_angle delta_angle step when turning to find the digit
     \param extra_eroding number of consecutive eroding steps, importted for low quality images
     \see Optical_Recognition_Algorithm, Template_Character_Recognition */
    Digit_Recognition(const DigitRecognitionAlgo &algorithm = DigitRecognitionAlgo::tesseractOCP, const unsigned int &suff_confidence = 80, const unsigned int &search_angle = 15, const double &d_angle = 3.0, const unsigned int &extra_eroding = 0);
    
    /**
     \brief set detection algorithm for digits e.g. tesseract or template matching
     \param algorithm a DigitRecognitionAlgo enum type that specifies the type of Character_Recognition_Algorithm used to perform the recognition of the digit
     \param suff_confidence confidence level [0-100] the recognized digit is accepted
     \param search_angle region +/- detected angle the algorithm will try to find the digit
     \param d_angle delta_angle step when turning to find the digit
     \param extra_eroding number of consecutive eroding steps, importted for low quality images
     */
    void set_algo(const DigitRecognitionAlgo &algorithm, const unsigned int &suff_confidence = 80, const unsigned int &search_angle = 15, const double &d_angle = 3.0, const unsigned int &extra_eroding = 0);
    
    ///detects all the digits of an unprepared images and returns people information
    std::vector<LAR::People> detect_digits_for_map(const cv::Mat img_input);
    
    /// \brief sets a hsv filter for better image recognition results
    /// \param filterRange a HSVFilterRange object that automatically creates a filter based on an input image
    void set_filter(HSVFilterRange filterRange);
    
    private:
    
    /// \brief extracts the rect information of regions where the filter was applied
    /// \return a vector of cv::Rect objects containing information about location and size of the digits in tne arg image
    std::vector<cv::Rect> get_regions_of_interest(cv::Mat &img);
    
    ///the algorithm typed used to detect digits
    DigitRecognitionAlgo picked_algorithm;
    
    Character_Recognition_Algorithm * algorithm = nullptr;
    
    void initialize_algorithm();
    
    ///checks if the detected digit is between 1 and 4
    bool is_valid(int &detectedDigit);
    
public:
    /**
     \brief Helper object to find people in the map
     */
    struct PeopleStorage {
        
        PeopleStorage(const Mat &img){
            findCircles(img);
            
            this->algorithm = DigitRecognitionAlgo::tesseractOCP;
            this->suff_confidence = 80;
            this->search_angle = 15;
            this->d_angle = 3;
            this->extra_eroding = 0;
        }
        
        PeopleStorage(const DigitRecognitionAlgo &algorithm = DigitRecognitionAlgo::tesseractOCP, const unsigned int &suff_confidence = 80, const unsigned int &search_angle = 15, const double &d_angle = 3.0, const unsigned int &extra_eroding = 0){
            
            this->algorithm = algorithm;
            this->suff_confidence = suff_confidence;
            this->search_angle = search_angle;
            this->d_angle = d_angle;
            this->extra_eroding = extra_eroding;
        }
        
    private:
        
        DigitRecognitionAlgo algorithm;
        unsigned int suff_confidence;
        unsigned int search_angle;
        double d_angle;
        unsigned int extra_eroding;
        
        
    public:
        ///detected People
        std::vector<LAR::People> people;
        
        std::string filter_img = "../data/calib/filter_2.png";
        /*!
         * detect circles in the map
         * @param img image of the map
         */
        void findCircles(const Mat &img){
            
            Digit_Recognition dg_recognition = Digit_Recognition(algorithm,suff_confidence,search_angle,d_angle,extra_eroding);
            
            dg_recognition.algorithm->filter_img = filter_img;
            
            std::vector<LAR::People> data = dg_recognition.detect_digits_for_map(img);
            for (int i = 0; i < data.size(); i++)
            {
                std::string conf;
                
                if(data[i].confidence > 75)
                    conf = " with high confidence";
                else if (data[i].confidence > 50)
                    conf = " with medium confidence";
                else if (data[i].confidence > 25)
                    conf = " with low confidence";
                else
                    conf = " with extremely low confidence";
                
                
                std::cout << "detected a guy called " << data[i].name << " at <" << data[i].center.x << "," << data[i].center.y << "> with a radius of " << data[i].radius << conf << "\n"  <<std::endl;
                
                people.push_back(data[i]);
            }
            
            checkSuccess();
            
        }
        
        ///extract people information as Circle objects
        std::vector<Circle*> getCircles(){
            
            std::vector<Circle*> circles;
            
            for(int i=0; i<this->people.size(); i++){
                cv::Point center;
                center.x = people[i].center.x;
                center.y = people[i].center.y;
                Circle * circle_d = new Circle;
                circle_d->setCenter(center);
                circle_d->setRadius(people[i].radius);
                circles.push_back(circle_d);
            }
            
            return circles;
        }
        
        bool success = true;
        
        ///check wether all people objects where correctly identified
        void checkSuccess(){
            for(auto &&guy : people)
                if(guy.confidence == 0 || people.size() < 2){
                    success = false;
                    break;
                }
        }
        
    };
};


}

#endif /* Digit_Recognition_hpp */
