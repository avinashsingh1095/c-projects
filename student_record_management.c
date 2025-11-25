#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_STUDENTS "students.dat"

// ---------------- STRUCTURE ----------------

typedef struct {
    int roll;
    char name[50];
    char dept[30];
    int semester;
    float marks;
} Student;

// ------------ FUNCTION DECLARATIONS -----------

void addStudent();
void viewStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

// ---------------- MENU ----------------

int main() {
    int choice;

    while (1) {
        printf("\n=========== STUDENT RECORD MANAGEMENT SYSTEM ===========\n");
        printf("1. Add Student Record\n");
        printf("2. View All Records\n");
        printf("3. Search Student by Roll Number\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Exit\n");
        printf("--------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

// ---------------- ADD STUDENT ----------------

void addStudent() {
    FILE *fp = fopen(FILE_STUDENTS, "ab+");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    Student s;

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);

    getchar();
    printf("Enter Name: ");
    fgets(s.name, 50, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Department: ");
    fgets(s.dept, 30, stdin);
    s.dept[strcspn(s.dept, "\n")] = '\0';

    do {
        printf("Enter Semester (1-8): ");
        scanf("%d", &s.semester);
    } while (s.semester < 1 || s.semester > 8);

    do {
        printf("Enter Marks (0-100): ");
        scanf("%f", &s.marks);
    } while (s.marks < 0 || s.marks > 100);

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("Record added successfully!\n");
}

// ---------------- VIEW ALL STUDENTS ----------------

void viewStudents() {
    FILE *fp = fopen(FILE_STUDENTS, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    Student s;
    int count = 0;

    printf("\n-------------------- STUDENT LIST --------------------\n");

    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("\nRoll Number : %d\nName        : %s\nDepartment  : %s\nSemester    : %d\nMarks       : %.2f\n",
               s.roll, s.name, s.dept, s.semester, s.marks);
        count++;
    }

    if (count == 0)
        printf("No records available.\n");

    fclose(fp);
}

// ---------------- SEARCH STUDENT ----------------

void searchStudent() {
    FILE *fp = fopen(FILE_STUDENTS, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int roll, found = 0;
    Student s;

    printf("\nEnter Roll Number to Search: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll == roll) {
            printf("\n--- Student Found ---\n");
            printf("Roll Number : %d\nName        : %s\nDepartment  : %s\nSemester    : %d\nMarks       : %.2f\n",
                   s.roll, s.name, s.dept, s.semester, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student not found.\n");

    fclose(fp);
}

// ---------------- UPDATE STUDENT ----------------

void updateStudent() {
    FILE *fp = fopen(FILE_STUDENTS, "rb+");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int roll, found = 0;
    Student s;

    printf("\nEnter Roll Number to Update: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll == roll) {
            found = 1;

            getchar();
            printf("Enter New Name: ");
            fgets(s.name, 50, stdin);
            s.name[strcspn(s.name, "\n")] = '\0';

            printf("Enter New Department: ");
            fgets(s.dept, 30, stdin);
            s.dept[strcspn(s.dept, "\n")] = '\0';

            do {
                printf("Enter New Semester (1-8): ");
                scanf("%d", &s.semester);
            } while (s.semester < 1 || s.semester > 8);

            do {
                printf("Enter New Marks (0-100): ");
                scanf("%f", &s.marks);
            } while (s.marks < 0 || s.marks > 100);

            fseek(fp, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);

            printf("Record updated successfully!\n");
            break;
        }
    }

    if (!found)
        printf("Student not found.\n");

    fclose(fp);
}

// ---------------- DELETE STUDENT ----------------

void deleteStudent() {
    FILE *fp = fopen(FILE_STUDENTS, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    int roll, found = 0;
    Student s;

    printf("\nEnter Roll Number to Delete: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll == roll) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_STUDENTS);
    rename("temp.dat", FILE_STUDENTS);

    if (found)
        printf("Student record deleted successfully!\n");
    else
        printf("Student not found.\n");
}
