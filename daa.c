#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_EMPLOYEES 10
#define MAX_DAYS 7

// Input parameters
int availability[MAX_EMPLOYEES][MAX_DAYS];
int preference[MAX_EMPLOYEES][MAX_DAYS];
int shiftsRequired[MAX_DAYS];
int totalEmployees, totalDays;

// DP table
int dp[MAX_DAYS][1 << MAX_EMPLOYEES];
int prevState[MAX_DAYS][1 << MAX_EMPLOYEES];

// Function to check if a state is valid
bool isValidState(int state, int day) {
    int count = 0;
    for (int i = 0; i < totalEmployees; i++) {
        if (state & (1 << i)) { // Employee i is working in this state
            if (!availability[i][day]) return false; // Not available
            count++;
        }
    }
    return count == shiftsRequired[day]; // Must match shifts required
}

// Function to calculate the cost of a state
int calculateCost(int state, int day) {
    int cost = 0;
    for (int i = 0; i < totalEmployees; i++) {
        if (state & (1 << i)) { // Employee i is working
            cost += preference[i][day];
        }
    }
    return cost;
}

// Main scheduling function
void schedule() {
    int maxState = 1 << totalEmployees;

    // Initialize DP table
    for (int i = 0; i < totalDays; i++) {
        for (int state = 0; state < maxState; state++) {
            dp[i][state] = INT_MAX; // Start with infinity
            prevState[i][state] = -1; // No previous state
        }
    }

    // Base case for day 0
    for (int state = 0; state < maxState; state++) {
        if (isValidState(state, 0)) {
            dp[0][state] = calculateCost(state, 0);
        }
    }

    // Fill DP table
    for (int day = 1; day < totalDays; day++) {
        for (int state = 0; state < maxState; state++) {
            if (!isValidState(state, day)) continue;

            for (int prev = 0; prev < maxState; prev++) {
                if (dp[day - 1][prev] != INT_MAX) {
                    int cost = dp[day - 1][prev] + calculateCost(state, day);
                    if (cost < dp[day][state]) {
                        dp[day][state] = cost;
                        prevState[day][state] = prev;
                    }
                }
            }
        }
    }

    // Find the optimal end state
    int minCost = INT_MAX, endState = -1;
    for (int state = 0; state < maxState; state++) {
        if (dp[totalDays - 1][state] < minCost) {
            minCost = dp[totalDays - 1][state];
            endState = state;
        }
    }

    // Backtrack to find the optimal schedule
    int schedule[MAX_DAYS];
    for (int day = totalDays - 1; day >= 0; day--) {
        schedule[day] = endState;
        endState = prevState[day][endState];
    }

    // Output the schedule
    printf("\nOptimal Schedule (binary representation of employees working):\n");
    for (int day = 0; day < totalDays; day++) {
        printf("Day %d: %d\n", day + 1, schedule[day]);
    }
    printf("Minimum Cost: %d\n", minCost);
}

// Main function
int main() {
    // Input data
    printf("Enter total employees and total days: ");
    scanf("%d %d", &totalEmployees, &totalDays);

    if (totalEmployees > MAX_EMPLOYEES || totalDays > MAX_DAYS) {
        printf("Error: Maximum employees or days exceeded.\n");
        return 1;
    }

    printf("Enter availability matrix (employees x days):\n");
    for (int i = 0; i < totalEmployees; i++) {
        for (int j = 0; j < totalDays; j++) {
            scanf("%d", &availability[i][j]);
        }
    }

    printf("Enter preference matrix (employees x days):\n");
    for (int i = 0; i < totalEmployees; i++) {
        for (int j = 0; j < totalDays; j++) {
            scanf("%d", &preference[i][j]);
        }
    }

    printf("Enter shifts required for each day:\n");
    for (int i = 0; i < totalDays; i++) {
        scanf("%d", &shiftsRequired[i]);
    }

    schedule();
    return 0;
}