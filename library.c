#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_TITLE 50
#define MAX_AUTHOR 50
#define MAX_ISBN 20

typedef struct {
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char isbn[MAX_ISBN];
    int available;
} Book;

void listBooks();
void borrowBook();
void returnBook();
void searchBook();
void logTransaction(const char *action, const char *isbn);

int main() {
    int choice;
    
    do {
        printf("\nLibrary Management System\n");
        printf("1. List Books\n2. Borrow Book\n3. Return Book\n4. Search Book\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();  // To handle newline
        
        switch (choice) {
            case 1: listBooks(); break;
            case 2: borrowBook(); break;
            case 3: returnBook(); break;
            case 4: searchBook(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);
    
    return 0;
}

void listBooks() {
    FILE *file = fopen("books.csv", "r");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }
    
    Book book;
    printf("\nBooks Available:\n");
    while (fscanf(file, "%49[^,],%49[^,],%19[^,],%d\n", book.title, book.author, book.isbn, &book.available) != EOF) {
        printf("%s by %s | ISBN: %s | %s\n", book.title, book.author, book.isbn, book.available ? "Available" : "Not Available");
    }
    
    fclose(file);
}

void borrowBook() {
    char isbn[MAX_ISBN];
    printf("Enter ISBN to borrow: ");
    scanf("%s", isbn);

    FILE *file = fopen("books.csv", "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!file || !temp) {
        printf("Error opening file.\n");
        return;
    }

    Book book;
    int found = 0;
    while (fscanf(file, "%49[^,],%49[^,],%19[^,],%d\n", book.title, book.author, book.isbn, &book.available) != EOF) {
        if (strcmp(book.isbn, isbn) == 0 && book.available) {
            book.available = 0;
            found = 1;
            logTransaction("Borrowed", isbn);
        }
        fprintf(temp, "%s,%s,%s,%d\n", book.title, book.author, book.isbn, book.available);
    }

    fclose(file);
    fclose(temp);
    remove("books.csv");
    rename("temp.csv", "books.csv");

    if (found)
        printf("Book borrowed successfully!\n");
    else
        printf("Book not available or incorrect ISBN.\n");
}

void returnBook() {
    char isbn[MAX_ISBN];
    printf("Enter ISBN to return: ");
    scanf("%s", isbn);

    FILE *file = fopen("books.csv", "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!file || !temp) {
        printf("Error opening file.\n");
        return;
    }

    Book book;
    int found = 0;
    while (fscanf(file, "%49[^,],%49[^,],%19[^,],%d\n", book.title, book.author, book.isbn, &book.available) != EOF) {
        if (strcmp(book.isbn, isbn) == 0 && !book.available) {
            book.available = 1;
            found = 1;
            logTransaction("Returned", isbn);
        }
        fprintf(temp, "%s,%s,%s,%d\n", book.title, book.author, book.isbn, book.available);
    }

    fclose(file);
    fclose(temp);
    remove("books.csv");
    rename("temp.csv", "books.csv");

    if (found)
        printf("Book returned successfully!\n");
    else
        printf("Invalid ISBN or book was not borrowed.\n");
}

void searchBook() {
    char query[MAX_TITLE];
    printf("Enter book title or author to search: ");
    getchar();
    fgets(query, MAX_TITLE, stdin);
    query[strcspn(query, "\n")] = 0; // Remove newline

    FILE *file = fopen("books.csv", "r");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Book book;
    int found = 0;
    printf("\nSearch Results:\n");
    while (fscanf(file, "%49[^,],%49[^,],%19[^,],%d\n", book.title, book.author, book.isbn, &book.available) != EOF) {
        if (strstr(book.title, query) || strstr(book.author, query)) {
            printf("%s by %s | ISBN: %s | %s\n", book.title, book.author, book.isbn, book.available ? "Available" : "Not Available");
            found = 1;
        }
    }

    fclose(file);
    if (!found) printf("No matching books found.\n");
}

void logTransaction(const char *action, const char *isbn) {
    FILE *logFile = fopen("log.txt", "a");
    if (!logFile) {
        printf("Error opening log file.\n");
        return;
    }

    fprintf(logFile, "%s ISBN: %s\n", action, isbn);
    fclose(logFile);
}