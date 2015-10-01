
#include"capture.h"
#include "detect.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {


	bool video;
	video = videoStream();

	if(video){
		addition();
	}


	//maching();
	Matvariance();

	return 0;

}

