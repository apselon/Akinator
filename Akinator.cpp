#include "Akinator.hpp"

void ask_question(Tree::tree_t* answers, Tree::node_t* cur){
	
	Assert(answers != NULL);
	Assert(cur != NULL);

	if (!cur->has_left && !cur->has_rigth){
		make_guess(answers, cur);
	}

	printf("%s?\n", cur->val);

	char resp = getchar();;

	if (resp == 'y'){
		ask_question(answers, cur->left);
	} else {
		ask_question(answers, cur->right);
	}

}


void make_guess(Tree::tree_t* answers, Tree::node_t* leaf){
	
	Assert(answers != NULL);
	Assert(leaf != NULL);

	printf("it is %s, right?\n", leaf->val);

	char resp = getchar();

	if (resp == 'y'){
		printf("I knew it\n");
	} else {
		add_new_character(answers, leaf);	
	}
}


void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character, FILE* source){

	Assert(answers != NULL);
	Assert(old_character != NULL);

	Tree::node_t* new_character = NEW_NODE;
	printf("What character did you have in minde?\n");
	scanf("%ms", &new_character->val);

	Tree::node_t* new_question = NEW_NODE;
	printf("What fact differs %s from %s?\n", new_character->val, old_character->val);
	scanf("%ms", &new_question->val);

	Tree::attach_left(answers, new_question, new_character);
	Tree::attach_right(answers, new_question, old_character);
	Tree::replace_left(old_character, new_question);

	Tree::save_to_file(answers, source);

}

