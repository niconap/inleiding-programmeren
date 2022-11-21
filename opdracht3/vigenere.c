#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_GROOTTE 20000
#define MAX_SLEUTEL 16

/* in: een array fn met daarin een bestandsnaam
     een array uit dat voldoende groot is om het bestand te bevatten.
   uit: -
   side effect: het bestand wordt ingelezen en in het uit array opgeslagen.
*/
void lees_bestand(const char fn[], char uit[]) {
  FILE *fh = fopen(fn, "r");
  if (fh==NULL) {
    perror("Kan invoer niet lezen");
    exit(1);
  }
  size_t grootte = fread(uit, 1, MAX_GROOTTE, fh);
  fclose(fh);
  if (grootte==MAX_GROOTTE) {
    fprintf(stderr, "De invoer was te lang.\n");
    exit(1);
  }
  uit[grootte] = '\0';
}

/*
* Deze functie haalt de string uit het bestand, haalt alle tekens weg en
* zorgt ervoor dat alle letters kleine letters zijn.
*
* Invoer:
* fn[]: het pad naar het bestand.
* uit[]: pointer naar de array waarin de gelezen string komt te staan.
*
* Side effect: verandert de inhoud van uit[].
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void lees_en_versimpel(const char fn[], char uit[]) {
  lees_bestand(fn, uit);

  for(int i = 0; uit[i] != 0; i++) {
    if(uit[i] >= 'A' && uit[i] <= 'Z') {
      uit[i] = (char)(uit[i] + 32);
    }
    if(uit[i] < 'a' || uit[i] > 'z') {
      // 1 is een tijdelijke waarde, in de volgende for loop wordt alles in
      // de string met waarde 1 weggehaald.
      uit[i] = 1;
    }
  }

  for (int i = 0; i <= (int)strlen(uit); i++) {
    if (uit[i] == 1) {
      // Schuif alles 1 plek naar links, zodat het element met waarde 1 weg
      // wordt gehaald.
      for(int j = i; j <=(int)strlen(uit); j++) {
        uit[j] = uit[j + 1];
      }
      i -= 1;
    }
  }
}

/*
* Deze functie zorgt ervoor dat alle letters in een string 3 plekken
* worden opgeschoven.
*
* Invoer:
* fn_bericht[]: het pad naar het bestand met de string.
*
* Side effects: schuift de letters in uit[] met 3 plekken en slaat
* het resultaat op in versleuteld[].
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void opdracht_versleutel_caesar(const char fn_bericht[]) {
  char uit[MAX_GROOTTE + 1];
  char versleuteld[MAX_GROOTTE + 1];

  lees_en_versimpel(fn_bericht, uit);

  for(int i = 0; i < (int)strlen(uit); i++) {
    versleuteld[i] = (char)(uit[i] + 3);
    if (versleuteld[i] > 'z') {
      versleuteld[i] = (char)(('a' - 1) + (versleuteld[i] - 'z'));
    }
  }

  printf("%s\n", versleuteld);
}

/*
* Deze functie zorgt ervoor dat alle letters in een string 3 plekken
* worden teruggeschoven.
*
* Invoer:
* fn_cijfertekst[]: het pad naar het bestand met de versleutelde string.
*
* Side effects: schuift de letters in uit[] terug met 3 plekken en slaat
* het resultaat op in ontsleuteld[].
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void opdracht_ontsleutel_caesar(const char fn_cijfertekst[]) {
  char uit[MAX_GROOTTE + 1];
  char ontsleuteld[MAX_GROOTTE + 1];

  lees_en_versimpel(fn_cijfertekst, uit);

  for(int i = 0; i < (int)strlen(uit); i++) {
    ontsleuteld[i] = (char)(uit[i] - 3);
    if (ontsleuteld[i] < 'a') {
      ontsleuteld[i] = (char)(('z' + 1) - ('a' - ontsleuteld[i]));
    }
  }

  printf("%s\n", ontsleuteld);
}

/*
* Deze functie zorgt ervoor dat elke letter in een string
* op basis van een sleutel wordt verschoven.
*
* Invoer:
* bericht[] is de originele string die versleuteld moet
* worden.
* cijfertekst[] is een array waar uiteindelijk de versleutelde string
* in komt te staan.
* sleutel[] is de sleutel die wordt gebruikt.
*
* Side effects: de array cijfertekst[] wordt aangepast en in
* de console geprint.
*
* Uitvoer: er is geen uitvoer
*/
void versleutel(const char bericht[], char cijfertekst[],
        const char sleutel[]) {
  int i;

  for(i = 0; i < (int)strlen(bericht); i++) {
    int num = i % (int)strlen(sleutel);

    if (i > (int)strlen(sleutel) - 1) {
      /*
      * Kijk of de shift ervoor zorgt dat het karakter
      * voorbij de z geschoven wordt, zo ja ga dan terug naar
      * de a.
      */
      if (bericht[i] + (sleutel[num] - 'a') > 'z') {
        char verschil = (char)('z' - bericht[i]);
        char shift = (char)(sleutel[num] - 'a' - verschil);
        cijfertekst[i] = (char)(('a' - 1) + shift);
      } else {
        cijfertekst[i] = (char)(bericht[i] + (sleutel[num] - 'a'));
      }
    } else {
      if (bericht[i] + (sleutel[num] - 'a') > 'z') {
        char temp = (char)('z' - bericht[i]);
        char shift = (char)(sleutel[i] - 'a' - temp);
        cijfertekst[i] = (char)(('a' - 1) + shift);
      } else {
        cijfertekst[i] = (char)(bericht[i] + (sleutel[i] - 'a'));
      }
    }
  }

  cijfertekst[i + 1] = '\0';
}

