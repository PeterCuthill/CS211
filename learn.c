#include <stdio.h>
#include <stdlib.h>

double **multiply(int m1Row, int m1Col, int m2Row, int m2Col, double **matrix1, double **matrix2);

void printMatrix(int row, int col, double **matrix);

double **transpose(int row, int col, double **matrix);

double **inverse(int row, int col, double **matrix, double **identity);

int main(int argc, char **argv){

	int i,j,row,col,row2,col2;
	double **matrix;
	double **matrixY;
	double **matrix2;
	double **transposeX;
	double **XTX;
	double **XTY;
	double **psuedo;
	double **final;
	double **result;
	double **identity;
	char *word;
	
	word = malloc(100);

	FILE *file = fopen(argv[1], "r");
	fscanf(file, "%s", word);
	FILE *file2 = fopen(argv[2], "r");
	fscanf(file2, "%s", word);
	
	free(word);
	
	fscanf(file, "%d", &col);
	fscanf(file, "%d", &row);
	fscanf(file2, "%d", &col2);
	fscanf(file2, "%d", &row2);


	matrix = malloc(row*sizeof(double*));
	matrixY = malloc(row*sizeof(double*));
	matrix2 = malloc(row2*sizeof(double*));
	identity = malloc((col + 1)*sizeof(double*));
	
	
	
	for(i = 0; i < row; i++){ //malloc training data
		matrix[i] = malloc((col + 1)*sizeof(double));
		matrixY[i] = malloc(sizeof(double));
	}
	
	for(i = 0; i < row2; i++){ //malloc data
		matrix2[i] = malloc((col2 + 1)*sizeof(double));
	}
	
	for(i = 0; i < row; i++){ //read input file and create matrix
		matrix[i][0] = 1;
		for(j = 1; j < col + 1; j++){
			fscanf(file, "%lf", &matrix[i][j]);
		}
		fscanf(file, "%lf", &matrixY[i][0]);
	}
	
	for(i = 0; i < row2; i++){ //read input file2 and create matrix
		matrix2[i][0] = 1;
		for(j = 1; j < col2 + 1; j++){
			fscanf(file2, "%lf", &matrix2[i][j]);
		}
	}
	
	for(i = 0; i < col + 1; i++){ //malloc identity matrix
		identity[i] = malloc((col + 1)*sizeof(double));
	}
	
	for(i = 0; i < col +1; i++){ //create identity matrix
		for(j = 0; j < col + 1; j++){
			if(i == j){
				identity[i][j] = 1;
			}else{
				identity[i][j] = 0;
			}
		}
	}
	
	col = col + 1;
	transposeX = transpose(row, col, matrix);
	XTX = multiply(col, row, row, col, transposeX, matrix);
	psuedo = inverse(col, col, XTX, identity);
	XTY = multiply(col, row, row, 1, transposeX, matrixY);
	final = multiply(col, col, col, 1, psuedo, XTY);
	result = multiply(row2, col2 + 1, col, 1, matrix2, final);
	printMatrix(row2, 1, result);
	
	for(i = 0; i < row; i++){
		free(matrix[i]);
		free(matrixY[i]);
	}
	
	free(matrix);
	free(matrixY);
	
	for(i = 0; i < col; i++){
		free(transposeX[i]);
		free(XTX[i]);
		free(XTY[i]);
		free(final[i]);
		free(psuedo[i]);
	}
	
	free(transposeX);
	free(XTX);
	free(XTY);
	free(final);
	free(psuedo);
	
	for(i = 0; i < row2; i++){
		free(result[i]);
	}
	
	free(result);
	
	return 0;
}


double **multiply(int m1Row, int m1Col, int m2Row, int m2Col, double **matrix1, double **matrix2){ // multiplies matrices

	int i,j,k;
	double **product;
	product = malloc(m1Row*(sizeof(double*)));
	
	for(i = 0; i < m1Row; i++){
		product[i] = malloc(m2Col*(sizeof(double)));
	}

	if(m1Col != m2Row){ //checks if matrices can't be multiplied
		printf("error");
		return 0;
	}

	for(i = 0; i < m1Row; i++){
		for(j = 0; j < m2Col ; j++){
			for(k = 0; k < m2Row; k++){
				product[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}

	return product;
}

void printMatrix(int row, int col, double **matrix){ //prints input matrix
	
	int i,j;

	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			printf("%.0f", matrix[i][j]);
		}
		printf("\n");
	}
}

double **transpose(int row, int col, double **matrix){ //transposes matrix
	
	int i,j;
	double **Tmatrix;

	Tmatrix = malloc(col*(sizeof(double*)));

	for(i = 0; i < col; i++){
		Tmatrix[i] = malloc(row*(sizeof(double)));
	}

	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			Tmatrix[j][i] = matrix[i][j]; //swaps row and column
		}
	}

	return Tmatrix;
}

double **inverse(int row, int col, double **matrix, double **identity){ //inverts matrix
	
	int i,j,k,m,n;
	double x, coef;

	for(i = 0; i < row; i++){ //converts to upper triangular matrix
		for(j = 0; j < col; j++){
			if(i == j){
				x = matrix[i][j];
				
				for(m = 0; m < col; m ++){
					matrix[i][m] = matrix[i][m]/x;
					identity[i][m] = identity[i][m]/x;
				}
				
				for(k = i + 1; k < row; k++){
					coef = matrix[k][i]/matrix[i][i];
					for(n = 0; n < col; n++){
						matrix[k][n] = matrix[k][n] - (matrix[i][n] * coef);
						identity[k][n] = identity[k][n] - (identity[i][n] * coef);
					}
				}
			}
		}
	}
	
	for(i = row - 1; i >= 0; i--){ //converts upper triangular matrix to identity matrix
		for(j = col - 1; j >= 0; j--){
			if(i == j){
				x = matrix[i][j];
	
			for(k = i - 1; k >= 0; k--){
				coef = matrix[k][i]/matrix[i][i];
					for(n = 0; n < col; n++){
						matrix[k][n] = matrix[k][n] - (matrix[i][n] * coef);
						identity[k][n] = identity[k][n] - (identity[i][n] * coef);
					}
				}
			}
		}
	}
	return identity;
}









