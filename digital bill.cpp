#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DISCOUNT_THRESHOLD 10000  // 10,000 for discount
#define DISCOUNT_RATE 0.40        // 40% discount

// Structure to store item details
struct Item {
    char name[50];
    float price;
    int quantity;
};

// Structure to store customer transactions
struct Customer {
    char name[50];
    char date[15];
    float total_spent;
};

// Function to get today's date automatically
void getCurrentDate(char *dateStr) {
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    strftime(dateStr, 15, "%Y-%m-%d", tm_info);  // Format: YYYY-MM-DD
}

// Function to calculate total amount
float calculateTotal(struct Item items[], int count) {
    float total = 0;
    for (int i = 0; i < count; i++) {
        total += items[i].price * items[i].quantity;
    }
    return total;
}

// Function to check total spent by a customer
float getCustomerTotalSpent(char customerName[]) {
    FILE *file = fopen("customers.txt", "r");
    if (!file) return 0;  // If file doesn't exist, return 0

    char name[50], date[15];
    float amount, total = 0;

    // Read file and calculate total spent by the customer
    while (fscanf(file, "%s %s %f", name, date, &amount) != EOF) {
        if (strcmp(name, customerName) == 0) {
            total += amount;
        }
    }

    fclose(file);
    return total;
}

// Function to save transaction
void saveTransaction(char customerName[], char date[], float amount) {
    FILE *file = fopen("customers.txt", "a");  // Open in append mode
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s %s %.2f\n", customerName, date, amount);
    fclose(file);
}

// Function to sort and rewrite transactions in alphabetical order
void sortAndRewriteFile() {
    struct Customer customers[100];
    int count = 0;

    FILE *file = fopen("customers.txt", "r");
    if (!file) return;

    // Read all transactions into an array
    while (fscanf(file, "%s %s %f", customers[count].name, customers[count].date, &customers[count].total_spent) != EOF) {
        count++;
    }
    fclose(file);

    // Sort customers alphabetically by name (Bubble Sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(customers[i].name, customers[j].name) > 0) {
                struct Customer temp = customers[i];
                customers[i] = customers[j];
                customers[j] = temp;
            }
        }
    }

    // Rewrite the file with sorted transactions
    file = fopen("customers.txt", "w");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %.2f\n", customers[i].name, customers[i].date, customers[i].total_spent);
    }

    fclose(file);
}

// Main function
int main() {
    struct Item items[50];
    int itemCount = 0;
    char customerName[50], date[15];
    float totalAmount, previousTotal;

    // Get customer name
    printf("Enter customer name: ");
    scanf("%s", customerName);

    // Automatically get today's date
    getCurrentDate(date);

    // Get previous total spent by customer
    previousTotal = getCustomerTotalSpent(customerName);

    // Input items
    printf("\nHow many items are being purchased? ");
    scanf("%d", &itemCount);

    for (int i = 0; i < itemCount; i++) {
        printf("\nEnter item %d name: ", i + 1);
        scanf(" %[^\n]", items[i].name); // Read full name (including spaces)
        printf("Enter item %d price: ", i + 1);
        scanf("%f", &items[i].price);
        printf("Enter item %d quantity: ", i + 1);
        scanf("%d", &items[i].quantity);
    }

    // Calculate total
    totalAmount = calculateTotal(items, itemCount);

    // Apply discount if eligible
    if (previousTotal < DISCOUNT_THRESHOLD && (previousTotal + totalAmount) >= DISCOUNT_THRESHOLD) {
        printf("\nCongratulations! You get a 40%% discount on this bill.\n");
        totalAmount *= (1 - DISCOUNT_RATE);
    }

    // Display bill
    printf("\n======== BILL SUMMARY ========\n");
    printf("Customer Name: %s\n", customerName);
    printf("Date: %s\n", date);
    printf("Total Items: %d\n", itemCount);
    printf("Total Amount: $%.2f\n", totalAmount);
    printf("==============================\n");

    // Save transaction
    saveTransaction(customerName, date, totalAmount);

    // Sort and rewrite transactions in file
    sortAndRewriteFile();

    printf("Transaction saved successfully!\n");

    return 0;
}

