#include <stdio.h>
#include <stdlib.h>
#include "../Tree/Tree.cpp"

#define Assert(x) ;

FILE* data_source = fopen("Akin_tree.txt", "r");

void ask_question(Tree::tree_t* answers, Tree::node_t* cur);
void make_guess(Tree::tree_t* answers, Tree::node_t* cur);
void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character, FILE* source = data_source);
