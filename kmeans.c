#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h> // for DBL_MAX
#include <time.h>   // for time()
#include <math.h>  // for pow()


/************************ Struct Types **************************************/
struct Point {
    double income;
    double score;
    double min_dist;
    int cluster_idx;
};


/************************ Global Variables **********************************/
#define MAX_LINE_LEN 1024


/****************************************************************************/
/* Helper method                                                            */
/* Get the Euclidean distance between the two 2D points p1 and p2           */
/****************************************************************************/
double distance(struct Point * p1, struct Point * p2) {
    return pow((p2->income - p1->income),2) + pow((p2->score - p1->score),2);
}

void assign_points_to_clusters(struct Point * points, struct Point  * centroids, int n, int k) {
	for (int c=0; c<k; c++) {
        for (int p=0; p<n; p++) {
            double dist = distance(&points[p], &centroids[c]);
            if (dist < points[p].min_dist) {
                points[p].min_dist = dist;
                points[p].cluster_idx = c;
                printf("Updating the distance %f %d\n", dist, c);

            }
        }
    }
    //for (int p=0; p<n; p++) {
    //    printf("Income = %f Score = %f Cluster = %d, distance = %f\n ",
    //            points[p].income, points[p].score, points[p].cluster_idx, points[p].min_dist);
    //}
}

void compute_new_cluster_centroids(struct Point * points, struct Point * centroids, int n, int k) {

    double income_sums[k];     // sum income for points in each centroid
    double score_sums[k];     // sum score for points in each centroid
    int n_points[k];

    //compute total incomes and total scores of all points in each cluster
    for (int p=0; p<n; p++) {
        //printf("Income = %f Score = %f Cluster = %d, distance = %f\n ",
        //        points[p].income, points[p].score, points[p].cluster_idx, points[p].min_dist);
        n_points[points[p].cluster_idx] += 1;
        income_sums[points[p].cluster_idx] += points[p].income;
        score_sums[points[p].cluster_idx] += points[p].score;
    }
    
    // compute new center = total / num_points
	for (int c=0; c<k; c++) {
        //printf("Income Sum = %f Score Sum = %f Cluster = %d, N points = %d\n ",
        //    income_sums[c], score_sums[c], c, n_points[c]);
        centroids[c].income = income_sums[c] / n_points[c];
        centroids[c].score = score_sums[c] / n_points[c];
    }

    return;
}

/****************************************************************************/
/* Kmeans:
1. Initialise the clusters: randomly choose k points and assign each data
   point to its nearest cluster point.
2. Repeat:
    - Compute the centroid of each cluster
    - Assign each point to the nearest centroid and redefine the cluster    */
/****************************************************************************/
void k_means(struct Point * points, int n, int k, int epochs) {
	assert(k <= n);

	// Pick k points at random to be the initial centroids
    srand(time(NULL)); // Seed the random number generator
    int centroids_idx[k];
    for (int i=0; i < k; i++) {
    	centroids_idx[i] = rand() % n;
    }
    struct Point centroids[k];
    for (int i=0; i < k; i++) {
    	centroids[i].income = points[centroids_idx[i]].income;
    	centroids[i].score = points[centroids_idx[i]].score;
    }
    for (int j=0; j < k; j++) {
        printf("Initial Centroid %d Income = %f Score = %f\n ", j, centroids[j].income, centroids[j].score);
    }

    for (int epoch=0; epoch<epochs; epoch++) {
        //printf("Before assigning points to clusters **********************************\n");
        //for (int p=0; p<n; p++) {
        //    printf("epoch %d Income = %f Score = %f Cluster = %d, distance = %f\n ", epoch, 
        //            points[p].income, points[p].score, points[p].cluster_idx, points[p].min_dist);
        //}
        // Assign points to a cluster
        assign_points_to_clusters(points, centroids, n, k);
        //printf("After assigning points to clusters **********************************\n");
        //for (int p=0; p<n; p++) {
        //    printf("epoch %d Income = %f Score = %f Cluster = %d, distance = %f\n ", epoch, 
        //            points[p].income, points[p].score, points[p].cluster_idx, points[p].min_dist);
        //}
        //for (int j=0; j < k; j++) {
        //    printf("Centroid Income = %f Score = %f\n ", centroids[j].income, centroids[j].score);
        //}
        // Compute new centroids
        compute_new_cluster_centroids(points, centroids, n, k);

        printf("After computing new clusters **********************************\n");
        //for (int p=0; p<n; p++) {
        //    printf("Income = %f Score = %f Cluster = %d, distance = %f\n ",
        //            points[p].income, points[p].score, points[p].cluster_idx, points[p].min_dist);
        //}
        for (int j=0; j < k; j++) {
            printf("epoch %d centroid %d Income = %f Score = %f\n ", epoch, j, centroids[j].income, centroids[j].score);
        }
    }
}


/****************************************************************************/
/* Helper method                                                            */
/****************************************************************************/
static struct Point get_spending_score_and_annual_income(char* line) {
    const char* tok;
    double income, score;

    //parse the first 3 columns
    tok = strtok(line, ","); strtok(NULL, ",\n"); strtok(NULL, ",\n");
    //parse 4th col: income, and 5th col: score
    income = atof(strtok(NULL, ",\n"));
    score = atof(strtok(NULL, ",\n"));

    struct Point p = {income, score, DBL_MAX, -1};
    return p;
}


/****************************************************************************/
/* Read the csv dataset file                                                */
/****************************************************************************/
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
        points[line_idx].min_dist = DBL_MAX;
        points[line_idx].cluster_idx = -1;
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
    int k = 5, n = 200, n_epochs=200;
	struct Point points[n];
    
    read_csv("data/Mall_Customers.csv", points);
    for (int j=0; j < n; j++) {
        //printf("Income = %f Score = %f\n ", points[j].income, points[j].score);

    }

    k_means(points, n, k, n_epochs);

    return 0;
}




