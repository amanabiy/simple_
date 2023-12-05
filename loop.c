#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_array_token(char **str)
{
	int i = 0;
	while (str[i] != NULL)
	{
		printf("token: %s\n", str[i]);
		i++;
	}
}

void free_array_tokens(char **str)
{
	int i = 0;
	while (str[i] != NULL)
		free(str[i++]);
	free(str);
}

char **tokenize(char *str1, char *delim)
{
	char *string_count = strdup(str1);
	char *str = strdup(str1);
	int lenght_array = 0;
	int i = 0;
	char **args;
	char *token = strtok(string_count, delim);

	while (token != NULL)
	{
		token = strtok(NULL, delim);
		lenght_array += 1;
	}
	args = malloc(sizeof(char *) * (lenght_array + 1));
	token = strtok(str, delim);

	while (token != NULL)
	{
		args[i] = (char *) malloc((strlen(token) + 1) * sizeof(char));
        strcpy(args[i], token);
		token = strtok(NULL, delim);
		i++;
	}

	args[i] = NULL;
	return args;
}

char **getCommand(char **argv)
{
	char *path = getenv("PATH");
	char **paths = tokenize(path, ":");
	int i = 0;
	char *x = strdup(argv[0]);

	while ((paths[i] != NULL) &  (access(x, F_OK) != 0))
	{
		size_t len = strlen(paths[i]) + 1 + strlen(argv[0]) + 1;
		char *y = (char *)malloc(len);
		if (y == NULL) {
			perror("Error allocating memory for y");
			exit(EXIT_FAILURE);
		}
		strcpy(y, paths[i]);
		strcat(y, "/");
		strcat(y, argv[0]);
		free(x);
		x = strdup(y);
		free(y);
		i++;
	}

	free_array_tokens(paths);
	free(argv[0]);

	if ((access(x, F_OK) != 0))
		argv[0] = NULL;
	else
		argv[0] = x;

	return argv;
}

void execute(char *input)
{
		pid_t child_pid;
		int status;
		char **argv = tokenize(input, " \n\r\t");
		child_pid = fork();

		if (child_pid == 0)
		{
			argv = getCommand(argv);
			if ((argv[0] == NULL) || execve(argv[0], argv, NULL) == -1)
			{
				perror("Faild to execute the command");
				exit(EXIT_FAILURE);
			}
		}

		waitpid(child_pid,&status,0);
		free_array_tokens(argv);
}

int main(void)
{
	char *input = NULL;
	size_t lenght = 0;
	ssize_t numberRead;

	while (1) {
		write(1, "$ :", 1);
		numberRead = getline(&input, &lenght, stdin);
		if (numberRead == -1)
		{
			free(input);
			exit(EXIT_FAILURE);
		}
		execute(input);
	}

	free(input);
	return (0);
}
