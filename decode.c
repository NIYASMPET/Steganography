#include <stdio.h>                    //header file
#include "decode.h"                    // user header file for function declaration
#include "types.h"                    // user header file for datatype
#include "common.h"                   // user header file for magic string
#include <string.h>                   //string libary

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status read_and_validate_decode_args(int agrc,char *argv[],DecodeInfo *decInfo)    //check the all arguments are correct,then return e_sucess(0)
{   
    if(agrc < 4)
    {
    if(argv[2] != NULL)
    {	
	if(strcmp (strstr(argv[2],".bmp"),".bmp") == 0)                //check the argv[2] id bmp file or not,its bmp file then stored.
	{
    		decInfo->src_image_fname=argv[2];
	}
	else                                                          //its not bmp file,then print the error msg
	{
	    printf("its not bmp file");
	    return e_failure;
	}
    }
    else
    {
	printf("pass the bmp file");
	return e_failure;
    }

    if(argv[3]!=NULL)                              //check the argv[3] is txt file or not,its txt file then stored.
    {
	decInfo->secret_fname=argv[3];
    }
    else
    {
	decInfo->secret_fname="Message.txt";         //its not present file,then default file name added
    }

	printf("INFO: ## Decoding Procedure Started ##\n");


    if(decode_open_files(decInfo)==e_success)         //open_files function call and its function return the e_success,then prints the successful message
    {

	printf("INFO: open required file\n");
    }
 return e_success;
    }
    else
    {
	printf("pass the argument");
	return e_failure;
    }

    return e_success;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Status do_decoding(DecodeInfo *decInfo)             //function declaration
{
    if(decode_magic_string(MAGIC_STRING, decInfo) == 0 )   //decode_magic_string function call and its function return the e_success,then prints the successful message
    {	
	printf("INFO: Decoding Magic String Signature\n");
	printf("Done\n");
    }
    else
    {
	printf("ERROR : Does not Decoding Magic string signature\n");
	return e_failure;
    }

    //////////////////////////////////////////////////////////////////////////////////


    if(decode_secret_file_extn_size(strlen(decInfo->extn_secret_file),decInfo) == 0)   //decode_secret_file_extn_size function call and its function return the e_success,then prints the successful message
    {
    	printf("INFO: Decoding Secret.txt Extenstion Size\n");
    	printf("Done\n");
	
    }
    else
    {
	printf("ERROR : Do not Decoding secret_file_extn_size \n");
	return e_failure;
    }


    ///////////////////////////////////////////////////////////////////////////////////


    if(decode_secret_file_extn(decInfo->extn_secret_file,decInfo) == 0)        //decode_secret_file_extn function call and its function return the e_success,then prints the successful message
    {
	printf("INFO: Decoding secret.txt File Extenstion\n");
    	printf("Done\n");

    }
    else
    {
	printf("ERROR: Do not Decoding secret.txt File Extenstion\n");
	return e_failure;

    }


    ////////////////////////////////////////////////////////////////////////////////////


    if(decode_secret_file_size(decInfo->size_secret_file,decInfo) == 0)        //decode_secret_file_size function call and its function return the e_success,then prints the successful message
    {
    	printf("INFO: Decoding secret.txt File Size\n");
    	printf("Done\n");

    }
    else
    {
    	printf("ERROR: Do not Decoding secret.txt File Size\n");
	return e_failure;

    }


     //////////////////////////////////////////////////////////////////////////////////


    if(decode_secret_file_data(decInfo)  == 0 )           //decode_secret_file_data function call and its function return the e_success,then prints the successful message
    {
    	printf("INFO: Decoding secret.txt File Data\n");
    	printf("Done\n");

    }
    else
    {
    	printf("ERROR: Do not Decoding secret.txt File Data\n");
	return e_failure;

    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_open_files(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    if (decInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);
	return e_failure;
    }
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);

	return e_failure;
    }
    return e_success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_int_tolsb(long int *data, char *image_buffer)          //function definition for decode int to lsb function
{
    int index = 0;
    for(int i=31;i>=0;i--)
    {
	index=index|(image_buffer[31-i] & 1)<<i;                    //get lsb bit from image buffer and or with index value
    }
    *data = index;
return e_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_byte_tolsb(char *data,char *image_buffer)          //function definition for decode byte to lsb
{
    char ch=0;
    for(int i=7;i>=0;i--)
    {
	ch=ch|(image_buffer[7-i] & 1)<<i;                       //get lsb bit from image buffer and or with ch value
    }
    *data = ch;
return e_success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)      //function definition
{
    int i=0;
    char str[8];
    char data[2];
       fseek(decInfo->fptr_src_image, 54, SEEK_SET);               //set the src image position
    for(i=0;i<2;i++)
    {
	fread(str,1,8,decInfo->fptr_src_image);                   //read 8 bits from src image
	    decode_byte_tolsb(&data[i],str);                       //function call
    }
    return e_success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_secret_file_extn_size(long int size,DecodeInfo *decInfo)   //function definition
{
    char str1[32];
    fread(str1,1,32,decInfo->fptr_src_image);                       //read 32 bits from src image
    decode_int_tolsb(&size,str1);                                    //function call
    decInfo->ext_size = size;
    return e_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_secret_file_extn(char *ext, DecodeInfo *decInfo)          //function definition
{
    int i=0;
    char str[8];
    	for(i=0;i<decInfo->ext_size;i++)
    {
	fread(str,1,8,decInfo->fptr_src_image);                     //read 8 bits from src image
	decode_byte_tolsb(&ext[i],str);                               //function call
    }
    ext[i] = '\0';
    return e_success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_secret_file_size(long file_size, DecodeInfo *decInfo)        //function definition
{
    char data[32];
    fread(data,1,32,decInfo->fptr_src_image);                             //read 32 bits from src image
    decode_int_tolsb(&decInfo->size_secret_file,data);                     //function call
    return e_success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_secret_file_data(DecodeInfo *decInfo)                       //function definition
{
    int i=0;
    char str[8];
    char sec[decInfo->size_secret_file+1];
    sec[decInfo->size_secret_file] = '\0';
    for(i=0;i<decInfo->size_secret_file;i++)
    {
	sec[i]=0;
	fread(str,1,8,decInfo->fptr_src_image);                           //read 8 bits from src image
	decode_byte_tolsb(&sec[i],str);                                   //function call
    }
//	rewind(decInfo->fptr_secret);
    fwrite(sec,1,decInfo->size_secret_file,decInfo->fptr_secret);          
    return e_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

