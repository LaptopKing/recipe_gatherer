#include <stdio.h>

#define BUFFER_SIZE 1186334
#define STRING_LENGTH 400

void parse_data(char data[], char parsed_data[][STRING_LENGTH], int i, int k, int rows);

int main()
{
    FILE *fp;
    char data[BUFFER_SIZE];
    int rows = 0;

    fp = fopen("nosalty.json", "r");
    fgets(data, BUFFER_SIZE, fp);

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (data[i] == ';')
        {
            rows++;
        }
    }
    char parsed_data[rows][STRING_LENGTH];
    int i = 0;
    int k = 0;

    parse_data(data, parsed_data, i, k, rows);

    /*
    for (int i = 0; i < rows; i++)
    {
        for (int k = 0; k < STRING_LENGTH; k++)
        {
            printf("%c", parsed_data[i][k]);
        }
        printf("\n");
    }
    */
    
    return 0;
}

void parse_data(char data[], char parsed_data[][STRING_LENGTH], int i, int k, int rows)
{
    char single_line[STRING_LENGTH];

    
    while (data[i] != ';')
    {
        parsed_data[k][i] = data[i];
        // printf("%c", data[i]);
        i++;
    }
    // printf ("\n");
    i++;
    k++;
    if (k != rows)
    {
        parse_data(data, parsed_data, i, k, rows);
    }
    else
    {
        return 0;
        // printf ("Data Parsed!\n");
    }
}

/* Check these resources
    https://stackoverflow.com/questions/17466563/two-dimensional-array-of-characters-in-c?msclkid=ac4f862fa9d611ecaf9d039223c05e54
    https://progur.com/2018/12/how-to-parse-json-in-c.html?msclkid=fae4322aa9cd11ecb3671590303251e3
    https://www.swirlzcupcakes.com/most-popular/how-do-i-read-a-text-file-in-c/#:~:text=%20How%20do%20you%20read%20a%20file%20and,functions%20to%20write%2Fread%20from%20the%20file.%20More%20?msclkid=0898e151a9ce11eca00ec5936c38d260
    
*/
