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
	double* T;		// Tableau 3D
	double* tmp;	// buffer 
	double* moy;	// Moyenne 
	double Tin;		// Température dans le rectangle
	double Tout;	// Température sur les bord
	double* coef;
	
}solveur_t;

#define geti(S, i, j, k) ((i)*(S)->M*(S)->N+(j)*(S)->N+(k))

void creer_solveur(solveur_t* S , int Duree , int M_ligne, int N_colonne,double Tin , double Tout);
/*------------------------Affichage----------------------------*/
void afficher_info_solveur(solveur_t* S);
void afficher_solveur(solveur_t* S);
void afficher_solveur_indice(solveur_t* S, int i);
void afficher_tmp(solveur_t* S);
/*------------------------Calcul-------------------------------*/
void solve_solveur(solveur_t* S);
//extern int geti(solveur_t* S, int i,int j, int k );
void calculer_moyenne_solveur(solveur_t* S);

void detruire_solveur(solveur_t* S);

#endif
