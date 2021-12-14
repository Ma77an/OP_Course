#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {


//	Config File Inputted Check:
    if(argc != 2) {
        write(2, "Config file not inputted\n", 25);
        exit(-1);
    }


//	Variables:
    int i, j, status = 11, bytesRead;
    char tav;
    char* config[3];
    char usersDir[100]; config[0] = usersDir;           // usersDir -> {/,u,s,e,r,F,o,l,e,r,P,a,t,h,\0}
    char inputFile[100]; config[1] = inputFile;
    char expOutFile[100]; config[2] = expOutFile;


//	Config File Opening:
    int fdConfig = open(argv[1], O_RDONLY);
    if(fdConfig == -1) {
        write(2, "Config file could not open\n", 27);
        exit(-1);
    }

//	Reading lines from config file;
    for(i = 0; i < 3; i++) {
        j = 0;
        bytesRead = read(fdConfig, &tav, 1);

        while(tav != '\n' && bytesRead != 0) {
            config[i][j] = tav;
            bytesRead = read(fdConfig, &tav, 1);
            j++;
        }
        config[i][j] = '\0'; // put null at the end
    }
    close(fdConfig);	// Close Config File




//	dirList File Opening:
    int fdDirList = open("dirList.txt", (O_CREAT | O_WRONLY | O_RDONLY));
    if(fdDirList == -1) {
        write(2, "dirList file could not open\n", 28);
        exit(-1);
    }

    perror("Error 1");
    char** commandArgv = (char**) malloc(3 * sizeof(char));
    commandArgv[0] = (char*) malloc( (strlen("ls") + 1) * sizeof(char) );
    commandArgv[1] = usersDir;
    commandArgv[2] = NULL;

    pid_t pid;
//	fork:
    if( (pid = fork()) == -1 ) {
        write(2, "fork failed\n", 12);
        exit(-1);
    }

//	Son:
    if(pid == 0) {
        perror("Error 2");
//		// Change STDOUT to dirList.txt
        close(1);
        dup(fdDirList);	// stdout became dirList.txt

        // linux command CLOSE all files we are using WATCH OUT(need to re-open again dirList.txt)
        // fdDirList of parent will point to DIFFERENT component!!!!!
        execvp("ls", commandArgv); // ls is printing now to dirList.
//		execvp failed:
        write(2, "exec failed\n", 12);
        exit(-1);
    }

//	Father:
    else {
        perror("Error 3");
        wait(&status);
        if(status != 0) {
            write(2, "ls command failed\n", 18);
            exit(-1);
        }
    }

    // Opening dirList.txt file again because SON CLOSE HIM!!!
    fdDirList = open("dirList.txt", (O_RDONLY));
    perror("Error 4");
    int succeedRead = -1;
    int usersCount = 0;
    // ls prints '\n' at the end - so the last byte will be '\n'.
    while(succeedRead != 0) {
        succeedRead = read(fdDirList, &tav, 1);
        if(succeedRead == 0) break;	// to make sure he won't count last place twice!!! because tav value stays '\n'
        if(tav == '\n') usersCount++;           // counting amount of users
    }


//	Allocation For Names:
    char** usersNames = (char**) malloc( usersCount*sizeof(char) );
    for(i = 0; i < usersCount; i++) {
        usersNames[i] = (char*) malloc( 50*sizeof(char) );
    }

    perror("Error 5");
    lseek(fdDirList, 0, SEEK_SET);
//	Read Names to Names Array:
    for(i = 0; i < usersCount; i++) {
        j = 0;
        bytesRead = read(fdDirList, &tav, 1);

        while(tav != '\n' && bytesRead != 0) {
            usersNames[i][j] = tav;
            bytesRead = read(fdDirList, &tav, 1);
            j++;
        }
        usersNames[i][j] = '\0';
    }
    close(fdDirList);



    char** usersCommandPath = (char**) malloc(usersCount * sizeof(char));
    for(i = 0; i < usersCount; i++) {
        usersCommandPath[i] = (char*) malloc( strlen(config[0]) + strlen("/") + strlen(usersNames[i]) + strlen("/main.exe") + 1);

        perror("Error 6");
        strcat(usersCommandPath[i], config[0]);
        strcat(usersCommandPath[i], "/");
        strcat(usersCommandPath[i], usersNames[i]);
        strcat(usersCommandPath[i], "/main.exe");
    }




//	Results File Opening:
    int fdResult = open("results.txt", (O_CREAT | O_WRONLY | O_TRUNC), 0777);
    if(fdResult == -1) {
        write(2, "Results file could not open\n", 28);
        exit(-1);
    }


//  	Open File:
    int fdUserOut;	// each student override content
//לעבור על זה פהההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההההה
    perror("Error 7");
