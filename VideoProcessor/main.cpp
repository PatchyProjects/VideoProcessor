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
using std::cin;
using std::endl;
using std::ofstream;

// Cuts off extention so we can name the output file
string getFileName(string fileNameAndExt)
{
	string fileName;
	//string fileName = strtok(fileNameAndExt,".");
	for(int i = 0; i<fileNameAndExt.size(); i++)
	{
		if(fileNameAndExt[i] == '.')
			break;
		fileName += fileNameAndExt[i];
	}
	return fileName;
}

int main(int argc, char* argv[])
{
	//string videoFile = "testColorMix.avi";
	string videoFile;
	cout<<"Enter Video File Name (ex. testColorMix.avi): ";
	std::cin>>videoFile;
	cout<<"File Name: "<<getFileName(videoFile)<<endl;
	cout<<"Opening "<<videoFile<<"..."<<endl;
	VideoCapture cap(videoFile);		//Open Video File
	if(!cap.isOpened())  // check if we succeeded
	{
		cout<<"Error Opening Video"<<endl;
        return -1;
	}
	cout<<"Video Opened Successfully!"<<endl;
	
	cout<<"Creating CSV File..."<<endl;
	string frameFileName = getFileName(videoFile) + ".txt";
	ofstream frameFile;
	frameFile.open (frameFileName);
	if (!frameFile.is_open()) //error opening writefile
	{
		cout<<"Error Opening CSV file"<<endl;
		return -1;
	}
	
	FrameProcessor processFrame;

    namedWindow("video",1);

	cout<<"Processing Video, Please Wait..."<<endl;
	int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);

	Mat firstFrame;
	cap >> firstFrame;
	if(processFrame.isTagFrame(firstFrame))
		processFrame.locateLEDLocations(firstFrame);
	else
		processFrame.basicLayout(firstFrame);
	processFrame.printLocations();
	for(int i = 1; i < frameCount; i++) 
    {
        Mat frame;
        cap >> frame; // get new frame from video

        imshow("video", frame);

		processFrame.process(frame);
		//processFrame.colorCorrect();		// Apply gamma Correction
		frameFile<<processFrame.getFrameString()<<endl;
		/*
		for(int j = 0; j < 16; j++) //write averaged RGB values to file in CSV format
		{
			Color originalColor = Color(processFrame.rFrameVals[j],processFrame.gFrameVals[j],processFrame.bFrameVals[j]);
			Color correctedColor = processFrame.gammaCorrect(originalColor);
			//cout<<"Original: "<<processFrame.rFrameVals[j]<<","<<processFrame.gFrameVals[j]<<","<<processFrame.bFrameVals[j]<<" ";
			//cout<<"Processed: "<<correctedColor.r<<","<<correctedColor.g<<","<<correctedColor.b<<endl;
			frameFile<<processFrame.rFrameVals[j]<<","<<processFrame.gFrameVals[j]<<","<<processFrame.bFrameVals[j]<<" ";
			
		}
		frameFile<<endl; 
		*/
		
		

    }
    
	frameFile.close();
	cout<<"Number of frames processed: "<<frameCount<<endl;
	cout<<"Approx. length of video: "<<frameCount/30<<" seconds"<<endl;

	int waiting;
	cout<<"Video Processing Complete!";
	std::cin>>waiting;
    return 0;
}