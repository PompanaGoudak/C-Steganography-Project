#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "common.h"


Status do_decoding(char *argv[], DecodeInfo *decInfo)
{
    printf("\n-----------DECODING STARTED ----------\n");

    if (validate_decode_args(argv, decInfo) == failure)
    {
        printf("Error: Argument validation failed\n");
        return failure;
    }



    if (open_image_files(decInfo) == failure)
    {
        printf("Error: File opening failed\n");
        return failure;
    }
    printf("SUCCESS: Stego image opened: %s\n",
           decInfo->stego_image_fname);

    if (skip_bmp_header(decInfo) == failure)
    {
        printf("Error: BMP header skipping failed\n");
        return failure;
    }
    printf("SUCCESS: BMP header skipped (54 bytes)\n");

    if (decode_magic_string(decInfo) == failure)
    {
        printf("Error: Magic string decoding failed\n");
        return failure;
    }

    if (decode_secret_file_extn(decInfo) == failure)
    {
        printf("Error: Secret file extension decoding failed\n");
        return failure;
    }

    if (decode_secret_file_data(decInfo) == failure)
    {
        printf("Error: Secret file data decoding failed\n");
        return failure;
    }

    fclose(decInfo->stego_image_fptr);
    fclose(decInfo->secret_fptr);

    
    return success;
}



Status validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    int i;
    char *ext;

    /* Check stego image extension */
    ext = strrchr(argv[2], '.');

    if (ext == NULL || strcmp(ext, ".bmp") != 0)
    {
        printf("Error: Invalid stego image. Use .bmp file\n");
        return failure;
    }
    else
    {
        decInfo->stego_image_fname = argv[2];
    }
    printf("Input stego image: %s\n", argv[2]);
    printf("Stego image validation successful\n");
    printf("Stego image filename: %s\n",decInfo->stego_image_fname);

    /* Check output filename */
    if (argv[3] == NULL)
    {
        strcpy(decInfo->output_fname, "decoded");
    }
    else
    {
        /* Check whether extension is present */
        for (i = 0; argv[3][i] != '\0'; i++)
        {
            if (argv[3][i] == '.')
            {
                break;
            }
        }

        if (argv[3][i] == '.')
        {
            /* Extension is present: remove it */
            strncpy(decInfo->output_fname, argv[3], i);
            decInfo->output_fname[i] = '\0';
        }
        else
        {
            /* No extension: store filename directly */
            strcpy(decInfo->output_fname, argv[3]);
        }
    }
    printf("Output filename base: %s\n",decInfo->output_fname);

    return success;
}
Status open_image_files(DecodeInfo *decInfo)
{
    decInfo->stego_image_fptr = fopen(decInfo->stego_image_fname, "r");

    if (decInfo->stego_image_fptr == NULL)
{
    printf("ERROR: Unable to open stego image\n");
    return failure;
}
printf("SUCCESS: Stego image opened\n");

    return success;
}
Status skip_bmp_header(DecodeInfo *decInfo)
{
    if (fseek(decInfo->stego_image_fptr, 54, SEEK_SET) != 0)
    {
        printf("Error: Unable to skip BMP header\n");
        return failure;
    }

    return success;
}
char decode_1byte_from_lsb(char *buffer)
{
    int i;
    char ch = 0;

    for (i = 0; i < 8; i++)
    {
        ch = (ch << 1) | (buffer[i] & 1);
    }

    return ch;
}
int decode_4byte_from_lsb(char *buffer)
{
    int i;
    unsigned int num = 0;

    for (i = 0; i < 32; i++)
    {
        num = (num << 1) | ((unsigned char)buffer[i] & 1U);
    }

    return (int)num;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    int i;
    char buffer[8];
    char magic_string[strlen(MAGIC_STRING) + 1];

    for (i = 0; i < strlen(MAGIC_STRING); i++)
    {
        if (fread(buffer, 1, 8, decInfo->stego_image_fptr) != 8)
        {
            return failure;
        }

        magic_string[i] = decode_1byte_from_lsb(buffer);
        printf("Decoded magic character %d: '%c' (ASCII: %d)\n",i, magic_string[i], (unsigned char)magic_string[i]);

        if (magic_string[i] != MAGIC_STRING[i])
        {
            printf("Error: Magic string does not match\n");
            return failure;
        }
    }

    magic_string[i] = '\0';
    printf("Expected magic string: %s\n", MAGIC_STRING);
    printf("Decoded magic string:  %s\n", magic_string);


    printf("Magic string decoded successfully: %s\n", magic_string);

    return success;
}


Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    int i;
    char buffer[32];
    char buffer_8[8];

    /* Decode extension size */
    if (fread(buffer, 1, 32, decInfo->stego_image_fptr) != 32)
    {
        printf("Error: Unable to read extension size\n");
        return failure;
    }

    decInfo->secret_extn_size = decode_4byte_from_lsb(buffer);
    printf("Decoded extension size: %d\n",decInfo->secret_extn_size);

    if (decInfo->secret_extn_size <= 0 ||
        decInfo->secret_extn_size >= sizeof(decInfo->secret_extn))
    {
        printf("Error: Invalid extension size\n");
        return failure;
    }

    /* Decode extension characters */
    for (i = 0; i < decInfo->secret_extn_size; i++)
    {
        if (fread(buffer_8, 1, 8, decInfo->stego_image_fptr) != 8)
        {
            printf("Error: Unable to read extension\n");
            return failure;
        }

        decInfo->secret_extn[i] = decode_1byte_from_lsb(buffer_8);
        printf("Decoded extension character %d: %c\n",i, decInfo->secret_extn[i]);
    }

    decInfo->secret_extn[i] = '\0';
    printf("Decoded extension: %s\n", decInfo->secret_extn);

    /* Create output filename */
    strcpy(decInfo->secret_fname, decInfo->output_fname);
    strcat(decInfo->secret_fname, decInfo->secret_extn);

    /* Open output file */
    decInfo->secret_fptr = fopen(decInfo->secret_fname, "w");

    if (decInfo->secret_fptr == NULL)
    {
        printf("Error: Unable to open secret output file\n");
        return failure;
    }

    printf("Secret output file created: %s\n", decInfo->secret_fname);
    return success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    int i;
    char buffer[32];
    char data;

    /* Decode secret file size */
    if (fread(buffer, 1, 32, decInfo->stego_image_fptr) != 32)
    {
        printf("Error: Unable to read secret file size\n");
        return failure;
    }

    decInfo->secret_file_size = decode_4byte_from_lsb(buffer);

    printf("Secret file size: %u bytes\n",decInfo->secret_file_size);

    /* Decode secret file data */
    for (i = 0; i < decInfo->secret_file_size; i++)
    {
        if (fread(buffer, 1, 8, decInfo->stego_image_fptr) != 8)
        {
            printf("Error: Unable to read secret data\n");
            return failure;
        }

        data = decode_1byte_from_lsb(buffer);

        if (fwrite(&data, 1, 1, decInfo->secret_fptr) != 1)
        {
            printf("Error: Unable to write secret data\n");
            return failure;
        }
    }
    printf("Total decoded bytes: %u\n",decInfo->secret_file_size);

    printf("Secret file data decoded successfully\n");

    return success;
}