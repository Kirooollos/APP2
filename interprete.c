#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"


/*
 *  Auteur(s) : Hussein Omar - Attia Kirollos
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}


int interprete (sequence_t* seq, bool debug) {
  // Version temporaire a remplacer par une lecture des commandes dans la
  // liste chainee et leur interpretation.

  char commande;
    
  Pile pile = {0, {}} ;
  cellule_pile element;
  cellule_pile blocV, blocF;
  //debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

  printf ("Programme:") ;
  afficher(seq) ;
  printf ("\n") ;
  if (debug) stop() ;

  // À partir d'ici, beaucoup de choses à modifier dans la suite.
  //printf ("\n>>>>>>>>>>> A Faire : interprete.c/interprete() <<<<<<<<<<<<<<<<\n") ;

  commande = seq ->tete->command ; // à modifier: premiere commande de la sequence
  int ret ; // utilisée pour les valeurs de retour
  int argument;
  
    while ( seq != NULL ) { //à modifier: condition de boucle seq->tete->suivant != NULL
      commande = seq->tete->command;
      seq->tete = seq->tete->suivant;
      afficher_pile(&pile);
      switch (commande) {

        case 'D' :
            droite() ;
            if (ret == VICTOIRE) {
              return VICTOIRE;
              
            }
            if (ret == RATE)
              return RATE ;
            break ;

        case 'G':
            gauche() ;
            if (ret == VICTOIRE) {
              return VICTOIRE;
            }
            if (ret == RATE)
              return RATE ;
            break ;

        case 'A' :
            ret = avance() ;
            if (ret == VICTOIRE) {
              return VICTOIRE; /* on a atteint la cible */
            }
            if (ret == RATE) {
              return RATE ; /* tombé dans l'eau ou sur un rocher */
            }
            break; /* à ne jamais oublier !!! */

        case 'P':
            element = depiler(&pile);
            pose(element.valeur);
            break;

        case 'X':
            echangeBloc(&pile);
            break;

        case 'M':
            element = depiler(&pile);
            argument = element.valeur;
            element.valeur = mesure(argument);
            element.choix = ENTIER;
            empiler(&pile, element);
            break;

        case 'C':
            element = depiler(&pile);
            clones(element, &pile);
            break;

        case 'I':
            depiler(&pile);
            break;

        case 'B':
            boucle(seq, &pile);
            break;
        
            
        case '{':
            printf("J'empile dans : ");
            afficher_pile(&pile);
            printf("\n");
            bloc(seq, &pile);
            break;

        case '!':
            element = depiler(&pile);
            ajout_en_tete(seq, element);
            break;

        case '?':
            blocF = depiler(&pile);
            blocV = depiler(&pile);
            element = depiler(&pile);

            if (element.valeur != 0) {
              if (blocV.bloc != NULL) {
                ajout_en_tete(seq, blocV);
                detruireBloc(blocF.bloc);
                blocV.bloc = NULL;
                break ;
              } else {
                break ;
              }
            } else {
              if (blocF.bloc != NULL) {
                ajout_en_tete(seq, blocF);
                detruireBloc(blocV.bloc);
                blocF.bloc = NULL;
                break ;
              } else {
                break ;
              }
            }

        case '+' :
            addition_acte2(&pile);
            break ;

        case '-' :
            soustraction_acte2(&pile);
            break ;

        case '*' :
            multiplication_acte2(&pile);
            break ;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            element.valeur = commande - '0';
            printf("valeur empilée : %d\n", element.valeur);
            element.choix = ENTIER;
            empiler(&pile, element);
            afficher_pile(&pile);
            break;

        case ' ':
        case '\n':
        case '}':
            break;

        default :
            eprintf("Caractère inconnu: '%c'\n", commande) ;
      }
      afficher_pile(&pile); 
      /* Affichage pour faciliter le debug */
      afficherCarte();
      printf("Programme:") ;
      afficher(seq) ;
      printf("\n") ;
      if (debug) {
        stop();
      }

  /* Si on sort de la boucle sans arriver sur la cible,
  * c'est raté :-( */
    }
  return CIBLERATEE ;
}