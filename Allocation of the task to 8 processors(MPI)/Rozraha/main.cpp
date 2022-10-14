#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include "mpi.h"
#include <Windows.h>
#include <conio.h>

using namespace std;

const int circle[] = { 0,2,6,7,5,4,1,3 };
const int procNumber = 8;
const int N1 = 200;
const int N2 = 264;
const int N3 = 150;
int procRank;

struct A {
	int row;
	int n1;
	int n2;
	double arr[N1 / 8 + N1][N2];
};

struct B {
	int col;
	int n2;
	int n3;
	double arr[N2][N3 / 8 + 1];
};

struct R {
	int n1;
	int n3;
	double arr[N1 / 8 + N1][N3];
};

double matrixA[N1][N2] = { 0 };
double matrixB[N2][N3] = { 0 };
A subA[procNumber];
B subB[procNumber];
R result[procNumber];
int procSize;

//пересилка підматриці і перемноження
void Mull(A sub_A, B sub_B, int proc) {
	for (int i = 0; i < sub_A.n1; i++)
		for (int k = 0; k < sub_A.n2; k++)
			for (int j = 0; j < sub_B.n3; j++)
				result[proc].arr[i][j + sub_B.col] += sub_A.arr[i][k] * sub_B.arr[k][j];
	result[proc].n1 = sub_A.n1;
	result[proc].n3 = N3;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
	MPI_Status Status;
	if (procSize != 8) {
		cout << " It has to be 8 process" << endl;
		MPI_Finalize();
		return 1;
	}

	//перемноження матриць
	//заповнення А
	for (int i = 0; i < N1; i++) {
		for (int j = 0; j < N2; j++) {
			matrixA[i][j] = 1;
		}
	}

	//заповнення В
	for (int i = 0; i < N2; i++) {
		for (int j = 0; j < N3; j++) {
			matrixB[i][j] = 1;
		}
	}

	//розбиття на підматриці
	//А
	int rowsRest = N1;
	int rowsSum = 0;
	int restProc = procNumber;
	for (int i = 0; i < procNumber; i++) {
		if ((rowsRest % (restProc)) == 0) {
			subA[i].row = rowsSum;
			subA[i].n1 = rowsRest / restProc;
			subA[i].n2 = N2;
			rowsRest -= rowsRest / restProc;
		}
		else {
			subA[i].row = rowsSum;
			subA[i].n1 = rowsRest / restProc;
			subA[i].n2 = N2;
			rowsRest -= subA[i].n1;
		}

		for (int k = 0; k < subA[i].n1; k++) {
			for (int j = 0; j < subA[i].n2; j++) {
				subA[i].arr[k][j] = matrixA[k + rowsSum][j];
			}
		}
		rowsSum += subA[i].n1;
		restProc--;
	}

	//В
	rowsSum = 0;
	restProc = procNumber;
	rowsRest = N3;
	for (int i = 0; i < procNumber; i++) {
		if ((rowsRest % (restProc)) == 0) {
			subB[i].col = rowsSum;
			subB[i].n3 = rowsRest / restProc;
			subB[i].n2 = N2;
			rowsRest -= rowsRest / restProc;
		}
		else {
			subB[i].col = rowsSum;
			subB[i].n3 = rowsRest / restProc;
			subB[i].n2 = N2;
			rowsRest -= rowsRest / restProc;
		}

		for (int k = 0; k < subB[i].n2; k++) {
			for (int j = 0; j < subB[i].n3; j++) {
				subB[i].arr[k][j] = matrixB[k][j + rowsSum];
			}
		}
		rowsSum += subB[i].n3;
		restProc--;
	}

if (procRank == 0) {
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S0->(B1)->S1" << endl;
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B2)->S2" << endl;
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S0->(B3)->S3" << endl;
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S0->(B4)->S4" << endl;
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S0->(B5)->S5" << endl;
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S0->(B6)->S6" << endl;
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S0->(B7)->S7" << endl;

	//A0*B0
	Mull(subA[0], subB[0], procRank);
	MPI_Send(&subB[0], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B0)->S2" << endl;

	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	//A0*B3
	Mull(subA[0], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B3)->S2" << endl;

	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	//A0*B1
	Mull(subA[0], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B1)->S2" << endl;

	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	//A0*B4
	Mull(subA[0], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B4)->S2" << endl;

	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	//A0*5
	Mull(subA[0], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B5)->S2" << endl;

	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	//A0*B7
	Mull(subA[0], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B7)->S2" << endl;

	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	//A0*B6
	Mull(subA[0], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B6)->S2" << endl;

	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	//A0*B2
	Mull(subA[0], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD);
	cout << "S0->(B2)->S2" << endl;

	MPI_Recv(&result[1], sizeof(R), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	MPI_Recv(&result[2], sizeof(R), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	MPI_Recv(&result[3], sizeof(R), MPI_BYTE, 3, 0, MPI_COMM_WORLD, &Status);
	MPI_Recv(&result[4], sizeof(R), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	MPI_Recv(&result[5], sizeof(R), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	MPI_Recv(&result[6], sizeof(R), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	MPI_Recv(&result[7], sizeof(R), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);

	cout << endl << endl << "RESULT:" << endl;
	for (int k = 0; k < 8; k++) {
		cout << "Line " << k+1 << endl;
		for (int i = 0; i < result[k].n1; i++) {
			for (int j = 0; j < result[k].n3; j++) {
				if (j == 8 || j == 17 || j  == 26 || j == 35 || j == 44 || j == 53 || j == 62) {
					cout << result[k].arr[i][j] << " ";
					cout <<endl;
				}
				else {
					cout << result[k].arr[i][j] << " ";
				}
			}
			cout << endl << endl;
		}
	}
}

if (procRank == 1) {
	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A1*B1
	Mull(subA[1], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B1)->S3" << endl;

	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	//A1*B4
	Mull(subA[1], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B4)->S3" << endl;

	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	//A1*B5
	Mull(subA[1], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B5)->S3" << endl;

	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	//A1*B7
	Mull(subA[1], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B7)->S3" << endl;

	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	//A1*B6
	Mull(subA[1], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B6)->S3" << endl;

	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	//A1*B2
	Mull(subA[1], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B2)->S3" << endl;

	MPI_Recv(&subB[0], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	//A1*B0
	Mull(subA[1], subB[0], procRank);
	MPI_Send(&subB[0], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B0)->S3" << endl;

	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD, &Status);
	//A1*B3
	Mull(subA[1], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 3, 0, MPI_COMM_WORLD);
	cout << "S1->(B3)->S3" << endl;
	MPI_Send(&result[1], sizeof(R), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S1->(R1)->S0" << endl;
}

if (procRank == 2) {
	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B2
	Mull(subA[2], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B2)->S6" << endl;

	MPI_Recv(&subB[0], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B0
	Mull(subA[2], subB[0], procRank);
	MPI_Send(&subB[0], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B0)->S6" << endl;

	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B3
	Mull(subA[2], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B3)->S6" << endl;

	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B1
	Mull(subA[2], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B1)->S6" << endl;

	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B4
	Mull(subA[2], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B4)->S6" << endl;

	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B5
	Mull(subA[2], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B5)->S6" << endl;

	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B7
	Mull(subA[2], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B7)->S6" << endl;

	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A2*B6
	Mull(subA[2], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD);
	cout << "S2->(B6)->S6" << endl;
	MPI_Send(&result[2], sizeof(R), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S2->(R2)->S0" << endl;
}

if (procRank == 3) {
	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A3*B3
	Mull(subA[3], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(B3)->S0" << endl;

	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	//A3*B1
	Mull(subA[3], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(B1)->S0" << endl;

	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	//A3*B4
	Mull(subA[3], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(B4)->S0" << endl;

	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	//A3*B5
	Mull(subA[3], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(B5)->S0" << endl;

	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	//A3*B7
	Mull(subA[3], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(B7)->S0" << endl;

	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	//A3*B6
	Mull(subA[3], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(B6)->S0" << endl;

	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	//A3*B2
	Mull(subA[3], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(B2)->S0" << endl;

	MPI_Recv(&subB[0], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD, &Status);
	//A3*B0
	Mull(subA[3], subB[0], procRank);

	MPI_Send(&result[3], sizeof(R), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S3->(R3)->S0" << endl;
}

if (procRank == 4) {
	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A4*B4
	Mull(subA[4], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B4)->S1" << endl;

	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	//A4*B5
	Mull(subA[4], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B5)->S1" << endl;

	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	//A4*B7
	Mull(subA[4], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B7)->S1" << endl;

	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	//A4*B6
	Mull(subA[4], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B6)->S1" << endl;

	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	//A4*B2
	Mull(subA[4], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B2)->S1" << endl;

	MPI_Recv(&subB[0], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	//A4*B0
	Mull(subA[4], subB[0], procRank);
	MPI_Send(&subB[0], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B0)->S1" << endl;

	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	//A4*B3
	Mull(subA[4], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B3)->S1" << endl;

	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD, &Status);
	//A4*B1
	Mull(subA[4], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
	cout << "S4->(B1)->S1" << endl;
	MPI_Send(&result[4], sizeof(R), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S4->(R4)->S0" << endl;
}

if (procRank == 5) {
	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A5*B5
	Mull(subA[5], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B5)->S4" << endl;

	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);
	//A5*B7
	Mull(subA[5], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B7)->S4" << endl;

	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);
	//A5*B6
	Mull(subA[5], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B6)->S4" << endl;

	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);
	//A5*B2
	Mull(subA[5], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B2)->S4" << endl;

	MPI_Recv(&subB[0], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);
	//A5*B0
	Mull(subA[5], subB[0], procRank);
	MPI_Send(&subB[0], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B0)->S4" << endl;

	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);
	//A5*B3
	Mull(subA[5], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B3)->S4" << endl;

	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);
	//A5*B1
	Mull(subA[5], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B1)->S4" << endl;

	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD, &Status);
	//A5*B4
	Mull(subA[5], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 4, 0, MPI_COMM_WORLD);
	cout << "S5->(B4)->S4" << endl;
	MPI_Send(&result[5], sizeof(R), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S5->(R5)->S0" << endl;
}

if (procRank == 6) {
	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A6*B6
	Mull(subA[6], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B6)->S7" << endl;

	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	//A6*B2
	Mull(subA[6], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B2)->S7" << endl;

	MPI_Recv(&subB[0], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	//A6*B0
	Mull(subA[6], subB[0], procRank);
	MPI_Send(&subB[0], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B0)->S7" << endl;

	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	//A6*B3
	Mull(subA[6], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B3)->S7" << endl;

	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	//A6*B1
	Mull(subA[6], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B1)->S7" << endl;

	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	//A6*B4
	Mull(subA[6], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B4)->S7" << endl;

	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	//A6*B5
	Mull(subA[6], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B5)->S7" << endl;

	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 2, 0, MPI_COMM_WORLD, &Status);
	//A6*B7
	Mull(subA[6], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 7, 0, MPI_COMM_WORLD);
	cout << "S6->(B7)->S7" << endl;
	MPI_Send(&result[6], sizeof(R), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S6->(R6)->S0" << endl;
}

if (procRank == 7) {
	MPI_Recv(&subB[7], sizeof(B), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Status);
	//A7*B7
	Mull(subA[7], subB[7], procRank);
	MPI_Send(&subB[7], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B7)->S5" << endl;

	MPI_Recv(&subB[6], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	//A7*B6
	Mull(subA[7], subB[6], procRank);
	MPI_Send(&subB[6], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B6)->S5" << endl;

	MPI_Recv(&subB[2], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	//A7*B2
	Mull(subA[7], subB[2], procRank);
	MPI_Send(&subB[2], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B2)->S5" << endl;

	MPI_Recv(&subB[0], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	//A7*B0
	Mull(subA[7], subB[0], procRank);
	MPI_Send(&subB[0], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B0)->S5" << endl;

	MPI_Recv(&subB[3], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	//A7*B3
	Mull(subA[7], subB[3], procRank);
	MPI_Send(&subB[3], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B3)->S5" << endl;

	MPI_Recv(&subB[1], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	//A7*B1
	Mull(subA[7], subB[1], procRank);
	MPI_Send(&subB[1], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B1)->S5" << endl;

	MPI_Recv(&subB[4], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	//A7*B4
	Mull(subA[7], subB[4], procRank);
	MPI_Send(&subB[4], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B4)->S5" << endl;

	MPI_Recv(&subB[5], sizeof(B), MPI_BYTE, 6, 0, MPI_COMM_WORLD, &Status);
	//A7*B5
	Mull(subA[7], subB[5], procRank);
	MPI_Send(&subB[5], sizeof(B), MPI_BYTE, 5, 0, MPI_COMM_WORLD);
	cout << "S7->(B5)->S5" << endl;
	MPI_Send(&result[7], sizeof(R), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
	cout << "S7->(R7)->S0" << endl;
}
	MPI_Finalize();
	return 0;
}