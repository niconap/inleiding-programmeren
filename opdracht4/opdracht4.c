/*
* Name : Nico Nap
* UvAnetID : 14259338
* Study : BSc Informatica
*
* opdracht4.c:
* - This is a program that can help with DNA analysis by finding similarities
*   and/or differences between DNA sequences.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DB_SIZE 100
#define MAX_INPUT_SIZE 100

char **DATABASE;
int DB_SIZE;

/*
* This function cleans up the memory used by the Levenshtein algorithm to
* prevent memory leaks.
*
* Input:
* **grid is the pointer towards the generated grid by the algorithm.
* rows is the amount of rows in the grid.
*
* Side effects:
* Frees the memory used by the grid.
*
* Output:
* This function does not have any output.
*/
void clean_levenshtein(int **grid, int rows) {
  for (int i = 0; i <= rows; i++) {
    free(grid[i]);
  }
  free(grid);
}

/*
* This function finds the smallest of two integers.
*
* Input:
* num1 and num2 are the integers that have to be compared.
*
* Output:
* This function returns the smallest integer.
*/
int min(int num1, int num2) {
  if (num1 > num2) {
    return num2;
  } else {
    return num1;
  }
}

/*
* This function is the Levenshtein algorithm. This algorithm
* calculates how many steps need to be taken to convert one string into another
* by using three basic operations: inserting, deleting or replacing a character.
* This is done by creating a grid and looking if the letters match. If they do,
* you copy the value that is diagonally left from the current spot to the
* current spot. If they do not match, you look at the values above, diagonally
* left from and left of the current spot, take the minimum of those three
* numbers, add 1, because that means an extra step needs to be taken, and put
* it in the current spot. Eventually you will end up with the amount of steps
* that need to be taken to convert one string into another in the bottom right
* cell of the grid.
* Source: https://www.youtube.com/watch?v=MiqoA-yF-0M
*
* Input:
* string_1[] is one of the strings that will be used in the algorithm.
* string_2 is a pointer to the second string that will be used in the algorithm.
*
* Output:
* This function returns the grid that holds all of the calculated values.
*/
int **levenshtein(char string_1[], char *string_2) {
  int length_1 = (int)strlen(string_1);
  int length_2 = (int)strlen(string_2);
  int **grid = malloc((size_t) (length_1 + 1) * sizeof(int **));

  for (int i = 0; i < (int)strlen(string_1); i++) {
    if (string_1[i] >= 'A' && string_1[i] <= 'Z') {
      string_1[i] = (char)(string_1[i] + 32);
    }
  }

  for (int i = 0; i < (int)strlen(string_2); i++) {
    if (string_2[i] >= 'A' && string_2[i] <= 'Z') {
      string_2[i] = (char)(string_2[i] + 32);
    }
  }

  for (int i = 0; i <= length_1; i++) {
    grid[i] = malloc((size_t) (length_2 + 1) * sizeof(int));
  }

  for (int z = 0; z <= length_1; z++) {
    grid[z][0] = z;
  }

  for (int a = 0; a <= length_2; a++) {
    grid[0][a] = a;
  }

  for (int i = 1; i <= length_1; i++) {
    for (int j = 1; j <= length_2; j++) {
      if (string_1[i - 1] == string_2[j - 1]) {
        // Characters are the same, so copy the value that is
        // diagonally left from the current position.
        grid[i][j] = grid[i - 1][j - 1];
      } else {
        // Take the values around the current position
        // and find the minimum value.
        int top = grid[i - 1][j];
        int diagonal = grid[i - 1][j - 1];
        int left = grid[i][j - 1];
        int min_temp = min(top, diagonal);
        int minimum = min(min_temp, left);
        grid[i][j] = minimum + 1;
      }
    }
  }

  return grid;
}

/*
* This function directly compares two given strings with the Levenshtein
* algorithm.
*
* Input:
* *string_1 and *string_2 are the pointers for the strings that have to
* be compared.
*
* Side effects:
* This function prints the Levenshtein grid and the distance between the
* two strings.
*
* Output:
* This function does not have any output.
*/
void compare(char *string_1, char *string_2) {
  int length_1 = (int)strlen(string_1);
  int length_2 = (int)strlen(string_2);
  int **grid = levenshtein(string_1, string_2);

  for (int rows = 0; rows <= length_1; rows++) {
    for (int cs = 0; cs <= length_2; cs++) {
      printf("%i ", grid[rows][cs]);
    }
    printf("\n");
  }

  printf("Difference = %d\n", grid[length_1][length_2]);
  clean_levenshtein(grid, (int)strlen(string_1));
}

