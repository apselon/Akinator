#include "Akinator.hpp"

void ask_question(Tree::tree_t* answers, Tree::node_t* cur, FILE* data_source){
	
	Assert(answers != NULL);
	Assert(cur != NULL);

	if (!cur->has_left && !cur->has_rigth){
		make_guess(answers, cur, data_source);
		return;
	}

	SAY("%s?\n", cur->val);

	char resp[MAX_BUFF_SIZE] = ""; 
	fgets(resp, MAX_BUFF_SIZE, stdin);

	if (strstr(resp, "yes") != NULL){
		ask_question(answers, cur->left, data_source);
	} else {
		ask_question(answers, cur->right, data_source);
	}

}


void make_guess(Tree::tree_t* answers, Tree::node_t* leaf, FILE* data_source){
	
	Assert(answers != NULL);
	Assert(leaf != NULL);

	SAY("I'm pretty sure it is %s. Am I right?\n", leaf->val);

	char resp[MAX_BUFF_SIZE] = ""; 
	fgets(resp, MAX_BUFF_SIZE, stdin);

	if (strstr(resp, "yes") != NULL){
		SAY("I knew it!\n");
	} else {
		add_new_character(answers, leaf, data_source);	
	}
}


void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character, FILE* data_source){

	Assert(answers != NULL);
	Assert(old_character != NULL);

	char resp[MAX_BUFF_SIZE] = ""; 
	Tree::node_t* new_character = NEW_NODE;
	Tree::node_t* new_question = NEW_NODE;

	do {
		SAY("I'm intrigued, so what did you have in mind?\n");
		scanf("%m[^\n]", &new_character->val);
		getchar();

		SAY("I don't really know this guy, what is the difference between %s and %s?\n", new_character->val, old_character->val);
		scanf("%m[^\n]", &new_question->val);
		getchar();

		SAY("Are you sure that in contrast to %s, %s is %s?", old_character->val, new_character->val, new_question->val);

		fgets(resp, MAX_BUFF_SIZE, stdin);

	} while (strstr(resp, "yes") == NULL);

	if (old_character == old_character->parent->left){
		Tree::replace_left(old_character->parent, new_question);
	}

	if (old_character == old_character->parent->right){
		Tree::replace_right(old_character->parent, new_question);
	}

	Tree::attach_left(answers, new_question, new_character);
	Tree::attach_right(answers, new_question, old_character);

	data_source = freopen(tree_filename, "w", data_source);
	Tree::save_to_file(answers, data_source);

	SAY("Thank you, even genius should learn something new.\n");

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


void print_definition(Tree::node_t* cur, Tree::node_t* finish, bool* flag){

	Assert(cur != NULL);


	if (cur->parent == NULL){
		return;
	}

	if (cur->parent != finish){
		print_definition(cur->parent, finish, flag);	
	}


	if (cur == cur->parent->left){
		SAY("%s, ", cur->parent->val);
		fflush(stdout);
		*flag = 1;
	}

	if (cur == cur->parent->right){
		SAY("not %s, ", cur->parent->val);
		fflush(stdout);
		*flag = 1;
	}

}

void guessing_mode(){

	Tree::tree_t answers = {}; 
	char resp[MAX_BUFF_SIZE] = ""; 
	char command = 0;

	do {
		FILE* data_source = fopen(tree_filename, "r");
		answers = Tree::read_from_file(data_source);

		ask_question(&answers, answers.root, data_source);
		fclose(data_source);

		printf("Press e to quit to the menu\n");
		printf("Press q to exit from the game\n");
		command = getchar();

	} while (command != 'e' && command != 'q');

	if (command == 'q') exit(EXIT_SUCCESS);
	Tree::destruct(answers.root);
}


void definition_mode(){

	Tree::tree_t answers = {}; 

	char resp[MAX_BUFF_SIZE] = ""; 
	bool flag = 0;
	char command = 0;

	do {
		FILE* data_source = fopen(tree_filename, "r");
		answers = Tree::read_from_file(data_source);

		Tree::node_t* desired = {};

		do {
			SAY("Difinition of what you wanna know?\n");
			scanf("%[^\n]", resp);
			getchar();
			desired = Tree::find_node_by_val(answers.root, resp, strcmp, answers.size);	
		} while (desired == NULL);

		print_definition(desired, answers.root, &flag);
		SAY("I am certanly a genius\n\n")

		fclose(data_source);

		printf("Press e to quit to the menu\n");
		printf("Press q to exit from the game\n");
		command = getchar();

	} while (command != 'e' && command != 'q');
	if (command == 'q') exit(EXIT_SUCCESS);
}


void comaparison_mode(){

	Tree::tree_t answers = {}; 

	char resp[MAX_BUFF_SIZE] = ""; 

	Tree::node_t* first = {};
	Tree::node_t* second = {};
	Tree::node_t* common = {};

	bool has_property = 0;
	char command = 0;

	do {
		FILE* data_source = fopen(tree_filename, "r");
		answers = Tree::read_from_file(data_source);


		do {
			SAY("What do you want to compare?\n");
			scanf("%[^\n]", resp);
			getchar();

			first = Tree::find_node_by_val(answers.root, resp, strcmp, answers.size);	
			if (first == NULL) continue;
		
			SAY("What do you want to compare %s with?\n", first->val);
			scanf("%[^\n]", resp);
			getchar();
		
			second = Tree::find_node_by_val(answers.root, resp, strcmp, answers.size);	
			if (second == NULL) continue;

			common = Tree::find_common_ancestor(first, second);	
		} while (first == NULL || second == NULL);


		SAY("Actually, %s and %s have in common: ", first->val, second->val);
		print_definition(common, answers.root, &has_property);
		if (!has_property){
			SAY("nothing, ");
		}

		SAY("however, %s is: ", first->val);
		print_definition(first, common, &has_property);
		SAY("when %s is: ", second->val);
		print_definition(second, common, &has_property);
		SAY("I am certanly a genius.\n");
		fclose(data_source);

		printf("Press e to quit to the menu\n");
		printf("Press q to exit from the game\n");
		command = getchar();
		
	} while (command != 'e' && command != 'q');
	if (command == 'q') exit(EXIT_SUCCESS);

	destruct(answers.root);
}

void show_tree(){
	Tree::tree_t answers = {}; 
	FILE* data_source = fopen(tree_filename, "r");

	answers = Tree::read_from_file(data_source);
	draw(&answers, 0);

	fclose(data_source);
}

int main(){	
		SAY("Hello dear, come in, you'll be my guest\n");
		int choice = 0;
	do {
		SAY("Do you want to:\n");
		SAY("\t1 - know a defintion of something?\n");
		SAY("\t2 - see a difference between two things?\n");
		SAY("\t3 - play a game with me?\n");
		SAY("\t4 - get my full knowledge?\n");
		SAY("\t5 - say goodbye?\n");

		scanf("%d", &choice);
		getchar();
		switch (choice){
			
			case 1: definition_mode();
					break;
			case 2: comaparison_mode();
					break;
			case 3: guessing_mode();
					break;
			case 4: show_tree();
					break;
			case 5: return 0;
		}

	} while (true);

}
