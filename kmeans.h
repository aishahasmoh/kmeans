#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>  // for DBL_MAX
#include <time.h>   // for time()
#include <math.h>   // for pow()


/************************** Struct Types **************************************/
struct Point {
    double income;
    double score;
    double min_dist;
    int cluster_idx;
};


/************************** Global Variables **********************************/
#define MAX_LINE_LEN 1024


double distance(struct Point * p1, struct Point * p2);

void assign_points_to_clusters(
    struct Point * points, int n, struct Point * centroids, int k);

void compute_new_cluster_centroids(
    struct Point * points, int n, struct Point * centroids, int k);

void k_means(
    struct Point * points, int n, struct Point * centroids, int k, int epochs);

static struct Point get_spending_score_and_annual_income(char* line);

void read_csv(struct Point * points, int n, const char * csv_file);

void save_csv(struct Point * points, int n, struct Point * centroids, int k,
        const char * csv_file);