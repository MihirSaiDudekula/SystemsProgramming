#include "add_to_cache.h"

int add_to_cache(char *data, int size, char* url)
{
    int temp_lock_val = pthread_mutex_lock(&lock);
    if (temp_lock_val != 0) {
        fprintf(stderr, "Error acquiring lock: %d\n", temp_lock_val);
        return 0;
    }

    int element_size = size + 1 + strlen(url) + sizeof(cache);

    if (element_size > MAX_ELEMENT_SIZE) {
        temp_lock_val = pthread_mutex_unlock(&lock);
        if (temp_lock_val != 0) {
            fprintf(stderr, "Error releasing lock: %d\n", temp_lock_val);
        }
        fprintf(stderr, "Element too large for cache\n");
        return 0;
    } else {
        while (cache_size + element_size > MAX_SIZE) {
            remove_cache();
        }

        cache* element = (cache*)malloc(sizeof(cache));
        if (element == NULL) {
            temp_lock_val = pthread_mutex_unlock(&lock);
            fprintf(stderr, "Memory allocation failed for cache element\n");
            return 0;
        }

        element->data = (char*)malloc(size + 1);
        if (element->data == NULL) {
            free(element);
            temp_lock_val = pthread_mutex_unlock(&lock);
            fprintf(stderr, "Memory allocation failed for data\n");
            return 0;
        }
        strcpy(element->data, data);

        element->url = (char*)malloc(strlen(url) + 1);
        if (element->url == NULL) {
            free(element->data);
            free(element);
            temp_lock_val = pthread_mutex_unlock(&lock);
            fprintf(stderr, "Memory allocation failed for URL\n");
            return 0;
        }
        strcpy(element->url, url);

        element->lru_time_track = time(NULL);
        element->next = head;
        element->len = size;

        cache_size += element_size;

        temp_lock_val = pthread_mutex_unlock(&lock);
        if (temp_lock_val != 0) {
            fprintf(stderr, "Error releasing lock: %d\n", temp_lock_val);
        }
        return 1;
    }
    return 0;
}
