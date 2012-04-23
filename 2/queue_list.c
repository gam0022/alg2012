#include "stdio.h"
#include "stdlib.h"

typedef struct queue {
	int data;
	struct queue *next;
} queue;
queue *head, *tail;

queue *ensure(void)
{
	queue *p = (queue*) malloc(sizeof(queue));
	if (p == NULL) {
		printf("Out of memory\n");
		exit(1);
	}
	return p;
}

void init_queue(void) /* 待ち行列の初期化 */
{
	queue *p = ensure();
	p->data = 0;
	p->next = NULL;
	head = tail = p;
}

void enqueue(int y)
{
	queue *p = ensure();
	p->data = y;
	p->next = NULL;
	tail->next = p;
	tail = p;
	printf("enqueue: %d\n", y);
}

int dequeue(void)
{
	queue *t = head->next;
	if (t == NULL) {
		perror("Underflow\n");
		exit(1);
	}
	int y = t->data;
	queue *buf = t;
	head->next = t->next;
	free(buf);
	printf("dequeue: %d\n", y);
	return y;
}

void print_queue(void) /* 待ち行列の内容の表示 */
{
	queue *p;
	printf("queue contains:");
	for(p = head->next; p != NULL; p = p->next)
		printf(" %d", p->data);

	printf("\n");
}

main(void)
{
	int n;

	init_queue();

	enqueue(1);
	print_queue();

	enqueue(2);
	print_queue();

	enqueue(3);
	print_queue();

	n = dequeue();
	print_queue();

	enqueue(4);
	print_queue();
}