/*
* Deze functie ontsleuteld versleutelde tekst op basis van een sleutel.
*
* Invoer:
* cijfertekst[]: de te tekst die ontsleuteld moet worden.
* bericht[]: het uiteindelijke ontsleutelde bericht.
* sleutel[]: de sleutel die gebruikt wordt.
*
* Side effects: past de array bericht[] aan.
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void ontsleutel(const char cijfertekst[], char bericht[],
        const char sleutel[]) {
  int i;

  for(i = 0; i < (int)strlen(cijfertekst); i++) {
    int num = i % (int)strlen(sleutel);

    if (i > (int)strlen(sleutel) - 1) {
      /*
      * Kijk of de shift ervoor zorgt dat het karakter
      * onder de a geschoven wordt, zo ja ga dan terug naar
      * de z.
      */
      if (cijfertekst[i] - (sleutel[num] - 'a') < 'a') {
        char verschil = (char)(cijfertekst[i] - 'a');
        char shift = (char)(sleutel[num] - 'a' - verschil);
        bericht[i] = (char)(('z' + 1) - shift);
      } else {
        bericht[i] = (char)(cijfertekst[i] - (sleutel[num] - 'a'));
      }
    } else {
      if (cijfertekst[i] - (sleutel[num] - 'a') < 'a') {
        char verschil = (char)(cijfertekst[i] - 'a');
        char shift = (char)(sleutel[num] - 'a' - verschil);
        bericht[i] = (char)(('z' + 1) - shift);
      } else {
        bericht[i] = (char)(cijfertekst[i] - (sleutel[i] - 'a'));
      }
    }
  }

  bericht[i + 1] = '\0';
}

/*
* Deze functie maakt nieuwe arrays aan en haalt de string op die versleuteld
* moet worden. Daarna roept hij de versleutelfunctie aan.
*
* Invoer:
* fn_cijfertekst[]: het pad naar de tekst.
* sleutel[]: de sleutel die wordt gebruikt.
*
* Side effects: print de versleutelde tekst in de console.
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void opdracht_versleutel(const char fn_bericht[], const char sleutel[]) {
  char bericht[MAX_GROOTTE + 1];
  char cijfertekst[MAX_GROOTTE + 1];

  lees_en_versimpel(fn_bericht, bericht);
  versleutel(bericht, cijfertekst, sleutel);

  printf("%s\n", cijfertekst);
}

/*
* Deze functie maakt nieuwe arrays aan en haalt de string op die ontsleuteld
* moet worden. Daarna roept hij de ontsleutelfunctie aan.
*
* Invoer:
* fn_cijfertekst[]: het pad naar de versleutelde tekst.
* sleutel[]: de gebruikte sleutel.
*
* Side effects: print de ontsleutelde tekst in de console.
*
* Uitvoer: deze functie heeft geen uitvoer.
*/
void opdracht_ontsleutel(const char fn_cijfertekst[], const char sleutel[]) {
  char bericht[MAX_GROOTTE + 1];
  char cijfertekst[MAX_GROOTTE + 1];

  lees_en_versimpel(fn_cijfertekst, cijfertekst);
  ontsleutel(cijfertekst, bericht, sleutel);

  printf("%s\n", bericht);
}

