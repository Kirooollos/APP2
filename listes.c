#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false ;


cellule_t* nouvelleCellule (void) {
  cellule_t *nouvelle_cellule = malloc(sizeof(cellule_t));
  if (nouvelle_cellule != NULL) {
    nouvelle_cellule->command = 0 ;
    nouvelle_cellule->suivant = NULL ;
  }
    //printf("\n>>>>>>>>>>> A Faire : liste.c/nouvelleCellule() <<<<<<<<<<<<<<<<\n") ;
    return nouvelle_cellule ;
}


void suppression_tete(sequence_t *seq) {
    cellule_pile val;
    if (seq->tete != NULL) {
      cellule_t *cell = seq->tete;
      seq->tete = seq->tete->suivant;
      if ((cell->command >= '0') && (cell->command <= '9')) {
        val.choix = ENTIER;
        val.valeur = cell->command;
      } else {
        val.choix = BLOC;
        val.bloc->command = cell->command;
      }
      free(cell);
      //cell = NULL;
      //seq->n--;
    } else {
      printf("La sequence est vide\n");
    }
}


void detruireCellule (cellule_t* cel) {
    if (cel != NULL) {
      free(cel) ;
    }
    //printf("\n>>>>>>>>>>> A Faire : liste.c/detruireCellule() <<<<<<<<<<<<<<<<\n") ;
}


void ajout_en_queue (sequence_t *l, char n) {
    cellule_t* nouvelleCellule = (cellule_t*)malloc(sizeof(cellule_t)) ;
    nouvelleCellule->command = n ;
    nouvelleCellule->suivant = NULL ;

    if (l->tete == NULL) {
        l->tete = nouvelleCellule ;
    } else {
        cellule_t* courante = l->tete ;
        while (courante->suivant != NULL) {
            courante = courante->suivant ;
        }

        courante->suivant = nouvelleCellule ;
    }
}


void conversion (char *texte, sequence_t *seq) {
    int i = 0 ;
    while (texte[i] != '\0') {
        ajout_en_queue(seq, texte[i]) ;
        i++ ;
    }
    //printf("\n>>>>>>>>>>> A Faire : liste.c/conversion() <<<<<<<<<<<<<<<<\n") ;
}


void afficher (sequence_t *seq) {
    cellule_t *t = seq->tete ;
    cellule_t *courant = t ;
    if (t != NULL) {
        while (courant != NULL) {
            printf("%c", courant->command) ;
            courant = courant->suivant ;
        }
    }  
}


void empiler(Pile *p, cellule_pile valeur) {
  assert(p->n >= 0);
  p->tab[p->n] = valeur ;
  p->n++ ;
}

cellule_pile depiler(Pile *p) {
  assert(p->n > 0);
    cellule_pile i = p->tab[p->n - 1];
    p->n-- ;
    return i ;
}


void afficher_pile(Pile *p) {
  cellule_pile cell;
  cellule_t *pile_cellule;
  printf("[");

  for (int i = 0; i < p->n; i++) {
    cell = p->tab[i];
    if (cell.choix == ENTIER) {
      printf(" %d ", cell.valeur);
    } else if (cell.choix == BLOC) {
      pile_cellule = cell.bloc;
      printf(" {");
      while (pile_cellule != NULL) {
        printf("%c", pile_cellule->command);
        pile_cellule = pile_cellule->suivant;
      }
      printf("} ");
    }
  }
  printf ("]\n") ;
}

void addition_acte2(Pile *p) {
  cellule_pile arg1 = depiler(p);
  cellule_pile arg2 = depiler(p);
  cellule_pile s;
  s.valeur = arg1.valeur + arg2.valeur;
  s.choix = ENTIER;
  empiler(p, s);
}

void soustraction_acte2(Pile *p) {
  cellule_pile arg1 = depiler(p);
  cellule_pile arg2 = depiler(p);
  cellule_pile d;
  d.valeur = arg2.valeur - arg1.valeur;
  d.choix = ENTIER;
  empiler(p, d);
}

