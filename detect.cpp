/*
 * detect.cpp
 *
 *  Created on: 16-09-2015
 *      Author: agnieszka
 */

//#include "detect.h"
// Global Variables

#include "detect.h"

 int xp1;
 int yp1;
 int xp2;
 int yp2;

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
 Returns max value from vector
 **************************************************/
int max_value(vector<int> Vect){
	int max = Vect[0];
	for(size_t i = 0; i < Vect.size(); i++ ){
		if(max < Vect[i]){
			max=Vect[i];
		}
	}
	return max;
}


/*************** min_value ************************
 Returns min value from vector
***************************************************/
int min_value(vector<int>Vect){
	int min = Vect[0];
	for(size_t i = 0; i < Vect.size(); i++ ){
		if(min > Vect[i]){
			min=Vect[i];
		}
	}
	return min;
}


/*************** createcfgFile ************************
 Creates config file with rainbow position
 x1, y1 - top left point
 x2, y2 - top right point
 x3, y3 - lower right point
 x4, y4 - lower left point
*******************************************************/
void createcfgFile(float x1, float y1, float x2, float y2,float x3, float y3, float x4, float y4){
	static const char *output_file = "newconfig.cfg";
	Config cfg;
	Setting &root = cfg.getRoot();

	// Add some settings to the configuration.
	Setting &position = root.add("position", Setting::TypeGroup);

	position.add("x1", Setting::TypeFloat) = x1;
	position.add("y1", Setting::TypeFloat) = y1;
	position.add("x2", Setting::TypeFloat) = x2;
	position.add("y2", Setting::TypeFloat) = y2;
	position.add("x3", Setting::TypeFloat) = x3;
	position.add("y3", Setting::TypeFloat) = y3;
	position.add("x4", Setting::TypeFloat) = x4;
	position.add("y4", Setting::TypeFloat) = y4;

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
   // cout << "Average of X = " << avX << endl;

    int avY = sumY/xList.size();
 // cout << "Average of Y = " << avY << endl;

    int relErrX[xList.size()];
    int relErrY[yList.size()];
    int rel= 0;

	for(size_t xitr=0; xitr != xList.size(); xitr++)
		{
		rel = relativeError( xList[xitr], avX);
		relErrX[xitr]= rel;
		}

	for(size_t yitr = 0; yitr != yList.size(); yitr++)
		{
		relErrY[yitr]= relativeError( yList[yitr], avY);
		}
	vector<int> xVal;
	vector<int> yVal;
	for(size_t n =0; n < yList.size(); n++){
		if(relErrY[n] > 25 || relErrX[n] > 65 ){

			dstn_relerr.at<uchar>(Point( xList[n], yList[n])) = 0;
			//cout<< "xList  " << xList[n] << endl;

		}else {
			xVal.push_back(xList[n]);
			yVal.push_back(yList[n]);

		}
	}
	 xp1 = min_value(xVal);
	 yp1 = max_value(yVal);
	 xp2 = max_value(xVal);
	 yp2 = min_value(yVal);
	//createcfgFile(x1 ,y1, x2, y2);
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

/*************** fillingWhite ************************
filling place white color
 *******************************************************/

void filllingWhite(){
	Mat img;

	img = imread( "VarianceNE.png");

	if( !img.data )
	{
		printf("Error loading VarianceNE.png \n");
		exit(-1);
	}
	cout<< "Img type " << img.type() << endl;
	Mat templ1( 4,4, img.type(), Scalar(255));
	//cout<< templ1 << endl;
	Mat res1;

	namedWindow( "Result1", WINDOW_AUTOSIZE );
	//namedWindow( "Result2", WINDOW_AUTOSIZE );

	/// Do the Matching and Normalize
	matchTemplate( img, templ1, res1, CV_TM_SQDIFF );
	normalize( res1, res1, 0, 255, NORM_MINMAX, -1, Mat());


	imwrite("Res1.png" ,res1);
	cout << "Saved " << "Res1.png"<< endl;

	imshow("Result1", res1);

	waitKey(0);
}

/****************** canny*********************************
creates more detected canny
 *******************************************************/
void canny(){
	Mat img;
	img = imread("Res1.png",CV_LOAD_IMAGE_GRAYSCALE);
	if( !img.data )
	{
		printf("Error loading Res1.png \n");
		exit(-1);
	}
	cout<< "img type" << img.type() << endl;

	namedWindow( "Canny", WINDOW_AUTOSIZE );
    int cols = img.cols;
    int rows = img.rows;
	Mat res3(rows, cols, CV_8UC1);


    	for(int y = 0; y < rows; y++)
    	{
    	    for(int x = 0; x < cols; x++)
    	    {
    	    	uchar color2 = img.at<uchar>(Point(x,y));

    	    	if(  color2 > 1)
    	    	{
    	    		res3.at<uchar>(Point(x,y)) = 255;
    	    	}
    	    	else
    	    	{
    	    		res3.at<uchar>(Point(x,y)) = 0;
    	    	}
    	    }
    	}

    	Mat dst(cols, rows,CV_8UC1 );
    	Mat cdst;
    	 Canny(res3, dst, 50, 200, 3);

    	 cvtColor(dst, cdst, COLOR_GRAY2BGR);

    	 vector<Vec4i> lines;
    	 HoughLinesP(dst, lines, 1, CV_PI/180, 15, 15, 15 );
		for( size_t i = 0; i < lines.size(); i++ )
		{
		  Vec4i l = lines[i];
		  line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 3);//CV_AA
		  //cout<< "PT1 = "<< l[0] << " " << l[1]<< endl;
		  //cout<<"Pt2 = "<< l[2]<< "  "<<l[3] <<endl;
		}


    	  imwrite("Canny.png" ,cdst);
    	  cout << "Saved " << "Canny.png"<< endl;
    	  imshow("Canny", cdst);
    	  waitKey(0);
}

