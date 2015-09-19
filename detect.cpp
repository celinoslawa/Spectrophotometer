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
	//char *argv[3]={0, "Rainbow.jpg","/Obrazy/templates/temp2.jpg"};

	Mat img;
	Mat templ;
	Mat result;

	  // Load image and template
	  img = imread( "Rainbow.jpg", 1 );
	  templ = imread( "temp2.jpg", 1 );

	  	namedWindow( "Calibration", WINDOW_AUTOSIZE );
	  	namedWindow( "Finder", WINDOW_AUTOSIZE );

	  	Mat img_display;
	  	img.copyTo( img_display );

	  	int result_cols =  img.cols - templ.cols + 1;
	  	int result_rows = img.rows - templ.rows + 1;

	  	result.create( result_rows, result_cols, img.type() ); // 32F
	  	 // cout >>  dst.type();
	  	matchTemplate( img, templ, result, CV_TM_CCOEFF );
	  	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	      double minVal; double maxVal; Point minLoc; Point maxLoc;
	  	Point matchLoc;

	  	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc);

	  	matchLoc = maxLoc;

	  	rectangle( img , matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar( 0, 0, 255   ), 2, 8, 0 );
	  	imshow( "Finder" , img );
	  	imshow( "Calibration" , result );
	  	waitKey(0);
}

