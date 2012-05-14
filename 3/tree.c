#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

typedef struct node {
	struct node *left, *right;
	char record;
} node;

struct node *stack[SIZE];
int idx = 0;

node *new(char val)
{
	node *p = malloc(sizeof(node));
	if (p == NULL) {
		perror("out of memory!\n");
		exit(1);
	}
	p->record = val;
	return p;
}

void push(node *p)
{
	if (idx == SIZE-1) {
		perror("over flow!\n");
		exit(1);
	}
	stack[idx++] = p;
}

node *pop(void) {
	if (idx == 0) {
		perror("under flow!\n");
		exit(1);
	}
	return stack[--idx];
}

node *parse_postfix(char *input)
{
	node *p;
	while(*input != '\0') {
		p = new(*input);
		switch (*input) {
			case '+':
			case '-':
			case '*':
			case '/':
				p->right = pop();
				p->left = pop();
			default:
				push(p);
		}
		++input;
	}
	return p;
}

void show_prefix(node *p)
{
	if(p == NULL) return;

	putchar(p->record);
	show_prefix(p->left);
	show_prefix(p->right);
}

void show_infix(node *p)
{
	if(p == NULL) return;

	int f = p->left && p->right;

	if(f) putchar('(');
	show_infix(p->left);
	putchar(p->record);
	show_infix(p->right);
	if(f) putchar(')');
}

void show_infix2(node *p, int parent)
{
	if(p == NULL) return;

	int self;// 演算子の優先度
	switch (p->record) {
		case '*':
		case '/':
			self = 4;
			break;
		case '+':
		case '-':
			self = 2;
			break;
		default:
			self = 6;
			break;
	}

	int f = self < parent;

	if(f) putchar('(');
	show_infix2(p->left, self);
	putchar(p->record);
	show_infix2(p->right, self+1);
	if(f) putchar(')');
}

int main(void)
{
	char input[256];
	printf("式を入力してください: ");
	scanf("%s", input);
	node *root = parse_postfix(input);

	printf("中間記法: ");
	show_infix(root);
	printf("\n");

	printf("中間記法2: ");
	show_infix2(root, 0);
	printf("\n");

	printf("前置記法: ");
	show_prefix(root);
	printf("\n");

}
