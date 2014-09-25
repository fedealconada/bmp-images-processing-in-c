/*
 * Entrega: Trabajo Final
 * Integrantes: Alconada Verzini, Federico Martín 
				García Chicote, Gonzalo
 * Módulo: Aquí se implementan todas las funciones referidas al manejo del archivo BMP y del procesamiento de la imagen.
*/
#ifndef _STDIO_
#include <stdio.h>
#endif
#ifndef _STDLIB_
#include <stdlib.h>
#endif
#ifndef _STDARG_
#include <stdarg.h>
#endif
#include "matriz.h"


/*
 * Función potencia de 2.
 */
int potencia(int a){
	int i,resul=1;
	for (i=1;i<=a;i++)
		resul=resul*2;
	return resul;
}

/*
 * Función que lee el header.
 */
header leer_header(header h,FILE *bmp){
	/*Se leen los datos del file_header*/
	fread(&h.fh.Signature, sizeof(short int), 1, bmp);
	fread(&h.fh.FileSize, sizeof(long int), 1, bmp);
	fread(&h.fh.Reserved, sizeof(long int), 1, bmp);
	fread(&h.fh.DataOffset, sizeof(long int), 1, bmp);
	/*Se leen los datos del info_header*/
	fread(&h.ih,sizeof(info_header),1,bmp);
	
	return h;
}

/*
 * Función que lee el header.
 */
void escribir_header(header h,FILE *bmp){
	fseek(bmp,SEEK_SET,SEEK_SET);
	/*Se escriben los datos del file_header*/
	fwrite(&h.fh.Signature, sizeof(short int), 1, bmp);
	fwrite(&h.fh.FileSize, sizeof(long int), 1, bmp);
	fwrite(&h.fh.Reserved, sizeof(long int), 1, bmp);
	fwrite(&h.fh.DataOffset, sizeof(long int), 1, bmp);
	/*Se escriben los datos del info_header*/
	fwrite(&h.ih,40,1,bmp);
}

/*
 * Función que lee la paleta de coloures (si la imagen posee una).
 */
rgb_paleta* leer_paleta(info_header ih, FILE *bmp){
	rgb_paleta* paleta;
	paleta = malloc (ih.coloursUsed*sizeof(rgb_paleta));
	if((ih.BitCount<=8)){
		int i;
		for (i=0;i<(ih.coloursUsed);i++){
			fread(&paleta[i],sizeof(rgb_paleta), 1, bmp);
		}
    }
	return paleta;
}

/*
 * Función que escribe la paleta de coloures (si la imagen posee una).
 */
void escribir_paleta(rgb_paleta* paleta, info_header ih, FILE *bmp){
	if((ih.BitCount<=8)){
		int i;
		for (i=0;i<(ih.coloursUsed);i++){
			fwrite(&paleta[i], sizeof(rgb_paleta), 1, bmp);
		}
    }
    paleta=NULL;
}
/*
 * Función que aloca el espacio necesario para una matriz de N filas por M columnas.
 */
 int** alocar_matriz(int filas, int columnas){
  int i;
   int** matriz;
  matriz = malloc (filas*sizeof( int*));
  if(matriz == NULL){
    free(matriz);
    printf("Memory allocation failed...\n");
    exit(-1);
  }
  for(i = 0; i < filas; ++i){
    matriz[i] = ( int*) malloc(columnas * sizeof( int));
    if(matriz[i] == NULL){
      printf("Memory allocation failed...\n");
      exit(-1);
    }
  }
  return matriz;
}

void liberar_matriz(int **matrix, int rows){
    int i;
    for(i=0; i<rows; i++){
        free(matrix[i]);
    }
    free(matrix);
    matrix=NULL;
}


/*
 * Función que lee la imagen del archivo recibido por parámetro y la guarda en una matriz.
 */
