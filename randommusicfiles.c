#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void shuffleFiles(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char **files = NULL;
    int fileCount = 0;

    while ((entry = readdir(dir)) != NULL) {
        struct stat fileStat;
        char filePath[256];
        sprintf(filePath, "%s/%s", path, entry->d_name);
        if (stat(filePath, &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
            files = (char **)realloc(files, (fileCount + 1) * sizeof(char *));
            files[fileCount] = strdup(entry->d_name);
            fileCount++;
        }
    }

    closedir(dir);

    srand(time(NULL));

    for (int i = fileCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = files[i];
        files[i] = files[j];
        files[j] = temp;
    }

    char tempPath[256];
    sprintf(tempPath, "%s/temp", path);
    mkdir(tempPath);

    for (int i = 0; i < fileCount; i++) {
        char oldPath[256];
        char newPath[256];
        sprintf(oldPath, "%s/%s", path, files[i]);
        sprintf(newPath, "%s/%s", tempPath, files[i]);
        if (rename(oldPath, newPath) != 0) {
            perror("Error when renaming file");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < fileCount; i++) {
        char oldPath[256];
        char newPath[256];
        sprintf(oldPath, "%s/%s", tempPath, files[i]);
        sprintf(newPath, "%s/%d_%s", path, i + 1, files[i]);
        if (rename(oldPath, newPath) != 0) {
            perror("Error when renaming file");
            exit(EXIT_FAILURE);
        }
    }

    rmdir(tempPath);

    for (int i = 0; i < fileCount; i++) {
        free(files[i]);
    }
    free(files);
}

int main() {
    const char *path = "C:/Users/gusta/Downloads/musicas"; // put music directory
    shuffleFiles(path);

    printf("The songs were successfully randomized!\n");

    return 0;
}
