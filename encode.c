#include <stdio.h>                                      //header file
#include<string.h>                                      //string libary
#include "encode.h"                                     // user header file for function declaration
#include "types.h"                                      // user header file for datatype
#include "common.h"                                     // user header file for magic string

/* Function Definitions */
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo)   //check the all arguments are correct,then return e_sucess(0)
{
    if(argc < 5)
    {
    if(strcmp (strstr(argv[2],".bmp"),".bmp") == 0)                       //check the argv[2] id bmp file or not,its bmp file then stored.
    {
        encInfo -> src_image_fname = argv[2];
    }
    else                                                                  //its not bmp file,then print the error msg 
    {
        printf("please pass the bmp file\n");
        return e_failure;
    }

    if(strcmp (strstr(argv[3],".txt"),".txt") == 0)                       //check the argv[3] is txt file or not,its txt file then stored.
    {
        encInfo -> secret_fname = argv[3];                                
        strcpy( encInfo -> extn_secret_file,".txt");                       //copy the extn using string libary
    }
    else                                                                   //its not txt file,then print the error msg
    {
        printf("please pass the secret file\n");
        return e_failure;
    }

    if(argv[4] != NULL )                                                   //check the argv[4] any file are presents or not
    {
        encInfo -> stego_image_fname = argv[4];                            //its present file,then stored file
    }
    else
    {
        //printf("Default file name added\n");
        encInfo -> stego_image_fname = "stego.bmp";                        //its not present file,then default file name added
    }
    return e_success; 
    }
    else
    {
	printf("pass the argument ");
	return e_failure;
    }
    return e_success;


}

//////////////////////////////////////////////////////////////////////////////////////////////////////

Status do_encoding(EncodeInfo *encInfo)                                     //function declaration
{
    printf("INFO: Opening required files\n");
    if(open_files(encInfo) == 0)                                            //open_files function call and its function return the e_success,then prints the successful message	
    {
        printf("## Encoding Procedure Started ##\n");
    }
    else                                                              
    {
        printf("Open files failed\n");
        return e_failure;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////

    if(check_capacity(encInfo) == 0)                                        //check_capacity function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Checking for beautiful.bmp caoacity to handle secret.txt\n");
	printf("INFO: Done. Found ok\n");
    }
    else
    {
        printf("check_capacity failure\n");
        return e_failure;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == 0)   //copy_bmp_header function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Copy image header\n");
	printf("INFO: Done\n");
    }
    else
    {
        printf("copied bmp header failed\n");
        return e_failure;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    if(encode_magic_string(MAGIC_STRING,encInfo) == 0)                                 //encode_magic_string function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Endocind Magic String Singnature\n");
	printf("INFO: Done\n");
    }
    else
    {
        printf("encode_magic_string failure\n");
        return e_failure;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    if ( encode_secret_file_extn_size( strlen(encInfo -> extn_secret_file),encInfo) == 0)      //encode_secret_file_extn_size function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Encoding secret.txt File Extension Size\n");
	printf("INFO: Done\n");
    }
    else
    {
        printf("encode_secret_file_extn_size failure\n");
        return e_failure;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    if( encode_secret_file_extn(encInfo -> extn_secret_file,encInfo) == 0)                      //encode_secret_file_extn function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Encoding secret.txt File Extension\n");
	printf("INFO: Done\n");
    }
    else
    {
        printf("encode_secret_file_extn failure\n");
        return e_failure;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////

    if(encode_secret_file_size(encInfo -> size_secret_file,encInfo) == 0 )         //encode_secret_file_size function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Encoding secret.txt File size\n");
	printf("INFO: Done\n");
    }
    else
    {
        printf("encode_secret_file_size failure\n");
        return e_failure;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    if(encode_secret_file_data(encInfo) == 0)                                      //encode_secret_file_data function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Encoding secret.txt File Data\n");
	printf("INFO: Done\n");
    }
    else
    {
        printf("encode_secret_file_data failure\n");
        return e_failure;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == 0)    //copy_remaining_img_data function call and its function return the e_success,then prints the successful message
    {
        printf("INFO: Copying Left Over Data\n");
	printf("INFO: Done\n");
    }
    else
    {
        printf("copy_remaining_img_data failure\n");
        return e_failure;
    }

    return e_success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

