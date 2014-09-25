/*
 * Entrega: Trabajo Final
 * Integrantes: Alconada Verzini, Federico Martín 
				García Chicote, Gonzalo
 * Módulo: Este es el módulo principal, encargado de abrir y cerrar los archivos así como de invocar a las funciones requeridas.
*/

#include <stdio.h>
#include <stdlib.h>  
#include <stdarg.h>
#include "matriz.h"
#include <string.h>
#define ERROR -1
#define OK 0

/* 
 * Muestra en pantalla la ayuda.
 */
void ayuda (int num){
	printf("\n"
	"------------------------------------------------------\n"
	"\t\t\tAYUDA\n"
	"------------------------------------------------------\n"
	"\n"
	"1). -h: o -?: shows a menu about how to run the script \n"
	"\n"
	"2). -s: shows information about the header of the BMP. If no other option is specified (except -i), it won't save any output file. \n"
	"\n"
	"3). -p: vertical flip. \n"
	"\n"
	"4). -r: rotates the matrix 90 degrees.\n"
	"\n"
	"5). -n: inverts image colours.\n"
	"\n"
	"6). -d: duplicates the image size.\n"
	"\n"
	"7). -f: reduces image size to half.\n"
	"\n"
	"8). -b RATIO: blur effect with RATIO pixels.\n"
	"\n"
	"9). -lh WIDTH SPACE COLOR: draws horizontal lines of specified colour with specified width pixels, separated by SPACE pixels.\n"
	"\n"
	"10). -lv WIDTH SPACE COLOR: draws vertical lines of specified colour with specified width pixels, separated by SPACE pixels. \n"
	"\n"
	"11). -o OUTPUT: specifies the name of the file in which the output image will be stored. If the name is not specified, out.bmp will be used. \n"
	"\n"
	"12). -i INPUT: name of the file with the image that you want to process. This argument MUST exists. \n"
	"\n"
	
	);
	if (num==ERROR)
	  exit(num);
}

/* 
 * Programa que lee un archivo de imagen BMP y almacena su contenido en 
 * la estructura archivo_bmp, la cual, a su vez, guardará el header, la 
 * paleta de coloures (si la hay) y la MATRIZ (pertenenciente a la 
 * imagen real) en respectivas variables. Luego, de acuerdo a los
 * parámetros recibidos, invocará a las diferentes funciones asociadas a
 * éstos, a fin de manipular la imagen.
 * Por último, de ser necesario, se escribirá la información en un nuevo
 * archivo.
 */
