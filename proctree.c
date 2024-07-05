#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define PATH_MAX 4096

int main(int argc, char **argv) {
    DIR *dp;
    struct dirent *dirp;
    //int n;
    //lint buf[BUFFSIZE];
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "getcwd failed\n");
        return 1;
    }

    if ((dp = opendir(cwd)) == NULL) {
        fprintf(stderr, "opendir failed\n");
        return 1;
    }


    struct stat buf;
    int level = 0;


    printf(".\n");

    while ((dirp = readdir(dp)) != NULL) {
        char abspath[PATH_MAX];

        if (!strcmp(dirp->dname, "..")) continue;
        if (!strcmp(dirp->dname, ".")) continue;

        snprintf(abspath, sizeof(abspath), "%s/%s", cwd, dirp->d_name);

        if (lstat(abspath, &buf) < 0) {
            fprintf(stderr, "lstat failed\n");
            return 1;
        }
        for (int i = level; i; i--) {
            if (i == 1) printf("--");
            printf("");
        }
        printf("%s %s a directory\n ", dirp->d_name, S_ISDIR(buf.st_mode) ? "is" : "is not");
        if (S_ISDIR(buf.st_mode) && !(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)) {
            pid_t pid;
            if ((pid = fork()) < 0) {
                fprintf(stderr, "fork error\n");
            } else if (pid != 0) { /* parent */
                int status; 
                if (wait(&status) < 0) {
                    fprintf(stderr, "err in child, exiting\n");
                }
            } else { /* child */
                ++level;
                strcat(cwd, "/"); strcat(cwd, dirp->d_name);
                printf("%s\n", cwd);
                closedir(dp);
                if ((dp = opendir(cwd)) == NULL) {
                    fprintf(stderr, "opendir failed\n");
                    return 1;
                }
            }


        }
    }
    closedir(dp);
    return 0;
}
