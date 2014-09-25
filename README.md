
BMP IMAGE PROCESSING IN C
----------------------------------------
Subject: C Seminar

University: Facultad de Informática - Universidad Nacional de La Plata

Student: Alconada Verzini, Federico Martín

OBJECTIVE:
----------

Make a programme in C that reads a BMP image file, stores its content in the structure bmp_file which saves the header, colours palette (if it exists) and the matrix (real image). Then, according to the parameters that the programme receives, it will invoke to different functions associated to the parameters.

HOW TO USE IT:
-------

-h: o -?: shows a menu about how to run the script.

-s: shows information about the header of the BMP. If no other option is specified (except -i), it won't save any output file.

-p: vertical flip.

-r: rotates the matrix 90 degrees.

-n: inverts image colours.

-d: duplicates the image size.

-f: reduces image size to half.

-b RATIO: blur effect with RATIO pixels.

-lh WIDTH SPACE COLOR: draws horizontal lines of specified colour with specified width pixels, separated by SPACE pixels.

-lv WIDTH SPACE COLOR: draws vertical lines of specified colour with specified width pixels, separated by SPACE pixels.

-o OUTPUT: specifies the name of the file in which the output image will be stored. If the name is not specified, out.bmp will be used.

-i INPUT: name of the file with the image that you want to process. This argument MUST exists.


The programme made work in Linux but in Windows has some bugs. 