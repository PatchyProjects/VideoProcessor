#include <iostream>

#include <cv.h>
#include <highgui.h>
#include <vector>
#include <string>

#include "FrameProcessor.h"
#include <iostream>
#include <fstream>

using namespace cv;
using std::cout;
using std::endl;
using std::ofstream;

int main(int argc, char* argv[])
{
	string videoFile = "testColorMix.avi";
   // VideoCapture cap(0); // open the default camera
	VideoCapture cap(videoFile);
	if(!cap.isOpened())  // check if we succeeded
	{
		cout<<"Error Opening Video"<<endl;
        return -1;
	}

	ofstream frameFile;
	frameFile.open ("frameFile.txt");
	if (!frameFile.is_open()) //error opening writefile
	{
		cout<<"Error Opening CSV file"<<endl;
		return -1;
	}
    
	
	FrameProcessor processFrame;

    namedWindow("video",1);

	int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
	for(int i = 0; i < frameCount; i++) 
    {
        Mat frame;
        cap >> frame; // get new frame from video


        imshow("video", frame);

		processFrame.doWork(frame);
		
		for(int j = 0; j < 16; j++) //write averaged RGB values to file in CSV format
		{
			frameFile<<processFrame.rFrameVals[j]<<","<<processFrame.gFrameVals[j]<<","<<processFrame.bFrameVals[j]<<" ";
		}
		frameFile<<endl; 
		
		char c = waitKey(30);
		if ('a' <= c && c <= 'z')
		{
			cout << "the input key is " << c << endl;
		}
		if(c == 'q') break;
		else if(c != -1)
		{
			processFrame.setKey(c);
		} 
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
	frameFile.close();
    return 0;
}