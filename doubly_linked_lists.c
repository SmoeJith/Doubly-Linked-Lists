/**************************************
 *                                    *
 *       doubly_linked_lists.c        *
 *             written by             *
 *       Joe Smith, 1/12/2021         *
 *                                    *
 *   Email: josephsmith@my.ccsu.edu   *
 *                                    *
 *            License: MIT            *
 *                                    *
 *       I originally made this       *
 *   small single-file library        *
 *   while writing a game project     *
 *   for my CS 355 class at CCSU      *
 *   during quarantine. This may      *
 *   not be the most efficient or     *
 *   clean implementation of doubly   *
 *   linked lists in pure C, but it   *
 *   is mine.                         *
 *                                    *
 *       If you use any portion of    *
 *   this library for any reason,     *
 *   don't be afraid to throw me an   *
 *   email letting me know!           *
 *                                    *
 **************************************/



#include<stdlib.h>
#include<stdio.h>



typedef struct LinkedListNode LinkedListNode;
struct LinkedListNode{
    void *data;
    LinkedListNode *next_node;
    LinkedListNode *prev_node;
};

typedef struct LinkedList LinkedList;
struct LinkedList{
    LinkedListNode *first_element;
    LinkedListNode *last_element;
    int list_size;
};



void *list_get(LinkedList *list, int index);
void list_add(LinkedList *list, int index, void *data);
void *list_remove(LinkedList *list, int index);

void *list_pop(LinkedList *list);
void list_push(LinkedList *list, void *data);

void list_rotate(LinkedList *list);

LinkedList *list_create(void);

void int_list_print(LinkedList *list);
void char_list_print(LinkedList *list);
void str_list_print(LinkedList *list);



/* This is a convenience function for constructing a node for a linked list. */
LinkedListNode *_list_node_create(void *data){
    LinkedListNode *return_node = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    
    return_node->next_node = NULL;
    return_node->prev_node = NULL;
    return_node->data = data;

    return return_node;
}



/* This function grabs the data at the specified element of the linked list. */
void *list_get(LinkedList *list, int index){
    
    /* Don't give what you don't have. */
    if((index >= list->list_size) || (index < 0))
        return NULL;
    else{
        int increment_dir = 1;
        LinkedListNode *current_node = list->first_element;

        /* Choose the closest end to the desired index to start iterating from. */
        if(index > (list->list_size - index)){
            index = list->list_size - index - 1;
            increment_dir = 0;
            current_node = list->last_element;
        }

        for(; index > 0; index--){
            if(increment_dir)
                current_node = current_node->next_node;
            else
                current_node = current_node->prev_node;
        }

        return current_node->data;
    }
}



/* This function adds a node to a linked list at the specified index. */
void list_add(LinkedList *list, int index, void *data){

    if(list->list_size == 0){
        list->first_element = list->last_element = _list_node_create(data);
        list->list_size = 1;
    }
    else if(index == list->list_size){
        list_push(list, data);
    }
    else if(index == 0){
        LinkedListNode *temp_node = list->first_element;
        list->first_element = _list_node_create(data);
        list->first_element->next_node = temp_node;
        temp_node->prev_node = list->first_element;

        list->list_size++;
    }
    else{

        int increment_dir = 1;
        LinkedListNode *current_node = list->first_element;

        /* Choose the closest end to the desired index to start iterating from. */
        if(index > (list->list_size - index)){
            index = list->list_size - index - 1;
            increment_dir = 0;
            current_node = list->last_element;
        }

        for(; index > 0; index--){
            if(increment_dir)
                current_node = current_node->next_node;
            else
                current_node = current_node->prev_node;
        }

        /* Set the pointers of the new element, *
         * the element preceding the new        *
         * element, and the element succeeding  *
         * the new element to fit the new       *
         * element in the list.                 */
        LinkedListNode *temp_node = current_node;
        
        current_node = _list_node_create(data);
        temp_node->prev_node->next_node = current_node;
        current_node->next_node = temp_node;
        current_node->prev_node = temp_node->prev_node;
        temp_node->prev_node = current_node;

        list->list_size++;
    }
}



