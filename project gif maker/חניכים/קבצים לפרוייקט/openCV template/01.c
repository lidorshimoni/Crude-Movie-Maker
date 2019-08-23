/*********************************
* Class: MAGSHIMIM C2			 *
* openCV template      			 *
**********************************/

#include <stdio.h>
#include <opencv2\highgui\highgui_c.h>

int main(void)
{
	int i;
	cvNamedWindow("Display window", CV_WINDOW_AUTOSIZE); //create a window
	//create an image
	IplImage* image = cvLoadImage("C:\\c1.png", 1);
	if (!image)//The image is empty.
	{
		printf("could not open image\n");
	}
	else
	{
		cvShowImage("Display window", image);
		cvWaitKey(0);
		system("pause");
		cvReleaseImage(&image);
	}
	return 0;
}