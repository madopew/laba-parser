#include <stdio.h>
#include <stdlib.h>

#define SIZE 6
/*21. Дана матрица размером 6*6. Найти сумму наименьших
  элементов ее нечетных строк и наибольших элементов ее
  четных строк*/

void fillMatrix(int matrix[SIZE][SIZE]);
void doTask(int matrix[SIZE][SIZE]);
int inputNumber();

int main() {
    int matrix[SIZE][SIZE];
    fillMatrix(matrix);
    doTask(matrix);
    return 0;
}

void fillMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("Input element of matrix[%d][%d]: ", i + 1, j + 1);
            matrix[i][j] = inputNumber();
        }
    }
}

int inputNumber() {
    char buff[16];
    int isInvalid;
    int number;
    do {
        gets(buff);
        number = atoi(buff);
        isInvalid = (number == 0 && !(*buff == 48));
        if (isInvalid) {
            printf("Invalid input. Please, input a number >= %d and <= %d\nTry again: ", INT_MIN, INT_MAX);
        }
    } while (isInvalid);

    return number;
}

void doTask(int matrix[SIZE][SIZE]) {
    int sum = 0;
    int elem;
    for (int i = 0; i < SIZE; i++) {
        elem = matrix[i][0];
        if ((i + 1) % 2 == 0) {
            for (int j = 1; j < SIZE; j+=1) {
                if (elem < matrix[i][j]) {
                    elem = matrix[i][j];
                }
            }
        }
        else {
            for (int j = 1; j < SIZE; j++) {
                if (elem > matrix[i][j]) {
                    elem = matrix[i][j];
                }
            }
        }
        sum += elem;
    }
    printf("Sum = %d ", sum);
}

