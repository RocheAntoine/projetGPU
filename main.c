#include "solveur.h"


int main(void){
	
	solveur_t S;
	creer_solveur(&S,300,500,500,0,2000);

	CvMat* mat = cvCreateMatHeader(S.M, S.N, CV_8UC3);
	cvCreateData(mat);

	afficher_info_solveur(&S);
	//afficher_solveur(&S);
	//afficher_solveur_indice(&S,0);
	//afficher_tmp(&S);
	solve_solveur(&S, mat);
	//afficher_solveur_indice(&S,0);
	//afficher_solveur(&S);
	detruire_solveur(&S);
	printf("Fin");
	
	return 1;
}
