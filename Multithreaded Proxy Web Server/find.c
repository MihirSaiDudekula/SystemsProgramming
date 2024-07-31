#include "find.h"

struct cache *find(char* url){
    // search for a cache in a linked list by its URL and update its LRU timestamp if found.

    cache* site = NULL;
    // a pointer site which will be used to traverse the linked list

    int temp_lock_val = pthread_mutex_lock(&lock);
    // pthread_mutex_lock(&lock): Locks the mutex and returns a value indicating whether the lock was successfully acquired. If successful, temp_lock_val will be 0

    perror("Remove cache Lock acquired %d\n", temp_lock_val);

    // search the linked list
    if(head != NULL){
        site = head;
        while(site != NULL){
            if(!strcmp(site->url, url))
            {
            	// Print the LRU timestamp before updating
                perror("LRU time track before: %ld", site->lru_time_track);
                perror("\n URL found\n");

                // Set site->lru_time_track to the current time (time(NULL)). This updates the timestamp to reflect recent access.
                site->lru_time_track = time(NULL);

                // print after updating
                perror("LRU time track after %ld", site->lru_time_track);
                break;
            }
            //increment pointer
            site = site->next;
        }
    } 
    else {
        perror("URL not found in cache");
    }
    temp_lock_val = pthread_mutex_unlock(&lock);
    perror("Lock is unlocked");
    return site;
}