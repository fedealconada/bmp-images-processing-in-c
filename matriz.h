#ifndef __TAD
#define __TAD

/* 
 * Tipo que representa una matriz 
 */
typedef struct
{
  unsigned int max_filas;
  unsigned int max_columnas;
  int ** m;
} t_matriz;

typedef struct{	
	short int Signature;
	long int FileSize, Reserved, DataOffset;
	
}file_header;

typedef struct{
   		long int Size, width, height;
   		short int Planes, BitCount;
		long int Compression, imageSize, XpixelsPerM, YpixelsPerM, coloursUsed, coloursImportant;
}info_header;

typedef struct{
	file_header fh;
	info_header ih;
}header;

typedef struct{
	char red;
	char green;
	char blue;
	char Reserved;
}rgb_paleta;
	
typedef struct{
	header h;
	rgb_paleta *rgb;
	t_matriz matriz;
}archivo_bmp;

header leer_header(header ,FILE*);
void escribir_header(header,FILE*);
rgb_paleta* leer_paleta(info_header, FILE*);
int** alocar_matriz(int,int);
void liberar_matriz(int**, int);
t_matriz leer_matriz(header, FILE*);
void escribir_matriz(archivo_bmp, FILE*);
archivo_bmp leer_bmp(FILE*);
void escribir_bmp(archivo_bmp, FILE*);
void  imprimir_header(header, rgb_paleta*);
t_matriz rotar(t_matriz);
int promedio(t_matriz,int,int,int,int);
t_matriz blur(t_matriz,int,int);
t_matriz duplicar(t_matriz,int);
t_matriz reducir(t_matriz,int);
t_matriz invertir(t_matriz);
t_matriz negativo(t_matriz,int);
t_matriz lineas_horizontales(t_matriz,int,int,int);
t_matriz lineas_verticales(t_matriz,int,int,int);
int potencia(int);
#endif
