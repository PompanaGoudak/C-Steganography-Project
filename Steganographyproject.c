#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "common.h"

/* Get image size
 * Input: Image file ptr
 * Output: width````` * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
unsigned int get_image_size_for_bmp(FILE *fptr_image)
{
    unsigned int width, height;
    // Seek to 18th byte
    printf("\n Getting BMP image size...\n");
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("Image width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("Image height = %u\n", height);

    printf("Image capacity = %u bytes\n", width * height * 3);
    // Return image capacity
    return width * height * 3;
}
unsigned int get_file_size(FILE *secret_fptr){
    unsigned int size;
    printf("\nGetting secret file size \n");
    if(secret_fptr==NULL){
        printf("Error:File cannot be opened");
        return 0;
    }
    fseek(secret_fptr,0,SEEK_END);
    size=ftell(secret_fptr);
    rewind(secret_fptr);
    printf("Secret file size = %u bytes\n", size);
    return size;
    
}
Opr_type check_operation(char *option){
    if(strcmp(option,"-e")==0){
        return encode;
    }
    else if(strcmp(option,"-d")==0){
        return decode;
    }
    else{
        return unsupported;
    }
}

Status do_encoding(char *argv[], EncodeInfo *encInfo)
{
     printf("\n-----------ENCODING STARTED ----------\n");

    if (validate_encode_args(argv, encInfo) == failure)
    {
        printf(" ERROR:Argument validation failed\n");    
        return failure;
    }

    if (open_files(encInfo) == failure){
        printf("Error:File opening failed\n");
        return failure;
    }

    if (check_capacity(encInfo) == failure){
        printf("ERROR :Image capacity is insufficient\n");
        return failure;
    }

    if (copy_bmp_header(encInfo) == failure){
         printf("Error:BMP header copying failed\n");
        return failure;
    }

    if (encode_magic_string(MAGIC_STRING, encInfo) == failure){
        printf("Error: Magic string encoding failed\n");
        return failure;
    }

    if (encode_secret_file_extn(encInfo) == failure){
        printf("Error: Secret file extension encoding failed\n");
        return failure;
    }

    rewind(encInfo->secret_fptr);
     printf("Secret file pointer reset to beginning\n");


    if (encode_secret_file_data(encInfo) == failure){
        printf("Error: Secret file data encoding failed\n");
        return failure;
    }

    if (copy_remaining_img_data(encInfo) == failure){
        printf("Error: Remaining image data copying failed\n");
        return failure;
    }
    
    return success;
}

Status validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *ext;
    printf("\n Validating source image: %s\n", argv[2]);

    ext = strrchr(argv[2], '.');

    if (ext != NULL && strcmp(ext, ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
        printf("Valid BMP image\n");
    }
    else
    {
        printf("Error: Source image must be .bmp\n");
        return failure;
    }

    /* Check secret file extension and store in char extn */
    printf("Validating secret file: %s\n", argv[3]);
    ext = strrchr(argv[3], '.');

    if (ext != NULL &&
        (strcmp(ext, ".txt") == 0 ||
         strcmp(ext, ".c") == 0 ||
         strcmp(ext, ".sh") == 0 ||
         strcmp(ext, ".cpp") == 0))
    {
        strcpy(encInfo->secret_extn, ext);
        encInfo->secret_extn_size = strlen(ext);
        encInfo->secret_fname = argv[3];
         printf(" Secret extension = %s\n",encInfo->secret_extn);
         printf("Extension length = %u\n",encInfo->secret_extn_size);
    }
    else
    {
        printf("Error: Invalid secret file\n");
        return failure;
    }

    /* Store output filename */
    if (argv[4] == NULL)
    {
        encInfo->output_image_fname = "stego.bmp";
    }
    else
    {
        ext = strrchr(argv[4], '.');

        if (ext != NULL && strcmp(ext, ".bmp") == 0)
        {
            encInfo->output_image_fname = argv[4];
        }
        else
        {
            printf("Error: Invalid output file\n");
            return failure;
        }
    }
    printf("Output image = %s\n",encInfo->output_image_fname);

    return success;
}
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: success or failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    encInfo->src_image_fptr = fopen(encInfo->src_image_fname, "r");

    if (encInfo->src_image_fptr == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return failure;
    }

    encInfo->secret_fptr = fopen(encInfo->secret_fname, "r");

    if (encInfo->secret_fptr == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return failure;
    }

    encInfo->output_image_fptr = fopen(encInfo->output_image_fname, "w");

    if (encInfo->output_image_fptr == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->output_image_fname);

    	return failure;
    }

    return success;
}
Status check_capacity(EncodeInfo *encInfo){
    unsigned int encoding_things;
    unsigned int image_capacity;
    printf("\nChecking encoding capacity\n");
    image_capacity=get_image_size_for_bmp(encInfo->src_image_fptr);
    encInfo->secret_file_size = get_file_size(encInfo->secret_fptr);
    encoding_things = (strlen(MAGIC_STRING) + sizeof(int) +
                   encInfo->secret_extn_size + sizeof(int) +
                   encInfo->secret_file_size) * 8;
    printf("Image capacity = %u bits\n",image_capacity);
    printf("Required encoding capacity = %u bits\n",encoding_things);
    printf("Secret file size = %lu bytes\n",encInfo->secret_file_size);

    
    
    if (image_capacity >= encoding_things)
    {
        printf("Enough capacity to encode\n");
        return success;
    }

    else
    {
        printf("Not enough capacity to encode\n");
        return failure;
    }
}
Status encode_1byte_to_lsb(char data, char *buffer_8){
    int i;
    unsigned char ch = (unsigned char)data;
    for(i=7;i>=0;i--){
        buffer_8[7 - i] =
            (buffer_8[7 - i] & 0xFE) |
            ((ch >> i) & 1U);

    }
    return success;
}

