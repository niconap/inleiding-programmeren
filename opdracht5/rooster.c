/*
* Naam : Nico Nap
* UvAnetID : 14259338
* Studie : BSc Informatica
*
* rooster.c:
* - In dit bestand wordt een struct gemaakt waarin alle belangrijke informatie
*   over het doolhof wordt opgeslagen. De functies zorgen er dan voor dat deze
*   informatie gelezen en veranderd kan worden door spel.c.
*/

#include "rooster.h"
#include <stdlib.h>

const int MAX_GROOTTE = 30;

struct rooster_data {
  char **grid;
  int breedte;
  int hoogte;
  toestand fase;
};

/*
* Deze functie maakt een rooster op basis van het gegeven bestand.
*
* Invoer:
* fh is de stream waaruit het doolhof gelezen moet worden.
*
* Uitvoer:
* Als alles goed gaat, een pointer naar een rooster (die op de heap is
* gealloceerd), dat overeenkomt met de gegeven beschrijving.
* De begintoestand is BEGIN.
* Als de beschrijving niet consistent is (bijvoorbeeld
* niet alle rijen zijn even lang, of er klopt iets anders niet), of
* als niet voldoende geheugen kan worden gereserveerd, dan wordt
* NULL teruggegeven. (In dat geval blijft geen gereserveerd geheugen
* achter.)
*/
rooster *rooster_lees(FILE *fh) {
  if (fh == NULL) {
    return NULL;
  }

  rooster *rp = malloc(sizeof(rooster));
  if (rp == NULL) {
    return NULL;
  }

  rp->grid = malloc((MAX_GROOTTE * MAX_GROOTTE + 1) * sizeof(char *));
  int huidige_breedte = 0;
  int vorige_breedte = 0;
  int hoogte = 0;
  for (int i = 0; i < MAX_GROOTTE; i++) {
    hoogte++;
    rp->grid[i] = malloc((MAX_GROOTTE + 1) * sizeof(char));

    int index = 0;
    char letter = getc(fh);
    while(letter != '\n') {
      huidige_breedte++;
      if (huidige_breedte < MAX_GROOTTE) {
        rp->grid[i][index++] = letter;
        letter = getc(fh);
      } else {
        rp->hoogte = hoogte;
        rooster_klaar(rp);
        return NULL;
      }

      if (letter == EOF) {
        break;
      }
    }

    if (huidige_breedte != vorige_breedte && i != 0) {
      rp->hoogte = hoogte;
      rooster_klaar(rp);
      return NULL;
    }

    if (letter == EOF) {
      break;
    }

    vorige_breedte = huidige_breedte;
    huidige_breedte = 0;
  }
  fclose(fh);

  rp->breedte = vorige_breedte;
  rp->hoogte = hoogte;
  rp->fase = BEGIN;
  return rp;
}

/*
* Deze functie slaat een rooster op met de gegeven bestandsnaam in de
* subdirectory "assets".
*
* Invoer:
* fh is de stream waar het doolhof heen geschreven moet worden.
*
* Side effects:
* Het opgegeven doolhof wordt naar de stream gestuurd.
*/
void rooster_schrijf(const rooster *rp, FILE *fh) {
  for (int i = 0; i < rooster_hoogte(rp); i++) {
    for (int j = 0; j < rooster_breedte(rp); j++) {
      fputc(rp->grid[i][j], fh);
    }
    fputc('\n', fh);
  }
  fclose(fh);
}

/*
* Vraag de huidige toestand van het spel op.
*
* Invoer:
* rp is een pointer naar het rooster.
*
* Uitvoer:
* Deze functie geeft de toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
*/
toestand rooster_vraag_toestand(const rooster *rp) {
  return rp->fase;
}

/*
* Verander de huidige toestand van het spel.
*
* Invoer:
* rp is een pointer naar het rooster.
* t is de nieuwe toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
*/
void rooster_zet_toestand(rooster *rp, toestand t) {
  rp->fase = t;
}

/*
* Geef alle resources vrij die zijn gealloceerd voor een rooster.
*
* rp is een pointer naar het rooster.
*/
void rooster_klaar(rooster *rp) {
  for (int i = 0; i < rp->hoogte; i++) {
    free(rp->grid[i]);
  }
  free(rp->grid);
  free(rp);
}

/*
* Geef de breedte van het rooster, dat wil zeggen, het aantal kolommen.
*
* Invoer:
* rp is een pointer naar het rooster.
*
* Uitvoer:
* Deze functie geeft de breedte van het rooster.
*/
int rooster_breedte(const rooster *rp) {
  return rp->breedte;
}

/*
* Geef de hoogte van het rooster, dat wil zeggen, het aantal rijen.
*
* Invoer:
* rp is een pointer naar het rooster.
*
* Uitvoer:
* Deze functie geeft de hoogte.
*/
int rooster_hoogte(const rooster *rp) {
  return rp->hoogte;
}

/*
* Kijk of de gegeven positie binnen het rooster valt.
*
* Invoer:
* rp iseen pointer naar het rooster.
* x,y zijn de positie.
*
* Uitvoer:
* Deze functie geeft 1 als de positie binnen het rooster valt, anders 0.
*/
int rooster_bevat(const rooster *rp, int x, int y) {
  if (x < rp->breedte && x >= 0 && y < rp->hoogte && y >= 0) {
    return 1;
  } else {
    return 0;
  }
}

/*
* Kijk welk object er staat op een bepaalde positie in het rooster.
*
* Invoer:
* rp : een pointer naar het rooster
* x,y: de betreffende positie.
*
* Uitvoer: het object op die positie, of '\0' als de positie buiten het
* rooster valt.
*/
char rooster_kijk(const rooster *rp, int x, int y) {
  if (rooster_bevat(rp, x, y)) {
    return rp->grid[y][x];
  } else {
    return '\0';
  }
}

/*
* Schrijf een bepaald object op een bepaalde positie in het rooster.
*
* Invoer:
* rp is een pointer naar het rooster
* x,y is de betreffende positie.
* c is het object.
*
* Side effects:
* Als (x,y) binnen het rooster ligt, wordt het object op de opgegeven
* positie geplaatst, anders verandert er niets.
*
* Uitvoer:
* Deze functie geeft 1 als het object is geplaatst, of 0 als het buiten de
* grenzen lag.
*/
int rooster_plaats(rooster *rp, int x, int y, char c) {
  if (rooster_bevat(rp, x, y)) {
    rp->grid[y][x] = c;
    return 1;
  }
  return 0;
}

/*
* Zoek een bepaald object in het rooster, en geef de coordinaten van het
* object terug via de gegeven pointers. Let op: als er meerdere objecten van
* het gezochte soort in het rooster voorkomen, is niet gedefinieerd van welke
* de positie wordt gevonden.
*
* rp is een pointer naar het rooster.
* c is het object dat moet worden gezocht.
* x en y zijn pointers naar de geheugenlocaties waar de gevonden positie
* moet worden geschreven.
*
* Uitvoer:
* Via de pointers x en y. Als het object niet wordt gevonden worden
* *x en *y op -1 gezet.
*/
void rooster_zoek(const rooster *rp, char c, int *x, int *y) {
  for (int i = 0; i < rp->hoogte; i++) {
    for (int j = 0; j < rp->breedte; j++) {
      if (rp->grid[i][j] == c) {
        *x = j;
        *y = i;
        return;
      }
    }
  }
  *x = -1;
  *y = -1;
}
