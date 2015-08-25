/*
 * capture.cpp
 *
 *  Created on: 24-08-2015
 *      Author: agnieszka
 */
#include "capture.h"

volatile sig_atomic_t flag = 0;

void signalHandler( int signum){
    cout << "Interrupt signal (" << signum << ") received.\n";
    flag = 1; // set flag

}

void videoStream(int cam,  string name){

	VideoCapture stream1(cam);   //0 is the id of video device.0 if you have only one camera.

	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
		exit(-1);
			//return 0;
	}

	double FPS = 10.0;
	double dWidth = stream1.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = stream1.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

		   cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    VideoWriter out("output.mov", CV_FOURCC('m','p', '4', 'v'), FPS, Size(dWidth, dHeight));

	if(!out.isOpened()) {
		cout <<"Error! Unable to open video file for output." << std::endl;
		exit(-1);
	}

	namedWindow("cam", CV_WINDOW_AUTOSIZE);

	cvStartWindowThread(); //enable to close window
	signal(SIGINT, signalHandler);

	//unconditional loop
	while (true) {
		Mat frame;
			//stream1 >> frame;
		bool bSuccess = stream1.read(frame); // read a new frame from video

			if (!bSuccess) {
				cout << "Cannot read a frame from video stream" << endl;
		        break;
		    }

			  out << frame;

			if(!frame.empty()){
				imshow("cam", frame);
			}

			if (waitKey(30) >= 27){
				break;
			}

			if(flag==1){
				flag=0;
				break;
			}
		}

}
