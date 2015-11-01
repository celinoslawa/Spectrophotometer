/*
 * detect.h
 *
 *  Created on: 16-09-2015
 *      Author: agnieszka
 */

#ifndef DETECT_H_
#define DETECT_H_

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <libconfig.h++>
#include <list>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <iomanip>
#include <fstream>

	    extern int xp1;
	    extern int yp1;
	    extern int xp2;
        extern int yp2;
	 	using namespace cv;
	 	using namespace std;
	 	using namespace libconfig;
	  	void maching();
	  	int max_value(vector<int>Vect);
	  	int min_value(vector<int>Vect);
	  	uchar varianceRGB(uchar b, uchar g, uchar r);
	  	void createcfgFile(float x1, float y1, float x2, float y2,float x3, float y3, float x4, float y4);
	  	int average(list<int>::iterator first, list<int>::iterator last);
	  	int relativeError( int x1, int average);
	  	void  Matvariance();
	  	void filllingWhite();
	  	void canny();
	  	void lineDetect();
	  	void slopedetect();
	  	void hough();
	  	void matching();
	  	void compareMatching();



#endif /* DETECT_H_ */

