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
	Mat D (img_display , Rect(matchLoc.x, matchLoc.y, templ.cols, templ.rows ) );
	imshow( "Finder" , img_display );

	cout << "D = " << endl << " " << D << endl << endl;
	imshow( "Calibration" , D );

	waitKey(0);
}



uchar varianceRGB(uchar b, uchar g, uchar r){
	uchar mean;
	uchar variance;
	uchar componentB;
	uchar componentG;
	uchar componentR;
	mean = (b + g + r )/3;
	componentB = ( b - mean ) * ( b - mean );
	componentG = ( g - mean ) * ( g - mean );
	componentR = ( r - mean ) * ( r - mean );
	variance =(componentB + componentG + componentR ) / 3;
	return variance;
}

void  Matvariance(){
	Mat img;


	namedWindow( "Variance Norm", WINDOW_AUTOSIZE );
	namedWindow( "Variance", WINDOW_AUTOSIZE );
	img = imread( "Rainbow.jpg");




	int cols = img.cols;
	int rows = img.rows;


	Mat dst(rows, cols,  CV_8UC1);
	Mat dstn(rows, cols,  CV_8UC1);
	uchar var;
	for(int y = 0; y < rows; y++)
	{
	    for(int x = 0; x < cols; x++){
	    	Vec3b color = img.at<Vec3b>(Point(x,y));
	    	var=varianceRGB(color[0] ,color[1], color[2]);
	    	dst.at<uchar>(Point(x,y)) = var;
	    	if( var > 140 ){
	    		dstn.at<uchar>(Point(x,y)) = 255;
	    	}
	    	else{
	    		dstn.at<uchar>(Point(x,y)) = 0;
	    	}


	    }
	}

	imwrite("VarianceN.jpg", dstn);
	cout << "Saved VarianceN.jpg"  << endl;

	imwrite("Variance.jpg", dst);
	cout << "Saved Variance.jpg"  << endl;

	imshow( "Variance Norm", dstn );
	imshow( "Variance", dst );
	//cout << "Dst = " << endl << " " << dst << endl << endl;

	waitKey(0);


}

