#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "md5.h"

const int PASS_LEN=50;        // Maximum any password can be
const int HASH_LEN=33;        // Length of MD5 hash strings


// Stucture to hold both a plaintext password and a hash.
struct entry 
{
    // TODO: FILL THIS IN
    char password[50];
    char hash[33];
};

int file_length(char *filename)
{
    struct stat info;
    stat(filename, &info);
    return info.st_size;
}

int eecomp(const void *a, const void *b)
{
    struct entry *ca = (struct entry *)a;
    struct entry *cb = (struct entry *)b;
    return strcmp(ca->password, (*cb).password);
}

// TODO
// Read in the dictionary file and return an array of structs.
// Each entry should contain both the hash and the dictionary
// word.
struct entry *read_dictionary(char *filename, int *size)
{
    *size = 0;
    
    int filelength = file_length(filename);
    FILE *d = fopen(filename, "r");
    if (!d)
    {
        printf("Can't open %s for reading\n", filename);
        exit(1);
    }
    
    char *contents = malloc(filelength);
    fread(contents, 1, filelength, d);
    fclose(d);
    
    // Loop through contents, replace \n with \0
    int lines = 0;
    for (int i = 0; i < filelength; i++)
    {
        if (contents[i] == '\n') {
            contents[i] = '\0';
            lines++;
        }
        //printf("%c", contents[i]);
    }
    printf("lines %d\n", lines);
    
    //char password[50];
    
    struct entry *ee = malloc(lines * sizeof(struct entry));
    strcpy(ee[0].password, &contents[0]);
    
    strcpy(ee[0].hash, md5(ee[0].password, strlen(ee[0].password)));
    
    int count = 1;
    for (int i = 0; i < filelength; i++)
    {
        if (contents[i] == '\0')
        {
            char *nextee = &contents[i] + 1;

            strcpy(ee[count].password, nextee);
            strcpy(ee[count].hash, md5(ee[count].password, strlen(ee[count].password)));
            count++;
        }
    }
    
    qsort(ee, lines, sizeof(struct entry), eecomp);
    
    *size = lines;
    return ee;
}

int ebcomp(const void *target, const void *elem)
{
    char *target_str = (char *)target;
    struct entry *eelem = (struct entry *)elem;
    return strcmp(target_str, (*eelem).password);
}

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    int dlen;
    // TODO: Read the dictionary file into an array of entry structures
    struct entry *dict = read_dictionary(argv[2], &dlen);
    
    // TODO: Sort the hashed dictionary using qsort.
    // You will need to provide a comparison function.
    qsort(dict, dlen, sizeof(struct entry), eecomp);

    // TODO
    // Open the hash file for reading.
    FILE *h = fopen(argv[1], "r");
    if(!h)
    {
        printf("Cant open %s for reading.", argv[1]);
        exit(1);
    }
    
    // TODO
    // For each hash, search for it in the dictionary using
    // binary search.
    // If you find it, get the corresponding plaintext dictionary word.
    // Print out both the hash and word.
    // Need only one loop. (Yay!)
    char hashes[33];
    while(fgets(hashes, 33, h))
    {
        hashes[strlen(hashes)-1] = '\0';
        
        struct entry *found = bsearch(hashes, dict, dlen, sizeof(struct entry), ebcomp);
        if (found == NULL)
        {
            printf("Not found\n");
        }
        else
        {
            printf("Found %s %s\n", found->password, found->hash);
        }
        
        
    }
    
}
