
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h> 
#include <algorithm>
#define MAX_LINES 100
#define MAX_ITEMS 100

// Define structure to hold item information
typedef struct {
    char name[100];
    float price;
    float quantity;
} Item;

int buffer_point = 69; // Set the buffer point here
	int b_p1=3;
	int b_p2=4;
    int buffer_time = 3;
// Function Declarations
void writeSelectedItemsToFile(const char *filename, Item items[], int totalItemsSelected, float totalWeight, float totalCost);
void knapsackWhole(Item items[], int n, int W, int &totalItemsSelected, float &totalWeight, float &totalCost);
void knapsackFractional(Item items[], int n, int W, int &totalItemsSelected, float &totalWeight, float &totalCost);

int main() {
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int category;
    int capacity_w;
    int capacity_f;
    char line[100];
    int lineCount_w = 0; // Count for category 1 items
    int lineCount_f = 0; // Count for category 2 items
    Item items_w[MAX_LINES];
    Item items_f[MAX_LINES];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check for category information
        if (sscanf(line, "CATEGORY_whole-(%d), MAX CAPACITY-%d", &category, &capacity_w) == 2) {
            continue; // Skip category information
        }
        if (sscanf(line, "CATEGORY_frac-(%d), MAX CAPACITY-%d", &category, &capacity_f) == 2) {
            continue; // Skip category information
        }
        // Check for category 1 items
        if (category == 1 && line[0] == '|') {
            sscanf(line, "| %99[^|] | %f | %f |", items_w[lineCount_w].name, &items_w[lineCount_w].price, &items_w[lineCount_w].quantity);
            lineCount_w++;
        }
        // Check for category 2 items
        if (category == 2 && line[0] == '|') {
            sscanf(line, "| %99[^|] | %f | %f |", items_f[lineCount_f].name, &items_f[lineCount_f].price, &items_f[lineCount_f].quantity);
            lineCount_f++;
        }
    }
    
    printf("/---------PROGRAM FOR READING THE SET OF ITEMS FROM TWO CATEGORIES AND THEN WRITITNG SELECTED ITEMS TO A FILE-------/\n");
    printf("INFORMATION:\nTHERE IS A FILE NAMELY 'test.txt', AND OTHER FILE WILL BE CREATED NAMELY 'FINAL.txt'\n--THIS WILL STORE THE SELECTED ITEMS IN THE KNAPSACK ACCORDING TO THE PREFFERED CHOICE OF THE USER i.e \nFROM EITHER WHOLE KNAPSACK CATEGORY OR FRACTIONAL CATEGORY\n");
    printf("                                  -------------------------------     -------------------------------                                \n");
	int choice;
	printf("                                //---------------------------------MENU--------------------------------//                                \n");
    printf("Choose knapsack type:\n");
    printf("1. Whole Knapsack\n");
    printf("2. Fractional Knapsack\n");
    printf("ENTER YOUR CHOICE");
    
    scanf("%d", &choice);
    printf("                READING THE FILE PLEASE WAIT PATIENTLY............               \n");
    sleep(buffer_time); // Pause for buffer_time seconds
	
    if(choice==1)
    {
        // Print items for category 1
        printf("Category 1 items:\n");
        printf("| %-20s | %-10s | %-10s |\n", "Name", "Cost", "Quantity");
        
        for (int i = 0; i < lineCount_w; i++) {
            printf("| %-20s | %-10.2f | %-10.2f |\n", items_w[i].name, items_w[i].price, items_w[i].quantity);
        }

         int totalItemsSelected = 0;
        float totalWeight = 0;
        float totalCost = 0;  
		knapsackWhole(items_w, lineCount_w, capacity_w, totalItemsSelected, totalWeight, totalCost);
		sleep(b_p2);
        printf("Total items selected for Whole Knapsack: %d\n", totalItemsSelected);
        printf("Total no of items for whole knapsack %d\n",lineCount_w);
        printf("capacity%d ",capacity_w);
        printf("Total weight: %.2f\n", totalWeight);
        printf("Total cost: %.2f\n", totalCost);
    }
    else if(choice==2)
    {
        // Print items for category 2
        printf("\nCategory 2 items:\n");
        printf("| %-20s | %-10s | %-10s |\n", "Name", "Cost", "Quantity");
        for (int i = 0; i < lineCount_f; i++) {
            printf("| %-20s | %-10.2f | %-10.2f |\n", items_f[i].name, items_f[i].price, items_f[i].quantity);
        }
        int totalItemsSelected = 0;
        float totalWeight = 0;
        float totalCost = 0;
        knapsackFractional(items_f, lineCount_f, capacity_f, totalItemsSelected, totalWeight, totalCost);
		sleep(b_p2);
        printf("Total items selected for Fractional Knapsack: %d\n", totalItemsSelected);
        printf("Total no of items for fractional knapsack %d\n",lineCount_f);
        printf("capacity %d\n",capacity_f);
        printf("Total weight: %.2f\n", totalWeight);
        printf("Total cost: %.2f\n", totalCost);    
    }
    else
    {
        printf("                   MASTI NAHI ,ENTER THE RIGHT NUMBER                   \n");
    }

    fclose(file);
    return 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

