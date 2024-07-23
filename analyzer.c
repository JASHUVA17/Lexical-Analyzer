#include "header.h"

extern int error_flag; // Declare external integer variable to track errors.
char* keyword[] = {"goto","char","int","float","double","break","continue","return","if","else","while","void","for","do","switch","case","default","typedef","struct","union","enum","extern","static","volatile","const","unsigned","signed","short","long","NULL","sizeof","auto","register"};

char operator[] = {'+','-','/','%','^','!','~','*',','}; // Array of operator characters.
char symbol[] = {'{','}','[',']'}; // Array of symbol characters.
char special[] = {'(',')',';'}; // Array of special characters.

char delim[] = {'\n','\t',' '}; // Array of delimiter characters.

void lexical_analyze(FILE* fptr) // Function to perform lexical analysis.
{
    char line[150]; // Buffer to store a line of code.
    while(fscanf(fptr,"%[^\n]\n",line) != EOF) // Read line by line from file.
    {
	if(line[0] == '#') // Check if the line is a preprocessor directive.
	{
	    continue; // Skip preprocessor directive lines.
	}
	check_type(line); // Call check_type to analyze the line.
    }
}

void check_type(char* line) // Function to check the type of tokens in a line.
{
    char* temp_line = line; // Temporary pointer to line.
    char arr[50]; // Buffer to store individual tokens.
    while(*temp_line) // Loop through each character in the line.
    {
	if( strchr(delim,*temp_line) != NULL) // Check for delimiters.
	{
	    temp_line++; // Skip delimiter.
	    continue;
	}
	int i = 0; // Initialize index for token buffer.
	if( *temp_line == '"') // Check for string literal.
	{
	    char ch; // Temporary character storage.
	    int flag = 0; // Flag to track escape sequences.
	    temp_line++; // Move to the next character.
	    while(*temp_line && *temp_line != '"') // Loop until end of string literal.
	    {
		if(*temp_line == '\\') // Check for escape sequence.
		{
		    ch = *(temp_line+1); // Get the next character.
		    if(ch != 'b' && ch != 'f' && ch != 'n' && ch != 'a' && ch != 'n' && ch != 'v' && ch != 't' && ch != '0')
		    {
			printf("Escape sequence is not allowed as %c%c\n",*temp_line,ch); // Print error for invalid escape sequence.
			error_flag = 1; // Set error flag.
			temp_line++; // Skip the escape sequence.
			continue;
		    }

		}
		if(*temp_line == ')' || *temp_line == ';') // Check for invalid characters in string literal.
		{
		    printf("ERROR: closing of \" missing\n"); // Print error for missing closing quote.
		    error_flag = 1; // Set error flag.
		    break;

		}
		if(flag==1) // If flag is set, break the loop.
		    break;
		arr[i++] = *temp_line++; // Store the character in token buffer.
	    }
	    arr[i] = '\0'; // Null-terminate the string.
	    printf("%-20s is a string literal\n",arr); // Print the string literal.
	    if(*temp_line == '"') // Move past the closing quote.
	    {
		temp_line++;
	    }
	    continue;
	}
	if(*temp_line == 39) // Check for character constant.
	{
	    temp_line++; // Move to the next character.
	    arr[i++] = *temp_line++; // Store the character in token buffer.
	    if(*temp_line == 39) // Check for closing single quote.
	    {
		printf("%-20c is a character constant\n",arr[0]); // Print the character constant.
		temp_line++;
		continue;
	    }
	    else
	    {
		int flag = 0; // Flag to track errors.
		while(*temp_line != 39 && *temp_line) // Loop until end of character constant.
		{
		    if(*temp_line == ';' || *temp_line == ')') // Check for invalid characters.
		    {
			flag = 1; // Set flag if invalid character found.
		    }
		    arr[i++] = *temp_line++; // Store the character in token buffer.
		}
		arr[i] = '\0'; // Null-terminate the string.
		if(flag == 0) // If no invalid characters found.
		{
		    printf("Can't assign %s into a character\n",arr); // Print error for invalid character assignment.
		    error_flag = 1; // Set error flag.
		}
		else
		{
		    printf("Missing of \' single quote\n"); // Print error for missing single quote.
		    error_flag = 1; // Set error flag.
		}
		if(*temp_line == 39) // Move past the closing single quote.
		{
		    temp_line++;
		}
		continue;
	    }
	}
	if( is_operator(*temp_line) == 1) // Check if character is an operator.
	{
	    printf("%-20c is an operator\n",*temp_line); // Print the operator.
	    temp_line++;
	    continue;
	}
	else if(is_special(*temp_line) == 1) // Check if character is special.
	{
	    printf("%-20c is a special character\n",*temp_line); // Print the special character.
	    temp_line++;
	    continue;
	}
	else if(is_assignment(*temp_line) == 1) // Check if character is an assignment operator.
	{
	    printf("%-20c is a assignment operator\n",*temp_line); // Print the assignment operator.
	    temp_line++;
	    continue;
	}
	else if(is_symbol(*temp_line) == 1) // Check if character is a symbol.
	{
	    printf("%-20c is a symbol\n",*temp_line); // Print the symbol.
	    temp_line++;
	    continue;
	}
	else if( *temp_line == '<' || *temp_line == '>') // Check if character is a relational operator.
	{
	    if( *(temp_line+1) == '=') // Check for compound relational operator.
	    {
		printf("%c%-20c is a relational operator\n",*temp_line,*(temp_line+1)); // Print the relational operator.
		temp_line++;
		temp_line++;
		continue;
	    }
	    printf("%-20c is a relational operator\n",*temp_line); // Print the relational operator.
	    temp_line++;
	}
	else if( *temp_line == '&') // Check if character is a bitwise AND or logical AND.
	{
	    if(*(temp_line+1) == '&') // Check for logical AND.
	    {
		printf("%c%-20c is a logical operator\n",*temp_line,*(++temp_line)); // Print the logical operator.
		temp_line++;
		continue;
	    }
	    printf("%-20c is a bitwise operator\n",*temp_line); // Print the bitwise operator.
	    temp_line++;
	}
	else if( *temp_line == '|') // Check if character is a bitwise OR or logical OR.
	{
	    if(*(temp_line+1) == '|') // Check for logical OR.
	    {
		printf("%c%-20c is a logical operator\n",*temp_line,*(++temp_line)); // Print the logical operator.
		temp_line++;
		continue;
	    }
	    printf("%-20c is a bitwise operator\n",*temp_line); // Print the bitwise operator.
	    temp_line++;
	}
	while( *temp_line && (!strchr(delim,*temp_line)) && ( !is_special(*temp_line)) && (!is_assignment(*temp_line)) && (!is_operator(*temp_line)) && (!is_symbol(*temp_line)))
	{
	    arr[i++] = *temp_line++; // Store the character in token buffer.
	}
	arr[i] = '\0'; // Null-terminate the string.
	if( strlen(arr) > 0)
	{
	    if( is_keyword(arr) == 0 && is_digit(arr) == 0 && is_bin(arr) == 0 && is_octal(arr) == 0 && is_hexa(arr) ==0 )
	    {
		if(*temp_line == '[') // Check if token is an array.
		{
		    char temp[50]; // Temporary buffer for array.
		    i=strlen(arr); // Get the length of the token.
		    if(*(temp_line+1) == ']') // Check for empty array.
		    {
			strcpy(temp,arr); // Copy token to temporary buffer.
			temp[i++] = *temp_line++; // Store the '[' character.
			temp[i++] = *temp_line++; // Store the ']' character.
			temp[i] = '\0'; // Null-terminate the string.
			printf("%-20s is an array\n",temp); // Print the array.
		    }
		    else
		    {
			char num[20]; // Temporary buffer for array size.
			strcpy(num,arr); // Copy token to temporary buffer.
			num[strlen(arr)] = '['; // Add '[' to the buffer.
			num[i+1] = '\0'; // Null-terminate the string.
			int i=0; // Initialize index for array size.
			temp_line++; // Move to the next character.
			while(*temp_line != ']' && *temp_line) // Loop until end of array size.
			{
			    if(*temp_line == ';' || *temp_line == ')') // Check for invalid characters.
			    {
				printf("Missing ] bracket\n"); // Print error for missing ']'.
				error_flag = 1; // Set error flag.
				break;
			    }
			    num[strlen(arr)+i+1] = *temp_line; // Store the character in buffer.
			    num[strlen(arr)+i+2] = '\0'; // Null-terminate the string.
			    temp_line++;
			}
			num[strlen(arr)+i+1] = ']'; // Add ']' to the buffer.
			num[strlen(arr)+i+2] = '\0'; // Null-terminate the string.
			if(is_digit(num) == 1) // Check if array size is a digit.
			{
			    printf("%-20s is an array\n",num); // Print the array.
			}
			else
			{
			    printf("Invalid array size\n"); // Print error for invalid array size.
			    error_flag = 1; // Set error flag.
			}
		    }
		}
		else
		{
		    printf("%-20s is an identifier\n",arr); // Print the identifier.
		}
	    }
	}
    }
}

