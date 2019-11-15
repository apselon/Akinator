#include "Akinator.hpp"

void ask_question(Tree::tree_t* answers, Tree::node_t* cur, FILE* data_source){
	
	Assert(answers != NULL);
	Assert(cur != NULL);

	if (!cur->has_left && !cur->has_rigth){
		make_guess(answers, cur, data_source);
		return;
	}

	printf("Akk: %s?\n", cur->val);

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

	printf("Akk: I'm pretty sure it is %s. Am I right?\n", leaf->val);

	char resp[MAX_BUFF_SIZE] = ""; 
	fgets(resp, MAX_BUFF_SIZE, stdin);

	if (strstr(resp, "yes") != NULL){
		printf("Akk: I knew it!!!\n");
	} else {
		add_new_character(answers, leaf, data_source);	
	}
}


void add_new_character(Tree::tree_t* answers, Tree::node_t* old_character, FILE* data_source){

	Assert(answers != NULL);
	Assert(old_character != NULL);

	Tree::node_t* new_character = NEW_NODE;
	printf("Akk: I'm intrigued, so what did you have in mind?\n");
	scanf("%m[^\n]", &new_character->val);
	getchar();

	Tree::node_t* new_question = NEW_NODE;
	printf("Akk: I don't really know this guy, what is the difference between %s and %s?\n", new_character->val, old_character->val);
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

	data_source = freopen(tree_filename, "w", data_source);
	Tree::save_to_file(answers, data_source);
	fclose(data_source);

	printf("Akk: Thank you, even genius should learn something new.\n");

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
		printf("%s, ", cur->parent->val);
		*flag = 1;
	}

	if (cur == cur->parent->right){
		printf("not %s, ", cur->parent->val);
		*flag = 1;
	}

}

void guessing_mode(){

	Tree::tree_t answers = {}; 

	char resp[MAX_BUFF_SIZE] = ""; 

	do {
		FILE* data_source = fopen(tree_filename, "r");
		answers = Tree::read_from_file(data_source);

		ask_question(&answers, answers.root, data_source);
		fclose(data_source);

		printf("press e to quit to the menu\n");

	} while (getchar() != 'e');

	Tree::destruct(answers.root);
}


void definition_mode(){

	Tree::tree_t answers = {}; 

	char resp[MAX_BUFF_SIZE] = ""; 
	bool flag = 0;

	do {
		FILE* data_source = fopen(tree_filename, "r");
		answers = Tree::read_from_file(data_source);

		Tree::node_t* desired = {};

		do {
			printf("Difinition of what you wanna know?\n");
			scanf("%[^\n]", resp);
			getchar();
			desired = Tree::find_node_by_val(answers.root, resp, strcmp, answers.size);	
		} while (desired == NULL);

		printf("Akk: ");
		print_definition(desired, answers.root, &flag);
		printf("\n\n");

		fclose(data_source);

		printf("press e to quit to the menu\n");

	} while (getchar() != 'e');
}


void comaparison_mode(){

	Tree::tree_t answers = {}; 

	char resp[MAX_BUFF_SIZE] = ""; 

	Tree::node_t* first = {};
	Tree::node_t* second = {};
	Tree::node_t* common = {};

	bool has_property;

	do {
		FILE* data_source = fopen(tree_filename, "r");
		answers = Tree::read_from_file(data_source);


		do {
			printf("What do you want to compare?\n");
			scanf("%[^\n]", resp);
			getchar();

			first = Tree::find_node_by_val(answers.root, resp, strcmp, answers.size);	
			if (first == NULL) continue;
		
			printf("What do you want to compare %s with?\n", first->val);
			scanf("%[^\n]", resp);
			getchar();
		
			second = Tree::find_node_by_val(answers.root, resp, strcmp, answers.size);	
			if (second == NULL) continue;

			common = Tree::find_common_ancestor(first, second);	
		} while (first == NULL || second == NULL);


		printf("Actually, %s and %s have in common: ", first->val, second->val);
		print_definition(common, answers.root, &has_property);
		if (!has_property){
			printf("nothing, ");
		}

		printf("however, %s is: ", first->val);
		print_definition(first, common, &has_property);
		printf("when %s is: ", second->val);
		print_definition(second, common, &has_property);
		printf("I am certanly a genius.\n");
		fclose(data_source);

		printf("press e to quit to the menu\n");
		
	} while (getchar() != 'e');

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
		printf("Akk: Hello dear, come in, you'll be my guest\n");
		int choice = 0;
	do {
		printf("Akk: Do you want to:\n");
		printf("\t1 - know a definion of something?\n");
		printf("\t2 - see a difference between to things?\n");
		printf("\t3 - play a game with me?\n");
		printf("\t4 - get my full knowledge?\n");
		printf("\tt - say goodbye?\n");

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
