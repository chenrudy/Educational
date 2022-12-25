#include <string.h>
#include <stdio.h>
#include "Dynamic_vector.h"

#define RUNTEST(test) {\
            if (!test)\
            {\
                printf("\x1b[0;34m");\
                printf("OK!\n\n");\
                printf("\x1b[0m");\
            }\
            else\
            {\
                printf("\x1b[1;35m");\
                printf("\nTest Failed!\n\n");\
                printf("\x1b[0m");\
            }\
        }
 
#define RUNTESTP(test) {\
            if ( NULL != test)\
            {\
                printf("\x1b[0;34m");\
                printf("OK!\n\n");\
                printf("\x1b[0m");\
            }\
            else\
            {\
                printf("\x1b[1;35m");\
                printf("NULL!\n");\
                printf("\x1b[0m");\
            }\
        }
        
int TestInt();
int TestChar();
int TestDouble();
int TestStr();
               
int main()
{
	TestInt();
	TestChar();
	TestDouble();
	TestStr();
	printf("---------------------thank you for your time-----------------------\n");
	return 0;
}

int TestInt()
{
	int a = 5;
	int b = 8;
	int c = 40;
	int d = -16;
	int e = 0;
	int f = -25;
	
	d_vector_t*v = DVectorCreate(sizeof(int), 4);
	
	printf("-----------------------int test------------------------\n");
	printf("capacity: %ld bytes, asked for 4 ints\n", DVectorCapacity(v));
	printf("Vector is empty? %d\n", DVectorIsEmpty(v));
	DVectorPushBack(v, &a);
	DVectorPushBack(v, &b);
	DVectorPushBack(v, &c);
	printf("num of elements after 3 pushes: %ld\n", DVectorSize(v));
	printf("capacity: still %ld bytes\n", DVectorCapacity(v));
	printf("\nauto double capacity with push:\n");
	DVectorPushBack(v, &d);
	printf("num of elements after 4 pushes: %ld\nreached full capacety - so i will automatically double it!\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	DVectorPushBack(v, &e);
	DVectorPushBack(v, &f);
	DVectorPushBack(v, &a);
	DVectorPushBack(v, &b);
	printf("num of elements after 8 pushes: %ld\nreached full capacety - auto double it again!\n",
	DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into element in index 2: %d\n",
	 *(int *)DVectorGetElement(v, 2));
	printf("\nauto shrink with pop:\n");
	DVectorPopBack(v);
	DVectorPopBack(v);
	DVectorPopBack(v);
	DVectorPopBack(v);
	DVectorPopBack(v);
	printf("Num of elements afer 5 poops: %ld - elements are 25 precent of capacety so i will shrink 50 precent\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("\nshrink to fit:\n");
	DVectorShrinkToFit(v);
	/*DVectorReserve(v, 2); will shrink!*/
	printf("capacity: %ld bytes, shrinked according to number of elements + 1\n",
	DVectorCapacity(v));
	printf("num of elements: %ld\n", DVectorSize(v));
	printf("\nReserve:\n");
	DVectorReserve(v, 10); 
	printf("capacity after reserving 10 elements: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into existing elements vs non existent elements:\n");
	printf("peeking into element in index 0: %d\n",
	 *(int *)DVectorGetElement(v, 0));
	/*printf("peeking into element in index 20: %d\n", *(int *)DVectorGetElement(v, 20));*/
		
	DVectorDestroy(v);
	v = NULL;
	
	return 0;
}

int TestChar()
{
	char a = 't';
	char b = 8;
	char c = 'c';
	char d = 'd';
	char e = 'e';
	char f = '!';
	
	d_vector_t*v = DVectorCreate(sizeof(char), 0);
	
	printf("-----------------------char test-----------------------\n");
	printf("capacity: %ld bytes, which is default\n", DVectorCapacity(v));
	printf("Vector is empty? %d\n", DVectorIsEmpty(v));
	DVectorPushBack(v, &a);
	DVectorPushBack(v, &b);
	printf("num of elements after 2 pushes: %ld\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("\nauto double capacity with push:\n");
	DVectorPushBack(v, &c);
	printf("num of elements after 3 pushes: %ld\nreached full capacety - so i will automatically double it!\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	DVectorPushBack(v, &d);
	DVectorPushBack(v, &e);
	DVectorPushBack(v, &f);
	printf("num of elements after 6 pushes: %ld\nreached full capacety - auto double it again!\n",
	DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into element in index 2: %c\n",
	 *(char *)DVectorGetElement(v, 2));
	DVectorPopBack(v);
	DVectorPopBack(v);
	DVectorPopBack(v);
	printf("auto shrink with pop:\n");
	printf("\nNum of elements afer 3 poops: %ld - elements are 25 precent of capacety so i will shrink 50 precent\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("\nshrink to fit:\n");
	DVectorShrinkToFit(v);
	/*DVectorReserve(v, 2); will shrink!*/
	printf("capacity: %ld bytes, shrinked according to number of elements + 1\n",
	DVectorCapacity(v));
	printf("num of elements : %ld\n", DVectorSize(v));
	printf("\nReserve:\n");
	DVectorReserve(v, 10); 
	printf("capacity after reserving 10 elements: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into existing elements vs non existent elements:\n");
	printf("peeking into element in index 0: %c\n",
	 *(char *)DVectorGetElement(v, 0));
	/*printf("peeking into element in index 20: %c\n", *(char *)DVectorGetElement(v, 20));*/
		
	DVectorDestroy(v);
	v = NULL;
		
	return 0;
}

int TestDouble()
{
	double a = 2.5;
	double b = 0.075;
	double c = 27.3;
	double d = 0.23;
	double e = 2.256;
	double f = 0.05;
	
	d_vector_t*v = DVectorCreate(sizeof(double), 0);
	
	printf("-----------------------double test-------------------------\n");
	printf("capacity: %ld bytes, which is default\n", DVectorCapacity(v));
	printf("Vector is empty? %d\n", DVectorIsEmpty(v));
	DVectorPushBack(v, &a);
	DVectorPushBack(v, &b);
	printf("num of elements after 2 pushes: %ld\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("\nauto double capacity with push:\n");
	DVectorPushBack(v, &c);
	printf("num of elements after 3 pushes: %ld\nreached full capacety - so i will automatically double it!\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	DVectorPushBack(v, &d);
	DVectorPushBack(v, &e);
	DVectorPushBack(v, &f);
	printf("num of elements after 6 pushes: %ld\nreached full capacety - auto double it again!\n",
	DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into element in index 2: %f\n",
	 *(double *)DVectorGetElement(v, 2));
	DVectorPopBack(v);
	DVectorPopBack(v);
	DVectorPopBack(v);
	printf("auto shrink with pop:\n");
	printf("\nNum of elements afer 3 poops: %ld - elements are 25 precent of capacety so i will shrink 50 precent\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("\nshrink to fit:\n");
	DVectorShrinkToFit(v);
	/*DVectorReserve(v, 2); will shrink!*/
	printf("capacity: %ld bytes, shrinked according to number of elements + 1\n",
	DVectorCapacity(v));
	printf("num of elements : %ld\n", DVectorSize(v));
	printf("\nReserve:\n");
	DVectorReserve(v, 10); 
	printf("capacity after reserving 10 elements: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into existing elements vs non existent elements:\n");
	printf("peeking into element in index 0: %f\n",
	 *(double *)DVectorGetElement(v, 0));
	/*printf("peeking into element in index 20: %f\n", *(double *)DVectorGetElement(v, 20));*/
		
	DVectorDestroy(v);
	v = NULL;
	return 0;
}


int TestStr()
{
	char *str1 = "your";
	char *str2 = "inside";
	char *str3 = "put these seeds";
	char *str4 = "You gotta ";
	char *str5 = "Morty.";
	char *str6 = "do it for Grandpa, ";
	
	d_vector_t*v = DVectorCreate(sizeof(double), 0);
	
	printf("-----------------------string test-------------------------\n");
	printf("capacity: %ld bytes, which is default\n", DVectorCapacity(v));
	printf("Vector is empty? %d\n", DVectorIsEmpty(v));
	
	DVectorPushBack(v, &str1);
	DVectorPushBack(v, &str2);
	printf("num of elements after 2 pushes: %ld\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("\nauto double capacity with push:\n");
	DVectorPushBack(v, &str3);
	printf("num of elements after 3 pushes: %ld\nreached full capacety - so i will double it!\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	DVectorPushBack(v, &str4);
	DVectorPushBack(v, &str5);
	DVectorPushBack(v, &str6);
	printf("num of elements after 6 pushes: %ld\nreached full capacety - double it again!\n",
	DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into element in index 2: %s\n", *(char **)DVectorGetElement(v, 2));
	printf("\nauto shrink with pop:\n");
	DVectorPopBack(v);
	DVectorPopBack(v);
	DVectorPopBack(v);
	printf("Num of elements afer 3 poops: %ld  - elements are 25 precent of capacety so i will shrink 50 precent\n", DVectorSize(v));
	printf("capacity: %ld bytes\n", DVectorCapacity(v));
	printf("\nshrink to fit:\n");
	DVectorShrinkToFit(v);
	/*DVectorReserve(v, 2); will shrink!*/
	printf("capacity: %ld bytes, shrinked according to number of elements + 1\n",
	DVectorCapacity(v));
	printf("\nReserve:\n");
	DVectorReserve(v, 10); 
	printf("capacity after reserving 10 elements: %ld bytes\n", DVectorCapacity(v));
	printf("peeking into existing elements vs non existent elements:\n");
	printf("peeking into element in index 0: %s\n", *(char **)DVectorGetElement(v, 0));
	/*printf("peeking into element in index 20: %s\n", *(char **)DVectorGetElement(v, 20));*/
	
	
	DVectorDestroy(v);
	v = NULL;
	
	return 0;
}



