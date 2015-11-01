
#include"capture.h"
#include "detect.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {
	bool video1;
	video1 = videoStream();
	if(video1){
		addition("Rainbow.png");
	}

	Matvariance();
	filllingWhite();
	canny();
	lineDetect();

return(0);

}


