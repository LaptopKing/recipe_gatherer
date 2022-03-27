#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fp = fopen("ingredients_ids", "r");
    
    int buffer = 1000;
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

    for (i = 0; i < rows; i++)
    {
        if (strcmp(ingredient_names[i], "tojas") == 0)
        {
            printf ("%s: ", ingredient_names[i]);
            for (int k = 0; k < max_length; k++)
            {
                if (splitted_ids[i][k] != 0)
                {
                    printf ("%d ", splitted_ids[i][k]);
                }
            }
        }
    }

  
    return 0;
}