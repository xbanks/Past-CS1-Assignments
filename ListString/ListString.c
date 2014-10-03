/*
	Xavier Banks
	Program 3
	ListString.c
	COP3502
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListString.h"

// My functions
node *obliterate(node *head);
node *addToLast(node *head, node *last);

// Main function to open and read from given file
int main(int argc, char **argv){
	char command, *file, buffer[1023], *str, key;
	node *head;
	FILE *ifp;

	// Making sure there is a second argument given
	if(argc != 2){
		printf("INVALID: number of arguments\n");
		exit(1);
	}

	ifp = fopen(argv[1], "r");

	// Gets the string that we're going to be working with
	fgets(buffer, 1023, ifp);
	str = malloc(sizeof(char) * strlen(buffer));
	strcpy(str, buffer);

	// Converts working string into a Linked List
	// and gets rid of any unwanted end line characters
	head = stringToList(str);
	head = replaceChar(head, '\n', NULL);

	// Debug to make sure head Linked List gets made
	if(head == NULL)
	{
		printf("head wasn't initialized\n");
		return 1;
	}

	// Goes through the commands in the file
	while(fscanf(ifp, "%c", &command) != EOF){
		// Chooses what to do with given commands
		switch(command){
			case '!':
				printList(head);
				break;
			case '~':
				head = reverseList(head);
				break;
			case '@':
				fscanf(ifp, " %c %s", &key, str);
				head = replaceChar(head, key, str);
				break;
			case '-':
				fscanf(ifp, " %c", &key);
				head = replaceChar(head, key, NULL);
				break;
		}
	}

	head = obliterate(head);
	free(str);
	fclose(ifp);
	return 0;
}

// Converts the given string to a Linked List
node *stringToList(char *str){
	int len, i;
	node *head;

	if(str == NULL || str == "" || str == "\0" || str == "\n" || strlen(str) == 0)
		return NULL;

	head = malloc(sizeof(node) * 1);
	head->data = *str;

	// Passes the location of the next letter
	// to next recursive call
	head->next = stringToList( str+1 );

	return head;
}

// Replaces the key character in the list with a certain string
node *replaceChar(node *head, char key, char *str){
	node *next;

	if(head == NULL)
		return NULL;

	head->next = replaceChar(head->next, key, str);

	// Done after recursive call so it doesn't interfere
	// with the rest of the nodes afterwards
	if(head->data == key){
		// Deletes the node 
		if(str == NULL || str == ""){
			next = head->next;
			free(head);
			head = next;
		}	
		// Replaces the node
		else{
			next = head->next;
			head = stringToList(str);
			head = addToLast(head, next);
		}
	}

	return head;
}

// Reverses the Linked List
node *reverseList(node *head){
	node *temp, *pre;

	pre = NULL;
	while(head != NULL){
		temp = head->next;
		head->next = pre;
		pre = head;
		head = temp;
	}
	
	return pre;
}

// Prints the Linked List String
void printList(node *head){
	node *temp;

	if(head == NULL){
		printf("(empty string)\n");
		return;
	}

	temp = head;

	while(temp != NULL){
		printf("%c", temp->data);
		temp = temp->next;
	}

	obliterate(temp);
	printf("\n");
}

// Adds a node to the last portion of the given List
node *addToLast(node *head, node *last){
	if(head == NULL){
		head = last;
		return head;
	}

	head->next = addToLast(head->next, last);
	return head;
}

// Destroys the Linked List
node *obliterate(node *head){
	if(head == NULL)
		return NULL;

	head->next = obliterate(head->next);
	free(head);
	return head;
}