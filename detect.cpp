/*
 * detect.cpp
 *
 *  Created on: 16-09-2015
 *      Author: agnieszka
 */

//#include "detect.h"
// Global Variables

#include "detect.h"


/*************** varianceRGB ************************
 return variance of red, green and blue values
 ****************************************************/
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


/*************** max_value ************************
 Returns iterator from list with max value
 **************************************************/
list<int>::const_iterator max_value(list<int>::iterator first, list<int>::iterator last){
	list<int>::const_iterator max = first;
	for(list<int>::const_iterator p = first; p!=last; ++p){
		if(*max < *p){
			max=p;
		}
	}
	return max;
}


/*************** min_value ************************
 Returns iterator from list with min value
***************************************************/
list<int>::const_iterator min_value(list<int>::iterator first, list<int>::iterator last){
	list<int>::const_iterator min = first;
	for(list<int>::const_iterator p = first; p!=last; ++p){
		if(*min > *p){
			min=p;
		}
	}
	return min;
}


/*************** createcfgFile ************************
 Creates config file with rainbow position
*******************************************************/
void createcfgFile(int x1, int y1, int x2, int y2){
	static const char *output_file = "newconfig.cfg";
	Config cfg;
	Setting &root = cfg.getRoot();

	// Add some settings to the configuration.
	Setting &position = root.add("position", Setting::TypeGroup);

	position.add("x1", Setting::TypeInt) = x1;
	position.add("y1", Setting::TypeInt) = y1;
	position.add("x2", Setting::TypeInt) = x2;
	position.add("y2", Setting::TypeInt) = y2;

	// Write out the new configuration.
	  try
	  {
	    cfg.writeFile(output_file);
	    cerr << "New configuration successfully written to: " << output_file
	         << endl;

	  }
	  catch(const FileIOException &fioex)
	  {
	    cerr << "I/O error while writing file: " << output_file << endl;
	    exit(-1);
	  }

}


/*************** Matvariance ************************
 Create Mats in gray scale depending on the variance
 of R G B values, thereunder function finds the rainbow
 *******************************************************/
void  Matvariance(){
	Mat img;
	list <int>xList;
	list <int>yList;
	img = imread( "Rainbow.jpg");

	namedWindow( "Variance Norm", WINDOW_AUTOSIZE );
	namedWindow( "Variance", WINDOW_AUTOSIZE );
	namedWindow( "Finder", WINDOW_AUTOSIZE );

	if( !img.data )
	{
		printf("Error loading Rainbow.jpg \n");
		exit(-1);
	}

	int cols = img.cols;
	int rows = img.rows;

	Mat dst(rows, cols,  CV_8UC1);
	Mat dstn(rows, cols,  CV_8UC1);
	uchar var;

	for(int y = 0; y < rows; y++)
	{
	    for(int x = 0; x < cols; x++)
	    {
	    	Vec3b color = img.at<Vec3b>(Point(x,y));
	    	var=varianceRGB(color[0] ,color[1], color[2]);
	    	dst.at<uchar>(Point(x,y)) = var;
	    	if( var > 140 )
	    	{
	    		dstn.at<uchar>(Point(x,y)) = 255;
	    		xList.push_back(x);
	    		yList.push_back(y);
	    	}
	    	else
	    	{
	    		dstn.at<uchar>(Point(x,y)) = 0;
	    	}
	    }
	}


	list<int>::const_iterator x1 = min_value(xList.begin(), xList.end());
	list<int>::const_iterator y1 = min_value(yList.begin(), yList.end());
	list<int>::const_iterator x2 = max_value(xList.begin(), xList.end());
	list<int>::const_iterator y2 = max_value(yList.begin(), yList.end());

	Point location1;
	Point location2;
	location1.x = *x1;
	location1.y = *y1;
	location2.x = *x2;
	location2.y = *y2;

	createcfgFile(*x1 ,*y1, *x2, *y2);

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

void slopedetect(){

	Mat img;
	img = imread( "hough1.jpg");
	static const char *output_file = "newconfig.cfg";
	Config cfg;
	namedWindow( "Src", WINDOW_AUTOSIZE );
	namedWindow( "Slope", WINDOW_AUTOSIZE );


	if( !img.data )
	{
		printf("Error loading Rainbow.jpg \n");
		exit(-1);
	}

	  try
	  {
	    cfg.readFile("newconfig.cfg");
	  }
	  catch(const FileIOException &fioex)
	  {
	    cerr << "I/O error while reading file." << std::endl;
	    exit(-1);
	  }
	  catch(const ParseException &pex)
	  {
	    cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
	              << " - " << pex.getError() << std::endl;
	    exit(-1);
	  }

	  Setting &root = cfg.getRoot();
	  int x1, y1, x2, y2;

	  try
	    {
	      const Setting &position = root["position"];



	        if(!(position.lookupValue("x1", x1)
	             && position.lookupValue("y1", y1)
	             && position.lookupValue("x2", x2)
	             && position.lookupValue("y2", y2))){

	        cout << "X1 " << x1 << endl;
	        cout << "Y1 " << y1 << endl;
	        cout << "X2 " << x1 << endl;
	        cout << "Y2 " << x1 << endl;
	        }

	      cout << endl;
	    }
	    catch(const SettingNotFoundException &nfex)
	    {
	      // Ignore.
	    }

	    Point location1;
	    Point location2;
	    location1.x = x1;
	    location1.y = y1;
	    location2.x = x2;
	    location2.y = y2;

	    Mat D (img , Rect(location1 , location2) );
	    Mat dst, cdst;
	    Canny(D, dst, 50, 200, 3);
	    cvtColor(dst, cdst, COLOR_GRAY2BGR);

	    vector<Vec4i> lines;
	     HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
	     for( size_t i = 0; i < lines.size(); i++ )
	     {
	       Vec4i l = lines[i];
	       line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
	     }

	     imwrite("Slope.jpg" ,cdst);
	     cout << "Saved " << "Slope.jpg"<< endl;
	    imshow("Src", img);
	    imshow("Slope", cdst);


	    waitKey();








}
