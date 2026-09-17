#include <stdio.h> // Include functions for input and output.
#include <stdlib.h> // Include functions for memory allocation.
#include <string.h> // Include functions for string operations.
#include <ctype.h> // Include functions for checking spaces.

#define INITIAL_CAPACITY 10 // Set the starting heap capacity.

// Email structure
typedef struct {
    char category[50]; // Store the sender category.
    int category_priority; // Store the priority of the sender.
    char subject[256]; // Store the email subject.
    char date[11]; // Store the email date.
    long date_val; // Store the date as a number.
} Email;

// MaxHeap structure (List-based / Dynamic Array)
typedef struct {
    Email* data; // Store the emails in a dynamic array.
    int size; // Store the current number of emails.
    int capacity; // Store the current array capacity.
} MaxHeap;

// Function declarations
MaxHeap* create_heap(); // Create a new heap.
void free_heap(MaxHeap* heap); // Free the heap memory.
int get_category_priority(const char* cat); // Get the priority of a category.
long parse_date(const char* date_str); // Convert a date into a number.
int compare_emails(Email a, Email b); // Compare two emails.
void swap(Email* a, Email* b); // Swap two emails.
void sift_up(MaxHeap* heap, int index); // Move an email up the heap.
void sift_down(MaxHeap* heap, int index); // Move an email down the heap.
void insert_email(MaxHeap* heap, Email email); // Insert an email into the heap.
Email extract_max(MaxHeap* heap); // Remove the highest-priority email.
Email peek_max(MaxHeap* heap); // Get the highest-priority email.
void trim_whitespace(char* str); // Remove extra spaces.
void process_command(MaxHeap* heap, char* line); // Process an input command.

// Main function
int main(int argc, char* argv[]) {
    FILE* file = stdin; // Use standard input by default.

    // Check if an input file was provided.
    if (argc > 1) {
        file = fopen(argv[1], "r"); // Open the input file.
        if (!file) { // Check if the file opened successfully.
            perror("Error opening file"); // Print an error message.
            return 1; // Stop the program if the file cannot be opened.
        }
    }

    MaxHeap* heap = create_heap(); // Create an empty heap.
    char line[512]; // Create a buffer for each input line.

    // Read each line from the input.
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0; // Remove the newline character.
        if (strlen(line) == 0) continue; // Skip empty lines.

        process_command(heap, line); // Process the current command.
    }

    // Close the file if an input file was used.
    if (file != stdin) {
        fclose(file); // Close the input file.
    }

    free_heap(heap); // Free the heap memory.
    return 0; // End the program successfully.
}

// Helper: Convert category to numerical priority
int get_category_priority(const char* cat) {
    if (strcmp(cat, "Boss") == 0) return 5; // Give Boss the highest priority.
    if (strcmp(cat, "Subordinate") == 0) return 4; // Give Subordinate priority 4.
    if (strcmp(cat, "Peer") == 0) return 3; // Give Peer priority 3.
    if (strcmp(cat, "ImportantPerson") == 0) return 2; // Give ImportantPerson priority 2.
    if (strcmp(cat, "OtherPerson") == 0) return 1; // Give OtherPerson the lowest priority.
    return 0; // Return 0 for an unknown category.
}

// Helper: Convert MM-DD-YYYY to a comparable integer YYYYMMDD
long parse_date(const char* date_str) {
    int mm, dd, yyyy; // Store the month, day, and year.

    // Read the month, day, and year from the date.
    if (sscanf(date_str, "%d-%d-%d", &mm, &dd, &yyyy) == 3) {
        return (long)yyyy * 10000 + mm * 100 + dd; // Convert the date to YYYYMMDD.
    }

    return 0; // Return 0 if the date cannot be parsed.
}

