

# Kmeans Implementation

## About 

### Kmeans
k-means clustering is an agorithm that finds a partitioning of points in a
dataset which minimizes the total variance within groups. The variance in a
cluster is the sum of square euclidean distances between each data point and
the cluster's centroid.

I implemented clustering on the [mall customer's dataset](https://github.com/robertmartin8/udemyML/blob/master/06_clustering/Mall_Customers.csv)  

For Simplicity, I used two features: spending score and annual income to create
a 2D dataset. 


### MPI
The Message Passing Interface (MPI) is a parallel computing Interface that
runs each process with its own local memory in parallel. Processes share data
explicitly by message passing between processes.

This program runs locally using processes on the same machine. However, the
primry purpose of using MPI is scaling beyond the processors and shared memory
of a single compute server, to the distributed memory and processors of multiple
compute servers combined together.


## Coding Standards
The code in this project follows the C style guidelines as outlined [here](https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide/)

I added unit tests for each function in the code including distance calculation,
updating points to clusters assignemnt, and updating centroid functions. 

## Setup
install `openmpi` on your machine. For macOS, download the package from this url
https://www.open-mpi.org/software/ompi/v2.0/ and
then follow the following instructions to install it on your machine: 
```sh
tar xf openmpi-2.0.2.tar
cd openmpi-2.0.2/
./configure --prefix=$HOME/opt/usr/local
make all
make install
nano ~/.bash_profile
```
append the following line to the ~/.bash_profile file
```sh
export PATH="$PATH:$HOME/opt/usr/local/bin/"
```
then run `source `

MPI should be installed and ready for use. To check that imstallation was
successful, run the following command:
```sh
mpirun --version
```

## Usage
To compile
```sh
mpicc -o mpi_kmeans mpi_kmeans.c
```
To run the program with 4 processes: 
```sh
mpirun -np 4 ./mpi_kmeans
```

### Sequential Kmeans
```sh
gcc -o kmeans kmeans.c
./kmeans
```

