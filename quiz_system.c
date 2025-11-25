#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_Q 20
#define FILE_Q "questions.txt"

// ---------------- STRUCTURE ----------------

typedef struct {
    char question[200];
    char optA[100];
    char optB[100];
    char optC[100];
    char optD[100];
    char correct;
} Quiz;

// ---------------- FUNCTION DECLARATIONS ----------------

int loadQuestions(Quiz q[]);
void startQuiz(Quiz q[], int count);
void showSummary(int total, int correct);

// ---------------- MAIN ----------------

int main() {
    Quiz q[MAX_Q];
    int count = loadQuestions(q);

    if (count == 0) {
        printf("No questions loaded! Check %s file.\n", FILE_Q);
        return 0;
    }

    printf("\n===== WELCOME TO THE QUIZ SYSTEM =====\n");
    printf("Total Questions Loaded: %d\n", count);
    printf("Press Enter to Start...");
    getchar();
    getchar();

    startQuiz(q, count);

    return 0;
}

// ---------------- LOAD QUESTIONS FROM FILE ----------------

int loadQuestions(Quiz q[]) {
    FILE *fp = fopen(FILE_Q, "r");
    if (!fp) {
        printf("Error: Cannot open %s file!\n", FILE_Q);
        return 0;
    }

    int count = 0;

    while (!feof(fp) && count < MAX_Q) {
        fgets(q[count].question, 200, fp);
        fgets(q[count].optA, 100, fp);
        fgets(q[count].optB, 100, fp);
        fgets(q[count].optC, 100, fp);
        fgets(q[count].optD, 100, fp);
        q[count].correct = fgetc(fp);
        fgetc(fp); // to skip newline

        // Remove newline characters
        q[count].question[strcspn(q[count].question, "\n")] = '\0';
        q[count].optA[strcspn(q[count].optA, "\n")] = '\0';
        q[count].optB[strcspn(q[count].optB, "\n")] = '\0';
        q[count].optC[strcspn(q[count].optC, "\n")] = '\0';
        q[count].optD[strcspn(q[count].optD, "\n")] = '\0';

        count++;
    }

    fclose(fp);
    return count;
}

// ---------------- START QUIZ ----------------

void startQuiz(Quiz q[], int count) {
    int correct = 0;
    char ans;

    for (int i = 0; i < count; i++) {
        system("cls"); // clear screen for Windows (use "clear" on Linux/Mac)

        printf("\n------------ Question %d ------------\n", i + 1);
        printf("%s\n", q[i].question);
        printf("A) %s\n", q[i].optA);
        printf("B) %s\n", q[i].optB);
        printf("C) %s\n", q[i].optC);
        printf("D) %s\n", q[i].optD);

        // Input validation loop
        do {
            printf("Your Answer (A/B/C/D): ");
            scanf(" %c", &ans);
            ans = toupper(ans);
        } while (ans < 'A' || ans > 'D');

        if (ans == q[i].correct)
            correct++;

        printf("Correct Answer: %c\n", q[i].correct);

        printf("\nPress Enter for next question...");
        getchar();
        getchar();
    }

    showSummary(count, correct);
}

// ---------------- SUMMARY ----------------

void showSummary(int total, int correct) {
    system("cls");
    printf("\n=========== QUIZ SUMMARY ===========\n");
    printf("Total Questions : %d\n", total);
    printf("Correct Answers : %d\n", correct);
    printf("Wrong Answers   : %d\n", total - correct);

    float percentage = (correct * 100.0) / total;
    printf("Score           : %.2f%%\n", percentage);

    printf("\nThank you for participating!\n");
}
