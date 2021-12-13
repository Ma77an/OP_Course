#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>



int main(int argc, char* argv[]) {
    if (argc!=2){
        printf("Not the right amount of arguments!!!!!!!!!!!");
        exit(-1);
    }

    //	Variables:
    int i, j, status = 11, bytesRead;
    char tav;
    char* configLines[3];
    char usersDir[50]; configLines[0] = usersDir;           // usersDir -> {/,u,s,e,r,F,o,l,e,r,P,a,t,h,\0}
    char inputFile[50]; configLines[1] = inputFile;
    char expOutFile[50]; configLines[2] = expOutFile;


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
            configLines[i][j] = tav;
            bytesRead = read(fdConfig, &tav, 1);
            j++;
        }
        configLines[i][j] = '\0'; // put null at the end
    }
    close(fdConfig);	// Close Config File




//	dirList File Opening:
    int fdDirList = open("dirList.txt", (O_CREAT | O_WRONLY | O_RDONLY));
    if(fdDirList == -1) {
        write(2, "dirList file could not open\n", 28);
        exit(-1);
    }

//    perror("stage 0");
//    int configFD= open(argv[1], O_RDONLY);
//    perror("stage 0.25");
//    FILE *config = fdopen(configFD, O_RDONLY);
//    perror("stage 0.5");
    size_t bufsize = 50;
//    char **configLines;
//    perror("stage 1");
//
//
//    //getting the config file lines into arrays.
//    for (int i = 0; i < 3; ++i) {
//        configLines[i] = (char *) malloc(bufsize * sizeof(char));
//        if (configLines[i] == NULL) {
//            perror("Unable to allocate buffer");
//            exit(1);
//        }
//        getline(&configLines[i], &bufsize, config);
//    }


    //using fork to get the students names from directories
    perror("stage 2");
    int pid = fork();
    if (pid < 0)
        printf("fork error");


    else if (pid == 0) {//getting the names of the students.
        int fd = open("./dirList.txt", O_WRONLY | O_CREAT);
        close(1);
        dup(fd);

        execvp("ls", (char *const *) configLines[0]);
        printf("Exec Error!");


    } else { //father
        FILE *students = fopen("./dirList", O_RDONLY);
        char **names;

        int count = 0;
        names[count] = (char *) malloc(bufsize * sizeof(char));
        while (getline(&names[count], &bufsize, students) > 0) {
            count++;
            names[count] = (char *) malloc(bufsize * sizeof(char));
        }

        for (int i = 0; i < count; ++i) {
            pid = fork();

            if (pid < 0)
                printf("fork error");

            if (pid == 0) {
                perror("stage 3");
                int inputFD = open(configLines[1], O_RDONLY);
                int outputFD = open("./program_output.txt", O_WRONLY | O_CREAT | O_TRUNC);
                dup2(inputFD, 0);
                dup2(outputFD, 1);
                execvp(strcat("./",(strcat(configLines[0], names[i]), "/main.exe")), (char *const *) "");
            }

            int pid2 = fork();
            if (pid2 < 0)
                printf("fork error");

            if (pid2 == 0) {
                perror("stage 4");
                int res = open("results.csv", O_WRONLY|O_CREAT|O_APPEND);
                dup2(res, 1);
                printf("%s,",names[i]);
                execl("./comp.out", strcat("program_output.txt ",configLines[2]));
            }
        }

        FILE *results = fopen("./results.csv", O_RDONLY);

        perror("stage 5");
        char* result = (char *) malloc(bufsize * sizeof(char));
        for (int i = 0; i < count; ++i) {
            getline(&result, &bufsize, results);
            perror(result);
        }
    }
}