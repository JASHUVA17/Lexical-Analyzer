#include "header.h"

int error_flag; // Declare an integer variable to indicate error state.
int main(int argc,char* argv[]) // Main function with command-line arguments.
{
    if(argc < 2) // Check if the number of arguments is less than 2.
    {
	printf("Usage: ./a.out <.c file>\n"); // Print usage information.
	return 0; // Exit the program.
    }
    char filename[50]; // Declare a character array to store the filename.
    strcpy(filename,argv[1]); // Copy the argument to the filename variable.
    if(!strstr(filename,".c")) // Check if the filename contains ".c".
    {
	printf("Error : Please pass <.c file>\n"); // Print error message for incorrect file type.
	return 0; // Exit the program.
    }
    FILE* fptr = fopen(filename,"r"); // Open the file in read mode.
    printf("Open : %s : ",filename); // Print the filename being opened.
    if(fptr) // Check if the file was successfully opened.
    {
	printf("SUCCESS\n"); // Print success message.
    }
    else // If the file failed to open.
    {
	printf("FAILED\n"); // Print failure message.
	return 0; // Exit the program.
    }
    printf("Parsing : %s : Started\n",filename); // Print parsing started message.
    lexical_analyze(fptr); // Call the lexical_analyze function.
    fclose(fptr); // Close the file.
    if(error_flag) // Check if there was an error during lexical analysis.
    {
	printf("Tokens cannot be generated.\n");; // Print error message.
    }
    else // If no error occurred.
    {
	printf("Tokens generated.\n"); // Print success message.
    }
    return 0; // Exit the program.
}

