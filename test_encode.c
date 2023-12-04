#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *agrv[])                                                 //main function
{
    int opr;
    EncodeInfo encInfo;                                                          //declarated structure
    DecodeInfo decInfo;
    opr = check_operation_type(agrv);                                            //function call
    if(opr == 0)                                                                 //this condition is true,then encoding the process is executed 
    {
//	printf("Encoding is Selected\n");
	int res = read_and_validate_encode_args(argc,agrv,&encInfo);             //function call for read and validation
	if(res == 0)
	{
		//printf("Read and validate encode args sucess\n");
		if( do_encoding(&encInfo) == 0)                                  //function call for do_encoding
		{
			printf("## Encoding Done successfully ##\n");
		}
		else
		{
			printf("Encoding is Unfinished\n");
			
		}

	}
	else
	{
	    printf("Read and validate encode args failed\n");
	}
    }
    else if(opr == 1)
    {
//	printf("decoding is selected\n");
	int res = read_and_validate_decode_args(argc,agrv,&decInfo);           // function call for read and validation
	if(res == 0)
	{
		//printf("Read and validate encode args sucess\n");
		if( do_decoding(&decInfo) == 0)                               //function call for do_decoding
		{
			printf("## Decoding Done successfully ##\n");
		}
		else
		{
			printf("decoding is Unfinished\n");
			
		}

	}
	else                                                             
	{
	    printf("Read and validate encode args failed\n");
	}
    }
    else                                                                          
    {
	printf("please pass the correct option\n");
    }



    return 0;
}

OperationType check_operation_type(char *agrv[])             //check the operation type
{
    if ( strcmp(agrv[1],"-e") == 0 )
	return e_encode;   //return 0
    else if ( strcmp(agrv[1],"-d" ) == 0 )
	return e_decode;   //return 1
    else
	return e_unsupported; //return 2
}


