#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "accounts.txt"
#define MAX_NAME 50
#define MAX_PASS 20

typedef struct {
    char name[MAX_NAME];
    char password[MAX_PASS];
    double balance;
} Account;

void createAccount();
int login(Account *acc);
void deposit(Account *acc);
void withdraw(Account *acc);
void viewBalance(Account *acc);
void updateAccount(Account *acc);

int main() {
    int choice;
    Account acc;
    while (1) {
        printf("\n--- Bank Application ---\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        if (choice == 1) {
            createAccount();
        } else if (choice == 2) {
            if (login(&acc)) {
                int loggedIn = 1;
                while (loggedIn) {
                    printf("\nWelcome, %s!\n", acc.name);
                    printf("1. Deposit\n");
                    printf("2. Withdraw\n");
                    printf("3. View Balance\n");
                    printf("4. Logout\n");
                    printf("Select an option: ");
                    scanf("%d", &choice);
                    getchar();
                    switch (choice) {
                        case 1: deposit(&acc); break;
                        case 2: withdraw(&acc); break;
                        case 3: viewBalance(&acc); break;
                        case 4: loggedIn = 0; updateAccount(&acc); break;
                        default: printf("Invalid choice!\n");
                    }
                }
            } else {
                printf("Login failed!\n");
            }
        } else if (choice == 3) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice!\n");
        }
    }
    return 0;
}

void createAccount() {
    Account acc;
    printf("Enter name: ");
    fgets(acc.name, MAX_NAME, stdin);
    acc.name[strcspn(acc.name, "\n")] = 0;
    printf("Enter password: ");
    fgets(acc.password, MAX_PASS, stdin);
    acc.password[strcspn(acc.password, "\n")] = 0;
    acc.balance = 0.0;

    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        printf("File error!\n");
        return;
    }
    fprintf(file, "%s,%s,%.2lf\n", acc.name, acc.password, acc.balance);
    fclose(file);
    printf("Account created successfully!\n");
}

int login(Account *acc) {
    char name[MAX_NAME], password[MAX_PASS];
    printf("Enter name: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;
    printf("Enter password: ");
    fgets(password, MAX_PASS, stdin);
    password[strcspn(password, "\n")] = 0;

    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("File error!\n");
        return 0;
    }
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char file_name[MAX_NAME], file_pass[MAX_PASS];
        double file_balance;
        sscanf(line, "%[^,],%[^,],%lf", file_name, file_pass, &file_balance);
        if (strcmp(name, file_name) == 0 && strcmp(password, file_pass) == 0) {
            strcpy(acc->name, file_name);
            strcpy(acc->password, file_pass);
            acc->balance = file_balance;
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void deposit(Account *acc) {
    double amount;
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);
    getchar();
    if (amount > 0) {
        acc->balance += amount;
        printf("Deposited successfully. New balance: %.2lf\n", acc->balance);
    } else {
        printf("Invalid amount!\n");
    }
}

void withdraw(Account *acc) {
    double amount;
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);
    getchar();
    if (amount > 0 && amount <= acc->balance) {
        acc->balance -= amount;
        printf("Withdrawn successfully. New balance: %.2lf\n", acc->balance);
    } else {
        printf("Invalid amount or insufficient funds!\n");
    }
}

void viewBalance(Account *acc) {
    printf("Current balance: %.2lf\n", acc->balance);
}

void updateAccount(Account *acc) {
    FILE *file = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) {
        printf("File error!\n");
        return;
    }
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char file_name[MAX_NAME], file_pass[MAX_PASS];
        double file_balance;
        sscanf(line, "%[^,],%[^,],%lf", file_name, file_pass, &file_balance);
        if (strcmp(acc->name, file_name) == 0 && strcmp(acc->password, file_pass) == 0) {
            fprintf(temp, "%s,%s,%.2lf\n", acc->name, acc->password, acc->balance);
        } else {
            fprintf(temp, "%s", line);
        }
    }
    fclose(file);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
}