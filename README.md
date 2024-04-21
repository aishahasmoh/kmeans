

# Kmeans Implementation in MPI

### MPI
The Message Passing Interface (MPI) is a parallel computing Interface that
runs each process with its own local memory in parallel. Processes share data
explicitly by message passing between processes.

This program runs locally using processes on the same machine. However, the
primry purpose of using MPI is scaling beyond the processors and shared memory
of a single compute server, to the distributed memory and processors of multiple
compute servers combined together.

### Kmeans


## Setup
install `mpi` on your machine. For macOS, install the package from this url
https://www.open-mpi.org/software/ompi/v2.0/ and
then follow the following instructions: 
```sh
tar xf openmpi-2.0.2.tar
cd openmpi-2.0.2/
./configure --prefix=$HOME/opt/usr/local
make all
make install
nano ~/.bash_profile
# append the following line to the ~/.bash_profile file
export PATH="$PATH:$HOME/opt/usr/local/bin/"

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