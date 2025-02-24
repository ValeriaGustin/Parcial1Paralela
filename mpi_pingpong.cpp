#include <mpi.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
using namespace std;

void printTable(const vector<double>& rttValues, int rank) {
        cout << "\n+------------------------------------------+\n";
        cout << "|  Nodo " << rank << " - RTT por iteración (μs)       |\n";
        cout << "+------------------------------------------+\n";
        cout << "|  Iteración  |   RTT (microsegundos)      |\n";
        cout << "+------------+---------------------------+\n";

        for (size_t i = 0; i < rttValues.size(); i++) {
                cout << "|  " << setw(8) << i << "  |   " << setw(10)
                        << fixed << setprecision(3) << rttValues[i] << " μs   |\n";
        }

        cout << "+------------+---------------------------+\n";
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    	random_device rd;
	double totalRTT = 0.0;
    	mt19937 gen(rd());
    	uniform_int_distribution<> dist(1, 100);

    	int number = dist(gen);
    	const int numIter = 10;
    	vector<double> rttValues;

    	for (int i = 0; i < numIter; i++) {
        	double startTime, endTime, rtt;

        	if (rank == 0) {
            		startTime = MPI_Wtime();

            		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            		MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            		endTime = MPI_Wtime();
            		rtt = (endTime - startTime) * 1e6;
        	}
        	else {
            		int received_data;

            		startTime = MPI_Wtime();

            		MPI_Recv(&received_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            		received_data += 1;
            		MPI_Send(&received_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            		endTime = MPI_Wtime();
            		rtt = (endTime - startTime) * 1e6;
        	}

        	rttValues.push_back(rtt);
    	}


    	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) {
        	double avgRTT = totalRTT / numIter;
        	printTable(rttValues, rank);
        	cout << "\nTiempo medio RTT: " << fixed << setprecision(3) << avgRTT << " μs\n";
    	}

	MPI_Finalize();
    	return 0;

}



