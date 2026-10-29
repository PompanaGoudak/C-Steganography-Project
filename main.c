#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "common.h"

int main(int argc, char *argv[])
{
    EncodeInfo encodeInfo;
    DecodeInfo decodeInfo;

    if( argc == 1 )
    {
	   printf("Error: Invalid arguments\n");
	   return 0;
    }

    int opr = check_operation(argv[1]);

    if( opr == encode )
    {
        if( argc < 4 || argc > 5 )
        {
            printf("Error:Invalid encoding Arguemments");
            return 0;
        }
        if (do_encoding(argv, &encodeInfo) == failure)
        {
            printf("Encoding failed\n");
            return 0;
        }

        printf("Encoding completed successfully\n");

    }
    else if( opr == decode )
    {
        if( argc < 3 || argc > 4 )	
        {
            printf("Error:Invalid decoding Arguemments");
            return 0;
        }
        if (do_decoding(argv, &decodeInfo) == failure)
        {
            printf("Decoding failed\n");
            return 0;
        }

        printf("Decoding completed successfully\n");
        
    }
    else
    {
	    printf("unsupported Arguemments");
    }
    return 0;
}
