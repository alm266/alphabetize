//Andrew Marshall - alm266
//Brandon Diaz - brd48

//Systems Programming
//Fall 2016
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
	cnodes are used to create a trie. Every node has an array whose indices correspond
	to the alphabetic characters - 26 lowercase letters
*/

typedef struct Cnode{
	char val;	//character value for this node itself
	struct Cnode *charlist[26];	//26 indices for letters
	int endWord;	//0 - no, greater - yes
}cnode;

//global root of trie. initialized at NULL
static cnode * trie;

//helper method to print recursively
int printHelp(cnode * n, char *word){
	//base case (null check):
	if(n==NULL){
		return 0;
	}
	
	int k = strlen(word);
	word[k] = n->val;	//add additional char in order to concatenate string
	int i;
	int validNodes = 0;
	
	//print short word(s) first in order to remain alphabetical
	if(n->endWord > 0){
		int j = 0;
		for(; j<n->endWord; j++){
			printf("%s\n", word);
		}
		++validNodes;
	}
	
	//recursively traverse the cnodes in the trie
	//print word when 'endWord' is 1
	//maintain NULL terminating char at end of word
	for(i=0; i<26; i++){
		if(n->charlist[i]){
			printHelp((n->charlist[i]), word);
			word[strlen(word)-1] = '\0';
			++validNodes;
		}
	}
	
	return validNodes;
}

void printTrie(){
	char word[2056];
	bzero(word, 2056);
	
	cnode * root = trie;
	int success = printHelp(root, word);
	if(success == 0)
		printf("No words were given\n");
	return;
}

void addToTrie(char line[]){
	cnode *t = trie;
	int i = 0;
	int cIndex = 0;
	
	for(; i<=strlen(line)-1; i++){
		//convert the letter to lowercase
		line[i] = tolower(line[i]);
		//convert the character ASCII value to an index on the array
		cIndex = (int)(line[i]) - 97;
		
		//make new cNode and put it in this position
		if(t->charlist[cIndex] == NULL){
			cnode *tmp = (cnode *)malloc(sizeof(cnode));
			tmp->val = line[i];
			t->charlist[cIndex] = tmp;
			t = tmp;
		}
		//use existing cNode
		else{
			t = t->charlist[cIndex];
		}
	}
	
	//this is the last char of a word
	t->endWord++;
	return;
}

void freehelp(cnode * n){
	//base case (null check):
	if(n==NULL){
		return;
	}
	int i = 0;
	
	for(; i<26; i++){
		if(n->charlist[i]){
			freehelp(n->charlist[i]);
		}
	}
	
	free(n);
	return;
}

void freeTrie(){
	int i = 0;
	cnode * n = NULL;
	for(; i<26; i++){
		
		if(trie->charlist[i]){
			n = trie->charlist[i];
			freehelp(n);
		}
	}
	
	free(trie);
	return;
}

int main(int argc, char **argv){
	//Print errors about invalid number of arguments
	if(argc < 2){
	  printf("You didn't provide enough arguments! I need at least one.\n");
	  //Error code 1: One too few arguments
	  return 1;
	}
	else{
	  if(argc > 2){
	    //Error code 2: Too many arguments
	    printf("I can't handle %d arguments!\nCan you try just one instead?\n",argc-1);
	    return 2;
	  }
	}
	int inputLength = strlen(argv[1]);
	if(inputLength > 2000){
		//Error code 3: Too many characters
		printf("That looks like way too many characters!\nGive me less than 2000 please.");
		return 3;
	}
	
	//initialize trie root
	trie = (cnode *)malloc(sizeof(cnode));
	trie->val = '\0';
	//zero all char values in the root of the trie
	bzero(trie->charlist, 26);
	
	int mainLoopPosition = 0;
	int substringSize = 0;
	int newWordStart = 0;
	//parse the input for valid words
	for(; mainLoopPosition<inputLength+1; ++mainLoopPosition){
		if(isalpha(argv[1][mainLoopPosition]) == 0){
			//we found a non-letter (delimiter)
			if(substringSize != 0){
				//if we have found letters before this, then we are ready to enter those previous letters as a word
				char *tempStr = malloc( (substringSize+1) * sizeof(char) );
				strncpy(tempStr, &argv[1][newWordStart], substringSize);
				addToTrie(tempStr);
				substringSize = 0;
				newWordStart = mainLoopPosition +1;
				free(tempStr);
			}
			else{
				//else we just ran into another delimiter and we need to start looking for words after this
			  	++newWordStart;
			}
		}
		else{
			//we found a letter! Increment the size of the expected word
			++substringSize;
		}
	}
	//print the sorted list of words
	printTrie();
	
	//clean up
	freeTrie();
	
	return 0;
}