// Compare two emails: returns 1 if 'a' has higher priority than 'b', 0 otherwise.
// Higher category priority comes first. For the same category, newer date comes first.
int compare_emails(Email a, Email b) {
    // Check the sender priorities first.
    if (a.category_priority != b.category_priority) {
        return a.category_priority > b.category_priority; // Return 1 if a has higher priority.
    }

    // Newer date means larger date value.
    return a.date_val > b.date_val; // Return 1 if a is newer.
}

// Create heap
MaxHeap* create_heap() {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap)); // Allocate memory for the heap.

    // Check if the heap memory was allocated.
    if (heap == NULL) {
        printf("Memory allocation failed.\n"); // Print an error message.
        exit(1); // Stop the program.
    }

    heap->capacity = INITIAL_CAPACITY; // Set the starting capacity.
    heap->size = 0; // Start with zero emails.
    heap->data = (Email*)malloc(heap->capacity * sizeof(Email)); // Allocate space for emails.

    // Check if the email array was allocated.
    if (heap->data == NULL) {
        printf("Memory allocation failed.\n"); // Print an error message.
        free(heap); // Free the heap structure.
        exit(1); // Stop the program.
    }

    return heap; // Return the new heap.
}

// Free heap memory
void free_heap(MaxHeap* heap) {
    if (heap) { // Check if the heap exists.
        free(heap->data); // Free the email array.
        free(heap); // Free the heap structure.
    }
}

// Swap two emails
void swap(Email* a, Email* b) {
    Email temp = *a; // Save the first email.
    *a = *b; // Put the second email into the first position.
    *b = temp; // Put the saved email into the second position.
}

// Sift Up operation
void sift_up(MaxHeap* heap, int index) {
    // Continue until the email reaches the root.
    while (index > 0) {
        int parent = (index - 1) / 2; // Find the parent index.

        // Check if the current email has higher priority.
        if (compare_emails(heap->data[index], heap->data[parent])) {
            swap(&heap->data[index], &heap->data[parent]); // Swap the email with its parent.
            index = parent; // Continue from the parent's position.
        } else {
            break; // Stop when the heap order is correct.
        }
    }
}

// Sift Down operation
void sift_down(MaxHeap* heap, int index) {
    int max_index = index; // Start with the current position.
    int left = 2 * index + 1; // Find the left child.
    int right = 2 * index + 2; // Find the right child.

    // Check if the left child has higher priority.
    if (left < heap->size && compare_emails(heap->data[left], heap->data[max_index])) {
        max_index = left; // Set the left child as the highest priority.
    }

    // Check if the right child has higher priority.
    if (right < heap->size && compare_emails(heap->data[right], heap->data[max_index])) {
        max_index = right; // Set the right child as the highest priority.
    }

    // Check if the current position needs to be changed.
    if (max_index != index) {
        swap(&heap->data[index], &heap->data[max_index]); // Swap with the higher-priority child.
        sift_down(heap, max_index); // Continue fixing the heap.
    }
}

// Insert email into MaxHeap
void insert_email(MaxHeap* heap, Email email) {
    // Check if the heap needs more space.
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2; // Double the heap capacity.

        // Create a temporary pointer for the resized array.
        Email* temp = (Email*)realloc(heap->data, heap->capacity * sizeof(Email));

        // Check if the memory was resized successfully.
        if (temp == NULL) {
            printf("Memory allocation failed.\n"); // Print an error message.
            free(heap->data); // Free the old email array.
            free(heap); // Free the heap.
            exit(1); // Stop the program.
        }

        heap->data = temp; // Use the resized array.
    }

    heap->data[heap->size] = email; // Add the new email at the end.
    sift_up(heap, heap->size); // Move the email to the correct position.
    heap->size++; // Increase the number of emails.
}

// Extract max email (READ action)
Email extract_max(MaxHeap* heap) {
    Email empty = {"", 0, "", "", 0}; // Create an empty email.

    // Check if there are no emails.
    if (heap->size == 0) return empty; // Return the empty email.

    Email root = heap->data[0]; // Save the highest-priority email.
    heap->data[0] = heap->data[heap->size - 1]; // Move the last email to the root.
    heap->size--; // Decrease the number of emails.
    sift_down(heap, 0); // Restore the heap order.
    return root; // Return the removed email.
}

