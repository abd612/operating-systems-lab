#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX 513

char buf[MAX];
char error_message[30] = "An error has occurred\n";
char prompt[10] = "mysh> ";
FILE *file = NULL;
int fd = 0;
int flag = 0;
int outCopy = 0;
int errCopy = 0;


int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		file = stdin;
		write(STDOUT_FILENO, prompt, strlen(prompt));
	}

	else if(argc == 2)
	{
		char *inFile = strdup(argv[1]);
		file = fopen(inFile, "r");
		
		if(file == NULL)
		{
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(1);
		}
	}

	else
	{
		write(STDERR_FILENO, error_message, strlen(error_message));
		exit(1);
	}

	while(fgets(buf, MAX, file))
	{
		flag = 0;

		if((buf[(int)strlen(buf) - 1]) != '\n')
		{
			while(getchar() != '\n');
			write(STDERR_FILENO, error_message, strlen(error_message));
		}

		if(strstr(buf,">") != NULL)
		{
			int m = 0;
			char *redr[sizeof(buf)];
			redr[0] = strtok(buf, ">");

			while(redr[m] != NULL)
			{
	    		m++;
		    	redr[m] = strtok(NULL, ">");
	   		}

	   		redr[m + 1] = NULL;

	   		if(m == 1)
	   		{
	   			write(STDERR_FILENO, error_message, strlen(error_message));
	   			
	   			if(argc == 1)
	   				write(STDOUT_FILENO, prompt, strlen(prompt));
	   			
	   			continue;
	   		}

	   		else if(m > 2)
	   		{
	   			write(STDERR_FILENO, error_message, strlen(error_message));
	   			
	   			if(argc == 1)
	   				write(STDOUT_FILENO, prompt, strlen(prompt));
	   			
	   			continue;
	   		}

	   		else
	   		{
	   			int n = 0;
	   			char *outp[sizeof(redr[1])];
	   			outp[0] = strtok(redr[1], " \n\t");

	   			while(outp[n] != NULL)
				{
		    		n++;
			    	outp[n] = strtok(NULL, " \n\t");
		   		}

		   		outp[n + 1] = NULL;

		   		if(n != 1)
		   		{
	   				write(STDERR_FILENO, error_message, strlen(error_message));
	   				
	   				if(argc == 1)
	   					write(STDOUT_FILENO, prompt, strlen(prompt));
	   				
	   				continue;
		   		}

		   		char *outFile = strdup(outp[0]);

				fd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IXUSR);

				if (fd < 0)
				{
		   			write(STDERR_FILENO, error_message, strlen(error_message));
		   			
		   			if(argc == 1)
		   				write(STDOUT_FILENO, prompt, strlen(prompt));
	   				
	   				continue;
				}

		   		outCopy = dup(1);
		   		errCopy = dup(2);

		   		dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);

                if (close(fd) < 0)
                {
                	write(STDERR_FILENO, error_message, strlen(error_message));
                	
                	if(argc == 1)
                		write(STDOUT_FILENO, prompt, strlen(prompt));
	   				
	   				continue;
                }

		   		flag = 1;
	   		}

	   		strcpy(buf, redr[0]);
		}

		char *token[sizeof(buf)];
		token[0] = strtok(buf, " \t\n");

		if(token[0] == NULL)
		{
			if(argc == 1)
				write(STDOUT_FILENO, prompt, strlen(prompt));
			continue;
		}

		int i = 0;

		while(token[i] != NULL)
		{
    		i++;
	    	token[i] = strtok(NULL, " \t\n");
   		}

   		token[i + 1] = NULL;

		if(strcmp(token[0], "pwd") == 0)
		{
				char path[MAX];
				if(getcwd(path, MAX) != NULL)
				{
					strcat(path, "\n");
					write(STDOUT_FILENO, path, strlen(path));
				}
				else
					write(STDERR_FILENO, error_message, strlen(error_message));
		}

		else if(strcmp(token[0], "cd") == 0)
		{
			if (i == 1)
			{
				if(chdir(getenv("HOME")) < 0)
					write(STDERR_FILENO, error_message, strlen(error_message));

			}

			else
			{
				if(chdir(token[1]) < 0)
					write(STDERR_FILENO, error_message, strlen(error_message));
			}
		}

		else if(strcmp(token[0], "wait") == 0)
		{
				wait(NULL);
		}

		else if(strcmp(token[0], "exit") == 0)
		{
				exit(0);
		}

		else if(strstr(token[0], ".py") != NULL)
		{
			int rc = fork();

			if(rc < 0)
				write(STDERR_FILENO, error_message, strlen(error_message));

			else if(rc == 0)
			{
				char *pypath = "/usr/bin/python";
				char *myargs[sizeof(buf) + sizeof(pypath)];

				myargs[0] = strdup(pypath);

				int k = 1;

				while(token[k-1] != NULL)
				{
			    	myargs[k] = token[k-1];
			    	k++;
		   		}

				myargs[k + 1] = NULL;

				if(execvp(myargs[0], myargs) < 0)
					write(STDERR_FILENO, error_message, strlen(error_message));
			}

			else
				wait(NULL);
		}

		else if(strcmp(token[0], "ls") == 0)
		{

			int rc = fork();

			if(rc < 0)
				write(STDERR_FILENO, error_message, strlen(error_message));


			else if(rc == 0)
			{
				char *lspath = "/bin/ls";
				char *myargs[sizeof(buf)];

				myargs[1] = NULL;
				myargs[0] = strdup(lspath);

				int k = 1;

				while(token[k] != NULL)
				{
			    	myargs[k] = token[k];
			    	k++;
		   		}

				myargs[k + 1] = NULL;

				if(execvp(myargs[0], myargs) < 0)
					write(STDERR_FILENO, error_message, strlen(error_message));
			}

			else
				wait(NULL);
		}

		else
		{
			write(STDERR_FILENO, error_message, strlen(error_message));
		}

        if(flag == 1)
        {
	        dup2(outCopy, 1);
	        dup2(errCopy, 2);
	        close(outCopy);
	        close(errCopy);         
        }

		if(argc == 1)
		{
			write(STDOUT_FILENO, prompt, strlen(prompt));
		}
	}
}