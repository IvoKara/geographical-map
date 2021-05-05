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

int digits_in_number(int num)
{
    int len = 0;
    while((int)num > 0)
    {
        num = num / 10;
        len++;
    }
    
    return len;
}

void print_terr_type(terrain terr, int use_coord)
{
    int wdigs = digits_in_number(terr.widthMeter);
    int hdigs = digits_in_number(terr.heightMeter);
    
    if(use_coord)
    {
        int count = 0;
        int len = terr.widthMeter*2 - 1;
        char dashes[len+1];
        
        memset(dashes, '-', len);
        dashes[len] = '\0';

        printf("%*c", hdigs+2, ' ');
        for(int i = 0; i < len; i++)
        {
            if(i % 10 == 0)
            {
                printf("%-*d", wdigs, count);
                dashes[i] = '+';
                count += 5;
                i += (wdigs-1);
            }
            else
                printf(" ");
        }
        printf("  x\n");
        printf("%*c%s>\n", hdigs+2, '.', dashes);
    }

    for(int i = 0; i < terr.heightMeter; i++)
    {
        if(use_coord)
        {
            if(i % 5 == 0)
            {
                printf("%*d +",hdigs, i);
            }
            else
                printf("%*c",hdigs+2, '|');
        }
        else
            printf(" ");     

        for(int j = 0; j < terr.widthMeter; j++)
            printf("%d ", terr.cells[i][j].type);
        printf("\n");
    }

    if(use_coord)
    {
        printf("%*c\n", hdigs+2, 'v');
        printf("%*c\n", hdigs, 'y');
    }
}

void print_terr_height(terrain terr)
{
    float longest = 0;
    for(int i = 0; i < terr.heightMeter; i++)
        for(int j = 0; j < terr.widthMeter; j++)
            if(longest < terr.cells[i][j].height)
                longest = terr.cells[i][j].height;
                
    int len = digits_in_number(longest);

    for(int i = 0; i < terr.heightMeter; i++)
    {
        for(int j = 0; j < terr.widthMeter; j++)
            printf("%*.2f ", len+4, terr.cells[i][j].height);
        printf("\n");
    }
}

void init_terrain(terrain *ter)
{
    int *width = &(ter->widthMeter);
    int *height = &(ter->heightMeter);

    do
    {
        printf("New Terrain height X width in meters: ");
        scanf("%d %d", height, width);
        if(*height <= 0 || *width <= 0)
        {
            if(*height <= 0)
                printf("*height cannot be negative or zero\n");
            if(*width <= 0)
                printf("*width cannot be negative or zero\n");   
        }
        
    } while (*height <= 0 || *width <= 0);
    

    cell **temp = (cell**) malloc((*height)*sizeof(cell*));
    for(int i = 0; i < *height; i++)
        temp[i] = (cell*) malloc((*width)*sizeof(cell));
        
    for(int i = 0; i < *height; i++)
    {
        for(int j = 0; j < *width; j++)
        {
            printf("\n| Coordinates %dh %dw data\n", i, j);
            
            int err;
            do 
            {
                err = 0;
                printf("height / type / attendance per day: ");
                scanf("%f %d %f", 
                    &(temp[i][j].height),
                    &(temp[i][j].type),
                    &(temp[i][j].attendance));
                if(temp[i][j].height <= 0)
                {
                    printf("*height cannot be negative or zero\n");
                    err = 1;
                }
                if(temp[i][j].type < 0 || temp[i][j].type > 4)
                {
                    printf("*type must be between 0 and 4\n");
                    err = 1;
                }
                if(temp[i][j].attendance <= 0)
                {
                    printf("*attendance cannot be negative or zero\n");
                    err = 1;
                }
            } while(err);
            
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

int create_road(terrain *ter)
{

    int x, y;
    int befX, befY;
    int count = 0;
    int Xmax = ter->widthMeter-1;
    int Ymax = ter->heightMeter-1;

    printf("Build road:\n");
    print_terr_type(*ter, 1);
    printf("\n");
    printf("Type -1 coordinate to stop (either for \'x\' or \'y\')\n");
    do
    {
        if(count == 0)
            printf("Choose starting coordinates (x, y): ");
        else if(count != 0)
            printf("Choose next coordinates of (%d, %d): ", befX, befY);
        scanf("%d %d", &x, &y);

        if(x == -1 || y == -1)
        {
            printf("Writing road to file.\n");
            break;
        }

        if(x > Xmax || y > Ymax)
        {
            if(x > Xmax)
                printf("*\'x\' is out of range (Max %d)\n", Xmax);
            if(y > Ymax)
                printf("*\'y\' is out of range (Max %d)\n", Ymax);
            continue;
        }

        if(x < -1 || y < -1)
        {
            if(x < -1)
                printf("*\'x\' must be positive (Type \'-1\' to stop)\n");
            if(y < -1)
                printf("*\'y\' must be positive (Type \'-1\' to stop)\n");
            continue;
        }

        if(count != 0)
        {
            if(x == befX && y == befY)
            {
                printf("*You are already on that point. Road built there.\n");
                continue;
            }
            else if((x < befX-1 || x > befX+1) || (y < befY-1 || y > befY+1))
            {
                printf("*Too far away. Need to be near current point (%d, %d)\n",
                    befX, befY);
                continue;
            }
        }
        else if(count == 0)
        {
            if((x != 0 && y != 0) && (x != Xmax && y != Ymax))
            {
                printf("*Cannot start from there. Need to start from \'borders\'.\n");
                continue;
            }
        }

        if(ter->cells[y][x].type == 3)
        {
            printf("Already road here. Continue from that point\n");
            befX = x;
            befY = y;
            if(count == 0)
                count++;
            continue;
        }
        else if(ter->cells[y][x].type > 1)
        {
            printf("*Cannot build road over ");
            switch (ter->cells[y][x].type)
            {
            case 2:
                printf("water\n");
                break;
            
            case 4:
                printf("building\n");
                break;
            }
            continue;
        }

        ter->cells[y][x].type = 3;
        count++;
        befX = x;
        befY = y;
        printf("________________\nNew map (types):\n");
        print_terr_type(*ter, 1);
        printf("\n");        
    } while(x != -1 && y != -1);

    write_file("terrain.dat", ter);

    return count;
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
        printf("|  4 - Build road                    |\n");
        printf("|  5 - Biker jumps                   |\n");
        printf("|  6 - Exit                          |\n");
        printf("+------------------------------------+\n");
        printf("   |\n");
        printf("   +-> ");

        c = getchar();
        getchar();
        switch(c)
        {
            case '1':
                printf("\n");
                printf("Map of types %dx%d:\n", 
                    terr->heightMeter, terr->widthMeter);
                print_terr_type(*terr, 0    );
                printf("\n");
                break;
            case '2':
                printf("\n");
                printf("Map of heights %dx%d:\n", 
                    terr->heightMeter, terr->widthMeter);
                print_terr_height(*terr);
                printf("\n");
                break;
            case '3':
                new_terrain();
                terr = read_file("terrain.dat");
                break;
            case '4':
                /* to be added */
                printf("\n");
                create_road(terr);
                printf("\n");
                break;
            case '5':
                /* to be added */
                break;
        }
        if(c == '6')
        {
            free_items(terr);
            free(terr);
            printf("Bye.\n");
            break;
        }

    }
    
    return 0;
}