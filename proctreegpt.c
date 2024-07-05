#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define PATH_MAX 4096

void traverse_directory(char *path, int level);

int main(int argc, char **argv) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "getcwd failed\n");
        return 1;
    }

    traverse_directory(cwd, 0);
    return 0;
}

void traverse_directory(char *path, int level) {
    DIR *dp;
    struct dirent *dirp;
    struct stat buf;

    if ((dp = opendir(path)) == NULL) {
        fprintf(stderr, "opendir failed\n");
        return;
    }

    while ((dirp = readdir(dp)) != NULL) {
        char new_path[PATH_MAX];

        snprintf(new_path, sizeof(new_path), "%s/%s", path, dirp->d_name);

        if (lstat(new_path, &buf) < 0) {
            fprintf(stderr, "lstat failed\n");
            continue;
        }

        for (int i = 0; i < level; i++) {
            printf("\t");
        }
        printf("%s %s a directory\n", dirp->d_name, S_ISDIR(buf.st_mode) ? "is" : "is not");

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
                closedir(dp);
                traverse_directory(new_path, level + 1);
                return; // child exits after traversing its subdirectory
            }
        }
    }
    closedir(dp);
}
