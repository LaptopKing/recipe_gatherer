#include <stdio.h>

char** read_file(int, FILE*);
void search();


int main()
{
    FILE *fPointer = fopen("filename", "r");
    char singleLine[150];
    int size = 0;
    // Determine size of file
    while (!feof(fPointer))
    {
        size++;
    }

    char datas[size][150] = read_file(size, fPointer);

    fclose(fPointer);

    return 0;
}

char** read_file(int size, FILE* fPointer)
{
    char datas[size][150];
    int i = 0;
    while (!feof(fPointer))
    {
        fgets(singleLine, 150, fPointer);
        for (int k = 0; k < 150; k++)
        {
            if (singleLine[k] != '')
            {
                break;
            }
            else
            {
                datas[i][k] = singleLine[k];
            }
        }
    }

    return datas;
}

void search(char** datas)
{
    return 0;
}

/* Check these resources
    https://stackoverflow.com/questions/17466563/two-dimensional-array-of-characters-in-c?msclkid=ac4f862fa9d611ecaf9d039223c05e54
    https://progur.com/2018/12/how-to-parse-json-in-c.html?msclkid=fae4322aa9cd11ecb3671590303251e3
    https://www.swirlzcupcakes.com/most-popular/how-do-i-read-a-text-file-in-c/#:~:text=%20How%20do%20you%20read%20a%20file%20and,functions%20to%20write%2Fread%20from%20the%20file.%20More%20?msclkid=0898e151a9ce11eca00ec5936c38d260
    
*/
