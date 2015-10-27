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


int relativeError( int x1, int average){
	double err = 0;
	err = abs(average - x1);
	err = err * 100;
	err = err/average;
	int erro = (int)err;
	return erro;
}
/*************** Matvariance ************************
 Create Mats in gray scale depending on the variance
 of R G B values, thereunder function finds the rainbow
 *******************************************************/
void  Matvariance(){
	Mat img;
	vector <int>xList;
	vector <int>yList;

	img = imread( "Rainbow.png");

	namedWindow( "Variance Norm", WINDOW_AUTOSIZE );
	namedWindow( "Variance", WINDOW_AUTOSIZE );
	namedWindow( "RelErr", WINDOW_AUTOSIZE );
	//namedWindow( "Temlate", WINDOW_AUTOSIZE );

	if( !img.data )
	{
		printf("Error loading Rainbow.png \n");
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
	    	if( var >= 80 )
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
	  Mat dstn_relerr;
	  dstn.copyTo( dstn_relerr );

	int sumX =0;
	int sumY =0;
    for( size_t i = 0; i < xList.size(); i++ ){
    	sumX = sumX + xList[i];
    	sumY = sumY + yList[i];

    }

    int avX = sumX/xList.size();
    cout << "Average of X = " << avX << endl;

    int avY = sumY/xList.size();
    cout << "Average of Y = " << avY << endl;

    int relErrX[xList.size()];
    int relErrY[yList.size()];
    int rel= 0;


	ofstream res2file;
	res2file.open ("xlist.txt");
	for(size_t xitr=0; xitr != xList.size(); xitr++)
		{
		rel = relativeError( xList[xitr], avX);
		relErrX[xitr]= rel;
		res2file <<"X = " << xList[xitr] <<"  RelErr = " << rel << endl;

		}
	res2file.close();

	list<int>::iterator yitr;
	ofstream res1file;
	res1file.open ("ylist.txt");
	for(size_t yitr = 0; yitr != yList.size(); yitr++)
		{
		relErrY[yitr]= relativeError( yList[yitr], avY);
		res1file <<"Y = " <<  yList[yitr]<<"  RelErr = " << relErrY[yitr] << endl;

		}
	res1file.close();

	vector <int>badpixels;

	for(size_t n =0; n < yList.size(); n++){
		if(relErrY[n] > 25 || relErrX[n] > 65 ){

			dstn_relerr.at<uchar>(Point( xList[n], yList[n])) = 0;

		}
	}
/*
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
*/
	imshow( "Variance Norm", dstn );
	imshow( "Variance", dst );
	imshow( "RelErr", dstn_relerr );



	imwrite("VarianceNE.png", dstn_relerr);
	cout << "Saved VarianceNE.png"  << endl;

	imwrite("VarianceN.png", dstn);
	cout << "Saved VarianceN.png"  << endl;

	imwrite("Variance.png", dst);
	cout << "Saved Variance.png"  << endl;


	waitKey(0);

}

void cannydetect(){
	Mat img;
	Mat canny;
	vector <int>xWhitePixels;
	vector <int>yWhitePixels;

	img = imread( "VarianceNE.png");
	namedWindow( "Canny", WINDOW_AUTOSIZE );
	namedWindow( "Variance", WINDOW_AUTOSIZE );
	namedWindow( "RelErr", WINDOW_AUTOSIZE );
	//namedWindow( "Temlate", WINDOW_AUTOSIZE );

	if( !img.data )
	{
		printf("Error loading VarianceNE.png \n");
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
		    	if( color[0] >= 100 )
		    	{
		    		xWhitePixels.push_back(x);
		    		yWhitePixels.push_back(y);
		    	}
		    }
		}

		vector <int>HUpperX;
		vector <int>HupperY;

		for(size_t n =0; n < xWhitePixels.size(); n++){

		}



}


void matching(){

	Mat img;
	img = imread( "VarianceN.png");

	if( !img.data )
	{
		printf("Error loading VarianceN.png \n");
		exit(-1);
	}

	Mat templ1( 1, 9, img.type(), Scalar(255));
	Mat templ2( 9, 1, img.type(), Scalar(255));
	Mat res1;
	Mat res2;
	Mat res3;
	//cout << img.type()<< endl;

	namedWindow( "Result1", WINDOW_AUTOSIZE );
	namedWindow( "Result2", WINDOW_AUTOSIZE );

	/// Do the Matching and Normalize
	matchTemplate( img, templ1, res1, CV_TM_SQDIFF );
	normalize( res1, res1, 0, 255, NORM_MINMAX, -1, Mat());

	matchTemplate( img, templ2, res2, CV_TM_SQDIFF );
	normalize( res2, res2, 0, 255, NORM_MINMAX, -1, Mat());

	imwrite("Res1.png" ,res1);
	cout << "Saved " << "Res1.png"<< endl;
	imwrite("Res2.png" ,res2);
	cout << "Saved " << "Res2.png"<< endl;

	imshow("Result1", res1);
	imshow("Result2", res2);

	waitKey(0);
}


