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
    Block * allocated_block = free_list;
    allocated_block->size = size;
    allocated_block->is_free = false;

    free_list = (Block *)((char *)free_list + size + sizeof(Block));
    allocated_block->next = free_list;
    return (char *)allocated_block + sizeof(Block);

}

void mem_free(void* block){
    Block *block_to_free = (Block *)((char *)block - sizeof(Block));
    block_to_free->is_free = true;
}

void* mem_resize(void* block, size_t size){
    Block *block_to_resize = (Block *)((char *)block - sizeof(Block));
    block_to_resize->size = size;
    return block;
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

// void list_display(Node** head){
//     Node* current = *head;
//     while(current != NULL){
//         printf("%d\n", current->data);
//         current = current->next;
//     }
// }

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

int main() {
    mem_init(1000);
    char text[] = "Hello World";
    int *block = mem_alloc(100);
    int *block2 = mem_alloc(110);
    int *block3 = mem_alloc(120);
    *block = 10;
    *block2 = 20;
    Block *block_meta = (Block *)((char *)block - sizeof(Block));
    Block *block_met1 = (Block *)((char *)block2 - sizeof(Block));
    // mem_free(block);
    printf("Block: %d\n", memory_pool[0]);


return 0;

}
