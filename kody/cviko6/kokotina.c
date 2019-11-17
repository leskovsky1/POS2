#include <stdio.h>

/* random record description - could be anything */
struct rec
{
    int x, y, z;
};

/* writes and then reads 10 arbitrary records
   from the file "junk". */
int main()
{
    int i, j;
    FILE *f;
    struct rec r;

    /* create the file of 10 records */
    f = fopen("junk", "w");
    if (!f)
        return 1;
    for (i = 1; i <= 10; i++)
    {
        r.x = i;
        fwrite(&r, sizeof(struct rec), 1, f);
    }
    fclose(f);

    /* read the 10 records */
    f = fopen("junk", "r");
    if (!f)
        return 1;
    for (i = 1; i <= 10; i++)
    {
        fread(&r, sizeof(struct rec), 1, f);
        printf("%d\n", r.x);
    }
    fclose(f);
    printf("\n");
}