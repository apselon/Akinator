#include <stdio.h>
#include <stdlib.h>
#include "../Tree/Tree.cpp"


FILE* data_source = fopen("tree.txt", "rw");
const int MAX_BUFF_SIZE = 512;

void ask_question(Tree::tree_t* answers, Tree::node_t* cur);
void make_guess(Tree::tree_t* answers, Tree::node_t* cur);
void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character);
