#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Point {
    int income;
    int score;
};

struct Point get_spending_score_and_annual_income(char* line) {
    const char* tok;
    int income;
    int score;
    tok = strtok(line, ",");
    strtok(NULL, ",\n");
    strtok(NULL, ",\n");
    income = atoi(strtok(NULL, ",\n"));
    score = atoi(strtok(NULL, ",\n"));

    struct Point p = {income, score};
    return p;
}


void k_means(struct Point * points, int epochs, int k);



void read_mall_customers_csv(struct Point * points) {
    // read the csv dataset file
    FILE* stream = fopen("data/Mall_Customers.csv", "r");

    char line[1024];
    fgets(line, 1024, stream);// Skip column names
    int i = 0;
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        struct Point p = get_spending_score_and_annual_income(tmp);
        points[i].income = p.income;
        points[i].score = p.score;
        i++;
        //printf("Income = %d Score = %d\n ", p.income, p.score);
        free(tmp);
    }
}


int main( int argc, char **argv ) {
    /*Kmeans:
    1. Initialise the clusters: randomly choose k points and assign each data
       point to its nearest cluster point.
    2. Repeat:
        - Compute the centroid of each cluster
        - Assign each point to the nearest centroid and redefine the cluster

     */
    int k, n;                          

    k = 5; // 5 clusters
    n = 200; // Mall Customers dataset contains 200 data points
    struct Point points[n];

    read_mall_customers_csv(points);
    for (int j=0; j < n; j++) {
        printf("Income = %d Score = %d\n ", points[j].income, points[j].score);

    }

    return 0;
}




