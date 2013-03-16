#include <cv.h>
#include "Image.h"
//#include "Rect.h"
#include <vector>
#include <iostream>
#include <math.h>


using namespace std;

/* Bounderies for area of LED locations */
struct LED
{
	int topX;
	int topY;
	int bottomX;
	int bottomY;
};

/*	TAG FRAME LED CONSTANTS	*/
	Color LED1Tag = Color(255,0,0);
	Color LED2Tag = Color(0,255,0);
	Color LED3Tag = Color(0,0,255);
	Color LED4Tag = Color(255,255,0);
	Color LED5Tag = Color(255,0,255);
	Color LED6Tag = Color(0,255,255);
	Color LED7Tag = Color(128,0,0);
	Color LED8Tag = Color(0,128,0);
	Color LED9Tag = Color(0,0,128);
	Color LED10Tag = Color(128,128,0);
	Color LED11Tag = Color(128,0,128);
	Color LED12Tag = Color(0,128,128);
	Color LED13Tag = Color(255,128,0);
	Color LED14Tag = Color(0,255,128);
	Color LED15Tag = Color(255,0,128);
	Color LED16Tag = Color(128,0,255);
	Color BorderTag = Color(128,128,128);

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
	
	bool isColorMatch(Color& testColor, Color& tagColor)
	{
		int fivePct = .05*255;		// +-5% for possible video color error
		if(testColor.r <= tagColor.r+fivePct && testColor.r >= tagColor.r-fivePct)
			if(testColor.g <= tagColor.g+fivePct && testColor.g >= tagColor.g-fivePct)
				if(testColor.b <= tagColor.b+fivePct && testColor.b >= tagColor.b-fivePct)
						return true;
		
		return false;
	}

	bool isTagFrame(Mat& frame)
	{
		processed(frame);
		int HEIGHT = processed.getHeight();
		int WIDTH = processed.getWidth();
		
		int colorMatches = 0;
		for(int x = 0; x<WIDTH; x++)
		{
			for(int y = 2; y<5; y++)
			{
				Color tempColor = processed.get(x,y);
				if(isColorMatch(tempColor,BorderTag))
					colorMatches++;
			}
		}
		float area = WIDTH*3;	//area of scanned location
		float pctMatch = colorMatches/area;
		if(pctMatch >= 0.75)		// 75% match to account for possible side margins in the video
		{
			cout<<"ITS A TAG FRAME!"<<endl;
			return true;
		}
		cout<<"its NOT a tag frame"<<endl;
		return false;
	}

	void locateLEDLocations(Mat &frame)
	{
		vector<Color> ledTagColors;

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
	
	bool process(Mat& frame)
	{
		vector<int> rVec;
		vector<int> gVec;
		vector<int> bVec;

		processed(frame);
		
		for(int i = 0; i<16; i++)
		{
			LED currentSection = ledLocationVec[i];
			int rAvg = 0;
			int gAvg = 0;
			int bAvg = 0;
			for(int x = currentSection.topX; x<currentSection.bottomX; x++)
			{
				for(int y = currentSection.topY; y<currentSection.bottomY; y++)
				{
					rAvg += processed.getR(x,y);
					gAvg += processed.getG(x,y);
					bAvg += processed.getB(x,y);
				} //k
			} //j
			int sectionArea = (currentSection.bottomX-currentSection.topX)*(currentSection.bottomY-currentSection.topY);
			if(sectionArea != 0) // Prevents divide by 0 in case section isn't present
			{
				rAvg = rAvg/sectionArea;
				gAvg = gAvg/sectionArea;
				bAvg = bAvg/sectionArea;
			}
			rVec.push_back(rAvg);
			gVec.push_back(gAvg);
			bVec.push_back(bAvg);
		} //i
		rFrameVals = rVec;
		gFrameVals = gVec;
		bFrameVals = bVec;

		return true;
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
	
	Color gammaCorrect(Color& originalColor)
	{
		cout<<"RGB Original: "<<originalColor.r<<" "<<originalColor.g<<" "<<originalColor.b<<" "<<endl;
		float gammaVal = 2.5;		// Determined experimentally
		//float rFactor = ((float)originalColor.r)/255;
		//cout<<"Correction Factor Printout: "<<rFactor<<" "<<pow((float)(originalColor.g/255),1/gammaVal)<<" "<<pow((float)(originalColor.b/255),1/gammaVal)<<endl;
		int correctR = 255 * pow( ((float)originalColor.r)/255,1/gammaVal);
		int correctG = 255 * pow( ((float)originalColor.g)/255,1/gammaVal);
		int correctB = 255 * pow( ((float)originalColor.b)/255,1/gammaVal);

		return Color(correctR,correctG,correctB);

	}


	void setKey(char c)
	{
		myChar = c;
	}

	Image processed;
	char myChar;

};