//    char** compArgv = (char**) malloc( 3 * sizeof(char*) );
//    compArgv[0] = (char*) malloc( ( strlen("./comp.out") + 1 ) * sizeof(char) );
//    strcpy(compArgv[0], "./comp.out");
//    compArgv[1] = (char*) malloc( ( strlen("./code/expected_output.txt") + 1 ) * sizeof(char) );
//    strcpy(compArgv[1], "./code/expected_output.txt");
//    compArgv[2] = (char*) malloc( ( strlen("./program_output.txt") + 1 ) * sizeof(char) );
//    strcpy(compArgv[2], "./program_output.txt");


//	no need to realloc commandArgv[1] because he points to char[] on the Stack
    commandArgv[1] = (char*) malloc( (strlen(config[1]) + 1 ) * sizeof(char) );
    strcpy(commandArgv[1], config[1]);
    perror("Error 8");
//	commandArgv[2] value is NULL from before
    for(i = 0; i < usersCount; i++) {
        fdUserOut = open("/home/matang99/Matala1/program_output.txt", (O_WRONLY | O_CREAT | O_TRUNC | O_SYNC), 0666);	// O_CREAT for creating at first iteration to creat file
        perror("error 8.4");
        if(fdUserOut == -1) {

            write(2, "failed to open userOutput.txt\n", 30);
            exit(-1);
        }
        perror("error 8.5");
        lseek(fdUserOut, 0, SEEK_SET);
        perror("error 8.6");
        commandArgv[0] = (char*)malloc((strlen(usersCommandPath[i]) + 1 ) * sizeof(char) );
        perror("error 8.7");
        strcpy(commandArgv[0], usersCommandPath[i]);
        perror("error 8.8");
        int fdInput = open("/home/matang99/Matala1/code/input.txt", O_RDONLY);

//		fork:
        if ((pid = fork()) == -1) {
            write(2, "first fork failed\n", 12);
            exit(-1);
        }

        perror("error 8.9");
//		Son:
        if (pid == 0) {
// 			Change STDOUT to userOutput.txt:
            close(1);
            dup(fdUserOut);
            close(0);
            dup(fdInput);
            perror("Error 9");
            printf("%s ," ,usersNames[i]);
            perror(commandArgv[0]);
            execvp(commandArgv[0], commandArgv);
//			If execvp failed:
            write(2, "exec(main.exe - user) failed\n", 29);
            exit(-1);
        }

//		Parent:
        else {
            perror("error 9.5");
            wait(&status);
            if(status != 0) {
                write(2, "main.exe(user) command failed\n", 30);
                exit(-1);
            }
            // open userOutput.txt again because SON CLOSED IT!!!!!!!! this time open on READ because no need to add
            // need to open userOutput.txt at NEXT ITERATION for next COMMAND on O_WRONLY, so it will override the content
            // for next user main.exe.
//	    		fork:
            if ((pid = fork()) == -1) {
                write(2, "second fork failed\n", 12);
                perror("Error 10");
                exit(-1);
            }

//	    		Son:
            if(pid == 0) {
                close(1);
                dup(fdResult);
                char* compArgv[] = {"./comp.out", "./code/expected_output.txt", "./program_output.txt"};
                // comp.c gets strings and open files alone.
//				Change comp.c only returns a value, not printing something so no need to do dup.
                // SON(comp.c) CLOSE AGAIN userOutput.txt
                execvp(compArgv[0], compArgv);
//				If execvp failed:
                write(2, "exec(comp.out) failed\n", 22);
                perror("Error 11");
                exit(-1);
            }

//	    		Parent:
            else {
                wait(&status);

                status = status/256;// status equal to 256 * x (exit(x) from other program ran by execvp;
                    printf("%s , %d\n", usersNames[i],status);
//                int exit_status = WEXITSTATUS(status);
//				User Succeeded(Give Him Grade 100):
                if(status == 2) {
                    write(fdResult, usersNames[i], strlen(usersNames[i]));
                    write(fdResult, ", 100\n", 6);
                    perror("Error 12");
                }


//				User Failed(Give Him Grade 0):
                else
                {
//                    write(2,status,1);
                    write(fdResult, usersNames[i], strlen(usersNames[i]));
                    write(fdResult, ", 0\n", 4);
                }


            }

        }

    }



//-------------Free Allocations:-------------\\


////	Free: compArgv
//    for(i = 0; i < 3; i++)
//        free(compArgv[i]);
//    free(compArgv);
//    perror("compArgv");


//	Free: commandArgv
    for(i = 0; i < 2; i++)
        free(commandArgv[i]);
    free(commandArgv);
    perror("commandArgv");


////	Free: userNames and usersCommandPath
//    for(i = 0; i < usersCount; i++) {
//        free(usersNames[i]);
//        free(usersCommandPath[i]);
//    }
//    free(usersNames);
//    perror("userNames");
//    free(usersCommandPath);
//    perror("userCommand");

    	exit(0);
}