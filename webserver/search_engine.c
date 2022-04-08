#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECIPE_ROWS 10000
#define RECIPE_COLS 35000


int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf ("You did not specify at least 1 or more ingredients!\nExiting program...\n");
        exit(0);
    }
    
    FILE *fp = fopen("ingredients_ids.static", "r");
    
    char *temp;
    temp = malloc(sizeof(char)*RECIPE_COLS);
    int temp_max = 0;
    
    int rows = 0;
    int max_length = 0;

    // Get number of rows and the maximum length needed for the longest string
    while (fgets(temp, RECIPE_COLS, fp))
    {
        rows++;
        for (int i = 0; i < RECIPE_COLS; i++)
        {
            if (temp[i] != '\n' && temp[i] != '\0')
            {
                // printf ("%c", temp[i]);
                temp_max++;
            }
            else
            {
                break;
            }
        }
        if (temp_max > max_length)
        {
            max_length = temp_max + 1;
            // printf ("%d\n", max);
            temp_max = 0;
        }
        else
        {
            temp_max = 0;
        }
    }
    fseek(fp, 0, SEEK_SET);
    free(temp);

    char **data = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++)
    {
        data[i] = (char*)malloc(max_length);
    }
    
    int max_line_length = 0;
    int temp_max_line_length = 0;

    int max_ids_length = 0;
    int temp_max_ids_length = 0;

    char line[max_length];
    int i = 0;

    for (i = 0; i < rows; i++)
    {
        fgets(line, max_length, fp);
        for (int k = 0; k < max_length; k++)
        {
            if (line[k] == ';')
            {
                temp_max_ids_length++;
            }

            if (line[k] != '\0')
            {
                data[i][k] = line[k];
            }
            else
            {
                data[i][k] = line[k];
            }
        }
        temp_max_line_length = 0;
        int d = 0;
        while (line[d] != ':')
        {
            temp_max_line_length++;
            d++;
        }
        d++;

        if (max_ids_length < temp_max_ids_length + 3)
        {
            max_ids_length = temp_max_ids_length + 3;
        }

        if (max_line_length < temp_max_line_length)
        {
            max_line_length = temp_max_line_length;
        }
        temp_max_ids_length = 0;
    }

    fclose(fp);

    char **ingredient_names = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++)
    {
        ingredient_names[i] = (char*)malloc(max_line_length);
    }

    int *splitted_ids[rows];
    char **ids = malloc(rows * sizeof(char *));

    for (int i = 0; i < rows; i++)
    {
        ids[i] = (char*)malloc(max_length);
        splitted_ids[i] = (int*)malloc(max_ids_length * sizeof(int));
    }

    char separator = ':';
    int checker = 0;

    for (i = 0; i < rows; i++)
    {
        for (int k = 0; k < max_length; k++)
        {
            if (data[i][k] != '\0')
            {
                if (data[i][k] != separator && checker == 0)
                {
                    ingredient_names[i][k] = data[i][k];
                }
                else if (data[i][k] != separator && checker == 1)
                {
                    ids[i][k] = data[i][k];
                }
                else
                {
                    checker = 1;
                    ingredient_names[i][k] = '\0';
                }
            }
            else
            {
                ids[i][k] = '\0';
                break;
            }
        }
        checker = 0;
    }

    separator = ';';

    for (i = 0; i < rows; i++)
    {
        char temp2[7];
        int j = 0;
        int h = 0;
        for (int k = 0; k < max_length; k++)
        {
            if (ids[i][k] != '\0')
            {
                if (ids[i][k] != separator && ids[i][k] != '\0' && ids[i][k] != '\n')
                {
                    temp2[j] = ids[i][k];
                    j++;
                }
                else if (ids[i][k] == separator || ids[i][k] == '\0' || ids[i][k] == '\n')
                {
                    splitted_ids[i][h] = atoi(temp2);

                    for (int g = 0; g < 7; g++)
                    {
                        temp2[g] = 'a';
                    }

                    j = 0;
                    h++;
                }
            }
        }
    }

    int needed_ids[max_ids_length];
    int counter = 0;

    for (i = 0; i < rows; i++)
    {
        for (int k = 1; k < argc; k++)
        {
            if (strcmp(ingredient_names[i], argv[k]) == 0)
            {
                for (int j = 0; j < max_ids_length; j++)
                {
                    if (splitted_ids[i][j] != 0 && splitted_ids[i][j] != needed_ids[counter])
                    {
                        needed_ids[counter] = splitted_ids[i][j];
                        counter++;
                    }
                }
            }
        }
    }

    int recipe_rows;
    int recipe_cols;
    // int recipe_buffer = 1900;
    char *recipe_temp;
    recipe_temp = malloc(RECIPE_COLS * sizeof(char));    
    int recipe_temp_max = 0;

    fp = fopen("nosalty.json.static", "r");
    while (fgets(recipe_temp, RECIPE_COLS, fp))
    {
        recipe_rows++;
        for (i = 0; i < RECIPE_COLS; i++)
        {
            if (recipe_temp[i] != '\0')
            {
                recipe_temp_max++;
            }
            else
            {
                break;
            }
        }
        if (recipe_temp_max > recipe_cols)
        {
            recipe_cols = recipe_temp_max;
            recipe_temp_max = 0;
        }
        else
        {
            recipe_temp_max = 0;
        }
    }
    fseek(fp, 0, SEEK_SET);

    int recipe_ids[recipe_rows];
    char **recipe_names = malloc(recipe_rows * sizeof(char *));
    for (int i = 0; i < recipe_rows; i++)
    {
        recipe_names[i] = (char*)malloc(recipe_cols);
    }
    char **recipe_urls = malloc(recipe_rows * sizeof(char *));
    for (int i = 0; i < recipe_rows; i++)
    {
        recipe_urls[i] = (char*)malloc(recipe_cols);
    }
    char **recipe_ingredients = malloc(recipe_rows * sizeof(char *));
    for (int i = 0; i < recipe_rows; i++)
    {
        recipe_ingredients[i] = (char*)malloc(recipe_cols);
    }
    int recipe_ids_check[recipe_rows];

    for (i = 0; i < recipe_rows; i++)
    {
        fgets(recipe_temp, recipe_cols, fp);
        int k = 7;
        int j = 0;

        char id[5];
        while (recipe_temp[k] != ',')
        {
            id[j] = recipe_temp[k];
            k++;
            j++;
        }
        recipe_ids[i] = atoi(id);
        j = 0;
        // printf ("%d\n", recipe_ids[i]);

        // get names
        char name[1900];        
        
        int col = 0;

        while (recipe_temp[k] != ':')
        {
            // printf ("%c", recipe_temp[k]);
            k++;
        }
        k += 3;
        // printf ("%c", recipe_temp[k]);
        while (recipe_temp[k] != '\'')
        {
            name[j] = recipe_temp[k];
            j++;
            k++;
        }
        for (int g = 0; g < recipe_cols; g++)
        {
            if (name[g] != '\0')
            {
                recipe_names[i][col] = name[g];
                col++;
            }
            else
            {
                break;
            }
        }
        col = 0;
        j = 0;

        // get urls
        char url[1900];

        while (recipe_temp[k] != ':')
        {
            k++;
        }
        k += 3;
        while(recipe_temp[k] != '\'')
        {
            url[j] = recipe_temp[k];
            j++;
            k++;
        }
        for (int g = 0; g < recipe_cols; g++)
        {
            if (url[g] != '\0')
            {
                recipe_urls[i][col] = url[g];
                col++;
            }
            else
            {
                break;
            }
        }

        col = 0;
        j = 0;

        // get ingredients
        char ingredient_name[1900];

        while (recipe_temp[k] != ':')
        {
            k++;
        }
        k += 3;
        while(recipe_temp[k] != ']')
        {
            ingredient_name[j] = recipe_temp[k];
            j++;
            k++;
        }
        int count_quotes = 0;
        for (int g = 0; g < recipe_cols; g++)
        {
            if (ingredient_name[g] != '\0')
            {
                if (ingredient_name[g] == '\'' && count_quotes < 2)
                {
                    recipe_ingredients[i][col] = ingredient_name[g];
                    col++;
                    count_quotes++;
                }
                else if (count_quotes == 2)
                {
                    if (ingredient_name[g] == ',')
                    {
                        recipe_ingredients[i][col] = ingredient_name[g];
                        col++;
                        count_quotes = 0;
                    }
                }
                else
                {
                    recipe_ingredients[i][col] = ingredient_name[g];
                    col++;
                }
            }
            else
            {
                recipe_ingredients[i][col - 1] = '\'';
                recipe_ingredients[i][col] = '\0';
                // printf ("%s\n", recipe_ingredients[i]);
                break;
            }
        }
    }

    free(recipe_temp);

    int g = 0;
    int check = 0;

    for (i = 0; i < counter; i++)
    {
        for (int f = 0; f < counter; f++)
        {
            if (recipe_ids_check[f] == recipe_ids[needed_ids[i] - 1])
            {
                check = 1;
            }
        }
        if (check != 1)
        {
            if (needed_ids[i] != 0)
            {
                printf ("{\"name\":\"%s\", \"recipe_url\":\"%s\", \"recipe_ingredients\":[%s]}\n", recipe_names[needed_ids[i] - 1], recipe_urls[needed_ids[i] - 1], recipe_ingredients[needed_ids[i] - 1]);
                recipe_ids_check[g] = recipe_ids[needed_ids[i] - 1];
                g++;
                check = 0;
            }
        }
    }

    return 0;
}