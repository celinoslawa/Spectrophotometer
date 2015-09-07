
#include"capture.h"

using namespace cv;
using namespace std;


int main() {

	bool video;
	video = videoStream();

	if(video){
		addition();
	}

	return 0;

}
