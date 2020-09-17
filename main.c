
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int lastReturn = 0;
int LINE_LIMIT = 256;
int ARG_LIMIT = 256;

//char *CMD_DELIMETERS = ";\n";
//char *ARG_DELIMETERS = " ";

char *RETCODE_NAME = "retcode";
char *ECHO_NAME = "echo";

int echo(int argc, char *argv[]) {
	for (int i = 1; i < argc; ++i) {
		printf("%s%c", argv[i], i == argc - 1 ? '\n' : ' ');
	}
	return argc - 1;
}

int retcode(int argc, char *argv[]) {
	if (argc > 1) {
		printf("No arguments expected for retcode to get\n");
		return 1;
	}
	printf("%d\n", lastReturn);
	return 0;
}

int main(int argc, char *argv[]) {
	char line[LINE_LIMIT];
	
	char *command;
	char *cmdArgv[ARG_LIMIT];
	int cmdArgc = 0;
	
	char *argument;
	
	char *lineTokenPtr = NULL;
	char *commandTokenPtr = NULL;
	
	int (*cmdToCall)(int, char **);
	
	while (fgets(line, LINE_LIMIT, stdin)) {
		command = strtok_r(line, ";\n", &lineTokenPtr);
		while (command != NULL) {
			cmdArgc = 0;
			
			argument = strtok_r(command, " ", &commandTokenPtr);
			while (argument != NULL && cmdArgc < 256) {
				cmdArgv[cmdArgc] = argument;
				cmdArgc++;
				argument = strtok_r(NULL, " ", &commandTokenPtr);
			}
			
			command = strtok_r(NULL, ";\n", &lineTokenPtr);
			
			// we got an empty command
			if (cmdArgc == 0)
				continue;
			
			if (!strcmp(cmdArgv[0], ECHO_NAME))
				cmdToCall = &echo;
			else if (!strcmp(cmdArgv[0], RETCODE_NAME))
				cmdToCall = &retcode;
			else
				printf("Unknown command: %s\n", cmdArgv[0]);
			
			lastReturn = cmdToCall(cmdArgc, cmdArgv);
		}
	}
	
	return 0;
}
