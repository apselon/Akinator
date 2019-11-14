#include "Akinator.hpp"

void ask_question(Tree::tree_t* answers, Tree::node_t* cur){
	
	Assert(answers != NULL);
	Assert(cur != NULL);

	if (!cur->has_left && !cur->has_rigth){
		make_guess(answers, cur);
		return;
	}

	printf("Akk: %s?\n", cur->val);

	char resp[MAX_BUFF_SIZE] = ""; 
	fgets(resp, MAX_BUFF_SIZE, stdin);

	if (strstr(resp, "Yes") != NULL){
		ask_question(answers, cur->left);
	} else {
		ask_question(answers, cur->right);
	}

}


void make_guess(Tree::tree_t* answers, Tree::node_t* leaf){
	
	Assert(answers != NULL);
	Assert(leaf != NULL);

	printf("Akk: I'm pretty sure it is %s. Am I right?\n", leaf->val);

	char resp[MAX_BUFF_SIZE] = ""; 
	fgets(resp, MAX_BUFF_SIZE, stdin);

	if (strstr(resp, "да") != NULL){
		printf("Akk: I knew it!!!\n");
	} else {
		add_new_character(answers, leaf);	
	}
}


void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character){

	Assert(answers != NULL);
	Assert(old_character != NULL);

	Tree::node_t* new_character = NEW_NODE;
	printf("Akk: I'm intrigued, so what did you have in mind?\n");
	scanf("%m[^\n]", &new_character->val);
	getchar();

	Tree::node_t* new_question = NEW_NODE;
	printf("Akk: I don't really know what is the difference between %s and %s?\n", new_character->val, old_character->val);
	scanf("%m[^\n]", &new_question->val);
	getchar();

	if (old_character == old_character->parent->left){
		Tree::replace_left(old_character->parent, new_question);
	}

	if (old_character == old_character->parent->right){
		Tree::replace_right(old_character->parent, new_question);
	}

	Tree::attach_left(answers, new_question, new_character);
	Tree::attach_right(answers, new_question, old_character);

	Tree::save_to_file(answers, data_source);

}


int list_vals_from_node_to_ancestor(Tree::node_t* cur, Tree::node_t* ancestor, Tree::elem_t* buffer){

	Assert(cur != NULL);
	Assert(ancestor != NULL);
	Assert(buffer != NULL);

	if (cur->level > ancestor->level){
		return 0;
	}

	int len = 0;
	
	while (cur != ancestor){
		buffer[len] = cur->val;
		++len;
		if (!cur->has_parent) break;
		cur = cur->parent;
	}

	return len;
}


Tree::node_t* find_common_ancestor(Tree::node_t* first, Tree::node_t* second){
	
	Assert(first != NULL);	
	Assert(second != NULL);	

	while (first->level > second->level){
		first = first->parent;
	}

	while (second->level > first->level){
		second = second->parent;
	}

	while (first != second){
		first = first->parent;
		second = second->parent;
	}

	return first;
}


int main(){
	Tree::tree_t answers = {}; 

	char resp[MAX_BUFF_SIZE] = ""; 

	do {
		fseek(data_source, 0, SEEK_SET);

		answers = Tree::read_from_file(data_source);
		ask_question(&answers, answers.root);

		printf("Akk: Do you wanna play again?\n");
		fgets(resp, MAX_BUFF_SIZE, stdin);
		if (strstr(resp, "no") != NULL) break;
	} while (true);

	fclose(data_source);
}
