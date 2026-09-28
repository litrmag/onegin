#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>

char*  ReadFromFile            (const char* filename, size_t* read_char);
size_t LineCounter             (char* buffer, size_t read_char);
void   DivisionIntoLines       (char* buffer, char** index_array, char** index_copy, size_t read_char);

void   BubbleSort              (char** index_array, size_t num_of_lines, int(*CompareFunc)(const void* address1, const void* address2));
int    CompareFromBegin        (const void* address1, const void* address2);
int    CompareFromEnd          (const void* address1, const void* address2);
int    StrcmpForAlphaFromBegin (char* str1, char* str2);
int    StrcmpForAlphaFromEnd   (char* str1, char* str2);
void   ChangeValues            (char** value1, char** value2);

void   PrintStrings            (char** index_array, FILE* file_write, size_t read_lines, const char* description);

void   MemoryCleansing         (char** index_array, char** index_copy_array, char*  buffer);

int main()
{
    size_t read_char = 0;
    char* buffer = ReadFromFile ("OneginSource.txt", &read_char);
    size_t read_lines = LineCounter (buffer, read_char);
    char** index_array = (char**)calloc(read_lines, sizeof (char*));
    char** index_copy_array = (char**)calloc(read_lines, sizeof (char*));
    DivisionIntoLines (buffer, index_array, index_copy_array, read_char);

    FILE* file_write = fopen ("onegin_sorted.txt", "w");
    if (!file_write) {
        printf ("Error while opening file for writing\n");
    }

    qsort (index_array, read_lines, sizeof (char**), &CompareFromBegin);
    //BubbleSort (index_array, read_lines, &CompareFromBegin); - this line was here earlier, now it's qsort
    PrintStrings (index_array, file_write, read_lines, "Eugene Onegin sorted from the beginning:\n\n");

    BubbleSort (index_array, read_lines, &CompareFromEnd);
    PrintStrings (index_array, file_write, read_lines, "Eugene Onegin sorted from the end:\n\n");

    PrintStrings (index_copy_array, file_write, read_lines, "The source of \"Eugene Onegin\":\n\n");

    fclose (file_write);

    MemoryCleansing (index_array, index_copy_array, buffer);

    return 0;
}

//------------------------------------------------------------------------------------------------------//

char* ReadFromFile (const char* filename, size_t* read_char)
{
    assert (filename);
    assert (read_char);

    struct stat file_stat;
    stat (filename, &file_stat);
    char* buffer = (char*)calloc(file_stat.st_size + 1, sizeof (char));
    if (!buffer) {
        printf ("Error in allocating memory for writing strings to the buffer\n");
    }

    FILE* file = fopen (filename, "r");
    if (!file) {
        printf ("Error while opening file for reading\n");
    }

    *read_char = fread (buffer, sizeof(char), file_stat.st_size, file);

    fclose (file);

    return buffer;
}

size_t LineCounter (char* buffer, size_t read_char)
{
    assert (buffer);

    size_t num_lines = 1;
    for (size_t num_char = 0; num_char < read_char; num_char++)
    {
        if (buffer[num_char] == '\n') {
            buffer[num_char] = '\0';
            num_lines++;
        }
    }
    return num_lines;
}

void DivisionIntoLines (char* buffer, char** index_array, char** index_copy_array, size_t read_char)
{
    assert (buffer);
    assert (index_array);
    assert (index_copy_array);

    index_array[0] = buffer;
    size_t num_line = 1;

    for (int num_char = 0; num_char < read_char; num_char++)
    {
        if (buffer[num_char] == '\0') {
            index_array[num_line] = &buffer[num_char+1];
            index_copy_array[num_line] = &buffer[num_char+1];
            num_line++;
        }
    }
}

//------------------------------------------------------------------------------------------------------//

void BubbleSort (char** index_array, size_t num_of_lines,
           int(*CompareFunc)(const void* address1, const void* address2))
{
    assert (index_array);

    for (int n_pass = num_of_lines; n_pass > 0; n_pass--)
    {
        for (size_t i = 0; i < num_of_lines - 1; i++)
        {
            if ((*CompareFunc)(&index_array[i], &index_array[i+1]) > 0)
            {
                ChangeValues (&index_array[i], &index_array[i+1]);
            }
        }
    }
}

int CompareFromBegin (const void* address1, const void* address2)
{
    assert(address1);
    assert(address2);

    char* str1 = *(char**)address1;
    char* str2 = *(char**)address2;

    assert (str1);
    assert (str2);

    return StrcmpForAlphaFromBegin (str1, str2);
}

int CompareFromEnd (const void* address1, const void* address2)
{
    assert (address1);
    assert (address2);

    char* str1 = *(char**)address1;
    char* str2 = *(char**)address2;

    assert (str1);
    assert (str2);

    return StrcmpForAlphaFromEnd (str1, str2);
}

int StrcmpForAlphaFromBegin (char* str1, char* str2)
{
    assert (str1);
    assert (str2);

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

int StrcmpForAlphaFromEnd (char* str1,  char* str2)
{
    assert (str1);
    assert (str2);

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

void ChangeValues (char** value1, char** value2)
{
    assert (value1);
    assert (value2);

    char* temp = *value2;
    *value2 = *value1;
    *value1 = temp;
}

//------------------------------------------------------------------------------------------------------//

void PrintStrings (char** index_array, FILE* file_write, size_t read_lines, const char* description)
{
    assert (index_array);
    assert (file_write);
    assert (description);

    fprintf (file_write, "%s", description);

    for (int i = 0; i < read_lines; i++)
    {
        if (index_array[i] && *index_array[i] != 0){
            fprintf (file_write, "<%s>\n", index_array[i]);
        }
    }

    fprintf (file_write, "\n");
}

//------------------------------------------------------------------------------------------------------//

void MemoryCleansing (char** index_array, char** index_copy_array, char* buffer)
{
    free (index_array);
    free (index_copy_array);
    free (buffer);
}
