#include "solveur.h"
#include "imageWriter.h"
/*!
typedef struct solveur_t{
	int M;			// Nombre de ligne
	int N;			// Nombre de colonne
	int D; 			// Nombre d'itération
	float* T;		// Tableau 3D 
	float* moy;	// Moyenne 
	float Tin;		// Température dans le rectangle
	float Tout;	// Température sur les bord
};
*/



void creer_solveur(solveur_t* S , int Duree , int M_ligne, int N_colonne,float Tin , float Tout){
	int i,j,k;
	float tmpCoef[9] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
	S->D = Duree;
	S->M = M_ligne;
	S->N = N_colonne;
	S->Tin = Tin;
	S->Tout = Tout;
	S->T = NULL;
	S->moy = NULL;
	S->moy = (float*)malloc(sizeof(float) * S->D);
	S->T = (float*)malloc(sizeof(float) * 2 * S->M * S->N);
	S->coef = (float*)malloc(sizeof(float) * 9);
	for(i = 0 ; i < 9; i++)
		S->coef[i] = tmpCoef[i];

	// Initialisation température du rectangle Tin
	// Pour T[0]

#pragma omp parallel for collapse(2) schedule(dynamic) private(k,j) shared(S)
	for( j = 0 ; j < S->M ; j++){//ligne 
		for( k = 0 ; k < S->N ; k++){//cols
			S->T[geti(S,0,j,k)] = S->Tin;
		}
	}

	//Initialisation température du rectangle Tout (au bord)
	// Pour tmp

#pragma omp parallel shared(S) private(j)
	{
#pragma omp for schedule(dynamic)
		for (j = 0; j < S->M; j++) {
			S->T[geti(S, 0, j, 0)] = S->Tout;
			S->T[geti(S, 0, j, S->N - 1)] = S->Tout;
		}
#pragma omp for schedule(dynamic)
		for (j = 0; j < S->N; j++) {
			S->T[geti(S, 0, 0, j)] = S->Tout;
			S->T[geti(S, 0, S->M - 1, j)] = S->Tout;
		}
		// Pour chaque grille de chaque durée
	}
}
void afficher_info_solveur(solveur_t* S){
	printf("Duree : %d\nLignes : %d\nColonnes : %d\nTemperature au bord : %f\nTemperature initiale : %f\n",S->D,S->M,S->N,S->Tout,S->Tin);
}

void afficher_solveur(solveur_t* S){
	int i;
	for( i = 0 ; i < 2 ; i++){
		printf("t = %d\n",i);
		afficher_solveur_indice(S,i);
	}
}

void afficher_solveur_indice(solveur_t* S, int temps){
	int i,j;
	float total = 0;
	for(i = 0 ; i < S->M ; i++){
		for(j = 0 ; j < S->N ; j++){
			total += S->T[geti(S,temps % 2,i,j)];
			printf("%f ",S->T[geti(S,temps % 2,i,j)]);
		}
		printf("\n");
	}
	//printf("Temperature totale : %lf\n", total);
}

/*!
*/
void solve_solveur(solveur_t* S, CvMat* mat){
	int i,j,k,l,m, frequence;
	float* tmp;
	char chemin[30];

	frequence = S->D > 100 ? S->D / 100 : S->D;
	//S->T[geti(S,0,S->M / 2 , S->M / 2 )] = S->Tout;

	print_img(S->T + geti(S, k % 2, 0,0), mat, S->M, S->N);

	#ifdef OMPON
#pragma omp parallel private(i, j, k,l, m, tmp) shared(S)
	{
#endif
	for(k = 1 ; k <= S->D ; k++){
		/*Diffusion selon i*/

		#ifdef OMPON
		#pragma omp for collapse(2) schedule(dynamic)
		#endif
		for(i = 0 ; i < S->M ; i++){
			for(j = 0 ; j < S->N ; j++){
				//S->tmp[geti(S,0,i2,j2)] = (S->T[geti(S,k-1,i2-1,j2)]+ 4 * S->T[geti(S,k-1,i2,j2)]+S->T[geti(S,k-1,i2+1,j2)]) / 6.0f;
				tmp = &(S->T[geti(S, k % 2, i, j)]);
				if(*tmp != S->Tout)
				{
					*tmp = 0;
					//printf("[%d, %d = %d]", i, j, ind1);
					for (l = 0; l < 3; l++)
					{
						for (m = 0; m < 3; m++)
						{
							if ((i + l - 1) >= 0 && (i + l - 1) < S->M && (j + m - 1) >= 0 && (j + m - 1) < S->N)
							{
								*tmp += S->T[geti(S, (k + 1) % 2, i + l - 1, j + m - 1)] * S->coef[m * 3 + l];
							} else
								*tmp += S->T[geti(S, (k + 1) % 2, i, j)] * S->coef[m * 3 + l];
						}
					}
				}
				//printf("Ecriture dans image : %d, %d\n", i, j);

				//printf("Image ecrite\n");
				//printf("%lf ", S->T[ind1]);
			}
		}
if(k%frequence == 0)
		{
#pragma omp single
			{
				print_img(S->T + geti(S, k % 2, 0,0), mat, S->M, S->N);
			}
#pragma omp barrier
		}
	}
	#ifdef OMPON
	}
	#endif
}

//void calculer_moyenne_solveur(solveur_t* S);

void detruire_solveur(solveur_t* S){
	if(S->moy != NULL)
		free(S->moy);
	if(S->T != NULL)
		free(S->T);
}


/*
//capacité max
D_max 50
M_max 10000
N_max 10000
//taille utilisé
D 
M 
N
float T[2][3][4]
0000...............................................................................
0000...............................................................................
0000...............................................................................
T[]
// S{D,M,N}
// D = 2 {0,1}
// M = 3 {0,1,2}
// N = 4 {0,1,2,3}
// (1,2,3) -> (1*3*4 = 12 + 2 * 4 = 20 + 3 = 23 )
*/
