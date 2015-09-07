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
#include <stdio.h>
#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;


bool videoStream();
void signalHandler( int signum );
void addition();

#endif /* CAPTURE_H_ */
