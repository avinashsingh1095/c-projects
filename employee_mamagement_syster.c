#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_EMP "employees.dat"

// ---------------- EMPLOYEE STRUCTURE ----------------

typedef struct {
    int empID;
    char name[50];
    char title[40];
    char dept[30];
    float salary;
} Employee;

// ---------------- FUNCTION DECLARATIONS ----------------

void addEmployee();
void displayEmployees();
void searchEmployee();
void modifyEmployee();
void deleteEmployee();
int idExists(int);

// ---------------- MAIN MENU ----------------

int main() {
    int choice;

    while (1) {
        printf("\n=========== EMPLOYEE DATABASE MANAGEMENT SYSTEM ===========\n");
        printf("1. Register New Employee\n");
        printf("2. View All Employees\n");
        printf("3. Search Employee by ID\n");
        printf("4. Modify Employee Details\n");
        printf("5. Delete Employee Record\n");
        printf("6. Exit\n");
        printf("-----------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: displayEmployees(); break;
            case 3: searchEmployee(); break;
            case 4: modifyEmployee(); break;
            case 5: deleteEmployee(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

// ---------------- CHECK DUPLICATE ID ----------------

int idExists(int id) {
    FILE *fp = fopen(FILE_EMP, "rb");
    if (!fp) return 0;

    Employee e;

    while (fread(&e, sizeof(Employee), 1, fp)) {
        if (e.empID == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ---------------- ADD EMPLOYEE ----------------

void addEmployee() {
    FILE *fp = fopen(FILE_EMP, "ab+");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    Employee e;

    do {
        printf("\nEnter Employee ID: ");
        scanf("%d", &e.empID);

        if (idExists(e.empID))
            printf("Error: Employee ID already exists! Try another.\n");

    } while (idExists(e.empID));

    getchar();
    printf("Enter Full Name: ");
    fgets(e.name, 50, stdin);
    e.name[strcspn(e.name, "\n")] = '\0';

    printf("Enter Job Title: ");
    fgets(e.title, 40, stdin);
    e.title[strcspn(e.title, "\n")] = '\0';

    printf("Enter Department: ");
    fgets(e.dept, 30, stdin);
    e.dept[strcspn(e.dept, "\n")] = '\0';

    do {
        printf("Enter Salary (non-negative): ");
        scanf("%f", &e.salary);
    } while (e.salary < 0);

    fwrite(&e, sizeof(Employee), 1, fp);
    fclose(fp);

    printf("Employee registered successfully!\n");
}

// ---------------- DISPLAY ALL EMPLOYEES ----------------

void displayEmployees() {
    FILE *fp = fopen(FILE_EMP, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    Employee e;
    int count = 0;

    printf("\n-------------------- EMPLOYEE LIST --------------------\n");

    while (fread(&e, sizeof(Employee), 1, fp)) {
        printf("\nID          : %d\nName        : %s\nJob Title   : %s\nDepartment  : %s\nSalary      : %.2f\n",
               e.empID, e.name, e.title, e.dept, e.salary);
        count++;
    }

    if (count == 0)
        printf("No employees available.\n");

    fclose(fp);
}

// ---------------- SEARCH EMPLOYEE ----------------

void searchEmployee() {
    FILE *fp = fopen(FILE_EMP, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int id, found = 0;
    Employee e;

    printf("\nEnter Employee ID to Search: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(Employee), 1, fp)) {
        if (e.empID == id) {
            printf("\n--- EMPLOYEE FOUND ---\n");
            printf("ID          : %d\nName        : %s\nJob Title   : %s\nDepartment  : %s\nSalary      : %.2f\n",
                   e.empID, e.name, e.title, e.dept, e.salary);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Employee not found.\n");

    fclose(fp);
}

// ---------------- MODIFY EMPLOYEE ----------------

void modifyEmployee() {
    FILE *fp = fopen(FILE_EMP, "rb+");
    if (!fp) {
        printf("No records available.\n");
        return;
    }

    int id, found = 0;
    Employee e;

    printf("\nEnter Employee ID to Modify: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(Employee), 1, fp)) {
        if (e.empID == id) {
            found = 1;

            getchar();
            printf("Enter New Name: ");
            fgets(e.name, 50, stdin);
            e.name[strcspn(e.name, "\n")] = '\0';

            printf("Enter New Job Title: ");
            fgets(e.title, 40, stdin);
            e.title[strcspn(e.title, "\n")] = '\0';

            printf("Enter New Department: ");
            fgets(e.dept, 30, stdin);
            e.dept[strcspn(e.dept, "\n")] = '\0';

            do {
                printf("Enter New Salary (non-negative): ");
                scanf("%f", &e.salary);
            } while (e.salary < 0);

            fseek(fp, -sizeof(Employee), SEEK_CUR);
            fwrite(&e, sizeof(Employee), 1, fp);

            printf("Record updated successfully!\n");
            break;
        }
    }

    if (!found)
        printf("Employee not found.\n");

    fclose(fp);
}

// ---------------- DELETE EMPLOYEE ----------------

void deleteEmployee() {
    FILE *fp = fopen(FILE_EMP, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    int id, found = 0;
    Employee e;

    printf("\nEnter Employee ID to Delete: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(Employee), 1, fp)) {
        if (e.empID == id) {
            found = 1;
        } else {
            fwrite(&e, sizeof(Employee), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_EMP);
    rename("temp.dat", FILE_EMP);

    if (found)
        printf("Employee deleted successfully!\n");
    else
        printf("Employee not found.\n");
}