// Peek max email (NEXT action)
Email peek_max(MaxHeap* heap) {
    Email empty = {"", 0, "", "", 0}; // Create an empty email.

    // Check if there are no emails.
    if (heap->size == 0) return empty; // Return the empty email.

    return heap->data[0]; // Return the highest-priority email.
}

// Trim leading and trailing whitespace
void trim_whitespace(char* str) {
    char* start = str; // Start at the beginning of the string.
    char* end; // Create a pointer for the end of the string.

    while (isspace((unsigned char)*start)) start++; // Skip spaces at the beginning.

    // Check if the string is empty.
    if (*start == 0) {
        str[0] = '\0'; // Make the string empty.
        return; // Stop the function.
    }

    end = start + strlen(start) - 1; // Find the last character.

    while (end > start && isspace((unsigned char)*end)) end--; // Remove spaces at the end.

    *(end + 1) = '\0'; // Add the string terminator.

    // Move the trimmed string to the beginning if needed.
    if (start != str) {
        memmove(str, start, strlen(start) + 1); // Move the string to the original location.
    }
}

// Process individual commands from the test file/input
void process_command(MaxHeap* heap, char* line) {
    // Check if the command is EMAIL.
    if (strncmp(line, "EMAIL", 5) == 0) {
        // Format: EMAIL <sender category>, <subject line>, <date>
        char* content = line + 5; // Skip the word EMAIL.

        while (*content && isspace((unsigned char)*content)) content++; // Skip spaces after EMAIL.

        char* token1 = strtok(content, ","); // Get the sender category.
        char* token2 = strtok(NULL, ","); // Get the subject.
        char* token3 = strtok(NULL, ","); // Get the date.

        // Check that all three email fields were found.
        if (token1 && token2 && token3) {
            Email new_email; // Create a new email.

            strncpy(new_email.category, token1, sizeof(new_email.category) - 1); // Copy the category.
            new_email.category[sizeof(new_email.category) - 1] = '\0'; // End the category string.
            trim_whitespace(new_email.category); // Remove extra spaces.

            strncpy(new_email.subject, token2, sizeof(new_email.subject) - 1); // Copy the subject.
            new_email.subject[sizeof(new_email.subject) - 1] = '\0'; // End the subject string.
            trim_whitespace(new_email.subject); // Remove extra spaces.

            strncpy(new_email.date, token3, sizeof(new_email.date) - 1); // Copy the date.
            new_email.date[sizeof(new_email.date) - 1] = '\0'; // End the date string.
            trim_whitespace(new_email.date); // Remove extra spaces.

            new_email.category_priority = get_category_priority(new_email.category); // Set the email priority.
            new_email.date_val = parse_date(new_email.date); // Convert the date to a number.

            insert_email(heap, new_email); // Add the email to the heap.
        }
    } 
    else if (strcmp(line, "NEXT") == 0) {
        // Check if there are no unread emails.
        if (heap->size == 0) {
            printf("No unread emails.\n"); // Display the empty heap message.
        } else {
            Email next_email = peek_max(heap); // Get the highest-priority email.
            printf("Next email:\n"); // Display the NEXT heading.
            printf("Sender: %s\n", next_email.category); // Display the sender.
            printf("Subject: %s\n", next_email.subject); // Display the subject.
            printf("Date: %s\n", next_email.date); // Display the date.
        }
    } 
    else if (strcmp(line, "READ") == 0) {
        // Check if there is an email to read.
        if (heap->size > 0) {
            extract_max(heap); // Remove the highest-priority email.
        }
    } 
    else if (strcmp(line, "COUNT") == 0) {
        printf("There are %d emails to read.\n", heap->size); // Display the number of unread emails.
    }
}
