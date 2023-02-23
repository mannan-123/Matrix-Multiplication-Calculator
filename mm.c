#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>
#define numMatrices 10
// const int numMatrices = 10;

double **matrices[numMatrices];
int Rows[numMatrices], Cols[numMatrices];
char arrFileNames[numMatrices][20] = {"Mat1.txt", "Mat2.txt", "Mat3.txt", "Mat4.txt", "Mat5.txt", "Mat6.txt", "Mat7.txt", "Mat8.txt", "Mat9.txt", "Mat10.txt"};
char result[20];
int numThreads;
void readMatrices();
void matrixPrinterWriter(double **matrix, int rows, int cols);
void multiplyMatrices(int mat1, int mat2);
void MatrixPrinter();
char *fileNameGenerator();
int countCommas(char *str);
int main()
{
    readMatrices(); // Read in all 10 matrices
    MatrixPrinter();
    int mat1, mat2;
    printf("Enter the indices of the two matrices you want to multiply (0-9): ");
    scanf("%d %d", &mat1, &mat2);

    printf("Enter the number of threads you want to set: ");
    scanf("%d", &numThreads);
    // Check if the matrices can be multiplied (i.e. number of columns in mat1 is equal to number of rows in mat2)
    if (Cols[mat1] != Rows[mat2])
    {
        if (Cols[mat2] != Rows[mat1])
        {
            printf("Error: Matrices are not multiplicable in the order of A*B or either B*A\n");
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
    int count = 0;
    int rows = 0;
    int cols = 0;
    // char *arrFileNames[] = {"Mat1.txt", "Mat2.txt", "Mat3.txt", "Mat4.txt", "Mat5.txt", "Mat6.txt", "Mat7.txt", "Mat8.txt", "Mat9.txt", "Mat10.txt"};

    while (count < numMatrices)
    {
        FILE *file = fopen(arrFileNames[count], "r");
        char line[256];

        while (fgets(line, sizeof(line), file))
        {
            rows++;
        }
        rewind(file);

        cols = countCommas(line) + 1;

        Rows[count] = rows;
        Cols[count] = cols;
        matrices[count] = malloc(sizeof(double *) * rows);
        for (int i = 0; i < rows; i++)
            matrices[count][i] = malloc(sizeof(double) * cols);

        int row = 0;
        while (fgets(line, sizeof(line), file))
        {
            int col = 0;
            char *token = strtok(line, ",");
            while (token != NULL)
            {
                matrices[count][row][col] = atof(token);
                token = strtok(NULL, ",");
                col++;
            }
            row++;
        }

        fclose(file);
        count++;
        rows = 0;
        cols = 0;
    }
}
void matrixPrinterWriter(double **matrix, int rows, int cols)
{
    char *filename = fileNameGenerator();
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%f", matrix[i][j]);
            fprintf(file, "%f", matrix[i][j]);
            if (j != cols - 1)
            {
                printf(",");
                fprintf(file, ",");
            }
        }
        printf("\n");
        fprintf(file, "\n");
    }
    fclose(file);
}

void multiplyMatrices(int mat1, int mat2)
{
    // Create a new matrix to store the result
    double **result = (double **)malloc(Rows[mat1] * sizeof(double *));
    for (int i = 0; i < Rows[mat1]; i++)
    {
        result[i] = (double *)malloc(Cols[mat2] * sizeof(double));
    }
    // Perform matrix multiplication
    int i, j, k;
    omp_set_num_threads(numThreads);
#pragma omp parallel for private(i, j, k) shared(matrices, mat1, mat2, result)
    for (i = 0; i < Rows[mat1]; i++)
    {
        for (j = 0; j < Cols[mat2]; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < Cols[mat1]; k++)
            {
                result[i][j] += matrices[mat1][i][k] * matrices[mat2][k][j];
            }
        }
    }

    printf("The result of multiplying matrices %d and %d is: \n", mat1, mat2);
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
                printf("%.6lf", matrices[count][i][j]);
                if (j != Cols[count] - 1)
                {
                    printf(",");
                }
            }
            printf("\n");
        }
        printf("-------------------------------- Matrix %d -------------------------------------\n", count);
        count++;
    }
}

char *fileNameGenerator()
{
    int count = 0;

    snprintf(result, sizeof(result), "Result %d.txt", count);
    return result;
}

int countCommas(char *str)
{
    int count = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == ',')
        {
            count++;
        }
    }
    return count;
}
