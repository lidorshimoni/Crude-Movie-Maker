
#include <stdio.h>
#include <string.h>
#include "Frame.h"
#include "view.h"
#include "linkedList.h"
#include <opencv2\highgui\highgui_c.h>
#include <opencv2\videoio\videoio_c.h>
#include <windows.h>
#include <dirent.h>


#define FALSE 0
#define TRUE 1
#define DEFAULT_DURATION 500

enum choice 
{
	EXIT = 0, 
	ADD_FRAME = 1, 
	REMOVE_FRAME = 2, 
	CHANGE_FRAME_INDEX = 3, 
	CHANGE_FRAME_DURATION = 4, 
	CHANGE_ALL_DURATION = 5, 
	LIST = 6, 
	PLAY = 7, 
	SAVE = 8, 
	REVERSE = 9, 
	ADD_FOLDER = 10, 
	ADD_FILTER = 11, 
	LOOP = 12, 
	WEBCAM = 13, 
	SORT = 14, 
	NOTES = 15};

void printMenu(void);
link_t *userChoiceFun(link_t* head, int choice);
void printList(link_t ** head);
void saveToFile(link_t * head);
link_t* getDataFromFile(void);
void playMovie(link_t ** head);
void getByFolder(link_t ** head);
void ignorDots(DIR *dir);

int main(void)
{
	link_t * head = NULL;
	link_t * curr;
	int userChoice = 0;
	MessageBoxA(NULL, "Hello,\nThis Gif maker supports jpeg, png and any 8-bit image formats.", "Magshimim Movie Maker", MB_ICONEXCLAMATION);
	userChoice = MessageBoxA(NULL, "Do you want to load an existing project?", "Magshimim Movie Maker", MB_YESNO | MB_ICONQUESTION);
	if (userChoice == IDYES)
	{
		head = getDataFromFile();
	}
	printf("Editing Project! what would you like to do ? \n");
	do 
	{
		printMenu();
		scanf("%d", &userChoice);
		getchar();
		head = userChoiceFun(head, userChoice);
	} while (userChoice);
	while (head) 
	{
		curr = head->next;
		freeAll(head);
		head = curr;
	}
	return 0;
}

void printMenu(void)
{
	printf("______________MAIN__MENU______________\n [0] exit\n [1] Add new Frame\n [2] Remove a Frame\n [3] change frame index\n [4] Change frame duration\n [5] Change duration of all frames\n [6] List frames\n [7] Play movie!\n [8] save project\n---Bonus---\n [9] Reverse movie\n [10] add frames folder (UNSTABLE)\n [11] play with image mod\n [12] play in loop\n [13] add a frame from webcam\n [14] sorts the movie (recommended after using [10])\n [15] Notes\n");
}

link_t *userChoiceFun(link_t* head, int choice)
{
	switch (choice)
	{
	case EXIT:
		break;
	case ADD_FRAME:
		addNewFrame(&head);
		break;
	case REMOVE_FRAME:
		removeFrame(&head);
		break;
	case CHANGE_FRAME_INDEX:
		changeFrameIndex(&head);
		break;
	case CHANGE_FRAME_DURATION:
		changeFrameDura(&head);
		break;
	case CHANGE_ALL_DURATION:
		changeAllDura(&head);
		break;
	case LIST:
		printList(&head);
		break;
	case PLAY:
		playMovie(&head);
		break;
	case SAVE:
		saveToFile(head);
		break;
	case REVERSE:
		reverse(&head);
		printf("\t\t---Frames reversed!---\n");
		break;
	case ADD_FOLDER:
		getByFolder(&head);
		break;
	case ADD_FILTER:
		play(head, FALSE, TRUE);
		break;
	case LOOP:
		printf("Press ESC to close window\n");
		play(head, TRUE, FALSE);
		break;
	case WEBCAM:
		webcamFrame(&head);
		break;
	case NOTES:
		printf("\t\t--- Notes--- \n 1. you can always skip a fram by pressing the SPACE key\n 2. you can fast forward the movie by holding the SPACE key\n 3. you can exit a loop or stop watching the gif by pressing the escape key (ESC)\n 4. using the \"get frames from folder\" option is highly recomended but it's also glitchy sometimes\n 5. the webcam option wont work on any computer/operating system\n 6. You can choose multiple mods at the mod selection menu but you use each mod only once (one text per image, one)\n----------------------------------\n");
		break;
	case SORT:
		head = quickSortRecur(head);
		break;
	default:
		printf("You should type one of the options - 0-15!");

	}
	return head;
}

void printList(link_t ** head) 
{
	link_t * curr = *head;
	printf("                Name            Duration        Path\n");
	while (curr)
	{
		printf("                %s               %d ms          %s\n", curr->frame->name, curr->frame->duration, curr->frame->path);
		curr = curr->next;
	}
}

void saveToFile(link_t * head) 
{
	char path[MAX_PATH_SIZE] = { 0 };
	printf("Enter the path of the project\n");
	fgets(path, MAX_PATH_SIZE, stdin);
	path[strcspn(path, "\n")] = 0;
	FILE * f = fopen(path, "w");
	if (f == NULL) 
	{
		printf("Eror opening the file\n");
	}
	else
	{
		link_t * curr = head;
		while (curr)
		{
			fprintf(f, "%s#%d$%s\n", curr->frame->name, curr->frame->duration, curr->frame->path);
			curr = curr->next;
		}
		fclose(f);
	}
}

