#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

void recover_File();
bool verifyUsage(int c, char *v[]);
bool check_JPEG_header(unsigned char b[]);
void read_file(unsigned char b[]);
void write_file(unsigned char b[]);
void create_file(int fNumber);

FILE *in_pointer = NULL;                        // datas file's pointer
FILE *out_pointer = NULL;                       // image file's pointer

int main(int argc, char *argv[])
{

    if (verifyUsage(argc, argv))                // if usage ok, continue algorithm
    {
        recover_File();
    }
    else
    {
        return 1;
    }

}

bool verifyUsage(int c, char *v[])              // verify commands usage
{
    if (c != 2)
    {
        printf("Usage: ./recover image\n");    // print usage if number of arguments is incorrect and return false
        return false;
    }
    else
    {
        in_pointer = fopen(v[1], "r");

        if (!in_pointer)
        {
            printf("Usage: ./recover image\n");    // print usage if file name is incorrect.
            return false;
        }
    }

    return true;
}

void recover_File()                                         // recover file algorithm
{

    unsigned char data_buffer[512];                         // buffer with data to be written
    bool first_JPEG = true;                                 // flag for first JPEG data_buffer
    int file_number = 0;                                    // file's name number

    while (true)
    {
        read_file(data_buffer);                             // read file and store data in data_buffer
        
        if (feof(in_pointer))                               // if data file's pointer is not at end
        {
            break;
        }
        
        if (check_JPEG_header(data_buffer))                 // verify if contains JPEG header
        {
            if (!first_JPEG)
            {
                fclose(out_pointer);
            }

            create_file(file_number);                       
            write_file(data_buffer);
            
            file_number++;
            
            first_JPEG = false;
        }
        
        else if (!first_JPEG)
        {
            write_file(data_buffer);
        }
    }
}

void read_file(unsigned char b[])                       // read file and store at buffer
{
    fread(b, sizeof(char), 512, in_pointer);
}

void create_file(int fNumber)                           // create file and point image's pointer
{
    char file_name[8];

    sprintf(file_name, "%03i.jpg", fNumber);
    
    out_pointer = fopen(file_name, "w");
    
}

void write_file(unsigned char b[])                      // write file from buffer
{
    fwrite(b, sizeof(char), 512, out_pointer);
}

bool check_JPEG_header(unsigned char b[])               //check if there is a JPEG header sequence
{

    if ((b[0] == 0xff) && 
        (b[1] == 0xd8) && 
        (b[2] == 0xff) && 
        (b[3] & 0xf0) == 0xe0)           
    {
        return true;
    }

    return false;
}