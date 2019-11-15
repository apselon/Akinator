#include <stdio.h>
#include <stdlib.h>
#include "../Tree/Tree.cpp"


char tree_filename[] = "tree.txt";
const int MAX_BUFF_SIZE = 512;

void ask_question(Tree::tree_t* answers, Tree::node_t* cur, FILE* data_source);
void make_guess(Tree::tree_t* answers, Tree::node_t* cur, FILE* data_source);
void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character, FILE* data_source);
