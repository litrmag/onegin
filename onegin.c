#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>

unsigned int ReadFromFile (char* filename, char* buffer);
char** RequestMemory (unsigned int read_lines);
unsigned int LineCounter (char* buffer, unsigned int read_char);
void DivisionIntoLines (char* buffer, char** index, char** index_copy, unsigned int read_char);
void Sort (char** index, unsigned int num_of_lines, int(*CompareFunc)(char** address1, char** address2));
int CompareBegin ( char** address1,  char** address2);
int CompareEnd (char** address1, char** address2);
void ChangeValues (char** value1, char** value2);
void PrintStrings (char** index, FILE* file_write, unsigned int read_lines, const char* description);
int StrcmpForAlphaBegin (char* str1,  char* str2);
int StrcmpForAlphaEnd (char* str1,  char* str2);

int main()
{
    char* buffer;
    unsigned int read_char = ReadFromFile ("OneginSource.txt", buffer);
    unsigned int read_lines = LineCounter (buffer, read_char);
    char** index = RequestMemory (read_lines);
    char** index_copy = RequestMemory (read_lines);
    DivisionIntoLines (buffer, index, index_copy, read_char);

    FILE* file_write = fopen ("onegin_sorted.txt", "w");

    Sort (index, read_lines, &CompareBegin);
    PrintStrings (index, file_write, read_lines, "Eugene Onegin sorted from the beginning:\n\n");

    Sort (index, read_lines, &CompareEnd);
    PrintStrings (index, file_write, read_lines, "Eugene Onegin sorted from the end:\n\n");

    PrintStrings (index_copy, file_write, read_lines, "The source of \"Eugene Onegin\":\n\n");

    fclose (file_write);
    free (*index);
    free (*index_copy);

    return 0;
}

char** RequestMemory (unsigned int read_lines)
{
    char** index = (char**)calloc(read_lines, sizeof (char*));
    return index;
}

unsigned int ReadFromFile (char* filename, char* buffer)
{
    struct stat file_stat;
    stat (filename, &file_stat);
    buffer = (char*)calloc(file_stat.st_size + 1, sizeof (char));
    FILE* file = fopen (filename, "r");

    unsigned int read_char = fread (buffer, sizeof(char), file_stat.st_size, file);

    fclose (file);

    return read_char;
}

unsigned int LineCounter (char* buffer, unsigned int read_char)
{
    unsigned int num_lines = 1;
    for (int num_char = 0; num_char < read_char; num_char++)
    {
        if (buffer[num_char] == '\n') {
            buffer[num_char] = '\0';
            num_lines++;
        }
    }
    return num_lines;
}

void DivisionIntoLines (char* buffer, char** index, char** index_copy, unsigned int read_char)
{
    unsigned int num_lines = 1;
    for (int num_char = 0; num_char < read_char; num_char++)
    {
        if (buffer[num_char] == '\n') {
            buffer[num_char] = '\0';
            index[num_line] = &buffer[num_char+1];
            index_copy[num_line] = &buffer[num_char+1];
            num_line++;
        }
    }
}

void Sort (char** index, unsigned int num_of_lines,
           int(*CompareFunc)(char** address1, char** address2))
{
    for (int n_pass = num_of_lines; n_pass > 0; n_pass--)
    {
        for (size_t i = 0; i < num_of_lines - 1; i++)
        {
            if ((*CompareFunc)(&index[i], &index[i+1]) > 0)
            {
                ChangeValues (&index[i], &index[i+1]);
            }
        }
    }
}

int CompareBegin (char** address1, char** address2)
{
    assert(*address1);
    assert(*address2);
    return StrcmpForAlphaBegin (*address1, *address2);
}

int CompareEnd (char** address1, char** address2)
{
    assert(*address1);
    assert(*address2);
    return StrcmpForAlphaEnd (*address1, *address2);
}

void ChangeValues (char** value1, char** value2)
{
     char* temp = *value2;
    *value2 = *value1;
    *value1 = temp;
}

void PrintStrings (char** index, FILE* file_write, unsigned int read_lines, const char* description)
{
    fprintf (file_write, "%s", description);

    for (int i = 0; i < read_lines; i++)
    {
        if (index[i] && *index[i] != 0){
            fprintf (file_write, "<%s>\n", index[i]);
        }
    }

    fprintf (file_write, "\n");
}

int StrcmpForAlphaBegin (char* str1,  char* str2)
{
    for (; ; str1++, str2++) {
        while (*str1 && !isalpha (*str1)) str1++;
        while (*str2 && !isalpha (*str2)) str2++;

        if (tolower (*str1) != tolower (*str2)) {
            return tolower (*str1) - tolower (*str2);
        }
        if (!*str1) {
            return 0;
        }
    }
}

int StrcmpForAlphaEnd (char* str1,  char* str2)
{
    char* end_str1 = str1 + strlen (str1) - 1;
    char* end_str2 = str2 + strlen (str2) - 1;

    for (; ; end_str1--, end_str2--) {
        while (*end_str1 && !isalpha (*end_str1)) end_str1--;
        while (*end_str2 && !isalpha (*end_str2)) end_str2--;

        if (tolower (*end_str1) != tolower (*end_str2)) {
            return tolower (*end_str1) - tolower (*end_str2);
        }
        if (!*end_str1) {
            return 0;
        }
    }
}
