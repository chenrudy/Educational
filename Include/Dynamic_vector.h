/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 Author: Chen Rudy     
 Date: 9 Mar 2022
 Reviewed By: Hagar Gur-Arie  

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

***************************************************************************
                                    DS
*****************************************************************************    
DS is a linear data structure

attributes and policy:
-	random access
-	Holds elements by value
-	growth factor - dynamic. when reaches last element, doubles capacity. when number of elements is 25% of capacity, capacety shrinks by 50%.

-------------------------------------------------------------------------------

Operations on DS:
- create vector
- destroy vector
- push back  
- pop back
- get value of  the index place in vector (star from 1)
- get element capacity
- get the number of elements in vector
- get if vector is empty
- resize vector
- fit vector to minimaize size

*******************************************************************************/
                      
#ifndef __DYNAMIC_VECTOR_H__
#define __DYNAMIC_VECTOR_H__

#include <stddef.h> /* size_t */

/*******************************************************************************
                                 Typedefs
*******************************************************************************/
typedef struct vector d_vector_t; 

/*******************************************************************************
                                Functions
*******************************************************************************/

d_vector_t *DVectorCreate(size_t element_size, size_t num_of_elements);
/******************************************************************************
DVectorCreate
Description: creates a new dynamic vector
Parameters: 
	- num_of_element: initial number of elements to be stored. 
	- element_size: sizeof of indevidual element
Return value:
	- a pointer to the dynamic vector
	- in case of failure, will return null
Complexity: O(1) time
Notes: if num of elements is zero, default capacity will be given.

*****************************************************************************/

void DVectorDestroy(d_vector_t *vector);
/******************************************************************************
DVectorDestroy
Description: Destroys given dynamic vector
Parameters: 
	- vector to be destroyed. 
Return value:
	- void
Complexity: O(1) time
	 
*****************************************************************************/

size_t DVectorSize(const d_vector_t *vector);
/******************************************************************************
DVectorSize
Description: calculates num of elements in vector
Parameters: 
	- vector 
Return value:
	-count of elements in vector
Complexity: O(1) time
Notes: 

*****************************************************************************/

size_t DVectorCapacity(const d_vector_t *vector);
/******************************************************************************
DVectorCapacity
Description: calculates current capacity of vector by bytes
Parameters: 
	- vector 
Return value:
	-sizeof vector 
Complexity: O(1) time
Notes: 

*****************************************************************************/

int DVectorPushBack(d_vector_t *vector,const void *data);
/******************************************************************************
DVectorPushBack
Description: addes elements to the end of the vector
Parameters: 
	- vector 
	- void pointer to data
Return value:
	- in case of success, will return - 0
	- in case of realloc failure, will return - 1
Complexity: O(n) time
Notes: if after addition the vector is full - doubles vectors capacity. if realloc failes - returns 1: pushed the element but vector is in full capacity so no more pushes can be made.

*****************************************************************************/

int DVectorPopBack(d_vector_t  *vector);
/******************************************************************************
DVectorPopBack
Description: deletes elements from the end of the vector
Parameters: 
	- vector 
Return value:
	- in case of success, will return - 0
	- in case of failure, will return - 1
Complexity: O(1) time
Notes: if num of elements is 25% of capacity, shrinks capacity to 50%

*****************************************************************************/

void *DVectorGetElement(const d_vector_t *vector, size_t index);
/******************************************************************************
DVectorGetElement
Description: gets value by index
Parameters: 
	- vector
	- index to element 
Return value:
	- returns a pointer to selected element
	- in case of failure, will return null
Complexity: O(1) time
Notes: 
*****************************************************************************/

int DVectorIsEmpty(const d_vector_t *vector);
/******************************************************************************
DVectorIsEmpty
Description: check if the vector is empty
Parameters: 
	- vector 
Return value:
	- in case vector is empty, will return - 0
	- in case vector is not empty, will return - 1
Complexity: O(1) time
Notes:

*****************************************************************************/

int DVectorReserve(d_vector_t *vector, size_t new_num_of_elements);
/******************************************************************************
DVectorReserve
Description: reserve more memory for vectors capacity 
Parameters: 
	- vector 
	- number of elements to allocate additional memory for
Return value:
	- in case of success, will return - 0
	- in case of failure, will return - 1
Complexity: O(1) time
Notes: if given new_num_of_elements is smaller then existing number of elements, will shrink vector to the size of its elements + 1 extra element space

*****************************************************************************/

int DVectorShrinkToFit(d_vector_t *vector);
/******************************************************************************
DVectorShrinkToFit
Description: shrinks memory allocated to vector to fit the elements
Parameters: 
	- vector 
Return value:
	- in case of success, will return - 0
	- in case of failure, will return - 1
Complexity: O(1) time
Notes: returns with 1 extra space for element

*****************************************************************************/

#endif /*__DYNAMIC_VECTOR_H__*/
