# C Steganography Project Using LSB Technique

## 1. Introduction

Steganography is a technique used to hide secret information inside another file, such as an image, audio file, or video. The main purpose of steganography is to hide the existence of the secret information.

In this project, we use an image to hide a secret text file. The project is developed using the C programming language and uses a technique called **Least Significant Bit (LSB) steganography**.

The program takes an original BMP image and a secret file as input. It hides the secret file data inside the image and creates a new image named `stego.bmp`. The resulting image looks almost the same as the original image, but it contains hidden information.

The hidden data can later be extracted from `stego.bmp` using the decoding process.

This project helps us understand file handling, bitwise operators, pointers, structures, and how binary data is stored and processed in C.

## 2. Objectives

The main objectives of this project are:

- To understand the basic concept of steganography.
- To learn how secret data can be hidden inside an image.
- To implement the Least Significant Bit (LSB) technique using C.
- To encode a secret file into a BMP image.
- To decode and recover the hidden file from the encoded image.
- To understand how bits are changed using bitwise operators.
- To practice reading and writing binary files in C.
- To learn how to use file pointers and file-handling functions.
- To understand how structures and header files help organize a C project.
- To learn how command-line arguments are used to pass input files to a program.

## 3. Technologies Used

- **Programming Language:** C
- **Compiler:** GCC
- **Code Editor:** Visual Studio Code
- **Operating Environment:** Linux / WSL
- **Image Format:** BMP
- **Technique Used:** Least Significant Bit (LSB) steganography

## 4. What Is Steganography?

Steganography means hiding information inside another file so that other people may not easily notice that secret information is present.

For example, suppose we have a picture and a text file containing a secret message. Instead of sending the text file directly, we can hide its contents inside the picture and send the resulting image.

The receiver can then use a decoding program to extract the hidden message.

There are three important files in this example:

1. **Original image:** The image used to store the secret data.
2. **Secret file:** The file containing the information to be hidden.
3. **Stego image:** The new image containing the hidden information.

In this project, `beautiful.bmp` is the sample original image, `secret.txt` is the sample input file, and `stego.bmp` is the generated image containing the hidden data.

## 5. What Is the LSB Technique?

LSB stands for **Least Significant Bit**. It is the rightmost bit of a binary number and represents the smallest value in that number.

For example, consider the binary value:

`10110100`

The rightmost bit is `0`, which is the least significant bit.

In an image, pixel information is stored using bytes. In a typical 24-bit BMP image, each pixel uses three color components: red, green, and blue. Each component is usually stored using one byte.

The LSB technique hides information by changing the last bit of selected image bytes.

### Example

Suppose an image byte contains:

`10010110`

If the secret data bit is `1`, the program changes the last bit:

`10010111`

Only the last bit has changed, so the numerical value changes from 150 to 151.

This small change is usually difficult to notice in the image.

If the secret data bit is `0`, the program changes the byte to end in `0`. If it already ends in `0`, no change is needed.

By repeating this process across multiple image bytes, the program can hide a larger amount of data.

## 6. Project Working

The project has two main operations: encoding and decoding.

### 6.1 Encoding Process

Encoding means hiding the secret file inside the original image.

The encoding process works as follows:

1. The user provides the original BMP image and the secret file through command-line arguments.
2. The program checks the selected operation and validates the input filenames.
3. It opens the original image and the secret file.
4. It reads information from the image, including its dimensions, to determine its storage capacity.
5. It checks whether the image has enough capacity to store the secret data and required information.
6. It copies the BMP header into the output image so the resulting file remains a valid BMP image.
7. It writes a special marker called a magic string into the image. The marker helps the decoding program identify the hidden data format.
8. It stores the secret file extension and the size of the secret file.
9. It reads the secret file data and hides its bits inside the least significant bits of image bytes.
10. It copies the remaining image data as required by the implementation.
11. It closes the files and saves the final output as `stego.bmp`.

The output image can then be used in the decoding process.

### 6.2 Decoding Process

Decoding means recovering the hidden information from the stego image.

The decoding process works as follows:

1. The user provides the encoded BMP image as input.
2. The program opens the image and reads its data.
3. It extracts the magic string from the image and checks whether it matches the expected marker.
4. It retrieves the hidden secret file extension.
5. It extracts the size of the secret file.
6. It reads the required number of image bytes.
7. It collects the least significant bits from those bytes and combines them to reconstruct the original data.
8. It writes the recovered data into an output file.
9. It closes the files after decoding is complete.