link_t* getDataFromFile(void) 
{
	char path[MAX_PATH_SIZE] = { 0 };
	link_t * head = (link_t*)malloc(sizeof(link_t));
	link_t * curr = head;
	char c = 0;
	int i = 0, num = 0;
	int b = FALSE, b1 = FALSE, b2 = FALSE, b3 = FALSE;
	char str[256] = { 0 };
	FILE * f;
	printf("Enter the path of the project <including name>\n");
	fgets(path, MAX_PATH_SIZE, stdin);
	path[strcspn(path, "\n")] = 0;
	f = fopen(path, "r");
	if (f == NULL) 
	{
		printf("Error!-cant open file, creating a new project");
	}
	else
	{
		head->frame = (frame_t *)malloc(sizeof(frame_t));
		while ((c = fgetc(f)) != EOF)
		{
			if (b)
			{
				curr->next = (link_t*)malloc(sizeof(link_t));
				curr = curr->next;
				curr->frame = (frame_t *)malloc(sizeof(frame_t));
				curr->next = NULL;
				b = FALSE;
			}
			if (c == '#')
			{
				b1 = TRUE;
				str[i] = 0;
				curr->frame->name = (char*)malloc(sizeof(char)*(strlen(str) + 1));
				strncpy(curr->frame->name, str, strlen(str) + 1);
				i = 0;
			}
			else if (c == '$')
			{
				b2 = TRUE;
				str[i] = 0;
				num = 0;
				for (i = 0; i < strlen(str); i++)
				{
					num = num * 10 + str[i] - 48;
				}
				curr->frame->duration = num;
				i = 0;
			}
			else if (c == '\n')
			{
				b3 = TRUE;
				str[i] = 0;
				curr->frame->path = (char*)malloc(sizeof(char)*(strlen(str) + 1));
				strncpy(curr->frame->path, str, strlen(str) + 1);
				i = 0;
				b = TRUE;
			}
			else
			{
				str[i] = c;
				i++;
			}
		}
		if (b1 != TRUE || b2 != TRUE || b3 != TRUE)
		{
			printf("Error!-cant open file, creating a new project");
		}
		else
		{
			curr->next = NULL;
			fclose(f);
			return head;
		}
	}
}

void playMovie(link_t ** head) 
{
	int userChoice = 0, i = 0, len = 0;
	link_t *curr = *head;
	printf("If you wish to play the whole movie press 1\nIf you want to play the movie from a certain point press 2\n");
	scanf("%d", &userChoice);
	if (userChoice == 1)
	{
		play(*head, FALSE, FALSE);
	}
	else if (userChoice == 2) 
	{
		printf("Where do you wish to start playing the movie from ? \n");
		scanf("%d", &i);
		getchar();
		while (curr) 
		{
			curr = curr->next;
			len++;
		}
		curr = *head;
		if (i > len) 
		{
			printf("the movie has only %d frames!\n", len);
		}
		else 
		{
			for (; i > 1; i--) 
			{
				curr = curr->next;
			}
			printf("%s", curr->frame->name);
			play(curr, FALSE, FALSE);
		}

	}
}

void getByFolder(link_t** head)
{
	DIR *dir;
	struct dirent *ent;
	int numberOfFiles = 0, b;
	char folderPath[MAX_PATH_SIZE] = { NULL }, folderPathCopy[MAX_PATH_SIZE] = { NULL }, files[MAX_NAME_SIZE][MAX_NAME_SIZE] = { NULL };
	link_t * curr = *head;
	frame_t * f ;
	printf("please enter the path of the folder that contain only images:\n");
	fgets(folderPath, MAX_NAME_SIZE, stdin);
	folderPath[strcspn(folderPath, "\n")] = 0;

	if ((dir = opendir(folderPath)))
	{
		ignorDots(dir); //clears the useless dots
		while ((ent = readdir(dir)))
		{
			strncpy(files[numberOfFiles], ent->d_name, strlen(ent->d_name));
			numberOfFiles++;
		}
		closedir(dir);
		numberOfFiles--;
		for(; numberOfFiles >= 0; numberOfFiles--)
		{
			f = (frame_t *)malloc(sizeof(frame_t));
			memset(folderPathCopy, '\0', sizeof(folderPathCopy));
			strcpy(folderPathCopy, folderPath);
			f->path = (char*)calloc(sizeof(char)*(strlen(folderPath) + strlen("\\") + strlen(files[numberOfFiles]) + 1), 1);
			strncpy(f->path, strcat(strcat(folderPathCopy, "\\"), files[numberOfFiles]), strlen(folderPath) + strlen("\\") + strlen(files[numberOfFiles]));
			
			f->duration = DEFAULT_DURATION;
			f->name = (char*)malloc(sizeof(char)*(strlen(files[numberOfFiles]) + 1));
			strcpy(f->name, files[numberOfFiles]);

			if (f) {
				if (!curr)
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
		reverse(head);
	}
}

void ignorDots(DIR *dir)
{
	readdir(dir);
	readdir(dir);
}