int is_keyword(char* token)
{
    for(int i = 0;i<32;i++)
    {
	if(strcmp(token,keyword[i]) == 0)
	{
	    printf("%-20s is a keyword\n",token);
	    return 1;
	}
    }
    return 0;
}
int is_digit(char* token)
{
    if(token[0] == '0' && strlen(token) > 1)
    {
	return 0;


    }
    int flag = 0,count = 0,char_flag = 0,digit_flag = 0;
    for(int i = 0; i < strlen(token) ;i++)
    {
	if(token[i] == 'f' && flag)
	{
	    digit_flag++;

	}
	else if( ((token[i] >= 'a' && token[i] <= 'z' ) || (token[i] >= 'A' && token[i] <='Z')) && digit_flag)
	{
	    if( token[2] != 'b' && token[2] != 'x' )
	    {
		char_flag = 1;
	    }
	}
	else if(  (token[i] >= '0' && token[i] <= '9'))
	{
	    digit_flag++;
	}
	else if(token[i] == '.')
	{
	    flag = 1;
	    count++;
	    digit_flag++;
	}
    }
    if(strlen(token) == digit_flag && flag == 0)
    {
	printf("%-20s is a integer constant\n",token);
	return 1;
    }
    else if( strlen(token) == digit_flag && flag == 1 && count < 2)
    {
	printf("%-20s is a float constant\n",token);
	return 1;
    }
    else if( char_flag == 1 || count > 1)
    {
	printf("ERROR: can't initialize a number as %s\n",token);
	error_flag = 1;
	return 1;
    }

    return 0;

}
int is_operator(char token)
{
    for(int i = 0;i<9;i++)
    {
	if(token == operator[i])
	    return 1;
    }
    return 0;

}
int is_assignment(char ch)
{
    if(ch == '=')
	return 1;
    return 0;
}
int is_symbol(char ch)
{
    for(int i=0;i<4;i++)
    {
	if(ch == symbol[i])
	    return 1;
    }
    return 0;

}
int is_special(char ch)
{
    for(int i=0;i<3;i++)
    {
	if(ch == special[i])
	    return 1;
    }
    return 0;
}
int is_bin(char* number)
{
    if(strncmp(number,"0b",2) == 0)
    {
	for(int i=2;i < strlen(number);i++)
	{
	    if(number[i] != '0' && number[i] != '1')
	    {
		printf("Can't initialize binary number as %s\n",number);
		error_flag = 1;
		return 1;
	    }
	}
	printf("%s is a binary number\n",number);
	return 1;

    }
    return 0;
}
int is_octal(char* number)
{
    if(number[0] == '0' &&  number[1] != 'x')
    {
	for(int i=0;i<strlen(number);i++)
	{
	    if( (!(number[i] >= '0' && number[i] <= '7')) || (number[i] >= 'a' && number[i] <= 'z') || (number[i] >='A' && number[i] <= 'Z'))
	    {
		printf("ERROR: can't initialize octal value as %s\n",number);
		error_flag = 1;
		return 1;
	    }
	}
	printf("%-20s is an octal value\n",number);
	return 1;
    }
    return 0;
}
int is_hexa(char* number)
{
    if(strncmp(number,"0x",2) == 0)
    {
	for(int i = 2;i<strlen(number);i++)
	{
	    if( (!(number[i] >= '0' && number[i] <= '9')) && (!(number[i] >= 'A' && number[i] <= 'F')) && (!(number[i] >='a' && number[i] <= 'f')))
	    {
		printf("ERROR: can't initialize %s as hexadecimal value\n",number);
		error_flag = 1;
		return 1;
	    }
	}
	printf("%-20s is a hexadecimal value\n",number);
	return 1;
    }
    return 0;
}