void multiplication_acte2(Pile *p) {
  cellule_pile arg1 = depiler(p);
  cellule_pile arg2 = depiler(p);
  cellule_pile produit;
  produit.valeur = arg1.valeur * arg2.valeur;
  produit.choix = ENTIER;
  empiler(p, produit);
}

void bloc(sequence_t *seq, Pile *p) { //pour creer un bloc
  cellule_t *tmp;
  cellule_pile element;
  int compteur_accolades = 1;

  if (seq->tete->command == '}') {
    element.bloc = NULL;
    element.choix = BLOC;
    empiler(p, element);
    tmp = NULL;
    return;
  }
  element.bloc = seq->tete;
  element.choix = BLOC;
  empiler(p, element);
  
  tmp = seq->tete;
  while (((tmp->suivant->command != '}') || (compteur_accolades != 1)) || (tmp->command == '{')) {
    if (tmp->command == '{') {
      compteur_accolades++ ;
    }
    if (tmp->command == '}') {
      compteur_accolades-- ;
    }
    tmp = tmp->suivant;
  }
  seq->tete = tmp->suivant;
  tmp->suivant = NULL;
  tmp = NULL;
}


void ajout_en_tete (sequence_t *seq, cellule_pile valeur) {
  cellule_t *tmp;
  if (valeur.choix == ENTIER) {
    tmp = nouvelleCellule();
    tmp->command = valeur.valeur - '0';
    tmp->suivant = seq->tete;
    seq->tete = tmp;
  } else if ((valeur.choix = BLOC) || (valeur.bloc != NULL)) {
    tmp = valeur.bloc;
    while (tmp->suivant != NULL) {
      tmp = tmp->suivant;
    }
    tmp->suivant = seq->tete;
    tmp = valeur.bloc;
    seq->tete = tmp;
  } else {
    tmp = seq->tete;
    seq->tete = seq->tete->suivant;
    detruireCellule(tmp);
  }
}


void detruireBloc (cellule_t *bloc) {
  cellule_t *tmp = bloc;
  while (tmp != NULL) {
    bloc = bloc->suivant;
    detruireCellule(tmp);
    tmp = bloc;
  }
}


void echangeBloc(Pile *p) {
  cellule_pile blocF = depiler(p);
  cellule_pile blocV = depiler(p);
  empiler(p, blocF);
  empiler(p, blocV);
}


cellule_t *sequencecopie(cellule_t *seqp) {
  cellule_t *tmp = seqp;
  cellule_t *cptete = nouvelleCellule();
  cptete->command = seqp->command;
  cptete->suivant = NULL;

  cellule_t *seqp_cp = cptete;
  cellule_t *seqp_tmp_cp = cptete;

  tmp = tmp->suivant;

  while (tmp != NULL) {
    seqp_cp = nouvelleCellule();
    seqp_cp->suivant = NULL;
    seqp_cp->command = tmp->command;
    seqp_tmp_cp->suivant = seqp_cp;
    seqp_tmp_cp = seqp_tmp_cp->suivant;
    tmp = tmp->suivant;
  }

  return cptete;
}


void clones(cellule_pile bloc, Pile *p) {
  cellule_pile clones = bloc;
  if (bloc.choix == ENTIER) {
    empiler(p, bloc);
    empiler(p, bloc);
  } else {
    clones.bloc = sequencecopie(bloc.bloc);
    clones.choix = BLOC;
    empiler(p, bloc);
    empiler(p, clones);
  }
}


void boucle(sequence_t *seq, Pile *p) {
  cellule_pile elem = depiler(p);
  int n = elem.valeur;
  if (n > 0) {
    n--;
    elem.valeur = n;
    elem.choix = ENTIER;
    cellule_pile bloc = depiler(p);
    ajout_en_tete(seq, bloc);
    if (n != 0) {
      empiler(p, bloc);
      empiler(p, elem);
    }
  } else {
    depiler(p);
    depiler(p);
  }
}