t_matriz leer_matriz (header h, FILE *bmp){

	t_matriz matriz; /* Declaro la matriz */
	
	matriz.max_filas=h.ih.height; /* Inicializo la matriz */
	matriz.max_columnas=h.ih.width; /* Inicializo la matriz */
  
	matriz.m=alocar_matriz(matriz.max_filas, matriz.max_columnas);
	
	int ancho,padding,relleno;
	
	fseek(bmp,h.fh.DataOffset,SEEK_SET);
	int i,j,k,bits[8];	
	unsigned byte;
	if (h.ih.BitCount>1){/* Si la imagen es de 8 o 24 bits... */
		for(j=0;j<h.ih.height;j++){
			for(i=0;i<h.ih.width;i++){
				fread(&matriz.m[j][i],h.ih.BitCount/8,1,bmp);
			}
			if(((h.ih.BitCount*h.ih.width)%32)!=0)
				fseek(bmp,((h.ih.BitCount*h.ih.width)%32)/8,SEEK_CUR);
		}
	}
	else{/* Si la imagen es de 1 bit... */	
	ancho=h.ih.width;
	padding=((h.ih.BitCount*h.ih.width)%32?(32-(h.ih.BitCount*h.ih.width)%32):0);
	relleno=padding;
	for(j=0;j<h.ih.height;j++){
		for(i=0;i<(h.ih.width+padding);){ 
			fread(&byte,1,1,bmp);
			for(k=7;k>=0;k--){
				bits[k]=byte%2;
				byte=byte/2;
			}
			for (k=0;k<8;k++){
				if (ancho){
					matriz.m[j][i]=bits[k];
					ancho--;		
				}
				else{
					relleno--;
				}
				i++;
			if (relleno==0){
				ancho=h.ih.width;
				relleno=padding;
			}
			}
		}
	}
	}
	return matriz;
}


/*
 * Función que escribe la imagen del archivo recibido por parámetro y la guarda en una matriz.
 */
 void escribir_matriz(archivo_bmp image, FILE *bmp){
	unsigned byte;
	int k,ancho,padding,relleno,cero,i,j;
	fseek(bmp,image.h.fh.DataOffset,SEEK_SET);
	
	if (image.h.ih.BitCount>1){
		for(j=0;j<image.h.ih.height;j++){
			for(i=0;i<image.h.ih.width;i++){
				fwrite(&image.matriz.m[j][i],image.h.ih.BitCount/8,1,bmp);
			}
			if(((image.h.ih.BitCount*image.h.ih.width)%32)!=0)
				fwrite(&cero,1,((image.h.ih.BitCount*image.h.ih.width)%32)/8,bmp);
		}
	}
	else{			
		ancho=0;
		padding=((image.h.ih.BitCount*image.h.ih.width)%32?(32-(image.h.ih.BitCount*image.h.ih.width)%32):0);
		relleno=padding;
		for(j=0;j<image.h.ih.height;j++){
			for(i=0;i<(image.h.ih.width+padding);){
				byte=0;
				for(k=7;k>=0;k--){
					if(ancho<image.h.ih.width){
						byte=byte+(image.matriz.m[j][i])*(potencia(k));
						ancho++;
					}
					else{
						byte=byte+0;
						relleno--;
						ancho++;
					}
					i++;
					if(relleno==0){
						relleno=padding;
						ancho=0;
					}						
				}
				fwrite(&byte,1,1,bmp);
			}
		}
	}
}

/*
 * Lee header, paleta y matriz.
 */
 archivo_bmp leer_bmp(FILE *bmp){
	archivo_bmp image;
	image.h=leer_header(image.h,bmp);
	image.rgb=leer_paleta(image.h.ih,bmp);
	image.matriz=leer_matriz(image.h,bmp);
	return image;
 }
 
 /*
 * Escribe header, paleta y matriz.
 */
  void escribir_bmp(archivo_bmp image, FILE *bmp){
	escribir_header(image.h,bmp);
	escribir_paleta(image.rgb,image.h.ih,bmp);
	escribir_matriz(image,bmp);
 }

