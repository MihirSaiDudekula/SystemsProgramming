#include "find.h"

struct cache *find(char* url) {
    cache* site = NULL;

    int temp_lock_val = pthread_mutex_lock(&lock);
    if (temp_lock_val != 0) {
        fprintf(stderr, "Error acquiring lock: %d\n", temp_lock_val);
        return NULL;
    }

    if (head != NULL) {
        site = head;
        while (site != NULL) {
            if (!strcmp(site->url, url)) {
                fprintf(stderr, "LRU time track before: %ld\n", site->lru_time_track);
                fprintf(stderr, "URL found\n");

                site->lru_time_track = time(NULL);

                fprintf(stderr, "LRU time track after: %ld\n", site->lru_time_track);
                break;
            }
            site = site->next;
        }
    } else {
        fprintf(stderr, "URL not found in cache\n");
    }

    temp_lock_val = pthread_mutex_unlock(&lock);
    if (temp_lock_val != 0) {
        fprintf(stderr, "Error releasing lock: %d\n", temp_lock_val);
    }

    return site;
}
