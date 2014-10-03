/*
	Xavier Banks
	Assigment 4 Tries
	COP3100C
	4/20/14
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trie.h"

TrieNode *buildTrie(char *filename);
TrieNode *createTrieNode(void);
TrieNode *getNode(TrieNode *root, char *str);
void insertString(TrieNode *root, char *str);
void printTrie(TrieNode *root);
void printTrie_Helper(TrieNode *root, char *str);
void addSubtries(TrieNode *root, char array[20][1024], int length);
void printSubTrie(TrieNode *root);
void strToLower(char *str);
TrieNode *destroyTrie(TrieNode *root);

// Inputs files and runs the whole program
int main(int argc, char const *argv[])
{
	char corpus[1024], input[1024], str[1024];
	TrieNode *trie, *subtrie;
	FILE *ifile;
	int i, len;

	// getting filenames
	strcpy(input, argv[2]);
	strcpy(corpus, argv[1]);

	trie = buildTrie(corpus);
	ifile = fopen(input, "r");
	if(ifile == NULL)
		return 1;

	// goes through and reads from the input file
	while(!feof(ifile)){

		fscanf(ifile ,"%s", str);

		if(*str == '!'){
			printTrie(trie);
		}
		else if(strcmp(str, "") == 0){} // Do nothing if it's the end of file
		else
		{
			printf("%s\n", str);
			strToLower(str);
			subtrie = getNode(trie, str);

			if(subtrie == NULL)
				printf("(INVALID STRING)\n");
			else
				printSubTrie(subtrie);
		}
		strcpy(str, "");
	}

	fclose(ifile);
	destroyTrie(trie);
	return 0;
}

// Builds the trie using the given filename
TrieNode *buildTrie(char *filename)
{
	char str[1024], array[20][1024];
	TrieNode *trie;
	FILE *ifp;
	int i = 0, j, len;

	ifp = fopen(filename, "r");
	if(ifp == NULL)
		return NULL;

	trie = createTrieNode();

	while(!feof(ifp)){

		fscanf(ifp ,"%s", str);
		strToLower(str);

		if(*str != '.'){
			insertString(trie,str);
			strcpy(array[i], str);
			i++;
		}
		else
		{
			addSubtries(trie, array, i);
			for (j = 0; i < i; j++)
				free(array[i]);

			i = 0;
		}

	}
	fclose(ifp);
	return trie;
}

// Used for creating nodes
TrieNode *createTrieNode(void)
{
	int i;
	TrieNode *node = malloc(sizeof(TrieNode) * 1);	
	node->count = 0;
	for(i = 0; i < 26; i++)
		node->children[i] = NULL;
	node->subtrie = NULL;
	return node;
}

// Finds the node pointer for given string
TrieNode *getNode(TrieNode *root, char *str)
{
	int ascii;
	if(root == NULL)
		return NULL;

	if(root->count > 0 && strlen(str) == 0)
		return root;
	else if(root->count == 0 && strlen(str) == 0)
		return NULL;

	ascii = tolower(*str) - 'a';
	if(root->children[ascii] != NULL)
		return getNode(root->children[ascii], str+1);
}

// Inserts string into trie
void insertString(TrieNode *root, char *str)
{
	int ascii;
	if(root == NULL)
		return;
	// Increment's string count
	if(*str == '\0'){
		root->count++;
		return;
	}

	ascii = *str - 'a';
	if(root->children[ascii] == NULL)
		root->children[ascii] = createTrieNode();

	insertString(root->children[ascii], str+1);
}

// Adds subtries when needed
void addSubtries(TrieNode *root, char array[20][1024], int length)
{
	int i, j, k = 0;
	TrieNode *trie;

	if(length <= 1)
		return;

	// Goes through the string array and adds subtries
	for(i = 0; i < length; i++){

		// Making sure there's no duplicate addition in subtries
		for(j = 0; j < i; j++)
			if( strcmp(array[i], array[j]) == 0 )
				k = 1;

		if(k == 0)
		{
			trie = getNode(root, array[i]);
			if(trie == NULL)
				return;

			for(j = 0; j < length; j++)
			{
				// strings will not co-occur with themselves
				if( strcmp(array[i], array[j]) != 0){
					if(trie->subtrie == NULL)
						trie->subtrie = createTrieNode();
					insertString(trie->subtrie, array[j]);
				}
			}
		}
		k = 0;
	}
}

// Print's subtries
void printSubTrie(TrieNode *root)
{
	char str[1024];
	strcpy(str, "- ");

	if(root->subtrie == NULL)
		printf("(EMPTY)\n");
	else
		printTrie_Helper(root->subtrie, str);
}

// Prints Trie
void printTrie(TrieNode *root)
{
	char str[1024];
	strcpy(str, "");
	printTrie_Helper(root, str);
}

void printTrie_Helper(TrieNode *root, char *str)
{
	int i, len;
	char curr[1024], ch;
	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", str ,root->count);

	strcpy(curr, str);

	for (i = 0; i < 26; i++)
	{
		if(root->children[i] != NULL)
		{
			strcpy(curr, str);
			ch = 'a' + i;
			len = strlen(curr);
			curr[len] = ch;
			curr[len+1] = '\0';
			printTrie_Helper(root->children[i], curr);
		}
	}
}

// Function used to keep all strings lowercase for consistency
void strToLower(char *str)
{
	int len, i;
	len = strlen(str);

	for(i = 0; i < len; i++)
		str[i] = tolower(str[i]);
}

// Frees up memory from tries
TrieNode *destroyTrie(TrieNode *root)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0 && root->subtrie != NULL)
		destroyTrie(root->subtrie);


	for(i = 0; i < 26; i++)
	{
		if(root->children[i] != NULL)
		{
			destroyTrie(root->children[i]);
			// free(root->children[i]);
		}
	}
	// free(root->children);
	free(root);
}