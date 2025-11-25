#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_ACCOUNTS "accounts.dat"

// ---------------------------- STRUCTURE ----------------------------

typedef struct {
    int accNo;
    char name[50];
    float balance;
} Account;

// ---------------------------- FUNCTIONS ----------------------------

void createAccount();
void viewAccount();
void depositMoney();
void withdrawMoney();
void showBalance();

// ---------------------------- MAIN MENU ----------------------------

int main() {
    int choice;

    while (1) {
        printf("\n========== BANKING MANAGEMENT SYSTEM ==========\n");
        printf("1. Create Account\n");
        printf("2. View Account Details\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Show Balance\n");
        printf("6. Exit\n");
        printf("----------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: showBalance(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

// ---------------------------- CREATE ACCOUNT ----------------------------

void createAccount() {
    FILE *fp = fopen(FILE_ACCOUNTS, "ab+");
    if (!fp) {
        printf("Error creating account file.\n");
        return;
    }

    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accNo);

    getchar();
    printf("Enter Account Holder Name: ");
    fgets(acc.name, 50, stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

    printf("Enter Initial Deposit Amount: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!\n");
}

// ---------------------------- VIEW ACCOUNT ----------------------------

void viewAccount() {
    FILE *fp = fopen(FILE_ACCOUNTS, "rb");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    int accNo, found = 0;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("\n--- ACCOUNT DETAILS ---\n");
            printf("Account Number : %d\n", acc.accNo);
            printf("Name           : %s\n", acc.name);
            printf("Balance        : %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");

    fclose(fp);
}

// ---------------------------- DEPOSIT MONEY ----------------------------

void depositMoney() {
    FILE *fp = fopen(FILE_ACCOUNTS, "rb+");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    int accNo, found = 0;
    float amount;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount.\n");
                fclose(fp);
                return;
            }

            acc.balance += amount;

            fseek(fp, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);

            printf("Amount deposited successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");

    fclose(fp);
}

// ---------------------------- WITHDRAW MONEY ----------------------------

void withdrawMoney() {
    FILE *fp = fopen(FILE_ACCOUNTS, "rb+");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    int accNo, found = 0;
    float amount;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount.\n");
                fclose(fp);
                return;
            }

            if (acc.balance < amount) {
                printf("Insufficient Balance.\n");
                fclose(fp);
                return;
            }

            acc.balance -= amount;

            fseek(fp, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);

            printf("Withdrawal successful.\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");

    fclose(fp);
}

// ---------------------------- SHOW BALANCE ----------------------------

void showBalance() {
    FILE *fp = fopen(FILE_ACCOUNTS, "rb");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    int accNo, found = 0;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("\nCurrent Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");

    fclose(fp);
}
