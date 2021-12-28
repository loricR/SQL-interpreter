//
// Created by flassabe on 16/11/2021.
//

#include "database.h"

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>

#include "utils.h"

void create_db_directory(char *name) {
    if (!directory_exists(name)) {
        mkdir(name, S_IRWXU);
    }
}

void recursive_rmdir(char *dirname) {
    DIR *rep = opendir(dirname);
    size_t path_len = strlen(dirname);

    if (rep == NULL) {
        printf("No file or directory found.\n");
    } else {
        struct dirent *ptr;
        char *buf;
        size_t len;

        while ((ptr = readdir(rep)) != NULL) {
            int r = -1;
            struct stat statbuf;

            // Skip the names "." and ".."
            if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, "..")) {
                continue;
            }

            len = path_len + strlen(ptr->d_name) + 2;
            buf = (char *) malloc(sizeof(char) * len);
            sprintf(buf,"%s/%s",dirname,ptr->d_name);

            if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode)) {
                    //r = unlinkat(dirfd(rep),buf,AT_REMOVEDIR);
                    r = rmdir(buf);
                    if (r != 0) {
                        recursive_rmdir(buf);
                        //r = unlinkat(dirfd(rep),buf,AT_REMOVEDIR);
                        r = rmdir(buf);
                    }
                } else {
                    unlink(buf);
                }
            }
            free(buf);
        }
        rmdir(dirname);
    }
    closedir(rep);
}
