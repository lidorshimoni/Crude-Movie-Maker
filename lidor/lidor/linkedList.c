
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "Frame.h"
#include "linkedList.h"

#define TRUE 1
#define FALSE 0

void addNewFrame(link_t ** head)
{
	link_t * curr = *head;
	frame_t * f = setFrameFromUser(head);
	if (f) {
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

void removeFrame(link_t ** head) 
{
	link_t * curr = *head;
	link_t * temp;
	char str[MAX_NAME_SIZE] = { 0 };
	int deleted = FALSE;
	printf("Enter the name of the frame you wish to erase\n");
	fgets(str, MAX_NAME_SIZE, stdin);
	str[strcspn(str, "\n")] = 0;
	

	// if the list is not empty (if list is empty - nothing to delete!)
	if (*head)
	{
		// the first node should be deleted?
		if (!strcmp((*head)->frame->name, str))
		{
			*head = (*head)->next;
			free(curr);
		}
		else
		{
			while (curr->next)
			{
				if ((0 == strcmp(curr->next->frame->name, str))) // waiting to be on the node BEFORE the one we want to delete
				{
					temp = curr->next; // put aside the node to delete
					curr->next = temp->next; // link the node before it, to the node after it
					free(temp); // delete the node
					deleted = TRUE;
				}
				else
				{
					curr = curr->next;
				}
			}
		}
	}
	if (!deleted)
	{
		printf("The frame was not found");
	}

}

void changeFrameIndex(link_t ** head)
{
	int i = 0;
	int choice = 0;
	int max = 0;
	int flag = 0;
	char name[MAX_NAME_SIZE] = { 0 };

	link_t* curr = *head;
	link_t* temp = NULL;
	link_t* beforeChosen = NULL;
	link_t* chosenFrame = NULL;
	link_t* returning = NULL;

	printf("enter the name of the frame\n");
	fgets(name, MAX_NAME_SIZE, stdin);
	name[strcspn(name, "\n")] = 0;
	printf("Enter the new index in the movie you wish to place the frame\n");
	scanf("%d", &choice);
	getchar();//clear buffer
//need impruvment

	// srry i actually did this function but accedently deleted it and forgot to ctrl+z :(

}

void changeFrameDura(link_t ** head) 
{
	link_t * curr = *head;
	char str[MAX_NAME_SIZE] = { 0 };
	int i = 0, index = 0, b = TRUE, d;
	printf("enter the name of the frame\n");
	fgets(str, MAX_NAME_SIZE, stdin);
	str[strcspn(str, "\n")] = 0;
	printf("enter the new duration\n");
	scanf("%d", &d);
	getchar();
	while (curr) 
	{
		if (!strcmp(curr->frame->name, str)) 
		{
			curr->frame->duration = d;
			b = FALSE;
		}
		curr = curr->next;
	}
	if (b) 
	{
		printf("the frame does not exist");
	}
}

void changeAllDura(link_t ** head) 
{
	link_t * curr = *head;
	int i = 0, index = 0, d;
	printf("enter the new duration: ");
	scanf("%d", &d);
	getchar();
	while (curr)
	{
		curr->frame->duration = d;
		curr = curr->next;
	}
}

void freeAll(link_t * del) 
{
	free(del->frame->name);
	free(del->frame->path);
	free(del->frame);
	free(del);
}

frame_t* setFrameFromUser(link_t ** head) 
{
	printf("\n\n                *** Creating new frame ***\n");
	link_t * curr = *head;
	frame_t * f = (frame_t *)malloc(sizeof(frame_t));
	char path[MAX_PATH_SIZE] = { 0 };
	char name[MAX_NAME_SIZE] = { 0 };
	int b;
	printf("Please insert frame path:\n");
	fgets(path, MAX_PATH_SIZE, stdin);
	path[strcspn(path, "\n")] = 0;
	f->path = (char*)malloc(sizeof(char)*(strlen(path) + 1));
	strncpy(f->path, path, strlen(path) + 1);
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
	FILE * file;
	file = fopen(path, "r");
	if (file) 
	{
		fclose(file);
	}
	else 
	{
		printf("Can't find file! Frame will not be added\n");
		f =  NULL;
	}
	return f;
}

void reverse(link_t** head_ref)
{
	link_t* prev = NULL;
	link_t* current = *head_ref;
	link_t* next = NULL;
	while (current != NULL)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*head_ref = prev;
}


int number(link_t* node)
{
	int i = 0;
	char str[MAX_NAME_SIZE] = { 0 };
	while (node->frame->name[i] != '.'&& i<strlen(node->frame->name))
	{
		str[i] = node->frame->name[i];
		i++;
	}
	return atoi(str);
}

// Partitions the list taking the last element as the pivot (for the recursive quick sort)
link_t *partition(link_t *head, link_t *end,
	link_t **newHead, link_t **newEnd)
{
	link_t *pivot = end;
	link_t *prev = NULL, *cur = head, *tail = pivot;
	
	while (cur != pivot)
	{
		
		if (number(cur) < number(pivot))
		{
			if ((*newHead) == NULL)
				(*newHead) = cur;

			prev = cur;
			cur = cur->next;
		}
		else 
		{
			// Move cur node to next of tail, and change tail
			if (prev)
				prev->next = cur->next;
			link_t *tmp = cur->next;
			cur->next = NULL;
			tail->next = cur;
			tail = cur;
			cur = tmp;
		}
	}

	if ((*newHead) == NULL)
		(*newHead) = pivot;

	// Update newEnd to the current last node
	(*newEnd) = tail;

	// Return the pivot node
	return pivot;
}


//recursive quick sort
link_t *quickSortRecur(link_t *head)
{
	link_t *end = head;
	while (end != NULL && end->next != NULL)
	{
		end = end->next;
	}

	if (!head || head == end)
		return head;

	link_t *newHead = NULL, *newEnd = NULL;
	link_t *pivot = partition(head, end, &newHead, &newEnd);

	if (newHead != pivot)
	{
		link_t *tmp = newHead;
		while (tmp->next != pivot)
			tmp = tmp->next;
		tmp->next = NULL;


		newHead = quickSortRecur(newHead, tmp);

		tmp = newHead;
		while (tmp != NULL && tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = pivot;
	}

	// Recur for the list after the pivot element
	pivot->next = quickSortRecur(pivot->next, newEnd);

	return newHead;
}