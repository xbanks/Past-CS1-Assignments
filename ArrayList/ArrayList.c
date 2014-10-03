/*
	Xavier Banks
	COP 3502C
	Assignment 1 ArrayLists
	2/2/14
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArrayList.h"

// Given Functions in the ArrayList Header
ArrayList *createArrayList(int length);
ArrayList *destroyArrayList(ArrayList *list);
ArrayList *expandArrayList(ArrayList *list, int length);
ArrayList *trimArrayList(ArrayList *list);
char *put(ArrayList *list, char *str);
char *get(ArrayList *list, int index);
char *set(ArrayList *list, int index, char *str);
char *insertElement(ArrayList *list, int index, char *str);
int removeElement(ArrayList *list, int index);
int getSize(ArrayList *list);
void printArrayList(ArrayList *list);

// My own functions
void setSize(ArrayList *list, int size);
int getCapacity(ArrayList *list);
void setCapacity(ArrayList *list, int length);

/*
	Returns an initialized ArrayList object to be used.
	Size is initialized to zero, and capacity is initialized to value length or DEFAULT_INIT_LEN
	whichever is larger.

	Returns NULL if:
		Any of the mallocs fail 
*/
ArrayList *createArrayList(int length)
{
	ArrayList *list = NULL;
	int i;

	// Attempts to create space for the new list
	list = ( ArrayList * ) malloc( sizeof(ArrayList) * 1);

	// If the malloc fails, return NULL
	if(list == NULL)
		return NULL;

	setSize(list, 0);

	// Sets the larger length to the capacity of the ArrayList
	/*
	if(DEFAULT_INIT_LEN > length)
		setCapacity(list, DEFAULT_INIT_LEN);
	else
		setCapacity(list, length);
	*/

	// Another way of doing it
	setCapacity(list, (DEFAULT_INIT_LEN > length ? DEFAULT_INIT_LEN : length) );
	
	list->array = NULL;
	list->array = (char **) malloc( sizeof(char*) * getCapacity(list) );

	// If the malloc fails, return NULL
	if(list->array == NULL)
	{
		// Making sure there is no memory leak
		free(list);
		return NULL;
	}

	// Initializes all elements in the list to NULL
	for (i = 0; i < getCapacity(list); ++i)
		list->array[i] = NULL;

	printf("-> Created new ArrayList of size %d.\n", getCapacity(list));

	return list;
}


/*
	Returns NULL after freeing all elements in the array, along with the array and the ArrayList itself
	Also, if list is NULL then return NULL also
*/
ArrayList *destroyArrayList(ArrayList *list)
{
	int i;

	if(list == NULL)
		return NULL;
	
	// Free's all elements in the list
	for(i = 0; i < getSize(list); i++)
		free(list->array[i]);

	// Free's the array and the ArrayList itself
	free(list->array);
	free(list);

	return NULL;
}


/*
	Expands the given ArrayList by allocating a new array of the given length.

	Returns NULL if:
		Given length is less than current capacity
		Malloc fails
*/
ArrayList *expandArrayList(ArrayList *list, int length)
{
	int i, size;
	char **new_array = NULL;

	if(list == NULL || getCapacity(list) >= length)
		return list;

	new_array = (char **) malloc( sizeof(char *) * length );
	if (new_array == NULL)
		return NULL;

	// Copies all of the elements from the old array into the new, larger one
	size = getSize(list);
	for (i = 0; i < size; i++)
		new_array[i] = list->array[i];

	// Free's the old array and sets the new array in its place
	free(list->array);
	list->array = new_array;

	setCapacity(list, length);

	printf("-> Expanded ArrayList to size %d.\n", getCapacity(list));

	return list;
}


/*
	Trims the ArrayList's capacity to that of the size, if the capacity is larger than the size.

	Returns NULL if:
		Given ArrayList is NULL
		Malloc fails
*/
ArrayList *trimArrayList(ArrayList *list)
{
	int i, size;
	char **new_array = NULL;

	if(list == NULL)
		return NULL;

	// Only does the Trimming operation if the capacity is actually larger than the size
	if(getCapacity(list) > getSize(list))
	{
	 	size = getSize(list);
	 	
	 	new_array = (char **) malloc( sizeof(char *) * size );

	 	// Return NULL if malloc fails
	 	if(new_array == NULL)
	 		return NULL;
		
		// Copies elements from old list into the new one
		for (i = 0; i < size; i++)
			new_array[i] = list->array[i];

		// Free's the old array and replaces it with the new, trimmed one
		free(list->array);
		list->array = new_array;
		setCapacity(list, size);

		printf("-> Trimmed ArrayList to size %d.\n", getCapacity(list));
	}

	return list;
}


/*
	Appends a copy of the given string to the end of the array.
	If there isn't enough space, then the ArrayList will be expanded.

	Returns NULL if:
		Given ArrayList is NULL
		Given String is NULL
*/
char *put(ArrayList *list, char *str)
{
	if (list == NULL || str == NULL)
		return NULL;

	// Expands the array if there isn't enough space for the new string
	if(getSize(list) == getCapacity(list))
		expandArrayList(list, (getCapacity(list)*2 + 1));

	list->array[getSize(list)] = (char *) malloc( sizeof(char) * ( strlen(str)+1 ) );

	// Returns NULL if malloc fails
	if(list->array[getSize(list)] == NULL)
		return NULL;

	strcpy(list->array[getSize(list)], str);
	setSize(list, getSize(list)+1);

	return get(list, getSize(list)-1);
}


