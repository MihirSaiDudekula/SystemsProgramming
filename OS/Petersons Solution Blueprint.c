do {
    flag[i] = TRUE;
    // The process Pi indicates its intention to enter the critical section by setting its flag to TRUE

    // flag[i] = true;  // Process i wants to enter the critical section
    // flag[i] = false; // Process i has left the critical section

    turn = j;
    // turn variable is the tiebreaker/deciding factor for 
    // which processes' turn it is to go in critical section now
    // it can either be process 1(i) or process 2(j) but not both
    while (flag[j] && turn == j);
    //if process j is willing to enter CS and it is j's turn to enter,
    // then process i must wait until j finishes, hence an infinite while loop here

    //if we reach here (outside the infinite loop)
    // then it means j was either not willing or it wasnt j's turn
    // so i can go into critical section

    // critical section code here

    flag[i] = FALSE;
    //process i has finished CS execution 

    // remainder section

} while (TRUE);