int main(int argc, char *argv[]){

	FILE *bmp;
	FILE *copia_bmp;
	archivo_bmp image;

	int j=1,aux;
	char entro='N';
	char okey='F';/*F=FALSE*/
	
	/*Se recorre el arreglo de parámetros al main.*/
	while ((*(argv[j]+1)!='i') &&(j<=argc)) /*Busco -i y abro el archivo.*/
		j++;
	if ((*(argv[j])=='-')&&(*(argv[j]+1)=='i')&&(*(argv[j]+2)=='\0')){
		if ((argc-1) == j)
			ayuda(ERROR);
		else{	
			j++;
			bmp=fopen(argv[j],"r");
			if (bmp==NULL){
				printf("No se pudo abrir el archivo. \n");
				exit(1);
			}
			image=leer_bmp(bmp);
			fclose(bmp);
		}
	}
	
	for (j=1;j<argc;j++){
	    /*Se consulta por el patrón "-" + argumento + " ".*/
		if(*argv[j]=='-'){
			if (*(argv[j]+2)=='\0'){
				switch (*(argv[j]+1)){
					case '?': ayuda(OK);
					break;
					case 'h': ayuda(OK);
					break;
					case 's':	imprimir_header(image.h, image.rgb);
								okey='T';/*T=TRUE*/
					break;
					case 'p': image.matriz=invertir(image.matriz);
					break;
					case 'r': 	image.matriz=rotar(image.matriz);
								image.h.ih.width=image.matriz.max_columnas;
								image.h.ih.height=image.matriz.max_filas;
								aux=image.h.ih.XpixelsPerM;
								image.h.ih.XpixelsPerM=image.h.ih.YpixelsPerM;
								image.h.ih.YpixelsPerM=aux;
					break;
					case 'n':	image.matriz=negativo(image.matriz,image.h.ih.BitCount);
					break;
					case 'd':	image.matriz=duplicar(image.matriz,image.h.ih.BitCount);
								image.h.ih.width=image.h.ih.width*2;
								image.h.ih.height=image.h.ih.height*2;
								image.h.ih.XpixelsPerM=image.h.ih.XpixelsPerM*2;
								image.h.ih.YpixelsPerM=image.h.ih.YpixelsPerM*2;
								image.h.ih.imageSize=image.h.ih.height*image.h.ih.width;
								image.h.fh.FileSize=(image.h.ih.height*image.h.ih.width)*(image.h.ih.BitCount/8)+54;
								if(image.h.ih.BitCount<=8)
									image.h.fh.FileSize+=(4*image.h.ih.coloursUsed);
					break;
					case 'f':	image.matriz=reducir(image.matriz,image.h.ih.BitCount);
								image.h.ih.width=image.h.ih.width/2;
								image.h.ih.height=image.h.ih.height/2;
								image.h.ih.XpixelsPerM=image.h.ih.XpixelsPerM/2;
								image.h.ih.YpixelsPerM=image.h.ih.YpixelsPerM/2;
								image.h.ih.imageSize=image.h.ih.height*image.h.ih.width;
								image.h.fh.FileSize=(image.h.ih.height*image.h.ih.width)*(image.h.ih.BitCount/8)+54;
								if(image.h.ih.BitCount<=8)
									image.h.fh.FileSize+=(4*image.h.ih.coloursUsed);

					break;
					case 'b':if ((argc-1) == j)
									ayuda(ERROR);
								else{
									j++;
									/*Con la función atoi() convierto un string a entero y, 
									* utilizando abs(), me aseguro que el resultado sea siempre positivo. (1)*/
									int radio=abs(atoi(argv[j]));
									image.matriz=blur(image.matriz,image.h.ih.BitCount,radio);
								}
							
					break;
					case 'o':	if ((argc-1) == j)
									ayuda(ERROR);
								else{	
									j++;
									copia_bmp=fopen(argv[j],"w");
									entro='S'; /*Utilizo la variable entro para indicar que se recibió el parámetro -o.*/
								}
					break;
					case 'i':	if ((argc-1) == j)
									ayuda(ERROR);
								else{	
									j++;
								}
					break;
					default:	printf("\nERROR: Incorrect parameters.\n");//Si el parámetro es incorrecto se interrumpe el programa.
								ayuda(ERROR);
					}
				}else{ 
					if((*(argv[j]+1)=='l')&&(*(argv[j]+3)=='\0')){ /* Si los parámetros recibidos son lh o lv..*/
						switch (*(argv[j]+2)){								
							case 'h':	if ((argc-3) <= j)
										ayuda(ERROR);
										else{
											int width=abs(atoi(argv[j+1])); /*Vale la aclaración (1).*/
											int space=abs(atoi(argv[j+2])); /*Vale la aclaración (1).*/
											int colour=abs(atoi(argv[j+3]));/*Vale la aclaración (1).*/
											image.matriz=lineas_horizontales(image.matriz,width,space,colour);
											j+=3;
										}
							break;
							case 'v':	if ((argc-3) <= j)
											ayuda(ERROR);
										else{
											int width=abs(atoi(argv[j+1]));/*Vale la aclaración (1).*/
											int space=abs(atoi(argv[j+2]));/*Vale la aclaración (1).*/
											int colour=abs(atoi(argv[j+3]));/*Vale la aclaración (1).*/
											image.matriz=lineas_verticales(image.matriz,width,space,colour);
											j+=3;
										}
							break;
							default: 	printf("\nERROR: Parametros incorrectos.\n");//Si el parámetro es incorrecto se interrumpe el programa.
										ayuda(ERROR);
							}
					}
				}
				}else{
					/*En caso que el patrón no se cumpla, se llama a la función ayuda 
					interrumpiendo el programa a través de la función EXIT (librería stdlib.h).*/
					printf("\nERROR: Parametros incorrectos.\n\n");
					ayuda(ERROR);
				}
			}
			/* Si la condición de los siguientes if es falsa, significa 
			 * que el único parámetro recibido (además de -i) fue -s, 
			 * por lo que NO debo crear una copia*/
			if((argc!=4)){
				if(entro=='N')
					copia_bmp=fopen("out.bmp","w");
				escribir_bmp(image,copia_bmp);
				fclose(copia_bmp);
			}
			else{
				if(okey=='F'){
					if(entro=='N')
						copia_bmp=fopen("out.bmp","w");
					escribir_bmp(image,copia_bmp);
					fclose(copia_bmp);
				}
			}
			liberar_matriz(image.matriz.m,image.matriz.max_filas);
			free(image.rgb);
			return 0;
}
	
	