/*
	Returns the address of a string at the given index in the given ArrayList.

	Returns NULL if:
		Given ArrayList is NULL
		Given index is invalid
*/
char *get(ArrayList *list, int index)
{ 
	if(list == NULL || index < 0 || index >= getSize(list) || list->array[index] == NULL)
		return NULL;

	return list->array[index];
}


/*
	Replaces whatever string is at the given index with a copy of str.

	Returns NULL if:
		Given ArrayList is NULL
		Given index is invalid (or if the value at the index is NULL)
		Given str is NULL
		Malloc fails
*/
char *set(ArrayList *list, int index, char *str)
{
	if(list == NULL || index < 0 || index >= getCapacity(list) || index >= getSize(list) || list->array[index] == NULL || str == NULL)
		return NULL;
	
	free(list->array[index]);
	list->array[index] = (char *) malloc( sizeof(char) * ( strlen(str) + 1 ) );
	
	// Returns NULL if malloc fails
	if(list->array[index] == NULL)
		return NULL;

	strcpy(list->array[index], str);
	
	return list->array[index];
}


/*
	Shifts all elements at to the at/after the given index to the right and inserts a copy of str at this index
	If the ArrayList does not have enought space for this operation, the list will be expanded

	Returns NULL if:
		Given ArrayList is NULL
		Given str is NULL
		Given index is invalid
		Malloc fails
*/
char *insertElement(ArrayList *list, int index, char *str)
{
	int i;

	if (list == NULL || str == NULL || index < 0 || list->array[index] == NULL)
		return NULL;

	if(getSize(list) == getCapacity(list))
		list = expandArrayList(list, (getCapacity(list)*2 + 1));

	if (index >= getSize(list))
		put(list, str);
	else
	{
		
		for(i = getSize(list); i > index; i--)
		{
			list->array[i] = (char *) malloc( sizeof(char) * ( strlen(list->array[i-1]) + 1) );

			// Returns NULL if malloc fails
			if(list->array[i] == NULL)
				return NULL;

			// Replaces the current element with the one to the left of it
			strcpy(list->array[i], list->array[i-1]);
			free(list->array[i-1]);
		}

		list->array[index] = (char *) malloc( sizeof(char) * (strlen(str) + 1) );

		// Returns NULL if malloc fails
		if(list->array[index] == NULL)
			return NULL;

		// Adds the given string to the array
		strcpy(list->array[index], str);
		setSize(list, getSize(list)+1);
	}

	return list->array[index];
}


/*
	Removes the string at index and shifts all elements to the right of index one space to the left.
	
	Returns NULL if:
		Given ArrayList is NULL
		Given index is invalid (or if the element at this index is NULL)
		Malloc fails
*/
int removeElement(ArrayList *list, int index)
{
	int i, size;

	if (list == NULL || index < 0 || index >= getSize(list) || list->array[index] == NULL)
		return 0;
	
	size = getSize(list);
	for(i = index; i < size-1; i++)
	{
		free(list->array[i]);
		list->array[i] = (char *) malloc( sizeof(char) * ( strlen( list->array[i+1] ) + 1) );

		// Returns NULL if malloc fails
		if(list->array[i] == NULL)
			return 0;

		// Replaces the current element with the one to the right of it
		strcpy(list->array[i], list->array[i+1]);
	}


	setSize(list, getSize(list)-1);

	// Free's the last (now empty) element in the array
	free(list->array[getSize(list)]);
	
	return 1;
}


/*
	Returns the size of the given ArrayList or returns -1 if the ArrayList is NULL
*/
int getSize(ArrayList *list)
{
	if (list == NULL)
		return -1;
	
	return list->size;
}


/*
	Sets the size value of the given ArrayList to given size
	does nothing if the list is NULL or the size is invalid
*/
void setSize(ArrayList *list, int size)
{
	if(list != NULL &&  size >= 0)
		list->size = size;
}


/*
	Returns the capacity of the given ArrayList or -1 if the ArrayList is NULL
*/
int getCapacity(ArrayList *list)
{
	if (list == NULL)
		return -1;

	return list->capacity;
}


/*
	Sets the capacity value of the given ArrayList to given length

	Does nothing if:
		Given ArrayList is NULL
		Given length is invalid
*/
void setCapacity(ArrayList *list, int length)
{
	if (list != NULL && length >= 0 && length >= getSize(list) )
		list->capacity = length;
}


/*
	Prints the strings within the given ArrayList with one string per line.

	Prints "(empty list)" if:
		Given ArrayList is NULL
		The size of the list is less than 1 (the list has no elements in it)
*/
void printArrayList(ArrayList *list)
{
	int size, i;

	if(list == NULL || getSize(list) < 1)
		printf("(empty list)\n");
	else
	{
		size = getSize(list);
		for(i = 0; i < size; i++)
			printf("%s\n", list->array[i]);
	}
}
