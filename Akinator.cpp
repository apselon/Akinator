#include <stdio.h>
#include "Akinator.hpp"

void ask_question(Tree::node_t* cur){
	
	Assert(cur != NULL);

	if (!cur->has_left && !cur->has_rigth){
		make_guess(cur);
	}

	printf("%s?\n", cur->val);

	char ans = getchar();;

	if (ans == 'y'){
		ask_question(cur->left);
	} else {
		ask_question(cur->right);
	}

}


void make_guess(Tree::node_t* leaf){
	
	Assert(leaf != NULL);

	printf("%s, right?\n", leaf->val);

	char ans = getchar();

	if (getchar() == 'y'){
		printf("I knew it\n");
	}

}
