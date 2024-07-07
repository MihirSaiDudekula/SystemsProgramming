do {
    wait(chopstick[i]); // Wait to pick up the left chopstick (chopstick[i])
    wait(chopstick[(i+1) % 5]); // Wait to pick up the right chopstick (chopstick[(i+1) % 5])

    // eat: The philosopher eats once both chopsticks are acquired

    signal(chopstick[i]); // Put down the left chopstick (chopstick[i])
    signal(chopstick[(i+1) % 5]); // Put down the right chopstick (chopstick[(i+1) % 5])

    // think: The philosopher thinks (not shown in code, typically a sleep or delay here)
} while (TRUE);
