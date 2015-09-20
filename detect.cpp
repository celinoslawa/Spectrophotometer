/*
 * detect.cpp
 *
 *  Created on: 16-09-2015
 *      Author: agnieszka
 */

//#include "detect.h"
// Global Variables

#include "detect.h"




void maching( ){


	Mat img;
	Mat gray_image;
	Mat templ;
	Mat result;

	  // Load image and template
	img = imread( "Rainbow.jpg");
	templ = imread( "temp2.jpg" );

	namedWindow( "Calibration", WINDOW_AUTOSIZE );
	namedWindow( "Finder", WINDOW_AUTOSIZE );

  	Mat img_display;
	img.copyTo( img_display );
	cvtColor( img_display, gray_image, COLOR_BGR2GRAY );
	int result_cols =  img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create( result_rows, result_cols, img.type() ); // 32F
	matchTemplate( img, templ, result, CV_TM_CCOEFF );
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc);
	matchLoc = maxLoc;


	rectangle(img_display , matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar( 0, 0, 255   ), 2, 8, 0 );
	Mat D (gray_image, Rect(matchLoc.x, matchLoc.y, templ.cols, templ.rows ) );
	imshow( "Finder" , img_display );

	cout << "D = " << endl << " " << D << endl << endl;
	imshow( "Calibration" , D );

	waitKey(0);
}


