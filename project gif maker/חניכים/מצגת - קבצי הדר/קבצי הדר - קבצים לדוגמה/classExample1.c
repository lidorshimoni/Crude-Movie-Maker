/*********************************
* Class: MAGSHIMIM C1			 *
* Week 8           				 *
* Class Example 1	  			 *
* Implementation of roots.h		 *
**********************************/

#include <stdio.h>
#include <math.h>

#include "classExample1.h"

/**
Computes the delta part of the quadratic formula
of the quadratic equation a*x^2 + b*x + c = 0

Input:
	a - the coefficient of x^2
	b - the coefficient of x
	c - the constant of the quadratic equation
Output:
	The discriminant of the quadratic equation
*/
float delta(float a, float b, float c)
{
	return pow(b,2) - 4 * a * c;
}

/**
Computes the delta part of the quadratic formula
of the quadratic equation a*x^2 + b*x + c = 0
and prints result to screen.

Input:
	a - the coefficient of x^2
	b - the coefficient of x
	c - the constant of the quadratic equation
Output:
	None
*/
void equation(float a, float b, float c)
{
	float sqrt_delta = delta(a,b,c);
	if (sqrt_delta > 0)
	{
		sqrt_delta = sqrt(sqrt_delta);
		printf("x1 = %.2f\n", (- b + sqrt_delta) / 2 / a);
		printf("x2 = %.2f\n", (- b - sqrt_delta) / 2 / a);
	}
	else if (sqrt_delta == 0)
	{
		printf ("x = %.2f\n", - b / 2 / a);
	}
	else
	{
		printf("No solution\n");
	}
}
