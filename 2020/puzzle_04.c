#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define STRPOOL_IMPLEMENTATION
#include "strpool.h"

int read_file_to_buffer(const char *filename,char **buffer,size_t *len)
{
    if (!(buffer && len))
    {
        return 0;
    }

    int result = 1;
    size_t string_size = 0,read_size = 0;
    FILE *file = fopen(filename,"r");

    if (file)
    {
        fseek(file,0,SEEK_END);
        long tell = ftell(file);

        if (tell < 0)
        {
            fclose(file);
            return 0;
        }

        string_size = (size_t)tell;

        rewind(file);
        char *mem = (char *)malloc(string_size + 1);
        if (mem == NULL)
        {
            fclose(file);
            return 0;
        }

        read_size = fread(mem,sizeof(char),string_size,file);
        mem[read_size] = '\0';

        *buffer = mem;
        *len = read_size;

        fclose(file);
        return 1;
    }

    return 0;
}

enum fields
{
    f_none,
    f_birth = 1,
    f_issue = 2,
    f_expir = 4,
    f_height = 8,
    f_hair = 16,
    f_eyes = 32,
    f_pid = 64,
};

int s3cmp(char *s,char *o)
{
    return s[0] == o[0] && s[1] == o[1] && s[2] == o[2];
}

enum fields get_field(char *s)
{
    if (s[3] == ':')
    {
        if (s3cmp(s,"byr"))
            return f_birth;
        else if (s3cmp(s,"iyr"))
            return f_issue;
        else if (s3cmp(s,"eyr"))
            return f_expir;
        else if (s3cmp(s,"hgt"))
            return f_height;
        else if (s3cmp(s,"hcl"))
            return f_hair;
        else if (s3cmp(s,"ecl"))
            return f_eyes;
        else if (s3cmp(s,"pid"))
            return f_pid;
    }

    return f_none;
}

struct passport
{
    STRPOOL_U64 str_id;
    unsigned char fields;
};

struct passport *passports = NULL;

int passport_cmp(void const *const a,void const *const b)
{
    if (((struct passport *)a)->fields < ((struct passport *)b)->fields)
    {
        return -1;
    }
    else if (((struct passport *)a)->fields > ((struct passport *)b)->fields)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main(int argc,char *argv[])
{
    char *buffer;
    size_t len;
    if (!read_file_to_buffer("input_04.txt",&buffer,&len))
    {
        printf("Failed to read file.\n");
        return 0;
    }

    strpool_config_t conf = strpool_default_config;
    strpool_t pool;
    strpool_init(&pool,&conf);

    int valid_count = 0;
    int passport_count = 0;
    unsigned char fields = 0;
    int idx = 0;
    int nlc = 0;
    int istart = 0;
    while (idx < len - 4)
    {
        if (buffer[idx] == '\n')
        {
            nlc++;
            if (nlc == 2)
            {
                if (fields == 127)
                    valid_count++;

                arrput(passports,((struct passport){
                                      .str_id = strpool_inject(&pool,buffer + istart,idx - istart - 1),
                                      .fields = fields,
                                  }));
                fields = 0;
                passport_count++;
                istart = idx;
            }
        }
        else
        {
            nlc = 0;
            enum fields f = get_field(&buffer[idx]);
            if (f != f_none)
            {
                fields |= f;
            }
        }
        idx++;
    }

    printf("Of %d passports,%d are valid.\n",passport_count,valid_count);

    qsort(passports,arrlen(passports),sizeof(struct passport),passport_cmp);

    int count = arrlen(passports);
    for (int i = 0; i < count; ++i)
    {
        const char *s = strpool_cstr(&pool,passports[i].str_id);
        unsigned char f = passports[i].fields;
        if (f == 0x1f || f == 0x5e || f == 0x75)
        {
            printf("0x%02x:%s\n\n",f,s);
        }
        else
        {
            printf("0x%02x\n",f);
        }
    }

    getc(stdin);

    strpool_term(&pool);
    return 0;
}