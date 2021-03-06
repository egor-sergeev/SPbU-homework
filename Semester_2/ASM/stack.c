#include "stack.h"

#define INITIAL_STACK_SIZE 8
#define SIZE_MULTIPLIER 2

Stack *createStack() {
    Stack *stack;
    stack = (Stack *) calloc(1, sizeof(Stack));
    if (!stack) {
        printf("ERROR: Cannot allocate memory for stack");
        exit(1); // TODO invent error number
    }
    stack->size = INITIAL_STACK_SIZE;
    stack->elements = (int *) calloc(stack->size, sizeof(int));
    if (!stack->elements) {
        printf("ERROR: Cannot allocate memory for stack elements");
    }
    stack->amount = 0;
    return stack;
}

void clearStack(Stack *stack) {
    if (!stack) {
        printf("ERROR: Stack doesn't exist");
        return;
    }
    if (stack->elements)
        free(stack->elements);
    stack->amount = -1;
    stack->elements = 0;
}

void deleteStack(Stack *stack) {
    if (!stack) {
        printf("ERROR: Stack doesn't exist");
        return;
    }
    clearStack(stack);
    free(stack);
}

void pushStack(Stack *stack, int element) {
    if (!stack) {
        printf("ERROR: Stack doesn't exist");
        return;
    }
    /* If all allocated memory is amount, then realloc memory (in SIZE_MULTIPLIER times more) and update size field
     * Push element and update amount field (if it hasn't been updated yet) */
    if (stack->size <= stack->amount + 1) {
        stack->size *= SIZE_MULTIPLIER;
        stack->elements = (int *) realloc(stack->elements, sizeof(int) * stack->size);
        if (!stack->elements) {
            printf("ERROR: Cannot allocate memory for stack elements");
        }
    }
    stack->elements[stack->amount] = element;
    stack->amount++;
}

int popStack(Stack *stack) {
    if (!stack) {
        printf("ERROR: Stack doesn't exist");
        exit(1);
    }

    if (stack->amount < 0) {
        printf("ERROR: Trying to pop from empty stack");
        exit(1);
    }

    int value = stack->elements[stack->amount - 1];
    stack->amount--;

    return value;
}
