#include "remove_from_cache.h"

// remove the cache element with the least recent access time from a linked list
void remove_from_cache(){
    // if cache is not empty, search for the node which has the least 
    // lru_time_track and delete it.

    cache *p;
    // p: Used to keep track of the node preceding the one to be remove
    cache *q;
    // q: Used to iterate through the list
    cache *temp;
    // temp: Used to keep track of the least recently used element


    // Removing elements from a linked list but with a mutex-lock
    int temp_lock_val = pthread_mutex_lock(&lock);
    perror("Lock is acquired\n");

    if(head != NULL){
        for(q=head, p=head, temp=head; q->next != NULL; q = q->next)
        {
            if(((q->next)->lru_time_track) < (temp->lru_time_track))
            {
            	// Compare the LRU timestamp of the next node (q->next) with the current temp node.

            	// Update temp to the node with the smaller LRU timestamp (i.e., the least recently used).
                temp = q->next;

                // Update p to be the node preceding the new temp node.
                p = q;
            }
        }
        // Remove the Least Recently Used Element
        if(temp==head)
        {
        	//  if LRU is head, simply increment head to 'remove' temp
            head = head->next;
        } 
        else {
            // Removing the least recently used
            p->next = temp->next;
        }

        cache_size = cache_size - (temp->len) - sizeof(cache) 
            - strlen(temp->url)+1 - 1;
/*            Adjust cache_size to reflect the removal of the element, including the size of:
            Element Data: temp->len
            Cache Element Struct: sizeof(cache)
            URL String: strlen(temp->url) + 1 (including the null terminator)*/

        // Deallocate: Free the memory allocated for:
/*		temp->data: The data stored in the cache element.
		temp->url: The URL string.
		temp: The cache element itself.*/
        free(temp->data);
        free(temp->url);
        free(temp);
    }

    // unlock 
    temp_lock_val = pthread_mutex_unlock(&lock);
    perror("Remove cache lock\n");
}