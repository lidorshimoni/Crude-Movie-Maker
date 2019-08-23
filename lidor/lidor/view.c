
#include "view.h"

#define FALSE 0
#define TRUE -1
#define ESC 27
#define FILTER_SIZE 4
#define FILTER_BW 2
#define FILTER_INVERT 3
#define MOD_TEXT 5
#define FILTER_XRAY 6
#define MOD_CIRCLE 7
#define EXIT 0




//play the movie!! with a choice for a mod or for loop
void play(link_t *head, int loop, int filter)
{
	int counter = 1, key = 1;
	link_t *list = head;
	IplImage* image;
	int filterType = CV_LOAD_IMAGE_UNCHANGED, height = 0, width = 0, i = 0, j = 0, k = 0;
	int filterInvert = FALSE, modText = FALSE, textPosX = 0, textPosY = 0;
	char text[MAX_NAME_SIZE] = { 0 };
	CvFont font;
	int lineWidth = 3, modCircle = FALSE, circleLineWidth = 0, circleRadius = 0, circlePosX = 0, circlePosY = 0;
	while (filter)
	{
		printf("\t\t---SELECT MODEFICATIONS---\n [0] TO EXIT MOD SELECTION\n [1] for normal\n [2] for black and white\n [3] for Inverted colors\n [4] for size limit (in pixels)\n [5] to add a text\n [6] for X-RAY filter\n [7] to add a circle\n");
		scanf("%d", &filterType);
		getchar();
		switch (filterType)
		{
		case EXIT:
			filter = 0;
			break;
		case FILTER_BW:
			filterType = CV_LOAD_IMAGE_GRAYSCALE;
			break;
		case FILTER_INVERT:
			filterInvert = TRUE;
			break;
		case FILTER_SIZE:
			printf("enter max height (default = 0)\n");
			scanf("%d", &height);
			printf("enter max width (default = 0)\n");
			scanf("%d", &width);
			getchar();
			break;
		case MOD_TEXT:
			modText = TRUE;
			printf("Please Enter Text: ");
			fgets(text, MAX_NAME_SIZE, stdin);
			text[strcspn(text, "\n")] = 0;
			printf("Please enter text position on x axis: ");
			scanf("%d", &textPosX);
			getchar();
			printf("Please enter text position on y axis: ");
			scanf("%d", &textPosY);
			getchar();
		case FILTER_XRAY:
			filterType = CV_LOAD_IMAGE_GRAYSCALE;
			filterInvert = TRUE;
			break;
		case MOD_CIRCLE:
			modCircle = TRUE;
			printf("Please enter the outer line thikness (-1 for filled circle): ");
			scanf("%d", &circleLineWidth);
			getchar();
			printf("please enter the circle's radius: ");
			scanf("%d", &circleRadius);
			getchar();
			printf("please enter the circle's X position: ");
			scanf("%d", &circlePosX);
			getchar();
			printf("please enter the circle's Y position: ");
			scanf("%d", &circlePosY);
			getchar();
			break;
		default:
			filterType = CV_LOAD_IMAGE_UNCHANGED;
		}
	}
	
	cvNamedWindow("Movie", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Movie", 500, 300);// move to the middle
	while (list && key )
	{
		
		image = cvLoadImage(list->frame->path, filterType);
		if (!image)//The image is empty.
		{
			printf("could not open or find image number %d\n", counter);
			list = list->next;
		}
		else
		{
			if (modCircle)
			{
				// draw a circle at (x,y) with a radius of (radius). Use green lines of width 1
				cvCircle(image, cvPoint(circlePosX, circlePosY), circleRadius, cvScalar(120.0, 20.0, 255.0, 0.0), circleLineWidth, 1, 0);
			}
			if (modText) //add text on the image
			{
				cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, 1.0, 1.0, 0, lineWidth, 0);
				cvPutText(image, text, cvPoint(20, 20), &font, cvScalar(255, 255, 0, 0));
			}
			if (height)// changes image height
			{
				image->height = height;
			}
			if (width)// changes image width
			{
				image->width = width;
			}
			if (filterInvert)
			{
				// inverts the image colors
				for (i = 0; i < image->height; i++)
				{
					for (j = 0; j < image->width; j++)
					{
						for (k = 0; k < image->nChannels; k++)
						{

							((uchar *)image->imageData)[i*image->widthStep + j * image->nChannels + k] = 255 - ((uchar *)image->imageData)[i*image->widthStep + j * image->nChannels + k];
						}
					}
				}
			}
			//cvResizeWindow("Movie", 100, 100);
			cvShowImage("Movie", image);//display the image
			key = cvWaitKey(list->frame->duration);//wait
			list = list->next;
			cvReleaseImage(&image);
		}
		counter++;
		if (!list && loop)
		{
			list = head;
			counter =1;
		}
		if (key == ESC)
		{
			key = FALSE;
		}
	}
	cvDestroyWindow("Movie");
	return;
}

//caputure a frame  form webcam and adds it to the end of the movie
void webcamFrame(link_t **head)
{
	//cvNamedWindow("Window", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCaptureFromCAM(-1);
	IplImage* frame = NULL;
	link_t *curr = head;
	frame_t * f = (frame_t *)malloc(sizeof(frame_t));
	int i = 0;
	char key = FALSE;
	char name[MAX_NAME_SIZE] = { 0 };
	int b;
	if (!capture)
	{
		printf("Error. Cannot capture.\n");
	}
	else
	{
		printf("Press any key to capture!\n");
		cvNamedWindow("Capture", CV_WINDOW_AUTOSIZE);

		while (key =! -1)
		{
			frame = cvQueryFrame(capture);
			if (!frame) {
				printf("Error. Cannot get the frame.\n");
				key = TRUE;
			}
			cvShowImage("Capture", frame);
			key = cvWaitKey(30);
		}
		cvReleaseCapture(&capture);
		cvDestroyWindow("Capture");
	}
	if (frame) // if the capture worked add the frame
	{
		printf("Frame captured!\n");
		printf("Please insert frame duration(in miliseconds):\n");
		scanf("%d", &(f->duration));
		getchar();
		printf("Please choose a name for that frame:\n");
		fgets(name, MAX_NAME_SIZE, stdin);
		name[strcspn(name, "\n")] = 0;
		f->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
		if ((*head) != NULL)
		{
			do {
				b = FALSE;
				curr = *head;
				while (curr)
				{
					if (!strcmp(curr->frame->name, name))
					{
						b = TRUE;
						printf("The name is already taken, please enter an other name\n");
						fgets(name, MAX_NAME_SIZE, stdin);
						name[strcspn(name, "\n")] = 0;
					}
					curr = curr->next;
				}
			} while (b);
		}
		strncpy(f->name, name, strlen(name) + 1);
		while (curr->next)
		{
			curr = curr->next;
			i++;
		}
		if (f) 
		{
			if (curr == NULL)
			{
				*head = (link_t*)malloc(sizeof(link_t));
				(*head)->next = NULL;
				curr = *head;
			}
			else
			{
				while (curr->next)
				{
					curr = curr->next;
				}
				curr->next = (link_t*)malloc(sizeof(link_t));
				curr->next->next = NULL;
				curr = curr->next;
			}
			curr->frame = f;
		}

	}
}