/*
* This function tries to find a given string inside of the database. If that
* string does not exist in the database, the closest strings will be given
* instead with their corresponding Levenshtein differences.
*
* Input:
* str is a pointer to the string entered by the user.
*
* Side effects:
* This function prints a message or the closest strings to the submitted string
* in the console.
*
* Output:
* This function does not have any output.
*/
void retrieve(char *str) {
  char *db_copy[DB_SIZE];
  int diff[DB_SIZE];

  if (DB_SIZE == 0) {
    printf("No match found; database is empty\n");
    return;
  }

  for (int i = 0; i < DB_SIZE; i++) {
    int **grid = levenshtein(str, DATABASE[i]);
    diff[i] = grid[strlen(str)][strlen(DATABASE[i])];
    clean_levenshtein(grid, (int)strlen(str));
    if (diff[i] == 0) {
      printf("Perfect match found: \"%s\" is in database\n", str);
      return;
    }
  }

  for (int i = 0; i < DB_SIZE; i++) {
    db_copy[i] = DATABASE[i];
  }

  /*
  * This loop is the gnome sort algorithm. Gnome sort looks at a
  * value in an array and checks if it is bigger than the value to the left,
  * if it is, nothing happens and it goes to the next value, if it is not,
  * it will swap the values and move one to the left. Once it is at the end of
  * the array it will stop.
  * Source: https://www.youtube.com/watch?v=pMjAllOR3eY
  */
  for (int i = 0; i < DB_SIZE; i++) {
    for (int j = i; j > 0; j--) {
      if (diff[j] < diff[j - 1]) {
        int temp1 = diff[j - 1];
        diff[j - 1] = diff[j];
        diff[j] = temp1;

        char *temp2 = db_copy[j - 1];
        db_copy[j - 1] = db_copy[j];
        db_copy[j] = temp2;
      }
    }
  }

  if (strcmp(str, db_copy[0]) == 1) {
    printf("No perfect match found; \"%s\" is not in database\n", str);
    printf("Best matches:\nDistance\tString\n");
    for (int i = 0; i < min(3, DB_SIZE); i++) {
      printf("%d\t\t%s\n", diff[i], db_copy[i]);
    }
  }
}

/*
* This function tries to find the string that has to be removed in the database
* and removes it if it exists in the database.
*
* Input:
* *str is the pointer towards the submitted string
*
* Side effects:
* This function modifies the DATABASE array.
*
* Output:
* This function does not have any output.
*/
void delete(char *str) {
  int location;

  if(DB_SIZE == 0) {
    printf("Nothing to remove; database is empty\n");
    return;
  }

  // Add 32 to uppercase letters to turn them into lowercase letters.
  for (int i = 0; i < (int)strlen(str); i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] = (char)(str[i] + 32);
    }
  }

  for (int i = 0; i < DB_SIZE; i++) {
    if (strcmp(str, DATABASE[i]) == 0) {
      location = i;
      break;
    }

    if (strcmp(str, DATABASE[i]) != 0) {
      location = -1;
    }
  }

  if (location == -1) {
      printf("\"%s\" not removed; not in database\n", str);
  } else {
    free(DATABASE[location]);

    // Move everything back one spot to remove the gap in the database.
    for (int i = location; i < DB_SIZE; i++) {
      DATABASE[i] = DATABASE[i + 1];
    }

    DB_SIZE--;
    printf("\"%s\" removed from database\n", str);
  }
}

/*
* This function adds a submitted string to the database.
*
* Input:
* *str is a pointer towards the submitted string.
*
* Side effects:
* This function modifies the DATABASE array.
*
* Output:
* This function does not have any output.
*/
void add(char *str) {
  if (str == NULL) {
    printf("Please enter a valid command\n");
  } else {
    if (DB_SIZE >= 100) {
      printf("%s not added; database is full\n", str);
      return;
    }

    // Add 32 to uppercase letters to turn them into lowercase letters.
    for (int i = 0; i < (int)strlen(str); i++) {
      if (str[i] >= 'A' && str[i] <= 'Z') {
        str[i] = (char)(str[i] + 32);
      }
    }

    for (int i = 0; i < DB_SIZE; i++) {
      if (strcmp(str, DATABASE[i]) == 0) {
        printf("\"%s\" not added; already in database\n", str);
        return;
      }
    }

    char *var = malloc(strlen(str)*sizeof(char) + 1);
    strcpy(var, str);
    DATABASE[DB_SIZE++] = var;
    printf("\"%s\" added to database\n", str);
  }
}

