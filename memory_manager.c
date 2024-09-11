#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct Block {
    size_t size;
    int is_free;
    struct Block *next;
} Block;

char *memory_pool;
Block *free_list;

void mem_init(size_t size){
    char pool[size];
    memory_pool = pool;
    free_list = (Block *)memory_pool;
}

void* mem_alloc(size_t size){
    Block * current = free_list;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            // Split the block if needed
            if (current->size > size + sizeof(Block)) {
                Block *new_block = (Block *)((char *)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->is_free = 1;
                new_block->next = current->next;
                current->next = new_block;
            }

            current->is_free = 0;
            current->size = size;

            return (void *)((char *)current + sizeof(Block));
        }

        current = current->next;
    }

    return NULL; // No suitable block found
}


void mem_free(void* block){
    Block *block_to_free = (Block *)((char *)block - sizeof(Block));
    block_to_free->is_free = 1;
   
}

void* mem_resize(void* block, size_t size){
    Block *block_to_resize = (Block *)((char *)block - sizeof(Block));
    block_to_resize->size = size;
    block_to_resize->is_free = 0;
    return (char *)block_to_resize + sizeof(Block);
}

void mem_deinit(){
    free_list = NULL;
    memory_pool = NULL;
}

typedef struct Node {
uint16_t data; 
struct Node* next;
}Node;

void list_init(Node** head) {
    *head = NULL;
}

void list_insert(Node** head, int data){
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = *head;
    *head = new_node;
}

void list_insert_after(Node* prev_node, int data){
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
}

void list_insert_before(Node** head, Node* next_node, int data){
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = next_node;
    Node* current = *head;
    while(current->next != next_node){
        current = current->next;
    }
    current->next = new_node;
}

void list_delete(Node** head, int data){
    Node* current = *head;
    Node* prev = NULL;
    while(current != NULL){
        if(current->data == data){
            if(prev == NULL){
                *head = current->next;
            }else{
                prev->next = current->next;
            }
            mem_free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

Node* list_search(Node** head, int data){
    Node* current = *head;
    while(current != NULL){
        if(current->data == data){
            return current;
        }
        current = current->next;
    }
    return NULL;
}



void list_display(Node** head, Node* start_node, Node* end_node){
    Node* current = start_node;
    while(current != end_node){
        printf("%d\n", current->data);
        current = current->next;
    }
}

int list_count_nodes(Node** head){
    Node* current = *head;
    int count = 0;
    while(current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

void list_cleanup(Node** head){
    Node* current = *head;
    Node* next;
    while(current != NULL){
        next = current->next;
        mem_free(current);
        current = next;
    }
    *head = NULL;
}
