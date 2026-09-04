#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef T
#error T needs to be defined as a type to include this library
#else
#ifndef SUFFIX
#define SUFFIX T
#endif

#define CAT(a, b) PR_CAT(a, b)
#define PR_CAT(a, b) a ## b

#define QUEUE_INIT CAT(queueInit_, SUFFIX)
#define QUEUE_ADD CAT(addToQueue_, SUFFIX)
#define QUEUE_RM CAT(removeFromQueue_, SUFFIX)
#define QUEUE_DEST CAT(destroyQueue_, SUFFIX)
#define QUEUE_TOP_PRINT CAT(queueTopPrint_, SUFFIX)
#define QUEUE_FULL_PRINT CAT(printFullQueue_, SUFFIX)
#define QUEUE_SIZE CAT(queueSize_, SUFFIX)

#define QUEUE_TYPE CAT(Queue_, SUFFIX)
#define QUEUE_MANAGE CAT(QUEUE_TYPE, _manager)

typedef bool Boolean;

typedef int* int_ptr;
typedef char* char_ptr;
typedef float* float_ptr;
typedef Boolean* Boolean_ptr;
typedef void* void_ptr;

#ifndef GENERIC_QUEUE_PRINT_HELPERS
#define GENERIC_QUEUE_PRINT_HELPERS
static inline void print_val_int(const void *p)   { printf("%d\n", *(const int*)p); }
static inline void print_val_float(const void *p) { printf("%f\n", *(const float*)p); }
static inline void print_val_char(const void *p)  { printf("%c\n", *(const char*)p); }
static inline void print_val_bool(const void *p)  { printf("%d\n", *(const bool*)p); }
static inline void print_val_unsupported(const void *p) {
    (void)p; 
    printf("This type cannot yet be formatted.\n");
}
#endif

#define print_val(x) ( _Generic((x), \
    int:     print_val_int, \
    float:   print_val_float, \
    char:    print_val_char, \
    bool:    print_val_bool, \
    default: print_val_unsupported \
)(&(x)) )

typedef struct CAT(queue_node, SUFFIX) {
        T* data;
        struct CAT(queue_node, SUFFIX)* next;
        int number;
} QUEUE_TYPE;

typedef struct {
        QUEUE_TYPE* head;
        QUEUE_TYPE* tail;
	int elementsRemoved;
} QUEUE_MANAGE;

static inline QUEUE_MANAGE *QUEUE_INIT(void){
	QUEUE_MANAGE *newQueue = malloc(sizeof(QUEUE_MANAGE));
	if (newQueue == NULL) return NULL;
	newQueue->head = NULL;
	newQueue->tail = NULL;
	newQueue->elementsRemoved = 0;
	return newQueue;
}

static inline int QUEUE_ADD(QUEUE_MANAGE *Queue, T* newData) {
	if (Queue == NULL) return 0;
	if (newData == NULL) return -3;

	QUEUE_TYPE *newFrame = malloc(sizeof(QUEUE_TYPE));
	if (newFrame == NULL) return -1;
	newFrame->data = malloc(sizeof(T));
	if (newFrame->data == NULL) {free(newFrame); return -2;}
	memcpy(newFrame->data, newData, sizeof(T));
	newFrame->next = NULL;
	if (Queue->head == NULL) {
		Queue->head = newFrame;
		Queue->tail = newFrame;
		newFrame->number = 1;
	} else {
		Queue->tail->next = newFrame;
		newFrame->number = Queue->tail->number + 1;
		Queue->tail = newFrame;
	}
	return 1;
}

static inline int QUEUE_RM(QUEUE_MANAGE *Queue){
	if (Queue == NULL) return 0;
	if (Queue->head == NULL) return -1;
	QUEUE_TYPE *nFrame = Queue->head->next;
	free(Queue->head->data);
	free(Queue->head);
	Queue->head = nFrame;
	if (nFrame == NULL) Queue->tail = NULL;
	Queue->elementsRemoved++;
	return 1;
}

static inline void QUEUE_DEST(QUEUE_MANAGE *Queue){
	if (Queue == NULL) return;
	int val;
	do {
		val = QUEUE_RM(Queue);
	} while(val != -1);
	free(Queue);
	return;
}

static inline void QUEUE_TOP_PRINT(QUEUE_MANAGE *Queue){
	if (Queue == NULL){
		printf("Invalid Queue Pointer. No data to be read.\n");
		return;
	}
	if (Queue->tail == NULL){
		printf("Empty Queue. There are no elements to be read.\n");
		return;
	}
	printf("QUEUE TOP VALUE\nQueue Value: ");
	print_val(*Queue->head->data);
	printf("\n");
}

static inline void QUEUE_FULL_PRINT(QUEUE_MANAGE *Queue){
	if (Queue == NULL) {
		printf("Invalid Queue. No data to be read.\n");
		return;
	} else if (Queue->head ==  NULL) {
		printf("Empty Queue. No data to be read.\n");
		return;
	}
	QUEUE_TYPE *cur = Queue->head;
	do {
		printf("Queue Value Number: %d\n", cur->number - Queue->elementsRemoved);
		printf("Queue Value Data: ");
		print_val(*cur->data);
		cur = cur->next;
	} while(cur != NULL);
	return;
}

static inline int QUEUE_SIZE(QUEUE_MANAGE *Queue){
	return Queue->tail->number - Queue->elementsRemoved;
}


#undef T
#undef SUFFIX
#undef CAT
#undef PR_CAT
#undef QUEUE_MANAGE
#undef QUEUE_TYPE
#undef QUEUE_INIT
#undef QUEUE_ADD
#undef QUEUE_RM
#undef QUEUE_DEST
#undef QUEUE_TOP_PRINT
#undef QUEUE_FULL_PRINT
#undef print_val
#endif