/****************** lineDetect*********************************
Detect lines and calculate corners of the rectangle
 *******************************************************/

void lineDetect(){
	Mat img;
	img = imread("Canny.png",CV_LOAD_IMAGE_GRAYSCALE);
		if( !img.data )
		{
			printf("Error loading Canny.png \n");
			exit(-1);
		}
		namedWindow( "Line detect", WINDOW_AUTOSIZE );
	    int cols = img.cols;
	    int rows = img.rows;
		Mat dst, cdst;
		Mat crosPoints( cols, rows,CV_8UC1 , Scalar(0));
		Canny(img, dst, 50, 200, 3);
		cvtColor(dst, cdst, COLOR_GRAY2BGR);
		vector<Vec2f> lines;
	  	vector<Point2f> Vlines1; //pionowe linie
	  	vector<Point2f> Vlines2; //pionowe linie
	  	vector<Point2f> Hlines1; // poziome linie
	  	vector<Point2f> Hlines2; // poziome linie
		HoughLines(dst, lines, 1, CV_PI/180, 27 , 4,4 );//65,65,10

		  for( size_t i = 0; i < lines.size(); i++ )
		  {
		     float rho = lines[i][0], theta = lines[i][1];
		     Point2f pt1, pt2;
		     double a = cos(theta), b = sin(theta);
		     //cout<< "a = " << a << "  b = " << b << endl;
		     double x0 = a*rho, y0 = b*rho;
		     pt1.x = cvRound(x0 + 1000*(-b));
		     pt1.y = cvRound(y0 + 1000*(a));
		     pt2.x = cvRound(x0 - 1000*(-b));
		     pt2.y = cvRound(y0 - 1000*(a));
		     if(a > 0.8888 || a < (-0.8888) ){
		    	 line( cdst, pt1, pt2, Scalar(0,255,0), 1, LINE_AA);
		    	 Vlines1.push_back(pt1);
		    	 Vlines2.push_back(pt2);
		     }else if(b > 0.999 || b < (-0.999) ){
			     line( cdst, pt1, pt2, Scalar(0,0,255), 1, LINE_AA);
		    	 Hlines1.push_back(pt1);
		    	 Hlines2.push_back(pt2);
		     }
		  }


		  vector<Point2f> Cpoints;
		  Point2f x, d1, d2, r;
		  for(size_t n = 0; n < Vlines1.size(); n++){
			  for(size_t m = 0; m < Hlines1.size(); m++){
				  x = Hlines1[m]-Vlines1[n];
				  d1 = Vlines2[n] - Vlines1[n];
				  d2 = Hlines2[m] - Hlines1[m];

				  float cross = d1.x*d2.y - d1.y*d2.x;
				  double t1 = (x.x * d2.y - x.y * d2.x)/cross;

				  r = Vlines1[n] + d1 * t1;
				  Cpoints.push_back(r);
			  }
		  }
vector<int>xVal;
vector<int>yVal;
Point2f Pt;
Point2f Pt1;
Point2f Pt2;
Point2f Pt3;
Point2f Pt4;

//cout<< "xp1 = " << xp1 << endl;
//cout<< "xp2 = " << xp2 << endl;
//cout<< "yp1 = " << yp1 << endl;
//cout<< "yp2 = " << yp2 << endl;


float avrx = (xp1+xp2)/2;
cout<< "avrX = " << avrx<< endl;
float avry = (yp1+yp2)/2;
cout<<"avrY = " << avry<<endl;

for(size_t j = 0; j < Cpoints.size(); j++){
	  Pt = Cpoints[j];


	  if(Pt.x < avrx && Pt.y < avry){ //lewy gorny
			  Pt1 = Pt;
	  }
	  if(Pt.x > avrx && Pt.y < avry){ // prawy gorny
			  Pt2= Pt;
	  }

	  if(Pt.x < avrx && Pt.y > avry){ //lewy dolny
			  Pt4 = Pt;
	  }

	  if(Pt.x > avrx && Pt.y > avry){ //prawy dolny
			  Pt3 = Pt;

	  }

}
//ofstream myfile;
//myfile.open ("points.txt");
		  for(size_t j = 0; j < Cpoints.size(); j++){
			  Pt = Cpoints[j];
			  if(Pt.x < avrx && Pt.y < avry){ //lewy gorny
				  //myfile<< "lewy gorny =" << Pt << endl;
				  if(Pt.x > Pt1.x ){
					  Pt1.x = Pt  .x;

				  }
				  if(Pt.y > Pt1.y){
					  Pt1.y=Pt.y;
				  }
			  }
			  if(Pt.x > avrx && Pt.y < avry){ // prawy gorny
				  //myfile<< "prawy gorny = " << Pt << endl;
				  if(Pt.x < Pt2.x){
					  Pt2.x= Pt.x;
				  }
				  if(Pt.y > Pt2.y){
					  Pt2.y= Pt.y;
				  }
			  }

			  if(Pt.x < avrx && Pt.y > avry){ //lewy dolny
				 // myfile<<" lewy dolny = "<< Pt <<endl;
				  if(Pt.x > Pt4.x){
					  Pt4.x = Pt.x;
				  }
				  if(Pt.y > Pt4.y){
					  Pt4.y = Pt.y;
				  }
			  }

			  if(Pt.x > avrx && Pt.y > avry){ //prawy dolny
				//  myfile << "prawy dolny = " << Pt <<endl;
				  if(Pt.x < Pt3.x ){
					  Pt3.x = Pt.x;
				  }
				  if(Pt.y < Pt3.y){
					  Pt3.y = Pt.y;

				  }
			  }

		  }
		 // myfile.close();

		  createcfgFile( Pt1.x, Pt1.y, Pt2.x, Pt2.y, Pt3.x, Pt3.y, Pt4.x, Pt4.y);
		  /*
		  cout<< "P1 = " << Pt1 << endl;
		  cout<< "P2 = " << Pt2 << endl;
		  cout<< "P3 = " << Pt3 << endl;
		  cout<< "P4 = " << Pt4 << endl;
	      */

		  imshow("Line detect",cdst);
		  imwrite("Lines.png" ,cdst);
		   cout << "Saved " << "Lines.png"<< endl;

		  waitKey(0);
}


