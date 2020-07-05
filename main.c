/* tp picante */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "diccionario.h"
/*

./analisis_ventas -out <salida> -in <entrada>   NULL
|		0        |  1  |   2   |  3 |    4    |	 5   |

gcc main.c -o analisis_ventas -ansi -pedantic -Wall

input:
ID_CLIENTE, FECHA, MONTO
1,1577880000,1412
1,1578657600,1786
1,1579089600,1029
2,1577880000,1644
2,1578657600,1668
2,1579089600,555
3,1577880000,948
3,1579089600,580
3,1579521600,1443

output:
Año: 2020
------------------------------------------
Cliente: 1
Mes: Enero Total: 21234
Mes: Marzo Total: 32143
Total: 53377
------------------------------------------
Cliente: 2
Mes: Enero Total: 21234
Mes: Marzo Total: 32143
Total: 53377
------------------------------------------
Total: 106754

	Cliente
	   |
	   V

	   1  -> id, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, Total
	   2  -> id, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, Total
	   3  -> id, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, Total

	   *
	   *
	   *

	   n  -> id, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, Total
*/

#define IN_FILE_ARGUMENT "-in"
#define OUT_FILE_ARGUMENT "-out"
#define SV ','
#define MAX_LINE_LENGTH 30


typedef char* string;

typedef enum{
	OK,
	ERROR_NULL_POINTER,
	ERROR_NO_MEMORY,
	ERROR_CORRUPTED_FILE,
	ERROR_INVALID_EJECUTION_PARAMETERS,
	ERROR_FILE_OP_FAILURE,
	ERROR_EMPTY_FILE,
	ERROR_EMPTY_FIELD
}status_t;


typedef struct{
	size_t
	 id;
	long
	 enero,
	 febrero,
	 marzo,
	 abril,
	 mayo,
	 junio,
	 julio,
	 agosto,
	 septiempbre,
	 octubre,
	 noviembre,
	 diciembre,
	 total;
}client_resume;

/*

*/


status_t cla_interpreter(const int argc, const string* argv, size_t *input_file, size_t *output_file);/*hecha*/

status_t csv_read_line(FILE *input_file, size_t* id, long *amount, long *month);/*devuelve los valores por interfaz. revisar*/

status_t csv_load_data(FILE *input_file, size_t *length);

status_t get_month(const time_t* t, long* month);

void show_error(status_t status);/*hecha*/


int main(int argc, string* argv) {

	size_t input_file_name, output_file_name, length;
	FILE *input, *output;
	/*time_t t;
	struct tm date;*/

/*Apertura de archivos*/
	show_error(cla_interpreter(argc,argv, &input_file_name, &output_file_name));

  /* si falla una apertura corta enseguida*/
  if ( (input = fopen(argv[output_file_name],"w")) == NULL){
    show_error(ERROR_FILE_OP_FAILURE);
    return EXIT_FAILURE;
  }
  
  if ( (output = fopen(argv[input_file_name],"r")) == NULL){
    show_error(ERROR_FILE_OP_FAILURE);
	fclose(input);
    return EXIT_FAILURE;
  }

	/*
	input=fopen(argv[output_file_name], "w");
	output=fopen(argv[input_file_name], "r");
	if(input==NULL||output==NULL){
		show_error(ERROR_FILE_OP_FAILURE);
		return EXIT_FAILURE;
	}
	*/

	if ( csv_load_data(input,&length) != OK)
    return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}


status_t cla_interpreter(const int argc, const string* argv, size_t *input_file, size_t *output_file){
	size_t i;

	if(input_file==NULL||output_file==NULL||argv==NULL)return ERROR_NULL_POINTER;

	if(argv[1]==NULL)return ERROR_INVALID_EJECUTION_PARAMETERS;

	for(i=1;i<argc;i++){

		/* identifica si es parametro de entrada*/

		if( !strcmp(argv[i], IN_FILE_ARGUMENT) && argv[i+1]!=NULL && strcmp(argv[i+1], OUT_FILE_ARGUMENT)!=0){
			*input_file=i+1;
		}else if(argv[i+1]==NULL || strcmp(argv[i+1], OUT_FILE_ARGUMENT)==0)return ERROR_INVALID_EJECUTION_PARAMETERS;

		/* identifica si es parametro de salida*/
		if( !strcmp(argv[i], OUT_FILE_ARGUMENT) && argv[i+1]!=NULL && strcmp(argv[i+1], IN_FILE_ARGUMENT)!=0){
			*output_file=i+1;
		}else if(argv[i+1]==NULL || strcmp(argv[i+1], IN_FILE_ARGUMENT)==0)return ERROR_INVALID_EJECUTION_PARAMETERS;

	}

	

	return OK;
}

void show_error(status_t status){
	switch(status){

	default:
	case OK:
	break;

	case ERROR_NULL_POINTER:
	fprintf(stderr,"%s\n", ERROR_MSJ_NULL_POINTER);
	break;

	case ERROR_NO_MEMORY:
		fprintf(stderr,"%s\n", ERROR_MSJ_NO_MEMORY);
	break;

	case ERROR_CORRUPTED_FILE:
		fprintf(stderr,"%s\n", ERROR_MSJ_CORRUPTED_FILE);
	break;

	case ERROR_INVALID_EJECUTION_PARAMETERS:
		fprintf(stderr,"%s\n", ERROR_MSJ_INVALID_EJECUTION_PARAMETERS);
	break;

	case ERROR_FILE_OP_FAILURE:
		fprintf(stderr,"%s\n", ERROR_MSJ_OP_FAILURE);
	break;
	case ERROR_EMPTY_FILE:
		fprintf(stderr,"%s\n", ERROR_MSJ_EMPTY_FILE);
	break;
	}
}

status_t csv_read_line(FILE *input_file, size_t* id, long *amount, long *month ){
	char line[MAX_LINE_LENGTH], **c;
	size_t i=0, j, k=0, comma=0;
	time_t t;
    status_t st;

	if(input_file==NULL||id==NULL||amount==NULL||month==NULL)return ERROR_NULL_POINTER;

	do{
		line[i]=fgetc(input_file);
		i++;
	}while(!feof(input_file) && line[i-1]!='\n' && i<=MAX_LINE_LENGTH); /*a partir de aca tenemos en In una cadena que deberia ser de tipo "XXX,XXXXXXXXXXX,XXXX\n" */

	/*iria aca*/
	
	/* validar que todos los campos esten presentes*/
	for(j=0;j<i;j=k){

		for(;line[k]!=SV && k<i;k++) 
		if(k-j<=1)return ERROR_EMPTY_FIELD;
		else comma++;

		switch(comma){
		
			case 1:
				*id=strtol( (line+j), c, k-j);
			break;

			case 2:
       		t= strtol( (line+j), c, k-j);
			if ((st = get_month(&t, month)) != OK)
          		return st;
			break;

			case 3:
				*amount=strtol( (line+j), c, k-j);
			break;

			default:
				return ERROR_CORRUPTED_FILE;
		}
	}

  return OK;
}

status_t get_month(const time_t *t, long *month)
{
  const time_t * tt;
  struct tm *ts;

  if (t==NULL || month==NULL)
    return ERROR_NULL_POINTER;

  tt = t;
  ts = gmtime(tt);

  *month = (long) ts -> tm_mon;

  return OK;
}