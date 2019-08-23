/*********************************
* Class: MAGSHIMIM C1			 *
* Week 7           				 *
* Class Example 6	  			 *
* Using user header files		 *
**********************************/

#include <stdio.h>
#include <stdlib.h>

#include "classExample1.h"

int main(void)
{
	float a = 0.0;
	float b = 0.0;
	float c = 0.0;
	printf("Write the a\n");
	scanf("%f",&a);
	printf("Write the b\n");
	scanf("%f",&b);
	printf("Write the c\n");
	scanf("%f",&c);

	equation(a,b,c);
	system("PAUSE");
	return 0;
}