/*
* This function reads a file and adds the contents to the database.
*
* Input:
* *filename is the pointer to the submitted filename.
*
* Side effects:
* This function modifies the DATABASE array.
*
* Output:
* This function does not have any output.
*/
void read(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("\"%s\" not found\n", filename);
  } else {
    char line[MAX_INPUT_SIZE];

    while(fscanf(file, "%s", line) != EOF) {
      add(line);
    }

    fclose(file);
  }
}

/*
* This function lists all the contents of the database.
*
* Input:
* This function does not have any input.
*
* Side effects:
* This function prints a list of all the contents of the database 
* in the console.
*
* Output:
* This function does not have any output.
*/
void list(void) {
  if (DB_SIZE == 0) {
    printf("Database is empty\n");
  }

  for (int i = 0; i < DB_SIZE; i++) {
    printf("%s\n", DATABASE[i]);
  }
}

/*
* This function lists all the possible commands.
*
* Input:
* This function does not have any input.
*
* Side effects:
* This function prints a list of commands in the console.
*
* Output:
* This function does not have any output.
*/
void print_commands(void) {
  printf("LIST OF COMMANDS\n");
  printf("  add ...          add to database\n");
  printf("  compare ... ...  compare two strings\n");
  printf("  help             print this text\n");
  printf("  list             print database\n");
  printf("  read ...         read from file and add to database\n");
  printf("  remove ...       remove from database\n");
  printf("  retrieve ...     find in database\n");
  printf("  size             print number of items in database\n");
  printf("  quit             stop\n");
}

/*
* This function cleans up the database.
*
* Input:
* This function does not have any input.
*
* Side effects:
* Frees the allocated memory for the DATABASE array.
*
* Output:
* This function does not have any output.
*/
void cleanup_db(void) {
  for (int i=0;i<DB_SIZE;i++) {
    free(DATABASE[i]);
    free(DATABASE);
  }
}

/*
* This function figures out which command the user used in the console.
*
* Input:
* *action is a pointer to the command the user submitted.
* *string_1 is a pointer to the first parameter after the command.
* *string_2 is a pointer to the second parameter after the command.
*
* Side effects:
* This function calls the function that corresponds with the submitted command.
*
* Output:
* This function does not have any output.
*/
void command(char *action, char *string_1, char string_2[]) {
  if (strcmp(action, "quit") == 0) {
    exit(1);
  } else if (strcmp(action, "help") == 0) {
    print_commands();
  } else if (strcmp(action, "list") == 0) {
    list();
  } else if (strcmp(action, "size") == 0) {
    printf ("%d items in database\n", DB_SIZE);
  } else if(strcmp(action, "read") == 0){
    if(string_1 == NULL) {
      printf("Please enter a valid command\n");
      return;
    } else if (string_2 != NULL) {
      printf("Please enter a valid command\n");
      return;
    } else {
      read(string_1);
    }
  } else if (strcmp(action, "add") == 0) {
    if (string_2 != NULL) {
      printf("Please enter a valid command\n");
      return;
    }
    add(string_1);
  } else if(strcmp(action, "remove") == 0){
    if (string_2 != NULL || string_1 == NULL) {
      printf("Please enter a valid command\n");
      return;
    }
    delete(string_1);
  } else if (strcmp(action, "retrieve") == 0) {
    if (string_2 != NULL || string_1 == NULL) {
      printf("Please enter a valid command\n");
      return;
    }
    retrieve(string_1);
  } else if (strcmp(action, "compare") == 0) {
    if (string_2 == NULL || string_1 == NULL) {
      printf("Please enter a valid command\n");
      return;
    }
    compare(string_1, string_2);
  } else {
    printf("Please enter a valid command\n");
  }
}

/*
* This function initializes the database and retrieves the user input.
*
* Input:
* This function does not have any input.
*
* Side effects:
* Allocates memory for the DATABASE array, asks the user for input
* and calls the command function after the input is received.
*
* Output:
* This function does not have any output.
*/
int main(void) {
  printf("Welcome to DNA Matcher v0.2\n");
  DATABASE = malloc(MAX_DB_SIZE * sizeof(char *));

  while(1) {
    printf("console> ");
    char input[MAX_INPUT_SIZE];

    if(fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
      printf("invalid input\n");
      return -1;
    };

    char *action = strtok(input, " \n");
    char *string_1 = strtok(NULL, " \n");
    char *string_2 = strtok(NULL, " \n");

    if (!action || strtok(NULL, " \n")) {
      printf("please enter a valid command\n");
    } else {
      command(action, string_1, string_2);
    }
  }

  return 0;
}
