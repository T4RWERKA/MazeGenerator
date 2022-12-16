// TODO: EllersAlgorithm все еще генерирует циклы

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Cell
{
    bool left : 1;
    bool top : 1;
    bool right : 1;
    bool bottom : 1;
} Cell;

void PrintThinMaze(Cell **maze, int hight, int width)
{
    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (maze[i][j].top)
                printf(" __ ");
            else
                printf("    ");
        }
        printf("\n");
        for (int j = 0; j < width; j++)
        {
            if (maze[i][j].left)
                printf("|   ");
            else
                printf("    ");
        }
        printf("|\n");
    }

    for (int j = 0; j < width; j++)
    {
        printf(" __ ");
    }
    printf("\n");
}

void PrintThickMaze(bool **maze, int hight, int width)
{
    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (maze[i][j])
                printf("D ");
            else
                printf("  ");
        }
        printf("\n");
    }
}

int Comparator(const int *i, const int *j)
{
    return *i - *j;
}

int BinSearch(int *arr, int length, int num)
{
    int low, high, middle;
    low = 0;
    high = length - 1;
    while (low <= high)
    {
        middle = (low + high) / 2;
        if (num < arr[middle])
            high = middle - 1;
        else if (num > arr[middle])
            low = middle + 1;
        else
            return middle;
    }
    return -1;
}

int FindMissingNum(int *arr, int length)
{
    int i;
    int *arr_copy = (int *)malloc(length * sizeof(int));

    for (i = 0; i < length; i++)
    {
        arr_copy[i] = arr[i];
    }

    qsort(arr_copy, length, sizeof(int), (int(*) (const void *, const void *))Comparator);
    for (i = 0; i < length; i++)
    {
        if (BinSearch(arr_copy, length, i) == -1)
        {
            free(arr_copy);
            return i;
        }     
    }
    free(arr_copy);
    return -1;
}


Cell **EllersAlgorithm(int hight, int width)
{
    Cell **maze = (Cell **)malloc(hight * sizeof(Cell *));
    int i, j, k, pos, tmp;
    int *sets_numbers = (int *)malloc(width * sizeof(int));
    bool isolated;

    srand(time(NULL));

    for (i = 0; i < hight; i++)
    {
        maze[i] = (Cell *)calloc(width, sizeof(Cell));
    }

    for (i = 0; i < width; i++)
    {
        maze[0][i].top = true;
        sets_numbers[i] = i;
    }

    for (i = 0; i < hight; i++)
    {
        // боковые внешние стенки 
        maze[i][0].left = true;
        maze[i][width - 1].right = true;

        // удалить из множества ячейки, имеющие нижнюю стену
        if (i > 0)
        {
            for (j = 0; j < width; j++)
            {
                if (maze[i - 1][j].bottom)
                {
                    sets_numbers[j] = -1;
                    sets_numbers[j] = FindMissingNum(sets_numbers, width);
                }   
            }
        }

        // расставить левые и правые стены
        for (j = 0; j < width - 1; j++)
        {
            if ((sets_numbers[j] == sets_numbers[j + 1]) || (rand() % 2))
            {
                maze[i][j].right = true;
                maze[i][j + 1].left = true;
            }
            else
            {
                tmp = sets_numbers[j + 1];
                k = j + 1;
                while ((k < width) && (sets_numbers[k] == tmp))
                    sets_numbers[k++] = sets_numbers[j];
            }
        }

        // нижние и верхние стены
        for (j = 0; j < width; j++)
        {
            if (rand() % 2)
            {
                maze[i][j].bottom = true;
                if (i < hight - 1)
                    maze[i + 1][j].top = true;
            }
        }

        // удаление top и bottom, если мн-во изолировано
        isolated = true;
        j = 0;
        k = 0;
        for (j = 0; j < width; j++)
        {
            if (isolated && !maze[i][j].bottom)
                isolated = false;
            if ((j == width - 1) || (sets_numbers[j] != sets_numbers[j + 1]))
            {
                if (isolated)
                {
                    pos = rand() % (j + 1 - k) + k;
                    maze[i][pos].bottom = false;
                    if (i < hight - 1)
                        maze[i + 1][pos].top = false;
                }
                k = j + 1;
                isolated = true;
            }
        }
    }

    // последняя строка
    for (i = 0; i < width - 1; i++)
    {
        maze[hight - 1][i].bottom = true;
        if (sets_numbers[i + 1] != sets_numbers[0])
        {
            tmp = sets_numbers[i + 1];
            k = i + 1;
            while ((k < width) && (sets_numbers[k] == tmp))
                sets_numbers[k++] = sets_numbers[0];
            maze[hight - 1][i].right = false;
            maze[hight - 1][i + 1].left = false;
        }
    }
    maze[hight - 1][width - 1].bottom = true;

    free(sets_numbers);
    return maze;
}

bool **ThickToThinWalls(Cell **thin_maze, int hight, int width)
{
    bool **thick_maze = (bool **)calloc(2 * hight + 1, sizeof(bool *));
    int i, j;

    for (i = 0; i < 2 * hight + 1; i++)
    {
        thick_maze[i] = (bool *)calloc(2 * width + 1, sizeof(bool));
        thick_maze[i][0] = true;
    }
    for (i = 1; i < 2 * width + 1; i++)
    {
        thick_maze[0][i] = true;
    }

    for (i = 0; i < hight; i++)
    {
        for (j = 0; j < width; j++)
        {
            thick_maze[2 * i + 2][2 * j + 2] = true;
            if (thin_maze[i][j].bottom)
                thick_maze[2 * i + 2][2 * j + 1] = true;
            if (thin_maze[i][j].right)
                thick_maze[2 * i + 1][2 * j + 2] = true;
        }
    }
    return thick_maze;
}

int main()
{
    int hight = 20, width = 20;
    Cell **thin_maze = EllersAlgorithm(hight, width);
    

    PrintThinMaze(thin_maze, hight, width);

    bool **thick_maze = ThickToThinWalls(thin_maze, hight, width);

    PrintThickMaze(thick_maze, 2 * hight + 1, 2 * width + 1);

    return 0;
}