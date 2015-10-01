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
#include <list>
#include <algorithm>
#include <iostream>
#include <stdio.h>

	 	using namespace cv;
	 	using namespace std;
	  	void maching();
	  	list<int>::const_iterator max_value(list<int>::iterator first,list<int>::iterator last);
	  	list<int>::const_iterator min_value(list<int>::iterator first, list<int>::iterator last);
	  	uchar varianceRGB(uchar b, uchar g, uchar r);
	  	void  Matvariance();


#endif /* DETECT_H_ */

