#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>   // for time()

struct Point {
    int income;
    int score;
};

#define MAX_LINE_LEN 1024



void assign_points_to_clusters(struct Point * points, int * centroids) {
	

}

void compute_new_cluster_centroids(struct Point * points, int * centroids) {

}

/****************************************************************************/
/* Kmeans:
1. Initialise the clusters: randomly choose k points and assign each data
   point to its nearest cluster point.
2. Repeat:
    - Compute the centroid of each cluster
    - Assign each point to the nearest centroid and redefine the cluster    */
/****************************************************************************/
void k_means(struct Point * points, int k, int n, int epochs) {
	assert(k <= n);

	// Pick k points at random to be the initial centroids
    // Seed the random number generator
    srand(time(NULL));
    int centroids[k];
    for (int i=0; i < k; i++) {
    	centroids[i] = rand() % n;
    }

    for (int epoch=0; epoch<epochs; epoch++) {
        // Assign points to a cluster
        assign_points_to_clusters(points, centroids);

        // Compute new centroids
        compute_new_cluster_centroids(points, centroids);
    }
}


/****************************************************************************/
/* Helper method */
/****************************************************************************/
static struct Point get_spending_score_and_annual_income(char* line) {
    const char* tok;
    int income, score;

    //parse the first 3 columns
    tok = strtok(line, ","); strtok(NULL, ",\n"); strtok(NULL, ",\n");
    //parse 4th col: income, and 5th col: score
    income = atoi(strtok(NULL, ",\n"));
    score = atoi(strtok(NULL, ",\n"));

    struct Point p = {income, score};
    return p;
}

/* Read the csv dataset file
*/
void read_csv(char * csv_file, struct Point * points) {    
    FILE* stream;
    char line[MAX_LINE_LEN];
    char* tmp;

    stream = fopen(csv_file, "r");
    assert(stream != NULL);

    fgets(line, MAX_LINE_LEN, stream);// Skip column names
    int line_idx = 0;
    while (fgets(line, MAX_LINE_LEN, stream))
    {
        tmp = strdup(line);
        struct Point p = get_spending_score_and_annual_income(tmp);

        points[line_idx].income = p.income;
        points[line_idx].score = p.score;
        line_idx++;
        free(tmp);
    }
}


/****************************************************************************/
/* The program reads the "data/Mall_Customers.csv" file and performs Kmeans */
/* Clustering on it. It assumes number of clusters (k) = 5.                 */
/* Mall Customers dataset contains 200 data points                          */
/****************************************************************************/
int main( int argc, char **argv ) {
    int k = 5, n = 200, n_epochs=100;
	struct Point points[n];
    
    read_csv("data/Mall_Customers.csv", points);
    for (int j=0; j < n; j++) {
        printf("Income = %d Score = %d\n ", points[j].income, points[j].score);

    }

    k_means(points, k, n, n_epochs);

    return 0;
}




