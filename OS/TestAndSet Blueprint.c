boolean test_and_set(boolean *target) {
    boolean rv = *target;   // Read the current value of `target` (lock)
    *target = TRUE;         // Set `target` (lock) to TRUE (or 1 in boolean context)
    return rv;              // Return the previous value of `target` (lock)

     // If rv is FALSE, it means the lock was successfully acquired (was FALSE before setting to TRUE). If rv is TRUE, it means the lock was already held by someone else.
    // here
    // rv = FALSE earlier means its unlocked to begin with
    // so set target to TRUE which makes it locked now (remember, rv should still remain false. it indicates previous value, target indicates current value).
}

do {
    while (test_and_set(&lock));
//          /* do nothing */


//     test_and_set(&lock) is a function that atomically tests the value of lock and sets it to TRUE (or 1 in boolean context).

// If lock was already TRUE (indicating that someone else holds the lock), test_and_set returns TRUE.

// The while loop continues looping (doing nothing) until test_and_set returns FALSE, indicating that the lock was successfully acquired (lock was FALSE before test_and_set).


    /* critical section */

    lock = false; //release the lock

    /* remainder section */

} while (true);



