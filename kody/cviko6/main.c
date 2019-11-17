/*
Vytvorte jednoduchý program pre evidenciu kníh:
 kniha je charakterizovaná názvom, rokom vydania a autorom;
 autor má meno, priezvisko a dátum narodenia;
 zoznam kníh a zoznam autorov sú uložené v dostatočne veľkých poliach.
 Program umožní:
 pridať nového autora do evidencie (len ak je v evidencii voľné miesto);
 pridať novú knihu do evidencie (len ak je v evidencii voľné miesto);
 odstrániť autora a všetky jeho knihy z evidencie;
 vypísať zoznam všetkých kníh;
 vypísať zoznam kníh, ktorých autori sa narodili v danom dátumovom
intervale (napr. 1.1.1800 – 10.2.1850);
 načítať evidenciu z binárneho súboru;
 uložiť evidenciu do binárneho súboru;
 importovať dáta do evidencie z textového súboru, ktorého ukážka je na
moodli.
 Pri implementácii programu sa snažte vyhnúť duplicitám, t. j.
štruktúra predstavujúca konkrétneho autora by sa mala
v programe vyskytovať iba jedenkrát aj v prípade, že v evidencii je
viac kníh od daného autora.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define STR_SIZE 256
#define ARRAY_SIZE 256

typedef struct date
{
    short day;
    short month;
    short year;
} DATE;

typedef struct author
{
    char name[STR_SIZE];
    char surname[STR_SIZE];
    DATE birthDate;
} AUTHOR;

typedef struct book
{
    char title[STR_SIZE];
    short year;
    AUTHOR *author;
} BOOK;

typedef struct evidence
{
    int authorSize;
    int bookSize;
    AUTHOR authors[ARRAY_SIZE];
    BOOK books[ARRAY_SIZE];
} EVIDENCE;

EVIDENCE initEvidence()
{
    EVIDENCE temEvidence;
    temEvidence.authorSize = 0;
    temEvidence.bookSize = 0;
    return temEvidence;
}

DATE initDate(int day, int month, int year)
{
    DATE temDate;
    temDate.day = day;
    temDate.month = month;
    temDate.year = year;
    return temDate;
}

char *dateToString(DATE *date)
{
    char *result = malloc(STR_SIZE);
    char *day = malloc(50);
    char *month = malloc(50);
    char *year = malloc(50);

    sprintf(day, "%d", date->day);
    sprintf(month, "%d", date->month);
    sprintf(year, "%d", date->year);

    strcpy(result, day);
    strcat(result, "/");
    strcat(result, month);
    strcat(result, "/");
    strcat(result, year);
    return result;
}

char *authorToString(AUTHOR *author)
{
    char *result = malloc(STR_SIZE);
    strcpy(result, author->name);
    strcat(result, " ");
    strcat(result, author->surname);
    strcat(result, " ");
    strcat(result, dateToString(&author->birthDate));
    return result;
}

char *bookToString(BOOK *book)
{
    char *result = malloc(STR_SIZE);
    char *year = malloc(50);
    sprintf(year, "%d", book->year);

    strcpy(result, book->title);
    strcat(result, " ");
    strcat(result, authorToString(book->author));
    strcat(result, " ");
    strcat(result, year);
    return result;
}

bool areDateEqual(const DATE date1, const DATE date2)
{

    return date1.day == date2.day &&
           date1.month == date2.month &&
           date1.year == date2.year;
}

int dateCmp(DATE *d1, DATE *d2)
{
    if (d1->year < d2->year)
        return -1;

    else if (d1->year > d2->year)
        return 1;

    if (d1->year == d2->year)
    {
        if (d1->month < d2->month)
            return -1;
        else if (d1->month > d2->month)
            return 1;
        else if (d1->day < d2->day)
            return -1;
        else if (d1->day > d2->day)
            return 1;
        else
            return 0;
    }
}

bool areAuthorsEqual(const AUTHOR *author1, const AUTHOR *author2)
{
    return strcmp(author1->name, author2->name) == 0 &&
           strcmp(author1->surname, author2->surname) == 0 &&
           areDateEqual(author1->birthDate, author2->birthDate);
}

bool areBookEqual(const BOOK *book1, const BOOK *book2)
{
    return (strcmp(book1->title, book2->title) == 0) &&
           book1->year == book2->year &&
           areAuthorsEqual(book1->author, book2->author);
}

AUTHOR *addAuthor(EVIDENCE *evid, DATE *date, char name[], char surname[])
{
    printf("### Pridavanie autora %s %s s datumom narodenia %s\n", name, surname, dateToString(date));

    AUTHOR temAuthor;
    strcpy(temAuthor.name, name);
    strcpy(temAuthor.surname, surname);
    temAuthor.birthDate = *date;

    for (int i = 0; i < evid->authorSize; i++)
    {
        if (areAuthorsEqual(&evid->authors[i], &temAuthor))
        {
            printf("### Autor %s %s s datumom narodenia %s uz existuje\n\n", name, surname, dateToString(date));
            return &evid->authors[i];
        }
    }
    evid->authors[evid->authorSize] = temAuthor;
    evid->authorSize++;
    printf("### Autor %s %s s datumom narodenia %s bol pridany \n\n", name, surname, dateToString(date));
    return &evid->authors[evid->authorSize - 1];
}

BOOK *addBook(EVIDENCE *evid, char title[], AUTHOR *author, short year)
{
    printf("### Pridavanie knihy %s s rokom vidania %d od autora %s %s s datumom narodenia %s\n", title, year, author->name, author->surname, dateToString(&author->birthDate));
    BOOK temBook;
    strcpy(temBook.title, title);
    temBook.author = author;
    temBook.year = year;

    for (int i = 0; i < evid->bookSize; i++)
    {
        if (areBookEqual(&evid->books[i], &temBook))
        {
            printf("### Kniha %s s rokom vidania %d od autora %s %s s datumom narodenia %s uz existuje '\n", title, year, author->name, author->surname, dateToString(&author->birthDate));
            return &evid->books[i];
        }
    }
    evid->books[evid->bookSize] = temBook;
    evid->bookSize++;
    printf("### Kniha %s s rokom vidania %d od autora %s %s s datumom narodenia %s bola pridana \n", title, year, author->name, author->surname, dateToString(&author->birthDate));
    return &evid->books[evid->bookSize];
}

void printEvidence(EVIDENCE *evidence)
{
    for (int i = 0; i < evidence->authorSize; i++)
    {
        printf("%s\n", authorToString(&evidence->authors[i]));
    }
    printf("\n");

    for (int i = 0; i < evidence->bookSize; i++)
    {
        printf("%s\n", bookToString(&evidence->books[i]));
    }
}
void splitConfigFileLine(char line[], char *title, int *bookYear, char *authorSurname, char *authorName, int *authorYear, int *authorMonth, int *authorDay)
{
    strcpy(title, strsep(&line, ";"));
    *bookYear = atoi(strsep(&line, ";"));
    strcpy(authorSurname, strsep(&line, ";"));
    strcpy(authorName, strsep(&line, ";"));

    *authorYear = atoi(strsep(&line, "/"));
    *authorMonth = atoi(strsep(&line, "/"));
    *authorDay = atoi(strsep(&line, "/"));
}

void removeEnter(char text[])
{
    text[strlen(text) - 1] = '\0';
}
void readConfigFile(char fileName[], EVIDENCE *evidence)
{
    printf("##### Citanie zo suboru %s zapocate \n", fileName);
    FILE *file;
    file = fopen(fileName, "r");
    char *line = NULL;
    size_t len = 0;
    size_t read;
    while ((read = getline(&line, &len, file)) != -1)
    {
        char title[STR_SIZE];
        int year;
        char surname[STR_SIZE];
        char name[STR_SIZE];
        int authorDay;
        int authorMonth;
        int authorYear;

        removeEnter(line);
        printf("\nNacitany riadok  -->%s<--\n", line);

        splitConfigFileLine(line, title, &year, surname, name, &authorYear, &authorMonth, &authorDay);

        printf("\t* Title \t-->%s<--\n", title);
        printf("\t* Year \t\t-->%d<--\n", year);
        printf("\t* Surname \t-->%s<--\n", surname);
        printf("\t* Name \t\t-->%s<--\n", name);
        printf("\t* AuthorDay \t-->%d<--\n", authorDay);
        printf("\t* authorMonth \t-->%d<--\n", authorMonth);
        printf("\t* authorYear \t-->%d<--\n", authorYear);

        DATE date = initDate(authorDay, authorMonth, authorYear);
        AUTHOR *author = addAuthor(evidence, &date, name, surname);
        BOOK *book = addBook(evidence, title, author, year);
        //printEvidence(evidence);
    }
    printf("##### Citanie zo suboru %s ukoncene  \n\n", fileName);
}

void writeoBinary(char fileName[], EVIDENCE *evidence)
{
    FILE *file = fopen(fileName, "w");
    fwrite(&evidence, sizeof(evidence), 1, file);
    fclose(file);
}

void readFromBinary(char fileName[], EVIDENCE *evidence)
{
    EVIDENCE *kokot;
    FILE *file = fopen(fileName, "r");
    fread(&kokot, sizeof(EVIDENCE), 1, file);
    *evidence = *kokot;
    fclose(file);
}

void printBooksFromTo(EVIDENCE *evidence, DATE *dateFrom, DATE *dateTo)
{
    for (int i = 0; i < evidence->bookSize; i++)
    {
        int com1 = dateCmp(&evidence->books[i].author->birthDate, dateFrom);
        int com2 = dateCmp(&evidence->books[i].author->birthDate, dateTo);
        // printf("%d\n", com1);
        // printf("%d\n", com2);

        if ((com1 == 1 || com1 == 0) && (com2 == -1 || com2 == 0))
        {
            printf("%s\n", bookToString(&evidence->books[i]));
        }
    }
}

bool removeBook(EVIDENCE *evidence, int id)
{
    for (int i = id; i < evidence->bookSize; i++)
    {
        evidence->books[i] = evidence->books[i + 1];
    }
    evidence->bookSize--;
}
bool removeAuthor(EVIDENCE *evidence, int id)
{
    AUTHOR *author = &evidence->authors[id];
    for (int i = 0; i < evidence->bookSize; i++)
    {
        if (areAuthorsEqual(evidence->books[i].author, author))
        {
            removeBook(evidence, i);
            i--;
        }
    }
}

int main()
{
    EVIDENCE evidence = initEvidence();
    EVIDENCE evidence2;
    readConfigFile("dataSource.txt", &evidence);
    writeoBinary("binFile.bin", &evidence);
    readFromBinary("binFile.bin", &evidence2);
    printf("\n\n\n");
    printEvidence(&evidence);
    printf("\n\n\n");
    printEvidence(&evidence2);

    printf("\n\n\n");
    printf("\n\n\n");

    DATE d1;
    d1.day = 1;
    d1.month = 1;
    d1.year = 1802;

    DATE d2;
    d2.day = 31;
    d2.month = 12;
    d2.year = 1829;
    //printBooksFromTo(&evidence, &d1, &d2);
    printf("\n\n\n");
    removeAuthor(&evidence, 1);
    //removeBook(&evidence, 1);
    printEvidence(&evidence);
}
