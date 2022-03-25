#include <stdio.h>

char** parse(char *string, char, int);

int main()
{
    FILE *fp = fopen("ingredients_ids", "r");
    int rows = 0;
    int buffer = 1000;
    char temp[buffer];
    while (fgets(temp, buffer, fp))
    {
        rows++;
    }
    fseek(fp, 0, SEEK_SET);
    printf("%d\n", rows);
    char data[rows][buffer];
    for (int i = 0; i < rows; i++)
    {
        fgets(temp, buffer, fp);
        for (int k = 0; k < buffer; k++)
        {
            data[i][k] = temp[k];
        }
    }

    // printf("%s", data[0]);
    parse(data[0], ':', buffer);
    
    return 0;
}

char** parse(char *string, char chr, int size)
{
    int new_arr_length = 1;
    for (int i = 0; i < size; i++)
    {
        if (string[i] == chr)
        {
            new_arr_length++;
        }
    }
    char new_arr[new_arr_length][size];

    int j = 0;
    for (int i = 0; i < new_arr_length - 1; i++)
    {
        for (int k = 0; k < size - 1; k++)
        {
            if (string[j] != chr)
            {
                new_arr[i][k] = string[j];
                printf ("%c", string[j]);
            }
            else
            {
                printf ()
            }
            j++;
        }
        j++;
    }

    return new_arr;
}