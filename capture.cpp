/*
 * capture.cpp
 *
 *  Created on: 24-08-2015
 *      Author: agnieszka
 */
#include "capture.h"

volatile sig_atomic_t flag = 0;

void signalHandler( int signum){
    flag = 1; // set flag

}

bool videoStream(){

	VideoCapture stream1(1);   //0 is the id of video device.0 if you have only one camera.

	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
		exit(-1);
		return false;
	}

	//double FPS = 10.0;
	double dWidth = stream1.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = stream1.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
		   cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("cam", CV_WINDOW_AUTOSIZE);

	cvStartWindowThread(); //enable to close window
	signal(SIGINT, signalHandler); //catch Ctrl+C

	int n = 0;
    char filename[200];

	//unconditional loop
	while (true) {
			Mat frame; // get a new frame from camera

            //stream1 >> frame;
    		bool bSuccess = stream1.read(frame); // read a new frame from video

			if (!bSuccess) {
				cout << "Cannot read a frame from video stream" << endl;
		        break;
		        return false;
		    }


			if(!frame.empty()){
				imshow("cam", frame);
			}

            char key = (char)waitKey(30); //delay N millis, usually long enough to display and capture input

            switch (key) {
                        case 'q':
                        case 'Q':
                        case 27: //escape key
                        	return false;
                        case ' ': //Save an image
                            sprintf(filename,"filename%.3d.jpg",n++);
                            imwrite(filename,frame);
                            cout << "Saved " << filename << endl;
                            if(n==10){
                            	return true;
                            }
                            break;
                        default:
                            break;
                        }

			if(flag==1){
				flag=0;
				return false;
			}
		}
	return true;
}

void addition(){

	double alpha = 0.5;
	double beta;
	Mat src1, src2, src3, src4, src5, src6, src7, src8, src9, src10, dst;

	src1 = imread("filename000.jpg");
	src2 = imread("filename001.jpg");
	src3 = imread("filename002.jpg");
	src4 = imread("filename003.jpg");
	src5 = imread("filename004.jpg");
	src6 = imread("filename005.jpg");
	src7 = imread("filename006.jpg");
	src8 = imread("filename007.jpg");
	src9 = imread("filename008.jpg");
	src10 = imread("filename009.jpg");


	if( !src1.data ) {
		printf("Error loading filename000.jpg \n");
		exit(-1);
	}
	if( !src2.data ) {
		printf("Error loading filename001.jpg \n");
		exit(-1);
	}


	if( !src3.data ) {
		printf("Error loading filename002.jpg \n");
		exit(-1);
	}

	if( !src4.data ) {
		printf("Error loading filename003.jpg \n");
		exit(-1);
	}

	if( !src5.data ) {
		printf("Error loading filename004.jpg \n");
		exit(-1);
	}
	if( !src6.data ) {
		printf("Error loading filename005.jpg \n");
		exit(-1);
	}

	if( !src7.data ) {
		printf("Error loading filename006.jpg \n");
		exit(-1);
	}
	if( !src8.data ) {
		printf("Error loading filename007.jpg \n");
		exit(-1);
	}

	if( !src9.data ) {
		printf("Error loading filename008.jpg\n");
		exit(-1);
	}
	if( !src10.data ) {
		printf("Error loading filename009.jpg \n");
		exit(-1);
	}
	 /// Create Windows
	 namedWindow("Rainbow", 1);
	 beta = ( 1.0 - alpha );

	 addWeighted( src1, alpha, src2, beta, 0.0, dst);
	 addWeighted( dst, alpha, src3, beta, 0.0, dst);
	 addWeighted( dst, alpha, src4, beta, 0.0, dst);
	 addWeighted( dst, alpha, src5, beta, 0.0, dst);
	 addWeighted( dst, alpha, src6, beta, 0.0, dst);
	 addWeighted( dst, alpha, src7, beta, 0.0, dst);
	 addWeighted( dst, alpha, src8, beta, 0.0, dst);
	 addWeighted( dst, alpha, src9, beta, 0.0, dst);
	 addWeighted( dst, alpha, src10, beta, 0.0, dst);





	 imwrite("Rainbow.jpg" ,dst);
	 cout << "Saved " << "Rainbow.jpg"<< endl;
	 imshow( "Rainbow", dst );

	 waitKey(0);
}
