#include <stdio.h>
#include <stdlib.h>
#include "../Tree/Tree.cpp"


#define SAY(...){ 												   			\
	char str_buff[MAX_BUFF_SIZE] = "";								    	\
	char say_buff[MAX_BUFF_SIZE] = "";								    	\
	sprintf(str_buff, __VA_ARGS__); 								    	\
	sprintf(say_buff, "espeak \"%s\" -s 170 2> /dev/null \n", str_buff); 	\
	printf("%s", str_buff);													\
	system(say_buff);												    	\
}																    		\


char tree_filename[] = "tree.txt";
const int MAX_BUFF_SIZE = 512;

void ask_question(Tree::tree_t* answers, Tree::node_t* cur, FILE* data_source);
void make_guess(Tree::tree_t* answers, Tree::node_t* cur, FILE* data_source);
void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character, FILE* data_source);