/* This function removes a node from a linked list at the supplied index, returning what it stored. */
void *list_remove(LinkedList *list, int index){
    if(list->list_size == 0){
        return NULL;
    }
    else if(index == list->list_size - 1){
        return list_pop(list);
    }
    else if(index == 0){
        LinkedListNode *temp_node = list->first_element;
        void *return_val = temp_node->data;
        
        list->first_element = temp_node->next_node;
        list->first_element->next_node->prev_node = list->first_element;

        free(temp_node);

        list->list_size--;

        return return_val;
    }
    else{
        
        int increment_dir = 1;
        LinkedListNode *current_node = list->first_element;

        /* Choose the closest end to the desired index to start iterating from. */
        if(index > (list->list_size - index)){
            index = list->list_size - index - 1;
            increment_dir = 0;
            current_node = list->last_element;
        }

        for(; index > 0; index--){
            if(increment_dir)
                current_node = current_node->next_node;
            else
                current_node = current_node->prev_node;
        }

        LinkedListNode *temp_node = current_node;

        current_node = temp_node->next_node;
        current_node->prev_node = temp_node->prev_node;
        current_node->prev_node->next_node = current_node;

        void *return_val = temp_node->data;

        /* Don't forget to take out the trash. */
        free(temp_node);

        list->list_size--;

        return return_val;
    }
}



/* This function treats a linked list as a stack and pops the top element, returning what it stored. */
void *list_pop(LinkedList *list){
    if(list->list_size == 0)
        return NULL;
    else if(list->list_size == 1){
        void *return_val = list->last_element->data;
        free(list->last_element);

        list->first_element = NULL;
        list->last_element = NULL;
        list->list_size = 0;

        return return_val;
    }
    else{
        LinkedListNode *popped_node = list->last_element;
        popped_node->prev_node->next_node = NULL;
        list->last_element = popped_node->prev_node;
        
        void *return_val = popped_node->data;
        
        free(popped_node);

        list->list_size--;

        return return_val;
    }
}



/* This function treats a linked list as a stack and pushes to it. */
void list_push(LinkedList *list, void *data){
    
    LinkedListNode *push_node = _list_node_create(data);

    if(list->list_size == 0){
        list->first_element = push_node;
        list->last_element = push_node;
    }
    else{
        push_node->prev_node = list->last_element;
        list->last_element->next_node = push_node;
        list->last_element = push_node;
    }

    list->list_size++;
}



/* This function rotates a linked list. */
void list_rotate(LinkedList *list){
    if(list->list_size > 1)
        list_add(list, 0, list_remove(list, list->list_size - 1));
}



/* This function returns a pointer to an empty linked list. */
LinkedList *list_create(){
    LinkedList *return_ptr = (LinkedList *)malloc(sizeof(LinkedList));
    return_ptr->list_size = 0;
    return_ptr->first_element = NULL;
    return_ptr->last_element = NULL;
    return return_ptr;
}



/* This function prints the contents of a linked list of integers nicely. */
void int_list_print(LinkedList *list){
    LinkedListNode *current_node = list->first_element;

    printf("{");

    if(current_node != NULL)
        printf("%d", *(int *)current_node->data);

    while((current_node = current_node->next_node) != NULL){
        printf(", %d", *(int *)current_node->data);
    }

    printf("}");
}



/* This function prints the contents of a linked list of characters nicely. */
void char_list_print(LinkedList *list){
    LinkedListNode *current_node = list->first_element;

    printf("{");

    if(current_node != NULL)
        printf("%c", *(char *)current_node->data);

    while((current_node = current_node->next_node) != NULL){
        printf(", %c", *(char *)current_node->data);
    }

    printf("}");
}



/* This function prints the contents of a linked list of strings nicely. */
void str_list_print(LinkedList *list){
    LinkedListNode *current_node = list->first_element;

    printf("{");

    if(current_node != NULL)
        printf("\"%s\"", *(char **)current_node->data);

    while((current_node = current_node->next_node) != NULL){
        printf(", \"%s\"", *(char **)current_node->data);
    }

    printf("}");
}