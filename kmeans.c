#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>  // for DBL_MAX
#include <time.h>   // for time()
#include <math.h>   // for pow()
#include "kmeans.h"


/******************************************************************************/
/* Helper method:  retrn Euclidean distance between two 2D points p1 and p2   */  
/******************************************************************************/
double distance(struct Point * p1, struct Point * p2) {
    return sqrt(pow((p2->income - p1->income), 2)
        + pow((p2->score - p1->score), 2));
}

/******************************************************************************/
/* Assign each data point to the cluster who's centeroid is closes to point   */
/******************************************************************************/
void assign_points_to_clusters(
        struct Point * points, struct Point  * centroids, int n, int k) {
    for (int p=0; p<n; p++) {
        	for (int c=0; c<k; c++) {
            double dist = distance(&points[p], &centroids[c]);
            if (dist < points[p].min_dist) { // point is closer to centroid c
                points[p].min_dist = dist;
                points[p].cluster_idx = c;
            }
        }
    }
}

/******************************************************************************/
/* Compute new centroids for each cluster using the points assigned to it.    */
/******************************************************************************/
void compute_new_cluster_centroids(
        struct Point * points, struct Point * centroids, int n, int k) {

    double income_sums[k];      // sum income for points in each centroid
    double score_sums[k];       // sum score for points in each centroid
    int n_points[k];            // number ofs points in each centroid

    //compute total incomes and total scores of all points in each cluster
    for (int p=0; p<n; p++) {
        n_points[points[p].cluster_idx] += 1;
        income_sums[points[p].cluster_idx] += points[p].income;
        score_sums[points[p].cluster_idx] += points[p].score;
        points[p].min_dist = DBL_MAX;  // reset distance
    }
    
    // compute new center's income and score = total / num_points
	for (int c=0; c<k; c++) {
        centroids[c].income = income_sums[c] / n_points[c];
        centroids[c].score = score_sums[c] / n_points[c];
    }

    return;
}

/******************************************************************************/
/* Kmeans clustering algorithm:                                                                    
1. Initialise the clusters: randomly choose k points and assign each data
   point to its nearest cluster point.
2. Repeat:
    - Compute the centroid of each cluster
    - Assign each point to the nearest centroid                               */
/******************************************************************************/
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
    printf("Initial Centroids");
    for (int j=0; j < k; j++) {
        printf("Centroid %d Income = %f Score = %f\n ",
            j, centroids[j].income, centroids[j].score);
    }

    for (int epoch=0; epoch<epochs; epoch++) {
        assign_points_to_clusters(points, centroids, n, k);
        compute_new_cluster_centroids(points, centroids, n, k);
    }

    printf("Final Centroids");
    for (int j=0; j < k; j++) {
        printf("Centroid %d Income = %f Score = %f\n ",
            j, centroids[j].income, centroids[j].score);
    }
}


/******************************************************************************/
/* Helper method: parse one line and return the income and score.             */
/******************************************************************************/
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


/******************************************************************************/
/* Read the csv dataset file                                                  */
/******************************************************************************/
void read_csv(struct Point * points, int n, const char * csv_file) {    
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

void save_csv(struct Point * points, int n, const char * csv_file) {
    FILE *fp = fopen(csv_file, "w"); // Open file for writing
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(fp, "income, score, cluster\n");

    // Write array elements to the file, separated by commas
    for (int i = 0; i < n; i++) {
        if (i != (n-1)) {
            fprintf(fp, "%f, %f, %d\n",
                points[i].income, points[i].score, points[i].cluster_idx);
        } else {
        fprintf(fp, "%f, %f, %d",
            points[i].income, points[i].score, points[i].cluster_idx);
        }
    }

    fclose(fp); // Close the file
}

/************************* Unit Tests *****************************************/
void test_distance_calculation() {
    struct Point p1 = {0, 0, 0, -1};
    struct Point p2 = {3, 4, 0, -1};
    double dist = distance(&p1, &p2);
    assert(dist == 5.0); // Distance between (0,0) and (3,4) = sqrt(5^2) = 5
}

void test_assign_points_to_clusters() {
    struct Point centroids[] = {{1, 1, -1, -1}, {4, 4, -1, -1}, {7, 7, -1, -1}};
    assert(centroids[0].income == 1 && centroids[0].score == 1);
    assert(centroids[1].income == 4 && centroids[1].score == 4);
    assert(centroids[2].income == 7 && centroids[2].score == 7);
    struct Point assigned_points[] = {
        {2, 2, DBL_MAX, -1}, {5, 5, DBL_MAX, -1}, {8, 8, DBL_MAX, -1}};
    assign_points_to_clusters(assigned_points, centroids, 3, 3);
    assert(assigned_points[0].cluster_idx ==  0);
    assert(assigned_points[1].cluster_idx ==  1);
    assert(assigned_points[2].cluster_idx ==  2);
}

void test_compute_new_cluster_centroids() {
    struct Point centroids[] = {{1, 1, -1, -1}, {4, 4, -1, -1}, {7, 7, -1, -1}};
    assert(centroids[0].income == 1 && centroids[0].score == 1);
    assert(centroids[1].income == 4 && centroids[1].score == 4);
    assert(centroids[2].income == 7 && centroids[2].score == 7);
    struct Point assigned_points[] = {
        {2, 2, DBL_MAX, 0}, {5, 5, DBL_MAX, 1}, {8, 8, DBL_MAX, 2}};
    assign_points_to_clusters(assigned_points, centroids, 3, 3);
    assert(assigned_points[0].cluster_idx ==  0);
    assert(assigned_points[1].cluster_idx ==  1);
    assert(assigned_points[2].cluster_idx ==  2);
    assert(centroids[0].income == 2 && centroids[0].score == 2);
    assert(centroids[1].income == 5 && centroids[1].score == 5);
    assert(centroids[2].income == 8 && centroids[2].score == 8);
}
/******************************************************************************/


/******************************************************************************/
/* The program reads the "data/mall_data.csv" file and performs Kmeans   */
/* Clustering on it. It assumes number of clusters (k) = 5.                   */
/* Mall Customers dataset contains 200 data points                            */
/******************************************************************************/
int main( int argc, char **argv ) {
    int k = 5, n = 200, n_epochs=1000;
	struct Point points[n];
    
    read_csv(points, n, "data/mall_data.csv");

    k_means(points, n, k, n_epochs);

    save_csv(points, n, "data/kmeans_results.csv");

    printf("Running unit tests...\n");
    test_distance_calculation();
    test_assign_points_to_clusters();
    // Call other unit tests
    printf("All unit tests passed!\n");
    return 0;
}

