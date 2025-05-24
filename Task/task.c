#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DESC_SIZE 100

typedef struct Task {
    char description[DESC_SIZE];
    int priority;  // 1: High, 2: Medium, 3: Low
    char date[11]; // Format: DD-MM-YYYY
    struct Task* next;
} Task;

typedef struct CompletedTask {
    char description[DESC_SIZE];
    char date[11];
    struct CompletedTask* next;
} CompletedTask;

Task* head = NULL;
CompletedTask* completedHead = NULL;

// Utility to create a new task
Task* createTask(char desc[], int priority, char date[]) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    if (!newTask) {
        printf("\nMemory allocation failed.\n");
        return NULL;
    }
    strcpy(newTask->description, desc);
    newTask->priority = priority;
    strcpy(newTask->date, date);
    newTask->next = NULL;
    return newTask;
}

// Insert task into priority queue (sorted by priority and then by date)
void addTask(char desc[], int priority, char date[]) {
    Task* newTask = createTask(desc, priority, date);
    if (!newTask) return;

    if (head == NULL || newTask->priority < head->priority || 
       (newTask->priority == head->priority && strcmp(newTask->date, head->date) < 0)) {
        newTask->next = head;
        head = newTask;
    } else {
        Task* temp = head;
        while (temp->next && 
              (temp->next->priority < priority || 
              (temp->next->priority == priority && strcmp(temp->next->date, date) <= 0))) {
            temp = temp->next;
        }
        newTask->next = temp->next;
        temp->next = newTask;
    }
    printf("\nTask added successfully!\n");
}

void completeTask(int pos) {
    if (!head) {
        printf("\nNo tasks to complete.\n");
        return;
    }

    Task* temp = head;
    Task* prev = NULL;
    int i = 1;
    while (temp && i < pos) {
        prev = temp;
        temp = temp->next;
        i++;
    }

    if (!temp) {
        printf("\nInvalid position.\n");
        return;
    }

    if (prev) prev->next = temp->next;
    else head = temp->next;

    // Store in completed list
    CompletedTask* done = (CompletedTask*)malloc(sizeof(CompletedTask));
    strcpy(done->description, temp->description);
    strcpy(done->date, temp->date);
    done->next = completedHead;
    completedHead = done;

    free(temp);
    printf("\nTask marked as completed.\n");
}

void deleteCompletedTasks() {
    CompletedTask* temp;
    while (completedHead) {
        temp = completedHead;
        completedHead = completedHead->next;
        free(temp);
    }
    printf("\nAll completed tasks deleted.\n");
}

void displayTasks() {
    if (!head) {
        printf("\nNo tasks in the list.\n");
        return;
    }
    printf("\n--- To-Do Tasks ---\n");
    int count = 1;
    Task* temp = head;
    while (temp) {
        printf("%d. [%s] Priority: %d | Date: %s\n", count++, temp->description, temp->priority, temp->date);
        temp = temp->next;
    }
}

void displayCompletedTasks() {
    if (!completedHead) {
        printf("\nNo completed tasks.\n");
        return;
    }
    printf("\n--- Completed Tasks ---\n");
    int count = 1;
    CompletedTask* temp = completedHead;
    while (temp) {
        printf("%d. [%s] | Completed on: %s\n", count++, temp->description, temp->date);
        temp = temp->next;
    }
}

void checkSpecialReminders() {
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    char currentDate[11];
    sprintf(currentDate, "%02d-%02d-%04d", today.tm_mday, today.tm_mon + 1, today.tm_year + 1900);

    Task* temp = head;
    int found = 0;
    while (temp) {
        if (strcmp(temp->date, currentDate) == 0) {
            printf("\n** Reminder: Today is '%s' **\n", temp->description);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) {
        printf("\nNo special events today.\n");
    }
}

void freeAllTasks() {
    Task* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
    deleteCompletedTasks();
}

int main() {
    int choice, priority, pos;
    char desc[DESC_SIZE], date[11];

    while (1) {
        printf("\n--- To-Do List Menu ---\n");
        printf("1. Add Task\n2. Complete Task\n3. Display Tasks\n4. Display Completed Tasks\n");
        printf("5. Delete Completed Tasks\n6. Special Day Reminders\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline buffer

        switch (choice) {
            case 1:
                printf("\nEnter task description: ");
                fgets(desc, sizeof(desc), stdin);
                desc[strcspn(desc, "\n")] = '\0';
                printf("Enter priority (1-High, 2-Medium, 3-Low): ");
                scanf("%d", &priority);
                getchar();
                int d, m, y;
                printf("Enter due date (DD-MM-YYYY): ");
                scanf("%d-%d-%d", &d, &m, &y);
                sprintf(date, "%02d-%02d-%04d", d, m, y);
                getchar();
                addTask(desc, priority, date);
                break;

            case 2:
                displayTasks();
                printf("\nEnter the task number to complete: ");
                scanf("%d", &pos);
                completeTask(pos);
                break;

            case 3:
                displayTasks();
                break;

            case 4:
                displayCompletedTasks();
                break;

            case 5:
                deleteCompletedTasks();
                break;

            case 6:
                checkSpecialReminders();
                break;

            case 7:
                freeAllTasks();
                printf("\nExiting the program.\n");
                return 0;

            default:
                printf("\nInvalid choice. Try again.\n");
        }
    }

    return 0;
}
