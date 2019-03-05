#ifndef SOLVEUR_H
#define SOLVEUR_H


#define OMPON

#include <stdio.h>
#include <stdlib.h>
#include "imageWriter.h"

typedef struct solveur_t{
	int M;			// Nombre de ligne
	int N;			// Nombre de colonne
	int D; 			// Nombre d'itération
	float* T;		// Tableau 3D
	float* tmp;	// buffer 
	float* moy;	// Moyenne 
	float Tin;		// Température dans le rectangle
	float Tout;	// Température sur les bord
	float* coef;
	
}solveur_t;

#define geti(S, i, j, k) ((i)*(S)->M*(S)->N+(j)*(S)->N+(k))

void creer_solveur(solveur_t* S , int Duree , int M_ligne, int N_colonne,float Tin , float Tout);
/*------------------------Affichage----------------------------*/
void afficher_info_solveur(solveur_t* S);
void afficher_solveur(solveur_t* S);
void afficher_solveur_indice(solveur_t* S, int i);
void afficher_tmp(solveur_t* S);
/*------------------------Calcul-------------------------------*/
void solve_solveur(solveur_t* S, CvMat*);
//extern int geti(solveur_t* S, int i,int j, int k );
void calculer_moyenne_solveur(solveur_t* S);

void detruire_solveur(solveur_t* S);

#endif
