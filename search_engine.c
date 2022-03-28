#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf ("You did not specify at least 1 or more ingredients!\nExiting program...\n");
        exit(0);
    }

    FILE *fp = fopen("ingredients_ids", "r");
    
    int buffer = 5000;
    char temp[buffer];
    int temp_max = 0;
    
    int rows = 0;
    int max_length = 0;

    // Get number of rows and the maximum length needed for the longest string
    while (fgets(temp, buffer, fp))
    {
        rows++;
        for (int i = 0; i < buffer; i++)
        {
            if (temp[i] != '\0')
            {
                // printf ("%c", temp[i]);
                temp_max++;
            }
            else
            {
                // printf ("%d\n", temp_max);
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
    
    char data[rows][max_length];
    char line[max_length];
    int i = 0;

    for (i = 0; i < rows; i++)
    {
        fgets(line, max_length, fp);
        for (int k = 0; k < max_length; k++)
        {
            if (line[k] == '\0')
            {
                data[i][k] = line[k];
            }
            else
            {
                data[i][k] = line[k];
            }
        }
        
    }

    fclose(fp);

    char ingredient_names[rows][max_length];
    int splitted_ids[rows][max_length];

    char ids[rows][max_length];
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

    int needed_ids[max_length];
    int counter = 0;
    int temp3[max_length];
    // char alphabetic_order = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'j', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    for (i = 0; i < rows; i++)
    {
        for (int k = 1; k < argc; k++)
        {
            if (strcmp(ingredient_names[i], argv[k]) == 0)
            {
                for (int j = 0; j < max_length; j++)
                {
                    if (splitted_ids[i][j] != 0)
                    {
                        if (splitted_ids[i][j] != needed_ids[counter])
                        {
                            needed_ids[counter] = splitted_ids[i][j];
                            counter++;
                        }
                    }
                }
            }
        }
    }

    int recipe_rows;
    int recipe_cols;
    int recipe_buffer = 1000;
    char recipe_temp[recipe_buffer];
    int recipe_temp_max = 0;

    FILE *fp2 = fopen("nosalty.json", "r");
    while (fgets(recipe_temp, recipe_buffer, fp2))
    {
        recipe_rows++;
        for (i = 0; i < recipe_buffer; i++)
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
    fseek(fp2, 0, SEEK_SET);

    int recipe_ids[recipe_rows];
    char recipe_names[recipe_rows][recipe_cols];
    char recipe_urls[recipe_rows][recipe_cols];
    char recipe_ingredients[recipe_rows][recipe_cols];

    for (i = 0; i < recipe_rows; i++)
    {
        fgets(recipe_temp, recipe_cols, fp2);
        int k = 0;

        // get ids
        char id[6];
        while (recipe_temp[k] != ',')
        {
            id[k] = recipe_temp[k];
            k++;
        }
        recipe_ids[i] = atoi(id);

        // get names
        char name[recipe_cols];        
        int j = 0;
        int col = 0;

        while (recipe_temp[k] != ':')
        {
            k++;
        }
        k += 2;
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
                recipe_names[i][col] = '\0';
                break;
            }
        }
        col = 0;
        j = 0;

        // get urls
        char url[recipe_cols];

        while (recipe_temp[k] != ':')
        {
            k++;
        }
        k += 2;
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
                recipe_urls[i][col] = '\0';
                break;
            }
        }
        col = 0;
        j = 0;

        // get ingredients
        char ingredient_name[recipe_cols];

        while (recipe_temp[k] != ':')
        {
            k++;
        }
        k += 2;
        while(recipe_temp[k] != ']')
        {
            ingredient_name[j] = recipe_temp[k];
            j++;
            k++;
        }
        for (int g = 0; g < recipe_cols; g++)
        {
            if (ingredient_name[g] != '\0')
            {
                recipe_ingredients[i][col] = ingredient_name[g];
                col++;
            }
            else
            {
                recipe_ingredients[i][col] = '\0';
                break;
            }
        }
    }

/*
    printf ("\n");
    for (i = 0; i < rows; i++)
    {
        printf ("\n%d. %s:\n\tRecipe Url: %s\n\tRecipe ingredients: %s\n\n", recipe_ids[i], recipe_names[i], recipe_urls[i], recipe_ingredients[i]);
    }
*/

/*
    printf ("\n");
    for (i = 1; i < argc; i++)
    {
        printf ("%s\n", argv[i]);
    }
    printf ("\n");
    for (i = 0; i < counter; i++)
    {
        if (needed_ids[i] != 0)
        {
            printf ("%d ", needed_ids[i]);
        }
    }
    printf ("\n");
*/

  
    return 0;
}