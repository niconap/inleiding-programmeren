/*
* Naam: Nico Nap
* UvAnetID: 14259338
* Studie: BSc Informatica
*
* deel3.c:
* - Dit programma vraagt de speler om een getal te raden.
* - Het spel duurt zo lang mogelijk: wanneer er geen getallen
*   meer zijn die geraden kunnen worden wint de speler.
*/

#include <stdio.h>

/*
* Vraag om een getal van de speler en vertel of dat getal te hoog
* of te laag is of dat de speler gewonnen heeft.
*
* Side effect: print feedback over of het getal te hoog, te laag of
* geraden is in de console.
*
* Uitvoer:
* deze functie heeft geen uitvoer.
*/
int main(void) {
  int hoogste = 101;
  int laagste = -1;
  int geraden = 0;
  int gok;

  while (!geraden) {
    printf("Doe een gok: ");
    if (scanf("%d", &gok) != 1) {
      gok = -1;
      printf("Ongeldige gok.\n");
      break;
    } else if (gok > 100 || gok < 0) {
      printf("Er kunnen alleen getallen tussen 0 en 100 ingevuld worden.\n");
    } else if (hoogste - 1 == gok && laagste + 1 == gok) {
      printf("Gewonnen!, spel stopt\n");
      geraden = 1;
    } else if (hoogste - gok < gok - laagste) {
      printf("te hoog\n");
      if (gok < hoogste) {
        hoogste = gok;
      }
    } else {
      printf("te laag\n");
      if (gok > laagste) {
        laagste = gok;
      }
    }
  }

  return 0;
}
