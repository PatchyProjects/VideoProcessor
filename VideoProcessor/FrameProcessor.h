#include <cv.h>
#include "Image.h"
//#include "Rect.h"
#include <vector>
#include <iostream>


using namespace std;

/* Bounderies for area of LED locations */
struct LED
{
	int topX;
	int topY;
	int bottomX;
	int bottomY;
};

class FrameProcessor
{
public:
	FrameProcessor():
	  myChar(0)
	{
		//make a new window to show our processed results
		namedWindow("processed",1);
	}
	//  Rect myRect = new Rect(0,0,15,15); 
	vector<LED> ledLocationVec;
	vector<int> rFrameVals;
	vector<int> bFrameVals;
	vector<int> gFrameVals;

	void printLocations()
	{
		for(int i = 0; i<ledLocationVec.size(); i++)
		{
			cout<<"LED"<<i<<endl;
			cout<<"Top("<<ledLocationVec[i].topX<<","<<ledLocationVec[i].topY<<")"<<endl;
			cout<<"Bottom("<<ledLocationVec[i].bottomX<<","<<ledLocationVec[i].bottomY<<")"<<endl;
		}
	}

	void basicLayout(Mat& frame)
	{	
		processed(frame);
		int HEIGHT = processed.getHeight();
		int WIDTH = processed.getWidth();
		cout<<"height: "<<HEIGHT<<"width: "<<WIDTH<<endl;
		for(int i = 0; i<16; i++)
		{
			int topX = WIDTH/4*(i%4);
			int topY = HEIGHT/4*((i/4)%4);
			int bottomX = topX + WIDTH/4;
			int bottomY = topY + HEIGHT/4;
			LED tempLED;
			tempLED.topX = topX;
			tempLED.topY = topY;
			tempLED.bottomX = bottomX;
			tempLED.bottomY = bottomY;
			
			ledLocationVec.push_back(tempLED);
		}
	}
	
	bool doWork(Mat& frame)
	{
		vector<int> rVec;
		vector<int> gVec;
		vector<int> bVec;

		processed(frame);
		int HEIGHT = processed.getHeight();
		int WIDTH = processed.getWidth();

		for(int i = 0; i<4; i++)
		{
			for(int j = 0; j<4; j++)
			{
				int rAvg = 0;
				int gAvg = 0;
				int bAvg = 0;
				for(int x = j*WIDTH/4; x < (j+1)*WIDTH/4; x++)
				{
					for(int y = i*HEIGHT/4; y < (i+1)*HEIGHT/4; y++)
					{
						rAvg = rAvg + processed.getR(x,y);
						gAvg = gAvg + processed.getG(x,y);
						bAvg = bAvg + processed.getB(x,y);
					}
				}
				rAvg = rAvg/(HEIGHT*WIDTH/16);
				gAvg = gAvg/(HEIGHT*WIDTH/16);
				bAvg = bAvg/(HEIGHT*WIDTH/16);

				rVec.push_back(rAvg);
				gVec.push_back(gAvg);
				bVec.push_back(bAvg);
			} //j
		} //i

		rFrameVals = rVec; 
		bFrameVals = bVec;
		gFrameVals = gVec;
		
		int ledLoc = 0; //led location, max = 16;
		
		for(int i = 0; i<4; i++)
		{
			for(int j = 0; j<4; j++)
			{
				for(int x = j*WIDTH/4; x < (j+1)*WIDTH/4; x++)
				{
					for(int y = i*HEIGHT/4; y < (i+1)*HEIGHT/4; y++)
					{
						processed.set(x,y,rVec[ledLoc],gVec[ledLoc],bVec[ledLoc]);
					}
				}
				ledLoc++;
			} //j
		} //i
		
		//show the processed image
		imshow("processed", processed.getImage());
		cvWaitKey(30);

		return true;
	}
	
	void setKey(char c)
	{
		myChar = c;
	}

	Image processed;
	char myChar;

};