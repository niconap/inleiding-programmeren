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
*
* Opmerking: met de move() functie wordt steeds berekend waar de cursor moet
* komen te staan om tekst in het midden van de console te printen. Het is niet
* toegestaan om strlen uit de string.h library te gebruiken, dus vandaar dat er
* getallen in de berekeningen kunnen staan die niet worden uitgelegd. Deze
* getallen zijn (afgerond) de helft van de string lengte.
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rooster.h"

int LEVENS = 0;

/*
* De volgende vier functies printen alle instructies onder het getoonde doolhof.
* Bij elk level komt er iets nieuws bij, dus elk level krijgt zijn eigen
* instructies. De move functie zorgt er steeds voor dat de tekst op dezelfde
* kolom begint als het doolhof.
*
* Invoer:
* breedte is een pointer naar de breedte van het rooster.
*
* Side effects:
* Print per level de instructies onder het doolhof.
*
* Uitvoer:
* Deze functies hebben allemaal geen uitvoer.
*/
void level0_tekst(int *breedte) {
  move(getcury(stdscr) + 2, stdscr->_maxx / 2 - *breedte / 2);
  printw("Om verder te gaan moet je naar het eind");
  move(getcury(stdscr) + 1, stdscr->_maxx / 2 - *breedte / 2);
  printw("van het doolhof ($), maar pas op voor");
  move(getcury(stdscr) + 1, stdscr->_maxx / 2 - *breedte / 2);
  printw("de vallen (X).");
}

void level1_tekst(int *breedte) {
  move(getcury(stdscr) + 2, stdscr->_maxx / 2 - *breedte / 2);
  printw("Om een deur (-) te openen moet je eerst");
  move(getcury(stdscr) + 1, stdscr->_maxx / 2 - *breedte / 2);
  printw("alle sleutels (+) verzamelen.");
}

void level2_tekst(int *breedte) {
  move(getcury(stdscr) + 2, stdscr->_maxx / 2 - *breedte / 2);
  printw("Je kunt rotsblokken (O) bewegen door er");
  move(getcury(stdscr) + 1, stdscr->_maxx / 2 - *breedte / 2);
  printw("tegenaan te lopen. Druk op r als je vast");
  move(getcury(stdscr) + 1, stdscr->_maxx / 2 - *breedte / 2);
  printw("komt te zitten om het level te resetten.");
}

void level3_tekst(int *breedte) {
  move(getcury(stdscr) + 2, stdscr->_maxx / 2 - *breedte / 2);
  printw("Pas op voor het monster (< of >)! Het monster");
  move(getcury(stdscr) + 1, stdscr->_maxx / 2 - *breedte / 2);
  printw("valt aan als je ervoor komt te staan.");
}

/*
* Deze functie toont het gegeven rooster met ncurses.
*
* Invoer:
* rp is een pointer naar het rooster.
*
* Side effects:
* Toont via ncurses het rooster in de terminal.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void toon_rooster(rooster *rp, int level) {
  int breedte = rooster_breedte(rp);
  int hoogte = rooster_hoogte(rp);

  clear();
  // move zorgt ervoor dat het rooster in het midden van de terminal
  // wordt getekend.
  move(stdscr->_maxy / 2 - hoogte / 2, stdscr->_maxx / 2 - 8);
  printw("---+ Level %d +---", level + 1);
  move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 4);
  printw("Levens: %d", LEVENS);
  for (int i = 0; i < hoogte; i++) {
    move(getcury(stdscr) + 1, stdscr->_maxx / 2 - breedte / 2);
    for (int j = 0; j < breedte; j++) {
     addch(rooster_kijk(rp, j, i));
    }
  }

  if (level == 0) {
    level0_tekst(&breedte);
  }

  if (level == 1) {
    level1_tekst(&breedte);
  }

  if (level == 2) {
    level2_tekst(&breedte);
  }

  if (level == 3) {
    level3_tekst(&breedte);
  }

  refresh();
}

/*
* Deze functie checkt of er nog + karakters (sleutels) aanwezig zijn in het
* veld om te bepalen of de speler door een deur heen kan.
*
* Invoer:
* rp is de pointer naar het rooster.
*
* Uitvoer:
* 1 als er geen sleutels meer zijn en 0 als er nog wel sleutels zijn.
*/
int check_char(rooster *rp, char c) {
  int x;
  int y;
  rooster_zoek(rp, c, &x, &y);
  if (x == -1) {
    return 1;
  } else {
    return 0;
  }
}

