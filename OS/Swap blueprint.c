void Swap(boolean *a, boolean *b)
{
    //this is basically swapping using calll-by-reference
    boolean temp = *a;
    *a= *b;
    *b = temp;
}


do {
    key = TRUE;
    //  key is used to indicate whether a thread is trying to enter the critical section.here we Initialize key to TRUE.

    while (key == TRUE)
        Swap(&lock, &key);

    // If lock was FALSE (0 in boolean context) before swapping, key becomes FALSE, and the loop exits- we move to critical section

    // If lock was already TRUE (1 in boolean context), indicating it's already locked, key remains TRUE, and the loop continues spinning (busy-waiting).

    // critical section

    lock = FALSE;

    // remainder section

} while (TRUE);

