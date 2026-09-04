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

#define STACK_INIT CAT(stackInit_, SUFFIX)
#define STACK_ADD CAT(addToStack_, SUFFIX)
#define STACK_RM CAT(removeFromStack_, SUFFIX)
#define STACK_DEST CAT(destroyStack_, SUFFIX)
#define STACK_TOP_PRINT CAT(stackTopPrint_, SUFFIX)
#define STACK_FULL_PRINT CAT(printFullStack_, SUFFIX)

#define STACK_TYPE CAT(Stack_, SUFFIX)
#define STACK_MANAGE CAT(STACK_TYPE, _manager)

typedef bool Boolean;

typedef int* int_ptr;
typedef char* char_ptr;
typedef float* float_ptr;
typedef Boolean* Boolean_ptr;
typedef void* void_ptr;

#ifndef GENERIC_STACK_PRINT_HELPERS
#define GENERIC_STACK_PRINT_HELPERS
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

typedef struct CAT(stack_node, SUFFIX) {
        T* data;
        struct CAT(stack_node, SUFFIX)* previous;
        int number;
} STACK_TYPE;

typedef struct {
        STACK_TYPE* head;
        STACK_TYPE* top;
} STACK_MANAGE;

static inline STACK_MANAGE *STACK_INIT(void){
	STACK_MANAGE *newStack = malloc(sizeof(STACK_MANAGE));
	if (newStack == NULL) return NULL;
	newStack->head = NULL;
	newStack->top = NULL;
	return newStack;
}

static inline int STACK_ADD(STACK_MANAGE *Stack, T* newData) {
	if (Stack == NULL) return 0;
	if (newData == NULL) return -3;

	STACK_TYPE *newFrame = malloc(sizeof(STACK_TYPE));
	if (newFrame == NULL) return -1;
	newFrame->data = malloc(sizeof(T));
	if (newFrame->data == NULL) {free(newFrame); return -2;}
	memcpy(newFrame->data, newData, sizeof(T));
	if (Stack->head == NULL) {
		Stack->head = newFrame;
		Stack->top = newFrame;
		newFrame->previous = NULL;
		newFrame->number = 1;
	} else {
		newFrame->previous = Stack->top;
		Stack->top = newFrame;
		newFrame->number = newFrame->previous->number + 1;
	}
	return 1;
}

static inline int STACK_RM(STACK_MANAGE *Stack){
	if (Stack == NULL) return 0;
	if (Stack->head == NULL) return -1;
	STACK_TYPE *preFrame = Stack->top->previous;
	free(Stack->top->data);
	free(Stack->top);
	Stack->top = preFrame;
	if (preFrame == NULL) Stack->head = NULL;
	return 1;
}

static inline void STACK_DEST(STACK_MANAGE *Stack){
	if (Stack == NULL) return;
	int val;
	do {
		val = STACK_RM(Stack);
	} while(val != -1);
	free(Stack);
	return;
}

static inline void STACK_TOP_PRINT(STACK_MANAGE *Stack){
	if (Stack == NULL){
		printf("Invalid Stack Pointer. No data to be read.\n");
		return;
	}
	if (Stack->top == NULL){
		printf("Empty Stack. There are no elements to be read.\n");
		return;
	}
	printf("STACK TOP VALUE\nStack Frame Number: %d\nStack Value: ", Stack->top->number);
	print_val(*Stack->top->data);
	printf("\n");
}

static inline void STACK_FULL_PRINT(STACK_MANAGE *Stack){
	if (Stack == NULL) {
		printf("Invalid Stack. No data to be read.\n");
		return;
	} else if (Stack->head ==  NULL) {
		printf("Empty Stack. No data to be read.\n");
		return;
	}
	STACK_TYPE *cur = Stack->top;
	do {
		printf("Stack Value Number: %d\n", cur->number);
		printf("Stack Value Data: ");
		print_val(*cur->data);
		cur = cur->previous;
	} while(cur != NULL);
	return;
}


#undef T
#undef SUFFIX
#undef CAT
#undef PR_CAT
#undef STACK_MANAGE
#undef STACK_TYPE
#undef STACK_INIT
#undef STACK_ADD
#undef STACK_RM
#undef STACK_DEST
#undef STACK_TOP_PRINT
#undef STACK_FULL_PRINT
#undef print_val
#endif
