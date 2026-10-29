
#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "common.h"

typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *stego_image_fptr;

    char output_fname[100];
    char secret_fname[120];
    FILE *secret_fptr;

    char secret_extn[8];
    int secret_extn_size;
    unsigned int secret_file_size;

} DecodeInfo;

/* Validate command-line arguments */
Status validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open stego image */
Status open_image_files(DecodeInfo *decInfo);

/* Skip BMP header */
Status skip_bmp_header(DecodeInfo *decInfo);

/* Decode one byte and four bytes */
char decode_1byte_from_lsb(char *buffer);
int decode_4byte_from_lsb(char *buffer);

/* Decode secret information */
Status decode_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Main decoding function */
Status do_decoding(char *argv[], DecodeInfo *decInfo);

#endif
