#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    //not the right amount of arguments
    if (argc!=3){
        return 1;
    }

    //open the files
    int a = open(argv[1], O_RDONLY);
    int b = open(argv[2], O_RDONLY);

    //if the files didn't open correctly
    if (a == -1 || b==-1) {
        return 1;
    }

    char ca, cb;

    int ia =read(a,&ca,1);
    int ib=read(b,&cb,1);

    while(ia==1 && ib ==1) {
        if (ca==cb){
//            printf("%c, %c\n", ca,cb);
            ia =read(a,&ca,1);
            ib=read(b,&cb,1);
            continue;
        }
        else {
//            printf("not ok1, ca: %c, cb: %c\n", ca, cb);
            return 1;
        }

    }

    //if both files reached the EOF together
    if (ia==0 && ib ==0){
//        printf("not ok2\n");
        return 2;
    }

    else {
//        printf("%d", ca);
//        printf("%d", cb);
//        printf("not ok3\n");
        return 1;
    }
}