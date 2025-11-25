#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_BOOKS "books.dat"
#define FILE_ISSUED "issued.dat"

// -------------------- STRUCTURES -----------------------

typedef struct {
    int id;
    char title[50];
    char author[50];
    int year;
    int available; 
} Book;

typedef struct {
    int book_id;
    char borrower[50];
    char date[20];
} IssuedBook;

// -------------------- FUNCTION DECLARATIONS -----------------------

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();
void showIssued();

// -------------------- MAIN MENU -----------------------
int main() {
    int choice;

    while (1) {
        printf("\n========= LIBRARY MANAGEMENT SYSTEM =========\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Show Issued Books\n");
        printf("8. Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7: showIssued(); break;
            case 8: exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

// -------------------- ADD BOOK ------------------------
void addBook() {
    FILE *fp = fopen(FILE_BOOKS, "ab+");
    if (!fp) {
        printf("Error creating file.\n");
        return;
    }

    Book b;
    printf("\nEnter Book ID: ");
    scanf("%d", &b.id);

    getchar(); // clear buffer
    printf("Enter Title: ");
    fgets(b.title, 50, stdin);
    b.title[strcspn(b.title, "\n")] = '\0';

    printf("Enter Author: ");
    fgets(b.author, 50, stdin);
    b.author[strcspn(b.author, "\n")] = '\0';

    printf("Enter Year: ");
    scanf("%d", &b.year);

    b.available = 1;

    fwrite(&b, sizeof(Book), 1, fp);
    fclose(fp);

    printf("Book added successfully.\n");
}

// -------------------- DISPLAY ALL BOOKS ------------------------
void displayBooks() {
    FILE *fp = fopen(FILE_BOOKS, "rb");
    if (!fp) {
        printf("No books available.\n");
        return;
    }

    Book b;
    int count = 0;

    printf("\n----- BOOK LIST -----\n");

    while (fread(&b, sizeof(Book), 1, fp)) {
        printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nStatus: %s\n",
               b.id, b.title, b.author, b.year,
               b.available ? "Available" : "Issued");
        count++;
    }

    if (count == 0)
        printf("No books found.\n");

    fclose(fp);
}

// -------------------- SEARCH BOOK ------------------------
void searchBook() {
    FILE *fp = fopen(FILE_BOOKS, "rb");
    if (!fp) {
        printf("No books found.\n");
        return;
    }

    int id, found = 0;
    printf("\nEnter Book ID to search: ");
    scanf("%d", &id);

    Book b;
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            printf("\nBook Found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nStatus: %s\n",
                   b.id, b.title, b.author, b.year,
                   b.available ? "Available" : "Issued");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found.\n");

    fclose(fp);
}

// -------------------- ISSUE BOOK ------------------------
void issueBook() {
    FILE *fp = fopen(FILE_BOOKS, "rb+");
    FILE *fi = fopen(FILE_ISSUED, "ab+");

    if (!fp || !fi) {
        printf("File error.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    Book b;

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id && b.available == 1) {
            found = 1;

            b.available = 0;
            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);

            IssuedBook ib;
            ib.book_id = id;

            getchar();
            printf("Enter Borrower Name: ");
            fgets(ib.borrower, 50, stdin);
            ib.borrower[strcspn(ib.borrower, "\n")] = '\0';

            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            sprintf(ib.date, "%02d/%02d/%04d",
                    tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

            fwrite(&ib, sizeof(IssuedBook), 1, fi);

            printf("Book issued successfully.\n");
            break;
        }
    }

    if (!found)
        printf("Book not found or already issued.\n");

    fclose(fp);
    fclose(fi);
}

// -------------------- RETURN BOOK ------------------------
void returnBook() {
    FILE *fp = fopen(FILE_BOOKS, "rb+");
    FILE *fi = fopen(FILE_ISSUED, "rb");
    FILE *fo = fopen("temp.dat", "wb");

    if (!fp || !fi || !fo) {
        printf("File error.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    IssuedBook ib;

    while (fread(&ib, sizeof(IssuedBook), 1, fi)) {
        if (ib.book_id == id)
            found = 1;
        else
            fwrite(&ib, sizeof(IssuedBook), 1, fo);
    }

    fclose(fi);
    fclose(fo);

    remove(FILE_ISSUED);
    rename("temp.dat", FILE_ISSUED);

    if (!found) {
        printf("Book was not issued.\n");
        return;
    }

    Book b;
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id) {
            b.available = 1;
            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);
            break;
        }
    }

    fclose(fp);

    printf("Book returned successfully.\n");
}

// -------------------- DELETE BOOK ------------------------
void deleteBook() {
    FILE *fp = fopen(FILE_BOOKS, "rb");
    FILE *fo = fopen("temp.dat", "wb");

    if (!fp || !fo) {
        printf("File error.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    Book b;

    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == id)
            found = 1;
        else
            fwrite(&b, sizeof(Book), 1, fo);
    }

    fclose(fp);
    fclose(fo);

    remove(FILE_BOOKS);
    rename("temp.dat", FILE_BOOKS);

    if (found)
        printf("Book deleted successfully.\n");
    else
        printf("Book not found.\n");
}

// -------------------- SHOW ISSUED BOOKS ------------------------
void showIssued() {
    FILE *fp = fopen(FILE_ISSUED, "rb");
    if (!fp) {
        printf("No issued books.\n");
        return;
    }

    IssuedBook ib;
    int c = 0;

    printf("\n----- ISSUED BOOK LIST -----\n");

    while (fread(&ib, sizeof(IssuedBook), 1, fp)) {
        printf("\nBook ID: %d\nBorrower: %s\nIssue Date: %s\n",
               ib.book_id, ib.borrower, ib.date);
        c++;
    }

    if (c == 0)
        printf("No books are currently issued.\n");

    fclose(fp);
}
