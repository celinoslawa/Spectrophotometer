/*
 * capture.h
 *
 *  Created on: 24-08-2015
 *      Author: agnieszka
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/videoio.hpp"

#include <signal.h>
#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;


void videoStream(int cam, string name);
void signalHandler( int signum );


#endif /* CAPTURE_H_ */
