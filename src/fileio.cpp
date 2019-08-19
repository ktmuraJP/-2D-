#include "src.h"
#include<fstream>


std::string loadfile(const char *path)
{
    std::ifstream file;
    // open the file stream
    file.open(path);
    // check if opening a file failed
    if (file.fail()) {
        fprintf(stderr,"file input error\n");
        file.close();
        exit(1);
    }
    std::string str;
    std::string line;
    while (getline(file, line))
        str = str + line + "\n";
    str.pop_back();

    // close the file stream
    file.close();
    return str;
}

void filewrite(const char *filename,const char *fstring)
{
    FILE *fp;
    
    fp = fopen(filename,"w");
    if(fp == NULL){
        fprintf(stderr,"file output error\n");
        exit(EXIT_FAILURE);
    }
    fputs(fstring,fp);
    fclose(fp);
}