/*
* Deze functie bepaalt de frequenties van elk van de letters
* in een bepaalde string.
*
* Invoer:
* tekst[] is de string waarvan je de frequenties wilt weten.
* p[] is de array waarin de frequenties van elke letter komen te staan.
*
* Side effects:
* Deze functie past de inhoud van p[] aan.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void bepaal_frequenties(const char tekst[], double p[]) {
  for (int i = 0; i <= (int)strlen(tekst) - 1; i++) {
    int tekstnummer = tekst[i] - 'a';
    p[tekstnummer] += 1;
  }

  for (int i = 0; i <= 25; i++) {
    p[i] = p[i] / (int)strlen(tekst);
  }
}

/*
* Deze functie haalt de tekst op waarvan de frequentie gemeten moet
* worden en stuurt deze door naar bepaal_frequenties.
*
* Invoer:
* fn_tekst[] is het pad naar het bestand met de tekst.
*
* Side effects:
* Deze functie print alle letters met de bijbehorende frequenties
* in de console.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void opdracht_frequenties(const char fn_tekst[]) {
  char uit[MAX_GROOTTE + 1];
  double p[26] = {0};

  lees_en_versimpel(fn_tekst, uit);
  bepaal_frequenties(uit, p);

  for (int i = 0; i <= 25; i++) {
    char letter;
    letter = (char)('a' + i);
    printf("%c: %.3lf\n", letter, p[i]);
  }
}

/*
* Deze functie rekent de gelijkenis van de frequenties van twee verschillende
* strings uit.
*
* Invoer:
* p[] en q[] zijn allebei arrays waarin de frequenties staan uit verschilende
* strings.
*
* Uitvoer:
* De berekende gelijkenis is de uitvoer van deze functie.
*/
double gelijkenis(const double p[], const double q[]) {
  double som = 0;

  for (int i=0; i <= 25; i++) {
    som += p[i] * q[i];
  }

  return som;
}

/*
* Deze functie haalt twee verschillende strings op, versimpeld ze, bepaalt de
* frequenties en de gelijkenis.
*
* Invoer:
* fn_tekst1[] en fn_tekst2[] zijn de paden naar de bestanden waarin de
* strings staan.
*
* Side effects:
* Deze functie print de gelijkenis van de twee strings in de console.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void opdracht_vergelijk(const char fn_tekst1[], const char fn_tekst2[]) {
  double p[26] = {0};
  double q[26] = {0};
  char tekst1[MAX_GROOTTE + 1];
  char tekst2[MAX_GROOTTE + 1];

  lees_en_versimpel(fn_tekst1, tekst1);
  lees_en_versimpel(fn_tekst2, tekst2);

  bepaal_frequenties(tekst1, p);
  bepaal_frequenties(tekst2, q);

  printf("%.3lf\n", gelijkenis(p, q));
}

/*
* Deze functie kijkt letter voor letter welke letter voor een betere sleutel
* zorgt en bepaalt dan de beste sleutel voor een bepaalde lengte.
*
* Invoer:
* cijfertekst[] is de tekst die ontsleuteld moet worden.
* sleutel[] is de array waarin de beste sleutel komt te staan.
* slen is de lengte van de te bepalen sleutel.
* frequenties_taal[] bevat de frequenties van de letters uit bint.txt.
*
* Side effects:
* sleutel[] wordt aangepast.
*
* Uitvoer:
* De score van de beste sleutel van lengte slen.
*/
double zoek_sleutel(const char cijfertekst[], char sleutel[], size_t slen,
          const double frequenties_taal[]) {
  double hoogste_gelijkenis = 0;
  double score = 0;
  double huidige_frequentie[26] = {0};
  char huidige_ontsleuteling[MAX_GROOTTE + 1];
  char huidige_kandidaat[18] = {0};
  char beste_letter = 'a';

  // Vul de kandidaatsleutel met a's tot de lengte slen.
  int m;
  for(m = 0; m < (int)slen; m++) {
    huidige_kandidaat[m] = 'a';
  }
  m++;
  huidige_kandidaat[m] = '\0';

  for(int i = 0; i <= (int)slen - 1; i++) {
    for(int j = 0; j <= 25; j++) {
      huidige_kandidaat[i] = (char)(j + 'a');

      ontsleutel(cijfertekst, huidige_ontsleuteling, huidige_kandidaat);
      bepaal_frequenties(huidige_ontsleuteling, huidige_frequentie);

      if (gelijkenis(frequenties_taal, huidige_frequentie)
          > hoogste_gelijkenis) {
        hoogste_gelijkenis = gelijkenis(frequenties_taal, huidige_frequentie);
        beste_letter = huidige_kandidaat[i];
      }
    }

    huidige_kandidaat[i] = beste_letter;
    strcpy(sleutel, huidige_kandidaat);
    beste_letter = 'a';
    score = hoogste_gelijkenis;
    hoogste_gelijkenis = 0;
  }

  return score;
}