Status check_capacity(EncodeInfo *encInfo)                 //function definition
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);      
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    if(encInfo->image_capacity > ((strlen(MAGIC_STRING)*8)+32+(strlen(encInfo ->extn_secret_file)*8)+32+((encInfo -> size_secret_file)*8)))
    {
    return e_success;
    }
    else
    {
	return e_failure;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
    char bmp_header[54];
    rewind(fptr_src_image);
    rewind(fptr_dest_image);
    fread(bmp_header,1,54,fptr_src_image);
    fwrite(bmp_header,1,54,fptr_dest_image);
    return e_success;
}




/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
   // printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");   // read the src image file and stored
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)                              //src image files is NULL ,then print the error msg
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }
    printf("INFO: Opened beautiful.bmp\n");

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");         //read the secret file and stored
    // Do Error handling
    if (encInfo->fptr_secret == NULL)                                 //secret file is NULL,then print the error msg
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }
    /*else
    {
        rewind(encInfo -> fptr_secret);
        fseek(encInfo -> fptr_secret, 0 , SEEK_END);
        encInfo -> size_secret_file = ftell(encInfo -> fptr_secret );        //find the secret file size and using ftell
    }*/
    printf("INFO: Opened Secret.txt\n");

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }
    printf("INFO: Opened steged_beautiful.bmp\n");
    printf("INFO: Done\n");


    // No failure return e_success
    return e_success;
}
//////////////////////////////////////////////////////////////////////////////////////

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);

}


///////////////////////////////////////////////////////////////////////////////////////

Status encode_byte_to_lsb(char data, char *image_buffer)                             //function definition for encode_byte_to_lsb
{
    for(int i=7;i>=0;i--)                                                           //get the  bit in data and that bit replace in image buffer's lsb bit 
    {
        image_buffer[7-i] = ((data & (1<<i)) >> i) |( image_buffer[i] & (~1));
    }
return e_success;
}
//////////////////////////////////////////////////////////////////////////////

Status encode_magic_string(char *magic_string,EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, strlen(magic_string), encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
    return e_success;
}

///////////////////////////////////////////////////////////////////////////////

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image) //function definition for encode data to image function
{
    char str[8];
    for(int i=0;i<size;i++)
    {
        fread(str,1,8,fptr_src_image);                                                          //read 8 bits from src image
        encode_byte_to_lsb(data[i],str);                                                        //function call
        fwrite(str,1,8,fptr_stego_image);                                                       //write in stego image
   
    }
return e_success;

}

/////////////////////////////////////////////////////////////////////////////////////
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)                               //function definition
{

    char str[32];
    fread(str,1,32,encInfo -> fptr_src_image);                                                  //read 32 bits from src image
    encode_size_to_lsb(size,str);                                                               //function call
    fwrite(str,1,32,encInfo -> fptr_stego_image);                                               //write in stego image
    return e_success;

}

///////////////////////////////////////////////////////////////////////////////////////

Status encode_size_to_lsb(int size, char *image_buffer)                                         //function definition
{
    for(int i=31;i>=0;i--)
    {
        image_buffer[31-i] = ((size & (1<<i)) >> i) |( image_buffer[i] & (~1));                 //           
    }
return e_success;
}

///////////////////////////////////////////////////////////////////////////////////////

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)             //function definition for file extension
{

    encode_data_to_image(file_extn, strlen(file_extn), encInfo -> fptr_src_image, encInfo -> fptr_stego_image); //function call 
        return e_success;
}

///////////////////////////////////////////////////////////////////////////////////////

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)             //function definition for secret file size
{
    char str[32];
    fread(str,1,32,encInfo -> fptr_src_image);                                  //read 32 bits from src image
    encode_size_to_lsb(file_size,str);                                          //function call
    fwrite(str,1,32,encInfo -> fptr_stego_image);                               //write in stego image
    return e_success;
}

//////////////////////////////////////////////////////////////////////////////////////

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char str[encInfo -> size_secret_file];
    rewind(encInfo -> fptr_secret);                                                     
    fread(str,1,encInfo -> size_secret_file,encInfo -> fptr_secret);
    encode_data_to_image(str,encInfo -> size_secret_file, encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
    return e_success;
}

///////////////////////////////////////////////////////////////////////////////////

Status copy_remaining_img_data(FILE *fptr_src,  FILE *fptr_dest)
{
    char ch;
    while ( fread(&ch,1,1,fptr_src) > 0)
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}
