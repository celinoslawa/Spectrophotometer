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

	 	using namespace cv;
	 	using namespace std;
	 	using namespace libconfig;
	  	void maching();
	  	list<int>::const_iterator max_value(list<int>::iterator first,list<int>::iterator last);
	  	list<int>::const_iterator min_value(list<int>::iterator first, list<int>::iterator last);
	  	uchar varianceRGB(uchar b, uchar g, uchar r);
	  	void createcfgFile(int x1, int y1, int x2, int y2);
	  	void  Matvariance();
	  	void slopedetect();


#endif /* DETECT_H_ */