If the encoding and decoding processes work correctly, the recovered file should contain the same data as the original secret file.

## 7. Project File Structure

The project contains the following files:

| File name | Purpose |
|---|---|
| `main.c` | Contains the main function and starts the encoding or decoding operation. |
| `encode.c` | Contains the functions used to hide secret data inside the image. |
| `encode.h` | Contains declarations and structures required by the encoding module. |
| `decode.c` | Contains the functions used to extract hidden data from the image. |
| `decode.h` | Contains declarations and structures required by the decoding module. |
| `common.h` | Contains common definitions, operation types, and status values used across the project. |
| `Steganographyproject.c` | Additional C source file included in the project folder. |
| `beautiful.bmp` | Sample BMP image used as the original input. |
| `secret.txt` | Sample secret file whose contents are hidden in the image. |
| `stego.bmp` | Output image containing the hidden secret data. |
| `decoded.txt` | Example output file containing recovered data. |
| `Steganography_Project.zip` | ZIP archive containing project materials. |

The files `a.out` and `stego` are compiled program files, so they do not need to be uploaded as source code.

## 8. Software Requirements

To compile and run the project, you need:

- A computer running Linux or Windows with WSL.
- GCC installed on the system.
- Visual Studio Code or another C editor.
- The source files and required input files.
- A terminal to compile and execute the program.

## 9. How to Compile the Program

Open the terminal in the project directory.

Compile the source files using GCC:

```bash
gcc main.c encode.c decode.c -o stego
```

Here:

- `gcc` is the C compiler.
- `main.c`, `encode.c`, and `decode.c` are the source files.
- `-o stego` specifies the name of the compiled executable.

If compilation succeeds, the executable named `stego` will be created.

## 10. How to Encode a Secret File

Run the following command:

```bash
./stego -e beautiful.bmp secret.txt
```

In this command:

- `./stego` runs the compiled program.
- `-e` selects encoding mode.
- `beautiful.bmp` is the original image.
- `secret.txt` is the file to hide.

If your program accepts these arguments and encoding succeeds, it creates `stego.bmp` containing the hidden secret data.

## 11. How to Decode the Secret File

The decoding command depends on the arguments implemented in `main.c`.

For example, if your program accepts the encoded image after the `-d` option, use:

```bash
./stego -d stego.bmp
```

The program should extract the hidden data and create an output file, such as `decoded.txt`, if that is how the decoding function is implemented.

Always use the exact command-line format supported by your source code.

## 12. Important C Programming Concepts Used

### File Handling

C file-handling functions are used to open, read, write, and close files. Examples include `fopen()`, `fread()`, `fwrite()`, and `fclose()`.

### Bitwise Operators

Bitwise operators work directly on the individual bits of a number. Operators such as `&`, `|`, `^`, `~`, `<<`, and `>>` are useful when extracting or modifying LSBs.

### Pointers

File pointers help the program access files. Pointers are also used to pass structures and buffers between functions.

### Structures

Structures group related information together. For example, an encoding structure can store filenames, file pointers, file sizes, and other information needed during encoding.

### Header Files

Header files contain function declarations, structure definitions, and common constants. They help organize the program into separate modules.

### Command-Line Arguments

Command-line arguments allow users to select an operation and provide filenames when running the program.

## 13. Advantages

- It hides information inside an image without creating a separate visible message file for transmission.
- The resulting image usually looks similar to the original image.
- It provides practical experience with bitwise operations and binary file handling.
- It separates encoding and decoding into different modules.
- It demonstrates a simple method of embedding data in image bytes.

## 14. Limitations

- The amount of data that can be hidden depends on the capacity of the image.
- Changing, resizing, compressing, or converting the encoded image may damage the hidden information.
- The decoding program must follow the same data format used during encoding.
- LSB steganography alone does not encrypt the secret data.
- Anyone who knows the embedding method may be able to extract the hidden information.

## 15. Applications

Steganography can be used for educational demonstrations, digital watermarking research, information hiding, and learning about data embedding techniques.

This project is mainly intended for learning and demonstrating the basic principles of LSB steganography.

## 16. Conclusion

The C Steganography Project demonstrates how secret information can be hidden inside a BMP image using the Least Significant Bit technique.

The encoding process stores the secret file data inside image bytes, while the decoding process extracts and reconstructs the hidden data.

By completing this project, we gain practical knowledge of C programming, file handling, pointers, structures, bitwise operations, and modular programming.

Overall, this project provides a simple introduction to image steganography and shows how binary data can be stored and recovered using the C programming language.
