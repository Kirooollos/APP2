#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les 
 * affichages.
 * Pour cela, le plus simple est de compiler avec la macro 'SILENT' définie, en 
 * faisant 'make silent'
 */

#ifdef SILENT

/* Desactive tous les affichages */
#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)
/* Desactive les 'Appuyer sur entrée pour continuer' */
#define getchar() ('\n')

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

extern bool silent_mode;

#define MAX_PILE 100


struct cellule {
    char   command;
    /* vous pouvez rajouter d'autres champs ici */
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
    int n;
};
typedef struct sequence sequence_t;

enum {
  ENTIER = 0,
  BLOC = 1
};


typedef struct {
    union {
        int valeur ;
        cellule_t *bloc ;
    };
    int choix ;
} cellule_pile;


typedef struct {
    int n;
    cellule_pile tab[MAX_PILE];
}Pile;

cellule_t *nouvelleCellule(void);

void suppression_tete(sequence_t *seq);

void detruireCellule(cellule_t *);

void ajout_en_queue (sequence_t *l, char n);

void conversion (char *texte, sequence_t *seq);

void afficher (sequence_t* seq);

void empiler (Pile *p, cellule_pile valeur);

cellule_pile depiler (Pile *p);

void afficher_pile(Pile *p);

void suppression_tete(sequence_t *seq);

void addition_acte2(Pile *p);

void soustraction_acte2(Pile *p);

void multiplication_acte2(Pile *p);

void bloc(sequence_t *seq, Pile *p);

void ajout_en_tete(sequence_t *seq, cellule_pile valeur);

void detruireBloc(cellule_t *bloc);

void echangeBloc(Pile *p);

cellule_t *sequencecopie(cellule_t *seqp);

void clones(cellule_pile bloc, Pile *p);

void boucle(sequence_t *seq, Pile *p);


#endif
