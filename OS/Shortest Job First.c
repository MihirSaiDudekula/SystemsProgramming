#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int executed;
};

// Function to sort processes based on burst time (SJF)
void sortProcessesByBurstTime(struct Process processes[], int n) {
    struct Process temp;
    //bubble sort the array in ascending order of burst time
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (processes[i].burst_time > processes[j].burst_time) {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

// Function to calculate waiting and turnaround times for each process
void calculateTimes(struct Process processes[], int n) {
    processes[0].waiting_time = 0;
    processes[0].turnaround_time = processes[0].burst_time;

    for (int i = 1; i < n; ++i) {

        //waiting time of current process = wait time of prev process + its burst time
        processes[i].waiting_time = processes[i - 1].waiting_time + processes[i - 1].burst_time;

        //turnaround time = wait + burst time 
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
}

// Function to display process details and average times
void displayProcesses(struct Process processes[], int n) {
    printf("PID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival_time,
               processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    // Calculate average waiting time and turnaround time
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; ++i) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_PROCESSES) {
        printf("Invalid number of processes. Please enter a number between 1 and %d.\n", MAX_PROCESSES);
        return 1;
    }

    struct Process processes[MAX_PROCESSES];

    // Input process details
    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        printf("Process %d:\n", processes[i].pid);
        printf("Arrival time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst time: ");
        scanf("%d", &processes[i].burst_time);
        processes[i].executed = 0;
    }

    // Sort processes by burst time (SJF)
    sortProcessesByBurstTime(processes, n);

    // Calculate waiting and turnaround times
    calculateTimes(processes, n);

    // Display process details and average times
    displayProcesses(processes, n);

    return 0;
}
