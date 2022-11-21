/*
* Naam : Nico Nap
* UvAnetID : 14259338
* Opleiding : BSc Informatica
*
* spel.c:
* - Dit bestand bevat alle benodigde functies om het doolhof in de console te
*   laten zien en om de speler te laten bewegen, verliezen of winnen.
* - De library ncurses wordt gebruikt om in de console het doolhof te laten zien
*   en om veranderingen in de positie van de speler te laten zien als de
*   gebruiker de pijltjestoetsen gebruikt. Om die veranderingen te laten zien
*   worden de functies uit rooster.c gebruikt.
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rooster.h"

/*
* Deze functie toont het gegeven rooster met ncurses.
*
* Invoer:
* rp is een pointer naar het rooster.
*/
void toon_rooster(rooster *rp) {
  int breedte = rooster_breedte(rp);
  int hoogte = rooster_hoogte(rp);

  clear();
  for (int i = 0; i < hoogte; i++) {
    for (int j = 0; j < breedte; j++) {
     addch(rooster_kijk(rp, j, i));
    }
    addch('\n');
  }
  refresh();
}


/*
* Voert de benodigde veranderingen in het rooster door als de speler in een
* bepaalde richting probeert te bewegen.
*
* Invoer:
* rp is een pointer naar het rooster.
* dx,dy geven de richting aan waarin de speler probeert te bewegen. De vier
* mogelijkheden voor (dx,dy) zijn (-1,0), (1,0), (0,-1), (0,1) voor
* respectievelijk links, rechts, omhoog en omlaag.
*
* Side effects:
* Het rooster wordt aangepast op basis van de handeling van de speler.
*/
void beweeg(rooster *rp, int dx, int dy) {
  int x;
  int y;
  rooster_zoek(rp, '*', &x, &y);

  int toegestaan = 1;
  char teken = rooster_kijk(rp, (x + dx), (y - dy));
  switch(teken) {
    case '#': toegestaan = 0; break;
    case 'X': rooster_zet_toestand(rp, VERLOREN); break;
    case '$': rooster_zet_toestand(rp, GEWONNEN); break;
  }

  if (toegestaan) {
    rooster_plaats(rp, x, y, ' ');
    rooster_plaats(rp, (x + dx), (y - dy), '*');
  }
}


/*
* Speelt het spel met een gegeven rooster tot de toestand niet langer
* AAN_HET_SPELEN is.
*
* Invoer:
* rp is een pointer naar het rooster.
*/
void speel(rooster *rp) {
  while (rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {
    toon_rooster(rp);

    int toets = getch();
    switch (toets) {
      case KEY_LEFT:  beweeg(rp, -1, 0); break;
      case KEY_RIGHT: beweeg(rp, 1, 0);  break;
      case KEY_DOWN:  beweeg(rp, 0, -1); break;
      case KEY_UP:    beweeg(rp, 0, 1);  break;
    }
  }

  clear();
}

/*
* Deze functie leest het doolhof in dat opgegeven wordt door de speler en
* start dan het spel met dat doolhof.
*
* Invoer:
* argc geeft het aantal opgegeven argumenten aan.
* argv is een pointer naar de array waarin de opgegeven argumenten staan.
*
* Side effects:
* Deze functie leest het opgegeven bestand en start het spel.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
int main(int argc, char *argv[]) {
  if (argv[1] == NULL) {
    printf("Geef een bestandsnaam op.\n");
    return -1;
  }

  FILE *fh = fopen(argv[1], "r");
  if (fh == NULL) {
    printf("Bestand niet gevonden.\n");
    return -1;
  }

  rooster *rp = rooster_lees(fh);
  if (rp == NULL) {
    printf("Er is iets fout gegaan, probeer het opnieuw.\n");
    return -1;
  }

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  rooster_zet_toestand(rp, AAN_HET_SPELEN);
  speel(rp);

  endwin();

  if (rooster_vraag_toestand(rp) == VERLOREN) {
    printf("Je hebt verloren!\n");
  } else {
    printf("Je hebt gewonnen!\n");
  }
  rooster_klaar(rp);

  return 0;
}
