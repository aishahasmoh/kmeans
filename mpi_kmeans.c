#include <mpi.h> //include MPI header to have access to MPI functions.
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
        printf("Income = %d Score = %d\n ", p.income, p.score);
        free(tmp);
    }
}

void kMeansClustering(struct Point * points, int epochs, int k);


int main( int argc, char **argv ) {
    int rank, size, k, n;                          
    MPI_Init( &argc, &argv ); //init the MPI execution environment
    //Each MPI process is assigned a unique integer ID starting at 0.
    //Retrieve the local MPI processID in the variable rank.
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    //Retrieve the number of MPI processes running in the variable size. 
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    /*Kmeans:
    1. Initialise the clusters: randomly choose k points and assign each data
       point to its nearest cluster point.
    2. Repeat:
        - Compute the centroid of each cluster
        - Assign each point to the nearest centroid and redefine the cluster

     */
    k = 5; // 5 clusters
    n = 200; // Mall Customers dataset contains 200 data points
    struct Point points[n];

    if ( rank == 0 ) {
        read_mall_customers_csv(points);
        //for (int j=0; j < n; j++) {
        //    printf("Income = %d Score = %d\n ", points[j].income, points[j].score);

        //}
        /* Rank 0 sends an integer to each of the other process ranks */
        int i;
        int value = 0;
        for (i = 1; i < size; i++) {
            value = value + i;
            MPI_Send(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        /* All other process ranks receive one number from Rank 0 */
        int value;
        MPI_Status status;
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Rank %d received value %d\n", rank, value);
    }


    MPI_Finalize( ); //This must be done before the end of the program.

    return 0;
}
