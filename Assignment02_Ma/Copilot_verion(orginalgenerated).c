#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAILS 1000
#define MAX_SUBJECT 256
#define MAX_CATEGORY 64

// ---------------------------
// Email Structure
// ---------------------------
typedef struct {
    char category[MAX_CATEGORY];
    char subject[MAX_SUBJECT];
    int month, day, year;
    int priority;   // computed priority
} Email;

// ---------------------------
// MaxHeap Structure
// ---------------------------
typedef struct {
    Email heap[MAX_EMAILS];
    int size;
} MaxHeap;

// ---------------------------
// Priority Mapping
// ---------------------------
int categoryPriority(const char *cat) {
    if (strcmp(cat, "Boss") == 0) return 5;
    if (strcmp(cat, "Subordinate") == 0) return 4;
    if (strcmp(cat, "Peer") == 0) return 3;
    if (strcmp(cat, "ImportantPerson") == 0) return 2;
    if (strcmp(cat, "OtherPerson") == 0) return 1;
    return 0;
}

// ---------------------------
// Compare Emails by Priority
// ---------------------------
int compareEmails(Email a, Email b) {
    if (a.priority != b.priority)
        return a.priority - b.priority;

    // If same category priority, newest date first
    if (a.year != b.year)
        return a.year - b.year;
    if (a.month != b.month)
        return a.month - b.month;
    return a.day - b.day;
}

// ---------------------------
// Heap Helper Functions
// ---------------------------
void swap(Email *a, Email *b) {
    Email temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MaxHeap *h, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (compareEmails(h->heap[index], h->heap[parent]) > 0) {
            swap(&h->heap[index], &h->heap[parent]);
            index = parent;
        } else break;
    }
}

void heapifyDown(MaxHeap *h, int index) {
    int left, right, largest;

    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index;

        if (left < h->size && compareEmails(h->heap[left], h->heap[largest]) > 0)
            largest = left;
        if (right < h->size && compareEmails(h->heap[right], h->heap[largest]) > 0)
            largest = right;

        if (largest != index) {
            swap(&h->heap[index], &h->heap[largest]);
            index = largest;
        } else break;
    }
}

// ---------------------------
// Heap Operations
// ---------------------------
void insertEmail(MaxHeap *h, Email e) {
    if (h->size >= MAX_EMAILS) {
        printf("Heap full, cannot insert.\n");
        return;
    }
    h->heap[h->size] = e;
    heapifyUp(h, h->size);
    h->size++;
}

Email* peekEmail(MaxHeap *h) {
    if (h->size == 0) return NULL;
    return &h->heap[0];
}

void popEmail(MaxHeap *h) {
    if (h->size == 0) return;
    h->heap[0] = h->heap[h->size - 1];
    h->size--;
    heapifyDown(h, 0);
}

// ---------------------------
// Parsing Email File Format
// ---------------------------
Email parseEmail(FILE *fp) {
    Email e;
    char line[512];

    fgets(line, sizeof(line), fp);
    sscanf(line, "EMAIL %s", e.category);

    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = 0;
    strcpy(e.subject, line);

    fgets(line, sizeof(line), fp);
    sscanf(line, "%d-%d-%d", &e.month, &e.day, &e.year);

    e.priority = categoryPriority(e.category);
    return e;
}

// ---------------------------
// Main Program
// ---------------------------
int main() {
    MaxHeap inbox = { .size = 0 };
    char command[64];

    while (scanf("%s", command) != EOF) {

        if (strcmp(command, "EMAIL") == 0) {
            // Put back "EMAIL" line into parsing buffer
            ungetc('\n', stdin);
            ungetc('L', stdin);
            ungetc('I', stdin);
            ungetc('A', stdin);
            ungetc('M', stdin);
            ungetc('E', stdin);

            Email e = parseEmail(stdin);
            insertEmail(&inbox, e);
        }

        else if (strcmp(command, "NEXT") == 0) {
            Email *e = peekEmail(&inbox);
            if (!e) {
                printf("No emails.\n");
            } else {
                printf("Sender: %s\n", e->category);
                printf("Subject: %s\n", e->subject);
                printf("Date: %02d-%02d-%04d\n", e->month, e->day, e->year);
            }
        }

        else if (strcmp(command, "READ") == 0) {
            if (inbox.size == 0) {
                // Nothing to delete
            } else {
                popEmail(&inbox);
            }
        }

        else if (strcmp(command, "COUNT") == 0) {
            printf("%d\n", inbox.size);
        }
    }

    return 0;
}