void compareMatching(){

	Mat res1 = imread( "Res1.png" );
	Mat res2 = imread( "Res2.png" );


	if( !res1.data )
	{
		printf("Error loading Res1.png \n");
		exit(-1);
	}

	if( !res2.data )
	{
		printf("Error loading Res2.png \n");
		exit(-1);
	}

	namedWindow( "Result3", WINDOW_AUTOSIZE );

    int cols = res1.cols;
    int rows = res2.rows;
	Mat res3(rows, cols,  CV_8UC1);


    	for(int y = 0; y < rows; y++)
    	{
    	    for(int x = 0; x < cols; x++)
    	    {
    	    	Vec3b color1 = res1.at<Vec3b>(Point(x,y));
    	    	Vec3b color2 = res2.at<Vec3b>(Point(x,y));

    	    	if(  color1[2] > 70 && color2[2] > 70)
    	    	{
    	    		res3.at<uchar>(Point(x,y)) = 255;
    	    	}
    	    	else
    	    	{
    	    		res3.at<uchar>(Point(x,y)) = 0;
    	    	}
    	    }
    	}

	imwrite("Res3.png" ,res3);
	cout << "Saved " << "Res3.png"<< endl;

	imshow("Result3", res3);

	waitKey(0);

}

void hough(){

	Mat img;
	img = imread( "VarianceNE.png");

	if( !img.data )
	{
		printf("Error loading Hough.png \n");
		exit(-1);
	}
	else
	{
		printf("Loading Hough.png \n");
	}
/*
	namedWindow( "Variance Norm", WINDOW_AUTOSIZE );
	namedWindow( "Variance", WINDOW_AUTOSIZE );
	namedWindow( "Finder", WINDOW_AUTOSIZE );

	static const char *output_file = "newconfig.cfg";
	Config cfg;

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
	    */
	    Mat  dst, cdst;

	    int cols = img.cols;
	    int rows = img.rows;

	    	Mat vdst(rows, cols,  CV_8UC1);
	    	Mat vdstn(rows, cols,  CV_8UC1);
	    	uchar var;
/*
	    	for(int y = 0; y < rows; y++)
	    	{
	    	    for(int x = 0; x < cols; x++)
	    	    {
	    	    	Vec3b color = D.at<Vec3b>(Point(x,y));
	    	    	var=varianceRGB(color[0] ,color[1], color[2]);
	    	    	vdst.at<uchar>(Point(x,y)) = var;
	    	    	if( var > 100)
	    	    	{
	    	    		vdstn.at<uchar>(Point(x,y)) = 255;

	    	    	}
	    	    	else
	    	    	{
	    	    		vdstn.at<uchar>(Point(x,y)) = 0;
	    	    	}
	    	    }
	    	}

*/
		    Canny(img, dst, 200, 200, 3);
		    cvtColor(dst, cdst, COLOR_GRAY2BGR);

		    vector<Vec2f> lines;
		    HoughLines(dst, lines, 1, CV_PI/180, 75, 0, 0 );

		    for( size_t i = 0; i < lines.size(); i++ )
		    {
		       float rho = lines[i][0], theta = lines[i][1];
		       //cout << "Rho: " << rho << endl;
		       //cout << "Theta: " << theta << endl;
		       Point pt1, pt2;
		       double a = cos(theta), b = sin(theta);
		       cout << "a = " << a << endl;
		       if(a<1){
		       double x0 = a*rho, y0 = b*rho;
		       pt1.x = cvRound(x0 + 1000*(-b));
		       pt1.y = cvRound(y0 + 1000*(a));
		       pt2.x = cvRound(x0 - 1000*(-b));
		       pt2.y = cvRound(y0 - 1000*(a));
		       line( dst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
		       }
		    }

		     imwrite("Slope.png" ,cdst);
		     cout << "Saved " << "Slope.png"<< endl;

		    // imwrite("D.png" ,D);
		    // cout << "Saved " << "D.png"<< endl;
/*
		     imwrite("Variancen.png" ,vdstn);
		     cout << "Saved " << "Variancen.png"<< endl;
		     imwrite("Variance.png" ,vdst);
		  	 cout << "Saved " << "Variance.png"<< endl;

		 	imshow( "Variance Norm", vdstn );
		 	imshow( "Variance", vdst );
		 	imshow( "Finder", dst );
*/
		    waitKey(0);

}








