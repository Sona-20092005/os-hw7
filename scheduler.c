#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
} Process;

int compare_arrival(const void *a, const void *b) {
    const Process *p1 = a;
    const Process *p2 = b;

    if (p1->arrival_time < p2->arrival_time) return -1;
    if (p1->arrival_time > p2->arrival_time) return 1;

    return (p1->pid > p2->pid) - (p1->pid < p2->pid);
}

void print_results(Process p[], int n, int order[], const char *name) {
    double total_wt = 0;
    double total_tat = 0;
    double total_rt = 0;

    printf("\n=== %s ===\n", name);

    printf("Gantt Chart: |");
    for (int i = 0; i < n; i++) {
        int j = order[i];
        printf(" P%d |", p[j].pid);
    }

    printf("\nPID\tAT\tBT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        int j = order[i];

        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
            p[j].pid,
            p[j].arrival_time,
            p[j].burst_time,
            p[j].waiting_time,
            p[j].turnaround_time,
            p[j].response_time);

        total_wt += p[j].waiting_time;
        total_tat += p[j].turnaround_time;
        total_rt += p[j].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time: %.2f\n", total_rt / n);
}

void fcfs(Process p[], int n) {
    Process arr[n];
    int order[n];
    int time = 0;

    for (int i = 0; i < n; i++) {
        arr[i] = p[i];
    }

    qsort(arr, n, sizeof(Process), compare_arrival);

    for (int i = 0; i < n; i++) {
        if (time < arr[i].arrival_time) {
            time = arr[i].arrival_time;
        }

        order[i] = i;

        arr[i].waiting_time = time - arr[i].arrival_time;
        arr[i].response_time = arr[i].waiting_time;

        time += arr[i].burst_time;

        arr[i].turnaround_time = time - arr[i].arrival_time;
    }

    print_results(arr, n, order, "First Come First Served (FCFS)");
}

void sjf(Process p[], int n) {
    Process arr[n];
    bool done[n];
    int order[n];
    int time = 0;
    int completed = 0;

    for (int i = 0; i < n; i++) {
        arr[i] = p[i];
        done[i] = false;
    }

    while (completed < n) {
        int shortest = -1;

        for (int i = 0; i < n; i++) {
            if (done[i] || arr[i].arrival_time > time) {
                continue;
            }

            if (shortest == -1 || arr[i].burst_time < arr[shortest].burst_time) {
                shortest = i;
            }
            else if (arr[i].burst_time == arr[shortest].burst_time && arr[i].arrival_time < arr[shortest].arrival_time) {
                shortest = i;
            }
        }

        if (shortest == -1) {
            int next = -1;

            for (int i = 0; i < n; i++) {
                if (!done[i] && (next == -1 || arr[i].arrival_time < arr[next].arrival_time)) {
                    next = i;
                }
            }

            time = arr[next].arrival_time;
            continue;
        }

        order[completed] = shortest;
        arr[shortest].waiting_time = time - arr[shortest].arrival_time;
        arr[shortest].response_time = arr[shortest].waiting_time;

        time += arr[shortest].burst_time;

        arr[shortest].turnaround_time = time - arr[shortest].arrival_time;

        done[shortest] = true;
        completed++;
    }

    print_results(arr, n, order, "Shortest Job First (SJF)");
}

int main(void) {
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;

        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &p[i].arrival_time, &p[i].burst_time);
    }

    fcfs(p, n);
    sjf(p, n);

    return 0;
}