/****************** transformation *********************************
read corners of the rectangle from cfg file then extends it
 *******************************************************/
void transformation(){
    Mat img;
    img = imread("Rainbow.png");
    if( !img.data )
    {
    	printf("Error loading Rainbow.png \n");
    	exit(-1);
    }
    namedWindow( "Transformation", WINDOW_AUTOSIZE );
    namedWindow( "Rainbow", WINDOW_AUTOSIZE );

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
	const Setting &root = cfg.getRoot();
	float x1, y1, x2, y2, x3, y3, x4, y4;
	try
	{
		const Setting &position = root["position"];

		if(!(position.lookupValue("x1", x1)
				&& position.lookupValue("y1", y1)
				&& position.lookupValue("x2", x2)
				&& position.lookupValue("y2", y2)
		        && position.lookupValue("x3", x3)
		        && position.lookupValue("y3", y3)
		        && position.lookupValue("x4", x4)
		        && position.lookupValue("y4", y4)))
			cout<<"brak danych w pliku cfg"<<endl;

		/*
		cout << "X1 " << x1 << endl;
		 cout << "Y1 " << y1 << endl;
		 cout << "X2 " << x2 << endl;
		 cout << "Y2 " << y2 << endl;
		 cout << "X3 " << x3 << endl;
		 cout << "Y3 " << y3 << endl;
		 cout << "X4 " << x4 << endl;
		 cout << "Y4 " << y4 << endl;
		 */

		      cout << endl;
		    }
		    catch(const SettingNotFoundException &nfex)
		    {
		      // Ignore.
		    }
		    Point2f Pt1;
		    Point2f Pt2;
		    Point2f Pt3;
		    Point2f Pt4;

		    Pt1.x = x1;
		    Pt1.y = y1;
		    Pt2.x = x2;
		    Pt2.y = y2;
		    Pt3.x = x3;
		    Pt3.y = y3;
		    Pt4.x = x4;
		    Pt4.y = y4;
		    cout<< "Pt1" <<Pt1 <<endl;
		    cout<< "Pt2" <<Pt2 <<endl;
		    cout<< "Pt3" <<Pt3 <<endl;
		    cout<< "Pt4" <<Pt4 <<endl;
            Mat dst(300,600, img.type());
            Mat transform_matrix;
            Point2f source_points[4];
            Point2f dest_points[4];

            source_points[0] = Pt1;
            source_points[1] = Pt2;
            source_points[2] = Pt3;
            source_points[3] =Pt4;

            Point2f lu_result(0,0);
            Point2f ru_result(600, 0);
            Point2f rd_result(600, 300);
            Point2f ld_result(0, 300);
            dest_points[0] = lu_result;
            dest_points[1] = ru_result;
            dest_points[2] = rd_result;
            dest_points[3] = ld_result;


            transform_matrix = getPerspectiveTransform(source_points, dest_points);
            warpPerspective(img, dst, transform_matrix,dst.size());
            //cout<< "TM = " << transform_matrix << endl;
            imshow("Transformation", dst);
            imwrite("transformation.png", dst);
            imshow("Rainbow", img);

            waitKey(0);

}






