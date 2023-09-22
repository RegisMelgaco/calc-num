#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.000001



/*
* make_matrix aloca uma matriz com l linhas e c colunas em m.
* caso haja falha na alocação da matriz, será desalocado todo o espaço de memória alocado e retornado 1.
*/
double** make_matrix(int l, int c) {
	int i, j;
    double **m;
	
	m = malloc(sizeof(double*) * l);
  	if (m == NULL) {
    	return NULL;
  	}
  	
  	for (i = 0; i < l; i++) {
  		m[i] = (double*)malloc(sizeof(double) * c);
	  	if (m[i] == NULL) {
			for (j = 0; j <= i; j++) {
		  		free(m[j]);
			}

			free(m);

			return NULL;
	  	}
	}
	
	return m;
}

/*
* read_matrix 
*/
void read_matrix(double** m, int l, int c) {
	int code;
	int i, j;
	
	for (i = 0; i < l; i++) {
		for (j = 0; j < c; j++) {
			printf("m[%d][%d] ", i+1, j+1);
			
			scanf("%lf", &m[i][j]);
		}
	}
}

void free_matrix(double** m, int l) {
	int i;
	
	for (i = 0; i < l; i++) {
  		free(m[i]);
	}
	
	free(m);
}

/*
* s_retro retorna um int que prese
* m é uma matriz aumentada de um SL com n variáveis.
* se compatível, a ou uma resposta será gravada em x.
* se determinado será retornado 0.
* se indeterminado será retornado 1.
* se incompatível será retornado 2.
*/
int s_retro(double **m, int n, double *x){
	int i, j, kind = 0;
	double sum = 0;
	
	for (i = n-1; i >= 0; i--) {
		sum = 0;
		for (j = i+1; j < n; j++) {
			sum += m[i][j]*x[j];
		}
		
		if (m[i][i] == 0) {
			if (fabs(m[i][n] - sum) < EPSILON) {
				kind = 1; // variável lívre
				x[i] = 0;
			} else {
				return 2;
			}
		} else {
			x[i] = (m[i][n] - sum) / m[i][i];
		}
	}
	
	return kind;
}

void print_matrix(double **m, int l, int c) {
	int i, j;
	for (i = 0; i < l; i++) {
	  	for (j = 0; j < c; j++) {
			printf("m[%d][%d] %10.3lf; ", i+1, j+1, m[i][j]);
		}
		
		printf(";\n");
	}
}

/*
* recebe m: matriz aumentada de um sistema linear de n variáveis.
* transforma m na matriz aumentada de um sistema linear triangualr superior equivalente.
*/
void Gauss(double **m, int n) {
	int i, j, k;
	double mult;
	double *aux;
	
	for (i = 0; i < n-1; i++) {
		if (m[i][i] == 0) {
			j = i+1;
			while (j < n && m[j][i] == 0) {
				j++;
			}
			
			if (j < n) {
				aux = m[i];
				m[i] = m[j];
				m[j] = aux;
			}
		}
		
		if (m[i][i] != 0) {
			for (j = i+1; j < n; j++){
				mult = -m[j][i]/m[i][i];
				m[j][i] = 0;
				
				for (k = i+1; k <= n; k++) {
					m[j][k] += mult * m[i][k];
				}
			}
		}
	}
}

int main() {
	int i, j, n, code;
	double** m;
	double* x;
	
	printf("quantidade de variáveis: ");
	scanf("%d", &n);
	
	x = malloc(sizeof(double)*n);
	if (x == NULL) {
		printf("erro: falta de memória\n");
		
		return 1;
	}
	
	m = make_matrix(n, n+1);
	if (code != 0) {
		return code;
	}
	
	read_matrix(m, n, n+1);
	print_matrix(m, n, n+1);
	
	Gauss(m, n);
	printf("SL triangularizado\n");
	print_matrix(m, n, n+1);
	
	code = s_retro(m, n, x);
	
	if (code == 0) {
		printf("sistema determinado\n");
	} else if (code == 1) {
		printf("sistema indeterminado\n");
	} else if (code == 2){
		printf("sistema incompativel\n");
	} else {
		printf("codigo desconhecido\n");
	}
	  
	if (code != 2) {
		for (i = 0; i < n; i++){
			printf("a[%d] = %lf\n", i+1, x[i]);
		}
	}
	
  	free_matrix(m, n);

  	return 0;
}