/*
 * Función que imprime el header.
 */
 void imprimir_header(header h, rgb_paleta *paleta){
	
	/*Se imprimen los datos del header.*/
	printf(
	"Signature: %X\n" 
	"Tamanio del archivo:%ld\n"
	"Reserved:%ld\n"
	"DataOffset:%ld\n",h.fh.Signature,h.fh.FileSize,h.fh.Reserved,h.fh.DataOffset);
	
	printf (
	"Tamanio del Info header:%ld\n"
	"Ancho en pixels:%ld\n"
	"Alto en pixels:%ld\n"
	"numero de planos:%d\n"
	"Bits por pixel:%d\n"
	"Compresion:%ld\n"
	"Tamanio de imagen:%ld\n"
	"Resolucion horizontal:%ld\n"
	"Resolucion Vertical:%ld\n"
	"numero de colores actualmente empleados:%ld\n"
	"numero de colores importantes:%ld\n",h.ih.Size,h.ih.width,h.ih.height,h.ih.Planes,h.ih.BitCount,h.ih.Compression,h.ih.imageSize,h.ih.XpixelsPerM,h.ih.YpixelsPerM,h.ih.coloursUsed,h.ih.coloursImportant);
    /*Se imprimen los datos de la paleta de colores.*/
	if (h.ih.BitCount<=8){	
		int i;
		int idcolour=0;
		printf(
		"-------------------------------------------------------------------------------\n"
		"\t\t\tPALETA DE COLORES\t\t\t\n"
		"-------------------------------------------------------------------------------\n"
		"\tID colour\tred\tgreen\tblue\n");
		for (i=0; i< h.ih.coloursUsed; i++){
			printf("\t  %d\t \t %d \t  %d \t  %d\n", idcolour,paleta[i].red,paleta[i].green,paleta[i].blue);
			idcolour++; 			
		}
	}
}

/* 
 * Rota la matriz 90°.
 */
t_matriz rotar(t_matriz matriz){
  int a,i,j;
  /*Declaro una matriz 'mr' (Matriz Rotada).*/
  t_matriz mr;
  mr.max_filas=matriz.max_columnas;
  mr.max_columnas=matriz.max_filas;
  mr.m=alocar_matriz(mr.max_filas,mr.max_columnas);
  a=mr.max_filas-1;
  for (i=0; i < mr.max_filas; i++){
    for (j=0; j < mr.max_columnas; j++){
	  mr.m[i][j]=matriz.m[j][a];
    }
	a--;
  }
  /*Se libera la zona de memoria reservada.*/
  liberar_matriz(matriz.m,matriz.max_filas);
  return mr;
}

/* 
 * Promedio de un pixel de radio "radio".
 */
int promedio (t_matriz matriz, int BitCount, int i, int j, int radio){
int cont,prom, m,n,R,G,B;
cont=0;prom=0;
B=0;G=0;R=0;
for (m=(i-radio); m <= (i+radio); m++){
	for (n=(j-radio); n <= (j+radio); n++){
		if((n>=0)&&(m>=0)&&(n<matriz.max_columnas)&&(m<matriz.max_filas)){
			if(BitCount==24){
				B=B+((matriz.m[m][n]&0xFF0000)>>16);
				G=G+((matriz.m[m][n]&0xFF00)>>8);
				R=R+(matriz.m[m][n]&0xFF);
			}
			else
				prom=prom+matriz.m[m][n];
			cont++;
		}
	}
}
if(BitCount==24){
	B=B/cont;
	G=G/cont;
	R=R/cont;
	prom=(B<<16)|(G<<8)|R;
}
else
	prom=prom/cont;
return prom;
}

/* 
 * Efecto blur.
 */
