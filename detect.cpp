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


//return variance of red, green and blue values
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

//Returns iterator from list with max value
list<int>::const_iterator max_value(list<int>::iterator first, list<int>::iterator last){
	list<int>::const_iterator max = first;
	for(list<int>::const_iterator p = first; p!=last; ++p){
		if(*max < *p){
			max=p;
		}
	}
	return max;
}

//Returns iterator from list with min value
list<int>::const_iterator min_value(list<int>::iterator first, list<int>::iterator last){
	list<int>::const_iterator min = first;
	for(list<int>::const_iterator p = first; p!=last; ++p){
		if(*min > *p){
			min=p;
		}
	}
	return min;
}

//creae Mats in gray scale depending on the variance of R G B values,
//thereunder function finds the rainbow
void  Matvariance(){
	Mat img;
	list <int>xList;
	list <int>yList;

	namedWindow( "Variance Norm", WINDOW_AUTOSIZE );
	namedWindow( "Variance", WINDOW_AUTOSIZE );
	namedWindow( "Finder", WINDOW_AUTOSIZE );

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
	    		xList.push_back(x);
	    		yList.push_back(y);
	    		//cout << " POINT" << endl << " " << Point(x,y) << endl << endl;
	    	}
	    	else{
	    		dstn.at<uchar>(Point(x,y)) = 0;

	    	}


	    }
	}


	Point location1;
	Point location2;
	list<int>::const_iterator x1 = min_value(xList.begin(), xList.end());
	list<int>::const_iterator y1 = min_value(yList.begin(), yList.end());
	list<int>::const_iterator x2 = max_value(xList.begin(), xList.end());
	list<int>::const_iterator y2 = max_value(yList.begin(), yList.end());
	location1.x = *x1;
	location1.y = *y1;
	location2.x = *x2;
	location2.y = *y2;

	cout << " POINT location1" << endl << " " << location1 << endl << endl;
	cout << " POINT location2" << endl << " " << location2 << endl << endl;


	rectangle(dstn ,location1 , location2 , Scalar( 0, 0, 255   ), 2, 8, 0 );
	Mat D (img , Rect(location1 , location2) );

	imshow( "Variance Norm", dstn );
	imshow( "Variance", dst );
	imshow( "Finder", D );

	imwrite("VarianceN.jpg", dstn);
	cout << "Saved VarianceN.jpg"  << endl;

	imwrite("Variance.jpg", dst);
	cout << "Saved Variance.jpg"  << endl;

	waitKey(0);


}