//void writeSelectedItemsToFile(const char* filename, Item items[], int totalItemsSelected, float totalWeight, float totalCost) {
//    FILE *fp;
//    fp = fopen(filename, "w");
//    fprintf(fp, "Selected items:\n");
//    fprintf(fp, "| Name                 | Price      | Quantity   |\n");
//    for (int i = 0; i < totalItemsSelected; i++) {
//        fprintf(fp, "| %-20s | %-10.2f | %-10.2f |\n", items[i].name, items[i].price, items[i].quantity);
//    }
//    fprintf(fp, "\nTotal Weight: %.2f\n", totalWeight);
//    fprintf(fp, "Total Cost: %.2f\n", totalCost);
//    fclose(fp);
//}


// Knapsack function for 0/1 knapsack problem
int knapsack(int W, int wt[], int val[], int n) {
    int i, w;
    int K[n + 1][W + 1];

    // Build table K[][] in bottom-up manner
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (wt[i - 1] <= w)
                K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            else
                K[i][w] = K[i - 1][w];
        }
    }

    return K[n][W];
}

// Whole Knapsack Function
void knapsackWhole(Item items[], int n, int W, int &totalItemsSelected, float &totalWeight, float &totalCost) {
    int wt[n];
    int val[n];

    // Convert float quantities to integers
    for (int i = 0; i < n; i++) {
        wt[i] = (int)items[i].quantity;
        val[i] = (int)items[i].price;
    }

    // Calculate the maximum value using knapsack function
    totalCost = knapsack(W, wt, val, n);
    totalWeight = W;
    totalItemsSelected = 0; // Reset the count of selected items
	printf("WAIT, SELECTING THE ITEMS TO BE INCLUDED IN THE WHOLE KNAPSACK...................... \n");
	sleep(b_p2);
    // Find the selected items
    printf("Selected items:\n");
    int i = n, w = W;
     FILE *filePointer;
    filePointer = fopen("NOOUTPUT.txt", "w");
    
    // Check if file opening was successful
    if (filePointer == NULL) {
        printf("Unable to create file.\n");
//        return 1; // Return non-zero to indicate failure
    }
   
    char line[] = "THE SELECTED ITEMS FOR THE WHOLE KNAPSACK\n";
    fprintf(filePointer, "%s", line);

    
    while (i > 0 && w > 0) {
        if (knapsack(w, wt, val, i) != knapsack(w, wt, val, i - 1)) {
            printf("| %-20s | %-10.2f | %-10.2f |\n", items[i - 1].name, items[i - 1].price, items[i - 1].quantity);
            fprintf(filePointer, "Name OF item: %s, cost: %.2f, quantity 2: %.2f\n",items[i - 1].name, items[i - 1].price, items[i - 1].quantity);
			totalItemsSelected++;
            w -= wt[i - 1];
        }
        i--;
    }
}

// Fractional Knapsack Function
void knapsackFractional(Item items[], int n, int W, int &totalItemsSelected, float &totalWeight, float &totalCost) {
    float cur_w = W; // Current weight capacity remaining
    float tot_v = 0; // Total value obtained
    int totalItemsIncluded = 0; // Total number of items included in the knapsack
	 FILE *filePointer;
 	filePointer = fopen("NOOUTPUT.txt", "w");
    
    // Check if file opening was successful
    if (filePointer == NULL) {
        printf("Unable to create file.\n");
//        return 1; // Return non-zero to indicate failure
    }
    char line[] = "THE SELECTED ITEMS FOR THE FRACTIONAL KNAPSACK\n";
    fprintf(filePointer, "%s", line);
    // Array to keep track of whether an item is used or not
    int used[MAX_LINES] = {0};
    printf("WAIT, SELECTING THE ITEMS TO BE INCLUDED IN THE FRACTIONAL KNAPSACK...................... ");
	sleep(b_p2);
    printf("Selected items for Fractional Knapsack:\n");

    // Continue until the capacity is fully utilized
    while (cur_w > 0) {
        int maxi = -1; // Index of the best object
        float max_ratio = 0; // Maximum value-to-weight ratio

        // Find the best object to add
        for (int i = 0; i < n; i++) {
            // Calculate value-to-weight ratio for the current item
            float ratio = (float) items[i].price / items[i].quantity;

            // Check if the item is unused and has a higher value-to-weight ratio
            if (!used[i] && ratio > max_ratio) {
                maxi = i;
                max_ratio = ratio;
            }
        }

        // If no suitable item is found, break the loop
        if (maxi == -1)
            break;

        // Mark the item as used
        used[maxi] = 1;

        // Calculate the quantity to add
        float quantityToAdd = (cur_w >= items[maxi].quantity) ? items[maxi].quantity : cur_w;

        // Update current weight and total value
        cur_w -= quantityToAdd;
        tot_v += quantityToAdd * max_ratio;

        // Output the selected item
        printf("Item: %s, Price: %.2f, Quantity: %.2f\n", items[maxi].name, items[maxi].price, quantityToAdd);
		fprintf(filePointer, "Name OF item: %s, cost: %.2f, quantity 2: %.2f\n", items[maxi].name, items[maxi].price, quantityToAdd);

        // Increment the total number of items included
        totalItemsIncluded++;
    }

    // Output total value obtained
    printf("Total value obtained: %.2f\n", tot_v);

    // Update total items selected, total weight, and total cost
    totalItemsSelected = totalItemsIncluded;
    totalWeight = W - cur_w;
    totalCost = tot_v;
}




