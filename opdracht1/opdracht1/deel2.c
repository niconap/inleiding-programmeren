/*
* Naam: Nico Nap
* UvAnetID: 14259338
* Studie: BSc Informatica
*
* deel2.c:
* - Dit programma vraagt om de salarissen van Floor en Ingrid
*   en rekent dan het verschil en de som van die salarissen uit.
*/

#include <stdio.h>
#include <ctype.h>

/*
* Vraagt om het salaris van Floor en Ingrid en rekent
* het verschil en de som uit.
*
* Side effect: print het verschil tussen en de som van de twee ingevoerde
* salarissen in de console.
*
* Uitvoer:
* deze functie heeft geen uitvoer.
*/
int main(void) {
  int salaris_floor;
  int salaris_ingrid;
  int verschil;
  int som;

  printf("Wat is het salaris van Floor?: ");
  if (scanf("%d", &salaris_floor) != 1 || salaris_floor < 0) {
    printf("Ongeldige waarde. Vul alleen getallen boven de 0 in.\n");
    return 0;
  }
  printf("Wat is het salaris van Ingrid?: ");
  if (scanf("%d", &salaris_ingrid) != 1 || salaris_ingrid < 0) {
    printf("Ongeldige waarde. Vul alleen getallen boven de 0 in.\n");
    return 0;
  }


  if (salaris_floor > salaris_ingrid) {
    verschil = salaris_floor - salaris_ingrid;
    printf("Het verschil is %d\n", verschil);
  } else {
    verschil = salaris_ingrid - salaris_floor;
    printf("Het verschil is %d\n", verschil);
  }

  som = salaris_floor + salaris_ingrid;
  printf("De som is %d\n", som);

  return 0;
}
