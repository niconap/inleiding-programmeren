/*
 * Naam : Nico Nap
 * UvAnetID : 14259338
 * Studie : BSc Informatica
 *
 * opdracht_2.c:
 * − Dit bestand bevat verschillende functies die te maken hebben
 * met de Collatz-reeks. Er kan uitgerekend worden wat het hoogste
 * getal is van de reeks, hoe veel stappen het duurt voordat de reeks
 * voorbij is en wat de langste en hoogste reeks is tussen twee getallen.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

/*
* Deze functie berekent het volgende getal van de reeks.
*
* Invoer:
* getal: het huidige getal.
*
* Uitvoer:
* het volgende getal in de reeks.
*/
int collatz_opvolger(int getal) {
  if (getal % 2 == 0) {
    return (getal / 2);
  } else {
    return (getal * 3 + 1);
  }
}

/*
* Deze functie neemt een getal en berekent hoe lang het duurt
* voordat de reeks bij de 1 komt.
*
* Invoer:
* reeks: een getal
*
* Uitvoer: aantal stappen dat genomen wordt totdat de reeks
* bij 1 komt
*/
int collatz_stoptijd(int reeks) {
  int stoptijd = 0;
  int vorige_getal = 0;
  int huidige_getal = reeks;

  while(vorige_getal != 1) {
    vorige_getal = huidige_getal;
    huidige_getal = collatz_opvolger(huidige_getal);
    stoptijd++;
  }

  return stoptijd;
}

/*
* Deze functie bepaalt wat het hoogste getal van een reeks is.
*
* Invoer:
* reeks: een getal
*
* Uitvoer: het hoogste getal uit de reeks
*/
int collatz_grootste(int reeks) {
  int hoogste = 0;
  int huidige_getal = reeks;
  int vorige_getal = 0;

  // Als het vorige getal 1 is, betekent dat dat de reeks afloopt,
  // dus dan stopt de loop.
  while(vorige_getal != 1) {
    if (huidige_getal > hoogste) {
      hoogste = huidige_getal;
    }

    vorige_getal = huidige_getal;
    huidige_getal = collatz_opvolger(vorige_getal);
  }

  return hoogste;
}

/*
* Deze functie zoekt tussen twee getallen het getal met de grootste
* stoptijd.
*
* Invoer:
* a: een getal
* b: een getal
*
* Side effects:
* De functie print het beginnende getal met het hoogste stopgetal,
* het stopgetal en het hoogste getal in de console.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void collatz_langste_tussen(int a, int b) {
  int grootste_stoptijd = 0;
  int grootste_begingetal = 0;
  int grootste_reeksgetal = 0;
  int huidige_stoptijd;

  while (a <= b) {
    huidige_stoptijd = collatz_stoptijd(a);

    if (huidige_stoptijd > grootste_stoptijd) {
      grootste_stoptijd = huidige_stoptijd;
      grootste_begingetal = a;
      grootste_reeksgetal = collatz_grootste(a);
    }

    a++;
  }

  printf(
    "%d %d %d\n",
    grootste_begingetal,
    grootste_stoptijd,
    grootste_reeksgetal
  );
}

/*
* Deze functie zoekt tussen twee getallen het getal met het hoogste getal
* in de Collatz-reeks.
*
* Invoer:
* a: een getal
* b: een getal
*
* Side effects:
* Deze functie print het getal waarvan de Collatz-reeks het hoogste getal
* van alle getallen tussen a en b samen met dat hoogste getal en de stoptijd.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void collatz_grootste_tussen(int a, int b) {
  int grootste_stoptijd = 0;
  int grootste_begingetal = 0;
  int grootste_reeksgetal = 0;
  int huidige_grootste;

  while (a <= b) {
    huidige_grootste = collatz_grootste(a);

    if (huidige_grootste > grootste_reeksgetal) {
      grootste_stoptijd = collatz_stoptijd(a);
      grootste_begingetal = a;
      grootste_reeksgetal = collatz_grootste(a);
    }

    a++;
  }

  printf(
    "%d %d %d\n",
    grootste_begingetal,
    grootste_stoptijd,
    grootste_reeksgetal
  );
}

/*
* Deze functie zoekt het eerste getal waarbij het berekenen
* van de Collatz-reeks ervoor zorgt dat er een overflow optreedt.
*
* Invoer: deze functie heeft geen invoer.
*
* Side effects: deze functie print het eerste getal dat voor
* een overflow zorgt in de console.
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void collatz_maximum(void) {
  int overflow = 0;
  int huidige_reeks = 1;
  int huidige_getal = 1;
  int vorige_getal = 0;

  while (!overflow) {
    while (vorige_getal != 1 && !overflow) {
      vorige_getal = huidige_getal;
      if (huidige_getal % 2 == 0) {
        huidige_getal = huidige_getal / 2;
      } else {
        // Overflow treedt op als het huidige getal groter is dan
        // het maximale getal - 1 / 3.
        if ((INT_MAX - 1) / 3 < huidige_getal) {
          printf(
            "Het eerste begingetal waarbij overflow optreedt is %d.\n",
            huidige_reeks
          );
          overflow = 1;
        } else {
          huidige_getal = huidige_getal * 3 + 1;
        }
      }
    }

    vorige_getal = 0;
    huidige_reeks++;
    huidige_getal = huidige_reeks;
  }
}

/*
* Deze functie werkt nog niet goed!
*
* Deze functie rekent de eerste reeks in de console die ervoor zorgt
* dat er een overflow zal optreden, maar in plaats van int wordt er
* nu unsigned long long gebruikt.
*
* Invoer: geen invoer.
*
* Side effects: print het eerste getal in de console wat voor een
* overflow zal zorgen.
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void collatz_maximum_verlengd(void) {
  int overflow = 0;
  // De reeks van 1 zorgt sowieso niet voor een overflow,
  // dus ik begin bij 2 om het makkelijker te maken
  unsigned long long huidige_reeks = 1;
  unsigned long long huidige_getal = 1;
  unsigned long long vorige_getal = 0;

  while (!overflow) {
    while (vorige_getal != 1) {
      if (huidige_getal % 2 == 0) {
        vorige_getal = huidige_getal;
        huidige_getal = huidige_getal / 2;
      } else {
        // Als het getal groter is dan het maximale getal - 1 / 3, dan zal er
        // overflow gaan plaatsvinden.
        if ((ULLONG_MAX - 1) / 3 < huidige_getal) {
          printf(
           "Het eerste begingetal waarbij overflow optreedt is %llu.\n",
            huidige_reeks
          );
          overflow = 1;
          break;
        } else {
          huidige_getal = huidige_getal * 3 + 1;
        }
      }
    }

    vorige_getal = 0;
    huidige_reeks++;
    huidige_getal = huidige_reeks;
  }
}

#ifndef COMPILE_MAIN
int main(int argc, char **argv) {
  // Check of genoeg argumenten van het juiste type aangeleverd zijn.
  if (argc != 3 || !isdigit(*argv[1]) || !isdigit(*argv[2])) {
    return EXIT_FAILURE;
  }

  // Converteer string argumenten naar integers.
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);

  // Check of de conditie a < b geldt en of a en b positieve getallen zijn.
  if (a >= b || a <= 0 || b <= 0) {
    return EXIT_FAILURE;
  }

  // Roep de collatz_langste_tussen en collatz_grootste_tussen functies aan.
  collatz_langste_tussen(a, b);
  collatz_grootste_tussen(a, b);
  // Bereken de grootste reeks die geen overflow veroorzaakt.
  collatz_maximum();
  // collatz_maximum_verlengd();

  return EXIT_SUCCESS;
}
#endif