/*
* Deze functie zorgt ervoor dat een monster (< of >) naar links en naar rechts
* kan bewegen in het doolhof.
*
* Invoer:
* rp is een pointer naar het rooster.
*
* Side effects:
* Deze functie past de positie van < of > in het rooster aan.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void beweeg_monster(rooster *rp) {
  int x, y;

  // Beweging naar rechts
  rooster_zoek(rp, '>', &x, &y);
  if (x != -1) {
    if (rooster_kijk(rp, x + 1, y) == ' ') {
      rooster_plaats(rp, x, y, ' ');
      rooster_plaats(rp, x + 1, y, '>');
    } else if (rooster_kijk(rp, x + 1, y) == '*') {
      return;
    } else {
      rooster_plaats(rp, x, y, '<');
    }
  }

  // Beweging naar links
  rooster_zoek(rp, '<', &x, &y);
  if (x != -1) {
    if (rooster_kijk(rp, x - 1, y) == ' ') {
      rooster_plaats(rp, x, y, ' ');
      rooster_plaats(rp, x - 1, y, '<');
    } else if (rooster_kijk(rp, x - 1, y) == '*') {
      return;
    } else {
      rooster_plaats(rp, x, y, '>');
    }
  }
}

/*
* Deze functie voert een aanval uit als de speler voor een monster staat, dus
* als >* of *< voorkomt in het rooster. Deze functie wordt uitgevoerd nadat
* de speler bewogen is.
*
* Invoer:
* rp is een pointer naar het rooster.
*
* Side effects:
* Past het aantal levens aan als er een aanval plaatsvindt.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void monster_aanval(rooster *rp) {
  int x, y;

  rooster_zoek(rp, '>', &x, &y);
  if (x != -1 && rooster_kijk(rp, x + 1, y) == '*') {
    LEVENS -= 1;
    return;
  }

  rooster_zoek(rp, '<', &x, &y);
  if (x != -1 && rooster_kijk(rp, x - 1, y) == '*') {
    LEVENS -= 1;
    return;
  }
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

  beweeg_monster(rp);

  int toegestaan = 1;
  char teken = rooster_kijk(rp, (x + dx), (y - dy));
  switch(teken) {
    case '#': toegestaan = 0; break;
    case '-': toegestaan = check_char(rp, '+');break;
    case 'O': {
      if (rooster_kijk(rp, x + 2 * dx, y - 2 * dy) == ' ') {
        rooster_plaats(rp, x + dx, y - dy, ' ');
        rooster_plaats(rp, x + 2 * dx, y - 2 * dy, 'O');
      } else {
        toegestaan = 0;
      }
      break;
    }
    case '>': toegestaan = 0; break;
    case '<': toegestaan = 0; break;
    case 'X': LEVENS -= 1; toegestaan = 0; break;
    case '$': rooster_zet_toestand(rp, GEWONNEN); break;
  }

  if (toegestaan) {
    rooster_plaats(rp, x, y, ' ');
    rooster_plaats(rp, (x + dx), (y - dy), '*');
  }

  monster_aanval(rp);

  if (LEVENS == 0) {
    rooster_zet_toestand(rp, VERLOREN);
  }
}

/*
* Speelt het spel met een gegeven rooster tot de toestand niet langer
* AAN_HET_SPELEN is.
*
* Invoer:
* rp is een pointer naar het rooster.
*/
void speel(rooster *rp, int level) {
  while (rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {
    toon_rooster(rp, level);

    int toets = getch();
    switch (toets) {
      case KEY_LEFT:  beweeg(rp, -1, 0); break;
      case KEY_RIGHT: beweeg(rp, 1, 0);  break;
      case KEY_DOWN:  beweeg(rp, 0, -1); break;
      case KEY_UP:    beweeg(rp, 0, 1);  break;
      case 'r':       rooster_zet_toestand(rp, RESET); break;
    }
  }

  clear();
}

/*
* Deze functie print het beginscherm en wacht op input van de gebruiker. Als de
* gebruiker op spatie drukt zal het spel beginnen. De ASCII-art is gegenereerd
* via https://textkool.com/en/ascii-art-generator.
*
* Invoer:
* Deze functie heeft geen invoer.
*
* Side effects:
* Print het beginscherm in de console.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void beginscherm(void) {
  while (1) {
    clear();
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');

    move(stdscr->_maxy / 2 - 4, stdscr->_maxx / 2 - 28);
    printw("  /\\/\\   __ _ _______   _ __ _   _ _ __  _ __   ___ _ __ ");
    move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 28);
    printw(" /    \\ / _` |_  / _ \\ | '__| | | | '_ \\| '_ \\ / _ \\ '__|");
    move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 28);
    printw("/ /\\/\\ \\ (_| |/ /  __/ | |  | |_| | | | | | | |  __/ |   ");
    move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 28);
    printw("\\/    \\/\\__,_/___\\___| |_|   \\__,_|_| |_|_| |_|\\___|_|   ");

    move(stdscr->_maxy / 2 + 1, stdscr->_maxx / 2 - 14);
    printw("Druk op spatie om te beginnen");

    char ch = getch();
    if (ch == ' ') {
      break;
    }
  }
}


/*
* Deze functie print het eindscherm en wacht op input van de gebruiker. Als de
* gebruiker op spatie drukt zal het spel opnieuw starten en als de gebruiker op
* een andere toets drukt zal het programma stoppen. De ASCII-art is gegenereerd
* via https://textkool.com/en/ascii-art-generator.
*
* Invoer:
* t is de toestand van het huidige rooster.
*
* Side effects:
* Print het eindscherm in de console.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void einde(toestand t) {
  while (1) {
    clear();
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');

    if (t == GEWONNEN) {
      move(stdscr->_maxy / 2 - 4, stdscr->_maxx / 2 - 26);
      printw("   ___                                            _ ");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 26);
      printw("  / _ \\_____      _____  _ __  _ __   ___ _ __   / \\");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 26);
      printw(" / /_\\/ _ \\ \\ /\\ / / _ \\| '_ \\| '_ \\ / _ \\ '_ \\ /  /");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 26);
      printw("/ /_\\\\  __/\\ V  V / (_) | | | | | | |  __/ | | /\\_/ ");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 26);
      printw("\\____/\\___| \\_/\\_/ \\___/|_| |_|_| |_|\\___|_| |_\\/   ");
    } else {
      move(stdscr->_maxy / 2 - 4, stdscr->_maxx / 2 - 20);
      printw("                 _                      ");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 20);
      printw(" /\\   /\\___ _ __| | ___  _ __ ___ _ __  ");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 20);
      printw(" \\ \\ / / _ \\ '__| |/ _ \\| '__/ _ \\ '_ \\ ");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 20);
      printw("  \\ V /  __/ |  | | (_) | | |  __/ | | |");
      move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 20);
      printw("   \\_/ \\___|_|  |_|\\___/|_|  \\___|_| |_|");
    }

    move(getcury(stdscr) + 2, stdscr->_maxx / 2 - 18);
    printw("Druk op spatie om opnieuw te spelen.");

    move(getcury(stdscr) + 1, stdscr->_maxx / 2 - 18);
    printw("Druk op c om het spel af te sluiten.");

    char ch = getch();
    if (ch == ' ') {
      break;
    } else if (ch == 'c') {
      endwin();
      exit(0);
    }
  }
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
int main(void) {
  char levels[4][20] = {
    "./assets/level1.txt",
    "./assets/level2.txt",
    "./assets/level3.txt",
    "./assets/level4.txt"
  };
  FILE *fh;
  rooster *level;
  LEVENS = 3;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  beginscherm();

  for (int i = 0; i < 4; i++) {
    fh = fopen(levels[i], "r");
    if (fh == NULL) {
      endwin();
      printf("Bestand niet gevonden.\n");
      exit(1);
    }

    level = rooster_lees(fh);
    if (level == NULL) {
      endwin();
      printf("Er is iets fout gegaan, probeer het opnieuw.\n");
      exit(1);
    }

    rooster_zet_toestand(level, AAN_HET_SPELEN);
    speel(level, i);
    if (rooster_vraag_toestand(level) == VERLOREN) {
      einde(VERLOREN);
      main();
    } else if (rooster_vraag_toestand(level) == RESET) {
      i = i - 1;
    }

    rooster_klaar(level);
  }

  einde(GEWONNEN);
  main();

  return 0;
}
