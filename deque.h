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

#define DEQUE_INIT CAT(dequeInit_, SUFFIX)
#define DEQUE_ADD_FRONT CAT(addToDequeFront_, SUFFIX)
#define DEQUE_ADD_BACK CAT(addToDequeBack_, SUFFIX)
#define DEQUE_RM_BACK CAT(removeFromDequeBack_, SUFFIX)
#define DEQUE_RM_FRONT CAT(removeFromDequeFront_, SUFFIX)
#define DEQUE_DEST CAT(destroyDeque_, SUFFIX)
#define DEQUE_TOP_PRINT CAT(dequeTopPrint_, SUFFIX)
#define DEQUE_FULL_PRINT CAT(printFullDeque_, SUFFIX)

#define DEQUE_TYPE CAT(Deque_, SUFFIX)
#define DEQUE_MANAGE CAT(DEQUE_TYPE, _manager)

typedef bool Boolean;

typedef int* int_ptr;
typedef char* char_ptr;
typedef float* float_ptr;
typedef Boolean* Boolean_ptr;
typedef void* void_ptr;

#ifndef GENERIC_DEQUE_PRINT_HELPERS
#define GENERIC_DEQUE_PRINT_HELPERS
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

typedef struct CAT(deque_node, SUFFIX) {
        T* data;
        struct CAT(deque_node, SUFFIX)* next;
	struct CAT(deque_node, SUFFIX)* previous;
} DEQUE_TYPE;

typedef struct {
        DEQUE_TYPE* head;
        DEQUE_TYPE* tail;
} DEQUE_MANAGE;

static inline DEQUE_MANAGE *DEQUE_INIT(void){
	DEQUE_MANAGE *newDeque = malloc(sizeof(DEQUE_MANAGE));
	if (newDeque == NULL) return NULL;
	newDeque->head = NULL;
	newDeque->tail = NULL;
	return newDeque;
}

static inline int DEQUE_ADD_BACK(DEQUE_MANAGE *Deque, T* newData) {
	if (Deque == NULL) return 0;
	if (newData == NULL) return -3;
	DEQUE_TYPE *newFrame = malloc(sizeof(DEQUE_TYPE));
	if (newFrame == NULL) return -1;
	newFrame->data = malloc(sizeof(T));
	if (newFrame->data == NULL) {free(newFrame); return -2;}
	memcpy(newFrame->data, newData, sizeof(T));
	newFrame->next = NULL;
	if (Deque->head == NULL) {
		Deque->head = newFrame;
		Deque->tail = newFrame;
		newFrame->previous = NULL;
	} else {
		newFrame->previous = Deque->tail;
		Deque->tail->next = newFrame;
		Deque->tail = newFrame;
	}
	return 1;
}

static inline int DEQUE_ADD_FRONT(DEQUE_MANAGE *Deque, T* newData) {
	if (Deque == NULL) return 0;
	if (newData == NULL) return -3;
	DEQUE_TYPE *newFrame = malloc(sizeof(DEQUE_TYPE));
	if (newFrame == NULL) return -1;
	newFrame->data = malloc(sizeof(T));
	if (newFrame->data == NULL) {free(newFrame); return -2;}
	memcpy(newFrame->data, newData, sizeof(T));
	newFrame->previous = NULL;
	if (Deque->head == NULL) {
		Deque->head = newFrame;
		Deque->tail = newFrame;
		newFrame->next = NULL;
	} else {
		newFrame->next = Deque->head;
		Deque->head->previous = newFrame;
		Deque->head = newFrame;
	}
	return 1;
}

static inline int DEQUE_RM_FRONT(DEQUE_MANAGE *Deque){
	if (Deque == NULL) return 0;
	if (Deque->head == NULL) return -1;
	DEQUE_TYPE *nFrame = Deque->head->next;
	free(Deque->head->data);
	free(Deque->head);
	Deque->head = nFrame;
	if (nFrame != NULL) Deque->head->previous = NULL;
	if (nFrame == NULL) Deque->tail = NULL;
	return 1;
}

static inline int DEQUE_RM_BACK(DEQUE_MANAGE *Deque){
	if (Deque == NULL) return 0;
	if (Deque->head == NULL) return -1;
	DEQUE_TYPE *preFrame = Deque->tail->previous;
	free(Deque->tail->data);
	free(Deque->tail);
	Deque->tail = preFrame;
	if (preFrame != NULL) Deque->tail->next = NULL;
	if (preFrame == NULL) Deque->head = NULL;
	return 1;
}

static inline void DEQUE_DEST(DEQUE_MANAGE *Deque){
	if (Deque == NULL) return;
	int val;
	do {
		val = DEQUE_RM_FRONT(Deque);
	} while(val != -1);
	free(Deque);
	return;
}

static inline void DEQUE_TOP_PRINT(DEQUE_MANAGE *Deque){
	if (Deque == NULL){
		printf("Invalid Deque Pointer. No data to be read.\n");
		return;
	}
	if (Deque->tail == NULL){
		printf("Empty Deque. There are no elements to be read.\n");
		return;
	}
	printf("DEQUE TOP VALUE\nDeque Value: ");
	print_val(*Deque->head->data);
	printf("\n");
}

static inline void DEQUE_FULL_PRINT(DEQUE_MANAGE *Deque){
	if (Deque == NULL) {
		printf("Invalid Deque. No data to be read.\n");
		return;
	} else if (Deque->head ==  NULL) {
		printf("Empty Deque. No data to be read.\n");
		return;
	}
	int walker = 1;
	DEQUE_TYPE *cur = Deque->head;
	do {
		printf("Deque Value Number: %d\n", walker);
		printf("Deque Value Data: ");
		print_val(*cur->data);
		cur = cur->next;
		walker++;
	} while(cur != NULL);
	return;
}


#undef T
#undef SUFFIX
#undef CAT
#undef PR_CAT
#undef DEQUE_MANAGE
#undef DEQUE_TYPE
#undef DEQUE_INIT
#undef DEQUE_ADD_FRONT
#undef DEQUE_ADD_BACK
#undef DEQUE_RM_FRONT
#undef DEQUE_RM_BACK
#undef DEQUE_DEST
#undef DEQUE_TOP_PRINT
#undef DEQUE_FULL_PRINT
#undef print_val
#endif
