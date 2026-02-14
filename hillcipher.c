/*============================================================================
| Assignment: Hill Cipher - Encrypting a plaintext file using the Hill cipher
|
| Author: Ryan Grau
| Language: c
| To Compile:
| gcc -o pa02 pa02.c
| To Execute:
| c -> ./pa02 kX.txt pX.txt
|
+===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100000
// Converting letter to number
int letter_to_num(char a)
{
    return a - 'a';
}
// Converting number to letter
char num_to_letter(int a)
{
    return a + 'a';
}
// Feeding the matrix
int **loadmatrix(const char *key_file, int *size)
{
    FILE *file = fopen(key_file, "r");
    if (!file)
    {
        printf("Error opening key file.\n");
        exit(1);
    }
    fscanf(file, "%d", size);
    int **matrix = malloc(*size * sizeof(int *));
    for (int i = 0; i < *size; i++)
    {
        matrix[i] = malloc(*size * sizeof(int));
        for (int j = 0; j < *size; j++)
        {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);
    return matrix;
}
// This function processes the plaintext
void process_plaintext(const char *plaintext_file, char *processed_text, int block_size)
{
    FILE *file = fopen(plaintext_file, "r");
    if (!file)
    {
        printf("Error opening plaintext file.\n");
        exit(1);
    }
    int idx = 0;
    char c;
    while ((c = fgetc(file)) != EOF && idx < MAX - 1)
    {
        if (isalpha(c))
        {
            processed_text[idx++] = tolower(c);
        }
    }
    processed_text[idx] = '\0';
    while (idx % block_size != 0)
    {
        processed_text[idx++] = 'x';
    }
    processed_text[idx] = '\0';
    fclose(file);
}
// Function to print the matrix
void print_matrix(int **matrix, int size)
{
    printf("\nKey matrix:\n");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}
// This function encrypts the plaintext to cyphertext
void hill_cipher_encrypt(char *plaintext, int **key_matrix, int size, char *ciphertext)
{
    int len = strlen(plaintext);
    int idx = 0;
    for (int i = 0; i < len; i += size)
    {
        int block[size];
        for (int j = 0; j < size; j++)
        {
            block[j] = letter_to_num(plaintext[i + j]);
        }
        // Matrix multiplication mod 26
        for (int row = 0; row < size; row++)
        {
            int sum = 0;
            for (int col = 0; col < size; col++)
            {
                sum += key_matrix[row][col] * block[col];
            }
            ciphertext[idx++] = num_to_letter(sum % 26);
        }
    }
    ciphertext[idx] = '\0';
}
// This function prints the blocks
void print_in_blocks(const char *text)
{
    int len = strlen(text);
    for (int i = 0; i < len; i++)
    {
        printf("%c", text[i]);
        if ((i + 1) % 80 == 0 && i != 0)
        {
            printf("\n");
        }
    }
    if (len % 80 != 0)
    {
        printf("\n");
    }
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./pa02.c key_file.txt plaintext_file.txt\n");
        return 1;
    }
    const char *key_file = argv[1];
    const char *plaintext_file = argv[2];
    int size;
    int **key_matrix = loadmatrix(key_file, &size);
    char plaintext[MAX];
    process_plaintext(plaintext_file, plaintext, size);
    char ciphertext[MAX];
    hill_cipher_encrypt(plaintext, key_matrix, size, ciphertext);
    print_matrix(key_matrix, size);
    printf("Plaintext:\n");
    print_in_blocks(plaintext);
    printf("\nCiphertext:\n");
    print_in_blocks(ciphertext);
    // Freeing memory
    for (int i = 0; i < size; i++)
    {
        free(key_matrix[i]);
    }
    free(key_matrix);
    return 0;
}
