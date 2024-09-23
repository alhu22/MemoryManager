#include <stdio.h>      // printf
#include <stdlib.h>       // malloc
#include <stdint.h>      // uint16_t

typedef struct Block {
    int size;
    int is_free;
    struct Block *next;
} Block;

int* memory_pool;
Block* head_pool;

void mem_init(int size){
    memory_pool = (int *)malloc(size);
    head_pool = (Block *)memory_pool;

    head_pool->size = size - sizeof(Block);
    head_pool->is_free = 1;
    head_pool->next = NULL;
}

void* mem_alloc(int size){
    Block * current = head_pool;
    while (current != NULL) {
        if (current->is_free && current->size >= size + sizeof(Block)) {
            if (current->next != NULL){   // if block exist but is free
                current->is_free = 0;
                return (void *)((char *)current + sizeof(Block));
            }
            Block *new_block = (Block *)((char *)current + sizeof(Block) + size);
            new_block->size = current->size - size - sizeof(Block);
            new_block->is_free = 1;
            current->next = new_block;

            current->is_free = 0;
            current->size = size + sizeof(Block);
            return (void *)((char *)current + sizeof(Block));
        }
        current = current->next;
    }
    return NULL;
}

void mem_free(void* block){
    Block *block_to_free = (Block *)((char *)block - sizeof(Block));
    block_to_free->is_free = 1;
    
}

void* mem_resize(void* block, size_t size){
    Block *block_to_resize = (Block *)((char *)block - sizeof(Block));
    if (block_to_resize->size >= size) {
        block_to_resize->size = size;
        return (char *)block_to_resize + sizeof(Block);
    } else {
        block_to_resize->is_free = 1;
        return (int*)mem_alloc(size);
    }
}

void mem_deinit(){
    head_pool = NULL;
    free(memory_pool);
}

typedef struct Node {
uint16_t data; 
struct Node* next;
} Node;

void list_init(Node** head) {
    *head = NULL;
}

void list_insert(Node** head, int data) {
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
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
    while(current->data != data){
        prev = current;
        current = current->next;
    }
    if(current->next != NULL){
        prev->next = current->next;
    }else{
        prev->next = NULL;
    }
    mem_free(current);
}
Node* list_search(Node** head, int data){
    Node* current = *head;
    while(current->data != data){
        current = current->next;
    }
    return current;
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

void print_all_blocks() {
    Block *current = head_pool;
    while (current != NULL) {
        if(current->size == sizeof(Node) + sizeof(Block)){
            Node* node = (Node*)((char*)current + sizeof(Block));
            printf("Block at %p, Size: %u, Is Free: %d Node at %p, Data: %d \n", 
            (void *)current, current->size, current->is_free, (void*)node, node->data);
            }else{
                printf("Block at %p, Size: %u, Is Free: %d\n", 
               (void *)current, current->size, current->is_free);
            }
        current = current->next;
        }
    }

void print_all_Nodes(Node** head){
    Node* current = *head;
    while(current != NULL){
        printf("Node at %p, Data: %d\n", (void*)current, current->data);
        current = current->next;
    }
}