Status encode_4byte_to_lsb(int data, char *buffer_32)
{
    int i;
    unsigned int ch = (unsigned int)data;

    for (i = 31; i >= 0; i--)
    {
        buffer_32[31 - i] =
            (buffer_32[31 - i] & 0xFE) |
            ((ch >> i) & 1U);
    }
    
    return success;
}

Status copy_bmp_header(EncodeInfo *encInfo)
{
    char header[54];
    printf("Copying 54-byte BMP header\n");

    rewind(encInfo->src_image_fptr);

    if (fread(header, sizeof(header), 1,
              encInfo->src_image_fptr) != 1)
    {
        return failure;
    }

    if (fwrite(header, sizeof(header), 1,
               encInfo->output_image_fptr) != 1)
    {
        return failure;
    }

    return success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo){
    char img_bytes[8];
    int i;
    for (i = 0; magic_string[i] != '\0'; i++)
    {
        printf("Encoding magic character: %c\n",
       magic_string[i]);
        if (fread(img_bytes, sizeof(img_bytes), 1, encInfo->src_image_fptr) != 1)
        {
            return failure;
        }

        if (encode_1byte_to_lsb(magic_string[i], img_bytes) == failure)
        {
            return failure;
        }

        if (fwrite(img_bytes, sizeof(img_bytes), 1, encInfo->output_image_fptr) != 1)
        {
            return failure;
        }
    }

    return success;
}
Status encode_secret_file_extn(EncodeInfo *encInfo){
    char buffer[32];
    int i;

    /* Encode extension length */
    if (fread(buffer,sizeof(buffer),1, encInfo->src_image_fptr) != 1)
    {
        return failure;
    }

    if (encode_4byte_to_lsb(encInfo->secret_extn_size, buffer) == failure)
    {
        return failure;
    }

    if (fwrite(buffer,sizeof(buffer),1, encInfo->output_image_fptr) != 1)
    {
        return failure;
    }

    /* Encode extension characters */
    for (i = 0; i < encInfo->secret_extn_size; i++)
    {
        if (fread(buffer,8,1, encInfo->src_image_fptr) != 1)
        {
            return failure;
        }

        if (encode_1byte_to_lsb(encInfo->secret_extn[i], buffer) == failure)
        {
            return failure;
        }

        if (fwrite(buffer,8,1, encInfo->output_image_fptr) != 1)
        {
            return failure;
        }
    }

    return success;
}
Status encode_secret_file_data(EncodeInfo *encInfo){
    char data;
    char buffer[32];
    unsigned long int i;

    /* Encode secret file size */
    if (fread(buffer,sizeof(buffer),1, encInfo->src_image_fptr) != 1)
    {
        return failure;
    }

    if (encode_4byte_to_lsb((int)encInfo->secret_file_size, buffer) == failure)
    {
        return failure;
    }

    if (fwrite(buffer,sizeof(buffer),1, encInfo->output_image_fptr) != 1)
    {
        return failure;
    }

    /* Encode secret file data */
    for (i = 0; i < encInfo->secret_file_size; i++)
    {
        if (fread(&data,1,1, encInfo->secret_fptr) != 1)
        {
            return failure;
        }

        if (fread(buffer, 1, 8, encInfo->src_image_fptr) != 8)
        {
            return failure;
        }

        if (encode_1byte_to_lsb(data, buffer) == failure)
        {
            return failure;
        }

        if (fwrite(buffer,1,8, encInfo->output_image_fptr) != 8)
        {
            return failure;
        }
    }
    return success;
}
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    printf("Copying remaining image data...\n");
    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer),
                          encInfo->src_image_fptr)) > 0)
    {
        if (fwrite(buffer, 1, bytes,
                   encInfo->output_image_fptr) != bytes)
        {
            return failure;
        }
    }

    if (ferror(encInfo->src_image_fptr))
    {
        return failure;
    }
    printf("Remaining image data copied\n");

    return success;
}





