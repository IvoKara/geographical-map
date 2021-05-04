#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cell_t {
    float height;
    int type; // 0-plant, 1-desert, 2-water, 3-road, 4-building
    float attendance;
};
typedef struct cell_t cell;

struct terrain_t {
    int widthMeter;
    int heightMeter;
    cell **cells;
};
typedef struct terrain_t terrain;

struct terr_list_t {
    int count;
    terrain *terrains;
};
typedef struct terr_list_t terr_list;

void print_terrain(terrain ter)
{
    printf("%dx%d\n", ter.widthMeter, ter.heightMeter);
    for(int i = 0; i < ter.heightMeter; i++)
    {
        for(int j = 0; j < ter.widthMeter; j++)
        {
            printf("%dh %dw: ", i, j);
            printf("%.2f ", ter.cells[i][j].height);
            printf("%d ", ter.cells[i][j].type);
            printf("%.2f\n", ter.cells[i][j].attendance);
        }
    }
}

void print_terr_type(terrain terr)
{
    printf("Map of types:\n");
    for(int i = 0; i < terr.heightMeter; i++)
    {
        for(int j = 0; j < terr.widthMeter; j++)
            printf(" %d", terr.cells[i][j].type);
        printf("\n");
    }
}

void print_terr_height(terrain terr)
{
    float longest = 0;
    for(int i = 0; i < terr.heightMeter; i++)
        for(int j = 0; j < terr.widthMeter; j++)
            if(longest < terr.cells[i][j].height)
                longest = terr.cells[i][j].height;
                
    int len = 0;
    while((int)longest > 0)
    {
        longest = longest / 10;
        len++;
    }

    printf("Map of heights:\n");
    for(int i = 0; i < terr.heightMeter; i++)
    {
        for(int j = 0; j < terr.widthMeter; j++)
            printf("%*.2f ", len+4, terr.cells[i][j].height);
        printf("\n");
    }
}

void input_str(char *str)
{
    char c = 0;
    int count = 0;
    char *temp = NULL;
    while(1)
    {
        c = getchar();
        temp = (char*) realloc(str, ++count*sizeof(char));
        str = temp;
        if(c == '\n')
        {
            str[count-1] = '\0';
            break;
        }
        str[count-1] = c;
    }
}

void init_terrain(terrain *ter)
{
    int *width = &(ter->widthMeter);
    int *height = &(ter->heightMeter);

    printf("New Terrain width in meters: ");
    scanf("%d", width);

    printf("New Terrain height in meters: ");
    scanf("%d", height);

    cell **temp = (cell**) malloc((*height)*sizeof(cell*));
    for(int i = 0; i < *height; i++)
        temp[i] = (cell*) malloc((*width)*sizeof(cell));
        
    for(int i = 0; i < *height; i++)
    {
        for(int j = 0; j < *width; j++)
        {
            printf("Coordinates %dh %dw data\n", i, j);
            
            printf("cell height - ");
            scanf("%f", &(temp[i][j].height));

            printf("cell type - ");
            scanf("%d", &(temp[i][j].type));

            printf("cell attendance for one day - ");
            scanf("%f", &(temp[i][j].attendance));
        }
    }
    
    ter->cells = temp;
}

void free_items(terrain *terr)
{
    cell **cells = terr->cells;
    for(int i = 0; i < terr->heightMeter; i++)
        free(cells[i]);
    free(cells);
}

void write_file(char *filename, terrain *ter)
{
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL)
    {
        perror("Cannot open file");
        exit(1);
    }

    int *width = &(ter->widthMeter);
    int *height = &(ter->heightMeter);
    cell **temp = ter->cells;

    if(fwrite(width, sizeof(int), 1, fp) != 1)
    {
        perror("Cannot write to file");
        exit(2);
    }
    if(fwrite(height, sizeof(int), 1, fp) != 1)
    {
        perror("Cannot write to file");
        exit(2);
    }

    for(int i = 0; i < *height; i++)
    {
        for(int j = 0; j < *width; j++)
        {
            if(fwrite(&temp[i][j], sizeof(cell), 1, fp) != 1)
            {
                perror("Cannot write to file");
                exit(2);
            }
        }
    }

    fclose(fp);
}

terrain *read_file(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        perror("Cannot open file");
        exit(1);
    }

    int width = 0;
    int height = 0;
    terrain *terr = (terrain*) malloc(sizeof(terrain));

    if(fread(&width, sizeof(int), 1, fp) != 1)
    {   
        perror("Cannot read from file");
        exit(2);
    }   
    if(fread(&height, sizeof(int), 1, fp) != 1)
    {
        perror("Cannot read from file");
        exit(2);
    }

    cell **temp = (cell**) malloc(height*sizeof(cell*));
    for(int i = 0; i < height; i++)
        temp[i] = (cell*) malloc(width*sizeof(cell));

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(fread(&temp[i][j], sizeof(cell), 1, fp) != 1)
            {
                perror("Cannot read from file");
                exit(2);
            }
        }
    }

    terr->cells = temp;
    terr->heightMeter = height;
    terr->widthMeter = width;

    fclose(fp);

    return terr;
}

void ask_proceed()
{
    int c;
    printf("Terrain data empty\n");
    while(1)
    {
        printf("Insert new data? y/n(exit program) ");
        c = getchar();
        if(c == 'y')
            break;
        if(c == 'n')
            exit(0);
    }
}

void new_terrain()
{
    terrain terr;
    init_terrain(&terr);
    getchar();
    write_file("terrain.dat", &terr);
    free_items(&terr);
}

int main()
{
    // choose option of the terrain 
    FILE *dat = fopen("terrain.dat", "rb");
    if(dat != NULL)
    {
        fseek(dat , 0, SEEK_END);
        if(ftell(dat) == 0)
        {
            fclose(dat);
            ask_proceed();
            new_terrain();
        }
    }
    else
    {
        ask_proceed();
        new_terrain();
    }


    char c = 0;
    terrain *terr = read_file("terrain.dat");
    while(1)
    {
        printf("+------------------------------------+\n");
        printf("| Choose options:                    |\n");
        printf("|  1 - View terrain as terrain types |\n");
        printf("|  2 - View terrain as heights       |\n");
        printf("|  3 - Enter new terrain             |\n");
        printf("|  4 - Exit                          |\n");
        printf("+------------------------------------+\n");
        printf("   |\n");
        printf("   +-> ");
        c = getchar();
        getchar();
        switch(c)
        {
            case '1':
                printf("\n");
                print_terr_type(*terr);
                printf("\n");
                break;
            case '2':
                printf("\n");
                print_terr_height(*terr);
                printf("\n");
                break;
            case '3':
                new_terrain();
                terr = read_file("terrain.dat");
                break;
        }
        if(c == '4')
        {
            free_items(terr);
            free(terr);
            printf("Bye.\n");
            break;
        }

    }
    
    return 0;
}