t_matriz blur(t_matriz matriz,int BitCount, int radio){
  int i,j;
  /*Declaro una matriz 'mb' (Matriz Blur).*/
  t_matriz mb;
  mb.max_filas=matriz.max_filas;
  mb.max_columnas=matriz.max_columnas;
  mb.m=alocar_matriz(mb.max_filas,mb.max_columnas);
  for (i=0; i < mb.max_filas; i++){
    for (j=0; j < mb.max_columnas; j++){
	  mb.m[i][j]=promedio(matriz,BitCount,i,j,radio);
    }
  }
  /*Se libera la zona de memoria reservada.*/
  liberar_matriz(matriz.m,matriz.max_filas); 
  return mb;
}

/* 
 * redimensionar al doble.
 */
t_matriz duplicar(t_matriz matriz, int BitCount){
  int i,j;
  /*Declaro una matriz 'md' (Matriz Duplicada).*/
  t_matriz md;
  md.max_filas=matriz.max_filas*2;
  md.max_columnas=matriz.max_columnas*2;
  md.m=alocar_matriz(md.max_filas,md.max_columnas);
  for (i=0; i < md.max_filas; i++){
    for (j=0; j < md.max_columnas; j++){
	  md.m[i][j]=promedio(matriz,BitCount,i/2,j/2,1);
    }
  }
  /*Se libera la zona de memoria reservada.*/
  liberar_matriz(matriz.m,matriz.max_filas); 
  return md;
}

/* 
 * redimensionar a la mitad.
 */
t_matriz reducir(t_matriz matriz, int BitCount){
  int i,j;
  /*Declaro una matriz 'mr' (Matriz reducida).*/
  t_matriz mr;
  mr.max_filas=matriz.max_filas/2;
  mr.max_columnas=matriz.max_columnas/2;
  mr.m=alocar_matriz(mr.max_filas,mr.max_columnas);
  for (i=0; i < mr.max_filas; i++){
    for (j=0; j < mr.max_columnas; j++){
		mr.m[i][j]=promedio(matriz,BitCount,i*2,j*2,1);
    }	
  }
  /*Se libera la zona de memoria reservada.*/
  liberar_matriz(matriz.m,matriz.max_filas); 
  return mr;
}

/* 
 * Función que invierte.
 */
t_matriz invertir(t_matriz matriz){
  int i,j,k;
  k=matriz.max_filas-1;
  /*Declaro una matriz 'mi' (Matriz Invertida).*/
  t_matriz mi; 
  mi.max_columnas=matriz.max_columnas;
  mi.max_filas=matriz.max_filas;
  mi.m=alocar_matriz(mi.max_filas,mi.max_columnas);
  for (i=0; i < mi.max_filas; i++){
    for (j=0; j < mi.max_columnas; j++){
	  mi.m[i][j]=matriz.m[k][j];
    }
	k--;	
  }
  /*Se libera la zona de memoria reservada.*/
  liberar_matriz(matriz.m,matriz.max_filas);
  return mi;
}

/* 
 * Negativo imagen.
 */
t_matriz negativo(t_matriz matriz,int BitCount){
	int i,j,k;
	for (i=0; i < matriz.max_filas; i++){
		for (j=0; j < matriz.max_columnas; j++){
			matriz.m[i][j]=potencia(BitCount)-1-matriz.m[i][j];
		}
		k--;	
	}
	return matriz;
}

/* 
 * Agrega líneas horizontales.
 */
t_matriz lineas_horizontales(t_matriz matriz, int width, int space, int colour){
  int i,j,k;
  i=0;
  while (i<matriz.max_filas){
	for(k=0;k<=width;k++){
		for (j=0; j < matriz.max_columnas; j++){
			matriz.m[i][j]=colour;
		}
		i++;
	}
	i+=space;
  }
  return matriz;
}

/* 
 * Agrega líneas verticales.
 */
t_matriz lineas_verticales(t_matriz matriz, int width, int space, int colour){
  int i,j,k;
  i=0;
  while (i<matriz.max_columnas){
	for(k=0;k<width;k++){
		for (j=0; j < matriz.max_filas; j++){
			matriz.m[j][i]=colour;
		}
		i++;
	}
	i+=space;
  }
  return matriz;
}
