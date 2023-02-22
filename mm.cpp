#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <omp.h>
#include <conio.h>
using namespace std;

// g++ -fopenmp -o <output-file-name> <source-file-name>

const int numMatrices = 10;
double **matrices[numMatrices];
int Rows[numMatrices], Cols[numMatrices];
string arrFileNames[] = {"Mat1.txt", "Mat2.txt", "Mat3.txt", "Mat4.txt", "Mat5.txt", "Mat6.txt", "Mat7.txt", "Mat8.txt", "Mat9.txt", "Mat10.txt"};
int numThreads;
void readMatrices();
void matrixPrinterWriter(double **matrix, int rows, int cols);
void multiplyMatrices(int mat1, int mat2);
void MatrixPrinter();
string fileNameGenerator();

int main()
{

    readMatrices(); // Read in all 10 matrices
    MatrixPrinter();
    int mat1, mat2;
    cout << "Enter the indices of the two matrices you want to multiply (0-9): ";
    cin >> mat1 >> mat2;

    cout << "Enter the number of threads you want to set ";
    cin >> numThreads;
    // Check if the matrices can be multiplied (i.e. number of columns in mat1 is equal to number of rows in mat2)
    if (Cols[mat1] != Rows[mat2])
    {
        if (Cols[mat2] != Rows[mat1])
        {
            cout << "Error: Matrices are not multiplicable in the order of A*B or either B*A" << endl;
            return 1;
        }
        else
        {
            multiplyMatrices(mat2, mat1);
        }
    }
    else
    {
        multiplyMatrices(mat1, mat2);
    }

    return 0;
}

void readMatrices()
{
    int Count = 0;
    int rows = 0;
    int cols = 0;

    while (Count < numMatrices)
    {
        ifstream file(arrFileNames[Count]);
        ifstream file1(arrFileNames[Count]);
        string line;
        string value;

        while (getline(file, line))
        {
            rows++;
        }

        cols = count(line.begin(), line.end(), ',') + 1;

        Rows[Count] = rows;
        Cols[Count] = cols;
        matrices[Count] = new double *[rows];
        for (int i = 0; i < rows; i++)
            matrices[Count][i] = new double[cols];
        int row = 0;
        while (getline(file1, line))
        {
            int col = 0;
            stringstream ss(line);
            while (getline(ss, value, ','))
            {
                matrices[Count][row][col] = stod(value);
                col++;
            }
            row++;
        }
        file.close();
        file1.close();
        Count++;
        rows = 0;
        cols = 0;
    }
}

void matrixPrinterWriter(double **matrix, int rows, int cols)
{
    ofstream file(fileNameGenerator());
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << matrix[i][j];
            file << matrix[i][j];
            if (j != cols - 1)
            {
                cout << ",";
                file << ",";
            }
        }
        cout << endl;
        file << endl;
    }
    file.close();
}

void multiplyMatrices(int mat1, int mat2)
{
    // Create a new matrix to store the result
    double **result = new double *[Rows[mat1]];
    for (int i = 0; i < Rows[mat1]; i++)
    {
        result[i] = new double[Cols[mat2]];
    }
    omp_set_num_threads(numThreads);
// Perform matrix multiplication
#pragma omp parallel for schedule(static)
    for (int i = 0; i < Rows[mat1]; i++)
    {
#pragma omp parallel for schedule(static)
        for (int j = 0; j < Cols[mat2]; j++)
        {
            result[i][j] = 0;
#pragma omp parallel for schedule(static)
            for (int k = 0; k < Cols[mat1]; k++)
            {
                result[i][j] += matrices[mat1][i][k] * matrices[mat2][k][j];
            }
        }
    }

    cout << "The result of multiplying matrices " << mat1 << " and " << mat2 << " is: " << endl;
    matrixPrinterWriter(result, Rows[mat1], Cols[mat2]);

    system("Pause");
}

void MatrixPrinter()
{
    int count = 0;
    while (count < numMatrices)
    {
        for (int i = 0; i < Rows[count]; i++)
        {
            for (int j = 0; j < Cols[count]; j++)
            {
                cout << matrices[count][i][j];
                if (j != Cols[count] - 1)
                {
                    cout << ",";
                }
            }
            cout << endl;
        }
        cout << "-------------------------------- Matrix " << count << " -------------------------------------" << endl;
        count++;
    }
}

string fileNameGenerator()
{
    int count = 0;
    string result = "Result ";
    string ext = ".txt";
    stringstream ss;
    ss << result << count << ext;
    result = ss.str();
    return result;
}