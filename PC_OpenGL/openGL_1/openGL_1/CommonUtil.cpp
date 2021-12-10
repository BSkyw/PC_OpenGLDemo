
//
//  CommonUtil.cpp
//  openGL_1
//
//  Created by Wky on 2020/7/27.
//  Copyright © 2020 wangky. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "CommonUtil.h"

char* CommonUtil::readFile(const char *fileName)
{
    char* result = 0;
    FILE* file = fopen(fileName, "r");
    if(file) {
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = (char*)malloc(fileSize + 1);
        fread(result, fileSize, 1, file);
        result[fileSize] = 0;

        fclose(file);
    }
    else {
        printf("cannot find file %s\n", fileName);
    }

    return result;
}
