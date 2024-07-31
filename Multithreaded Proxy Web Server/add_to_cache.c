#include "add_to_cache.h"

int add_to_cache(char *data, int size, char* url)
{
	// Add a new cache element with given data and URL to the cache, while managing cache size and ensuring thread safety.

	//lock
    int temp_lock_val = pthread_mutex_lock(&lock);
    perror("Remove cache Lock acquired %d\n", temp_lock_val);


    int element_size = size + 1 + strlen(url) + sizeof(cache);
/*    Compute the total size required for the new cache element.
    size: Size of the data.
    +1: Space for the null terminator of the data string.
    strlen(url): Length of the URL string.
    sizeof(cache): Size of the cache structure itself*/

    if(element_size > MAX_ELEMENT_SIZE){
        // element is too big, do something else
        temp_lock_val = pthread_mutex_unlock(&lock);
        perror("Add cache lock is unlocked");
        // If the element is too large, unlock the mutex and exit the function to prevent adding the oversized element.
        // Indicates that the lock is released and the operation is not proceeding further.
    } 
    else {
    	// If adding the new element would exceed the cache size limit (MAX_SIZE), remove existing elements from the cache until there is enough space.
        while(cache_size + element_size > MAX_SIZE){
            remove_cache();
        }

        // Allocate memory for the new cache element and its associated data.
        cache* element = (cache*)malloc(sizeof(cache));

        // Copy the data and url strings into the allocated memory
        element->data = (char*)malloc(size+1);
        strcpy(element->data, data);

        element->url = (char*)malloc(1 + (strlen(url) * sizeof(char)));
        strcpy(element->url, url);

        element->lru_time_track = time(NULL);
        // Set the LRU timestamp to the current time.
        element->next = head;
        // Set the next pointer to point to the current head of the list, effectively inserting the new element at the beginning of the list.
        element->len = size;
        //set length of the data as size 

        // Update Cache Size and Release Mutex Lock
        cache_size += element_size;
        temp_lock_val = pthread_mutex_unlock(&lock);
        perror("Add cache lock is unlocked\n");
        return 1;
    }
    return 0;
} 