/*
* Deze functie bepaalt welke sleutel van een reeks van 16 sleutelkandidaten
* de beste is om een versleuteld bericht te ontsleutelen.
*
* Invoer:
* fn_cijfertekst[] is het pad naar het bestand met de versleutelde tekst.
* fn_taal[] is het pad naar het bestand waarmee de frequenties van de
* Nederlandse taal kunnen worden bepaald.
*
* Side effects:
* Deze functie print de best gevonden sleutels van lengtes 1 t/m 16, print
* daarna de beste van die 16 sleutels en print daarna het ontsleutelde bericht.
*
* Uitvoer:
* Deze functie heeft geen uitvoer.
*/
void opdracht_kraak(const char fn_cijfertekst[], const char fn_taal[]) {
  char cijfertekst[MAX_GROOTTE + 1];
  char taal[MAX_GROOTTE + 1];
  char beste_sleutel[MAX_SLEUTEL + 1];
  char bericht[MAX_GROOTTE + 1];
  char sleutel[MAX_SLEUTEL + 1] = "aaaaaaaaaaaaaaaa";
  double frequenties_taal[26] = {0};
  double hoogste_score = 0;

  lees_en_versimpel(fn_taal, taal);
  lees_en_versimpel(fn_cijfertekst, cijfertekst);

  bepaal_frequenties(taal, frequenties_taal);

  for (int i = 1; i <= 16; i++) {
    double score = zoek_sleutel(
      cijfertekst,
      sleutel,
      (long unsigned int)i,
      frequenties_taal
    );

    printf("Sleutel %s: score=%.3lf\n", sleutel, score);

    score = (ceil(score * 1000) / 1000);
    if (score > hoogste_score) {
      hoogste_score = score;
      strcpy(beste_sleutel, sleutel);
    }
  }

  printf("sleutel: %s\n", beste_sleutel);

  ontsleutel(cijfertekst, bericht, beste_sleutel);
  printf("bericht: %s\n", bericht);
}

/* Side effect:
   Druk gebruiksinformatie af en sluit het programma af met een foutcode.
*/
void gebruik(void) {
  fprintf(stderr, "gebruik: vigenere versleutel_caesar <bericht>\n"
          "     vigenere ontsleutel_caesar <bericht>\n"
          "     vigenere versleutel <bericht> <sleutel>\n"
          "     vigenere ontsleutel <cijfertekst> <sleutel>\n"
          "     vigenere frequenties <tekst>\n"
          "     vigenere vergelijk <tekst1> <tekst2>\n"
          "     vigenere kraak <cijfertekst> <voorbeeld>\n");
  exit(1);
}

/* Side effect:
   decodeer de opdracht en roep de juiste functie aan.
*/
int main(int argc, char *argv[]) {

  if (argc < 2) {
    gebruik();
  }

  const char *opdr = argv[1];
  if (strcmp(opdr, "versleutel_caesar")==0 && argc==3) {
    opdracht_versleutel_caesar(argv[2]);
  } else if (strcmp(opdr, "ontsleutel_caesar")==0 && argc==3) {
    opdracht_ontsleutel_caesar(argv[2]);
  } else if (strcmp(opdr, "versleutel")==0 && argc==4) {
    opdracht_versleutel(argv[2], argv[3]);
  } else if (strcmp(opdr, "ontsleutel")==0 && argc==4) {
    opdracht_ontsleutel(argv[2], argv[3]);
  } else if (strcmp(opdr, "frequenties")==0 && argc==3) {
    opdracht_frequenties(argv[2]);
  } else if (strcmp(opdr, "vergelijk")==0 && argc==4) {
    opdracht_vergelijk(argv[2], argv[3]);
  } else if (strcmp(opdr, "kraak")==0 && argc==4) {
    opdracht_kraak(argv[2], argv[3]);
  } else {
    gebruik();
  }
  return 0;
}
