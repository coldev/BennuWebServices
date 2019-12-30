// bennuwebservices.cpp: define las funciones exportadas de la aplicación DLL.
//

#include "stdafx.h"




#ifdef _WIN32
// windows code goes here
#include <windows.h>

#endif




#include "bennuphoton.h"


#include <stdio.h>
#include <string.h>
#include "jansson.h" 
#include "tinyxml2.h" 



// =======================================================================




//EXTERNAL FUNCTIONS
extern char* base64_encode(const unsigned char *str, int length, int *ret_length);
extern char *base64_decode(const unsigned char *str, int length, int *ret_length);

//extern   unsigned int CRC32xx(void *pData, size_t iLen);

extern const char * encripta_cadena(const char *cadena, unsigned char *clave);
extern const char * desencripta_cadena(char *cadena, unsigned char *clave);


extern const char * sha256_update(char *cadena);
 



 


//funciones internas de Bennu
//-----------------------------------------------------------------------

#if defined(__GNUC__)
#define M_DLLAPI    WINAPI *    //devcpp
#else
#define M_DLLAPI    __cdecl  *  //visual c++
#endif

typedef const char * (M_DLLAPI tstrget)(int code);
typedef void (M_DLLAPI tstrdiscard)(int code);
typedef int (M_DLLAPI tstrnew)(const char *s);
typedef int (M_DLLAPI tstrconcat)(int code1, char * str2);
typedef void (M_DLLAPI tstruse)(int code);







const char * string_get(int code)
{
	const char *cad = "";

 
	HINSTANCE hDLL = LoadLibrary("libbgdrtm.dll");

	if (hDLL)
	{
		tstrget pfunc = (tstrget)GetProcAddress(hDLL, "string_get");
		if (pfunc)
			cad = (*pfunc)(code);

		FreeLibrary(hDLL);
	}
 

	return cad;
}

void   string_discard(int code)
{

 
	HINSTANCE hDLL = LoadLibrary("libbgdrtm.dll");

	if (hDLL)
	{
		tstrdiscard pfunc = (tstrdiscard)GetProcAddress(hDLL, "string_discard");
		if (pfunc)
			(*pfunc)(code);

		FreeLibrary(hDLL);
	}
 

}

int   string_new(char *s)
{
 
	HINSTANCE hDLL = LoadLibrary("libbgdrtm.dll");
 

	int res = 0;

 
	if (hDLL)
	{
		tstrnew pfunc = (tstrnew)GetProcAddress(hDLL, "string_new");
		if (pfunc)
			res = (*pfunc)(s);

		FreeLibrary(hDLL);
	}
 

	return res;
}


int   string_concat(int code1, char * str2)
{
 
	HINSTANCE hDLL = LoadLibrary("libbgdrtm.dll");
 

	int res = 0;

 
	if (hDLL)
	{
		tstrconcat pfunc = (tstrconcat)GetProcAddress(hDLL, "string_concat");
		if (pfunc)
			res = (*pfunc)(code1, str2);

		FreeLibrary(hDLL);
	}
 

	return res;
}


void   string_use(int code)
{
 
	HINSTANCE hDLL = LoadLibrary("libbgdrtm.dll");
 
	if (hDLL)
	{
		tstruse pfunc = (tstruse)GetProcAddress(hDLL, "string_use");
		if (pfunc)
			(*pfunc)(code);

		FreeLibrary(hDLL);
	}
 
}








extern "C" DLLEXPORT char *  __bgdexport(bennuwebservices, globals_def) =
"INT demo123 = 0; \n"
"INT demo1234 = 0; \n"
 


;



extern "C" DLLEXPORT DLVARFIXUP __bgdexport(bennuwebservices, globals_fixup)[] =
{
	/* Nombre de variable global, puntero al dato, tamaño del elemento, cantidad de elementos */

	{ "demo123"  , NULL, -1, -1 },
	{ "demo1234"  , NULL, -1, -1 },
 

  

	{ NULL              , NULL, -1, -1 }
};


 

 


//---------------------------------------------------
//FUNCIONES PPALES
//---------------------------------------------------
 





//=================================== HASHES AND ENCRYPT =====================================
static  int  f_encrypt_aes256(INSTANCE * my, int * params)
{
	const char *cadena = string_get(params[0]);
	const char *llave = string_get(params[1]);

	const char *cad = encripta_cadena( cadena, (unsigned char*)llave);
	
	 
	//liberar memoria
	string_discard(params[0]);
	string_discard(params[1]);

	int res;

	if (strlen(cad) > 0 )
		res = string_new((char*)cad);
	else
		res = string_new("");

	string_use(res);
	

	return res;

}


static  int  f_decrypt_aes256(INSTANCE * my, int * params)
{
	const char *cadena = string_get(params[0]);
	const char *llave = string_get(params[1]);

	const char *cad = desencripta_cadena((char*)cadena, (unsigned char*)llave);
	
	 
	//liberar memoria
	string_discard(params[0]);
	string_discard(params[1]);

	int res;

	if (strlen(cad) > 0)
		res = string_new((char*)cad);
	else
		res = string_new("");

	string_use(res);

	return res;

}


static  int  f_sha256_update(INSTANCE * my, int * params)
{
	const char *cadena = string_get(params[0]);
 
	 
	const char *cad = sha256_update((char*)cadena);

	//liberar memoria
	string_discard(params[0]);
	
	int res;

	if (cad)
		res = string_new((char*)cad);
	else
		res = string_new("");

	string_use(res);

	return res;

}


//=================================== fin + HASHES AND ENCRYPT  =====================================








//=================================== CRC32 =====================================
#include "CRC32/crc32.cpp"

typedef signed int	Sint32;

extern unsigned int CRC32xx(void *pData, size_t iLen);

static  int  f_crc32(INSTANCE * my, int * params)
{
	void *puntero = (void*)params[0];
	int longitud = params[1];

	float res = CRC32xx(puntero, longitud);

		return *(Sint32 *)&res;

}


static  int  f_crc32_string(INSTANCE * my, int * params)
{
	const char *puntero = string_get(params[0]);

	int longitud =  strlen(puntero);

	float res = CRC32xx((void*)puntero, longitud);

	//liberar memoria
	string_discard(params[0]);

	return *(Sint32 *)&res;

 

}

//=================================== fin + CRC32 =====================================







//=================================== APP + PROTECT =====================================
#ifdef _WIN32

#include <windows.h>      
#include <intrin.h> 
#include <stdint.h>

int16_t getCpuHash() {
	int cpuinfo[4] = { 0, 0, 0, 0 };
	__cpuid(cpuinfo, 0);
	int16_t hash = 0;
	int16_t *ptr = (int16_t *)(&cpuinfo[0]);
	for (int32_t i = 0; i < 8; i++)
		hash += ptr[i];

	return hash;
}

#endif



static  int  f_app_hardware_id(INSTANCE * my, int * params)
{
	 
#ifdef _WIN32	  
	return(getCpuHash());

	 
#endif
	return(0);

}




static  int  f_app_exists(INSTANCE * my, int * params)
{
	const char *cadena = string_get(params[0]);
	int res = 0;

#ifdef _WIN32	  
	HANDLE m_singleInstanceMutex = CreateMutex(NULL, TRUE, (char*)cadena);

	if (m_singleInstanceMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//	HWND existingApp = FindWindow(0, L"Your app's window title");
		//	if (existingApp) SetForegroundWindow(existingApp);

		res= 1; // Exit the app. For MFC, return false from InitInstance.		 
	}

#endif

	string_discard(params[0]);

	return(res);

}


//=================================== fin + APP  PROTECT =====================================






// ====================================  COMPRESS 

#include "ColdevCompress.h"

ColdevCompressData* comprimeBASE64 = NULL;


static  int  f_data_compress(INSTANCE * my, int * params)
{
	const unsigned char *data = (const unsigned char *)params[0];
	int longitud = params[1];
	int longitud_retorno;

	//===============   comprime

	int destbuffer, destlength;
	char *cad = NULL;

	if (comprimeBASE64) delete comprimeBASE64;

	comprimeBASE64 = new ColdevCompressData;

	if (comprimeBASE64->cd_compressBytes(data, longitud, &destbuffer, &destlength) == 0)
	{
		cad = base64_encode((const unsigned char *)destbuffer, destlength, &longitud_retorno);
	}


	//======================================

	int res;

	if (cad)
		res = string_new((char*)cad);
	else
		res = string_new("");

	string_use(res);

	return res;

}



static  int  f_data_uncompress(INSTANCE * my, int * params)
{
	const char *cadena = string_get(params[0]);
	int *longitud_descomprimido = (int*)params[1];
	 
	int longitud_retorno;
	int longitud = strlen((char*)cadena);
	//===============   comprime

	char *cad = base64_decode((const unsigned char*)cadena, longitud, &longitud_retorno);

	string_discard(params[0]);
	 

	//===============  des comprime

	int destbuffer;

	if (comprimeBASE64) delete comprimeBASE64;

	comprimeBASE64 = new ColdevCompressData;

	int ret = NULL;

	if (comprimeBASE64->cd_uncompressBytes((unsigned char *)cad, longitud_retorno, &destbuffer, longitud_descomprimido) == 0)
	{
		ret = destbuffer;
	}


	 

	return ret;

}
// ==================================== fin +   COMPRESS 





//=====================================   BASE64  =======================================



static  int  f_base64_encode(INSTANCE * my, int * params)
{
	int longitud = params[1];
	const unsigned char *data = (const unsigned char *)params[0];
	int longitud_retorno;

	//======================================
	char *cad = base64_encode(data, longitud, &longitud_retorno);
		 	 

	int res;

	if (cad)
		res = string_new((char*)cad);
	else
		res = string_new("");

	string_use(res);
	free(cad);

	return res;

}


char bufferBASE64Compartido[1024 * 1000]; //1mb



static  int  f_base64_decode(INSTANCE * my, int * params)
{
	 
	const unsigned char *data = (const unsigned char *)string_get(params[0]);
	int longitud = strlen((char*)data);

	int *longitud_retorno = (int *)params[1];

	//======================================
	char *cad = base64_decode( data, longitud, longitud_retorno);

	memcpy(bufferBASE64Compartido, cad, *longitud_retorno + 1);

	free(cad);	
	string_discard(params[0]);

	return  (int)bufferBASE64Compartido;

}



static  int  f_base64_encode_string(INSTANCE * my, int * params)
{
	
	const char *cadena = string_get(params[0]);
	int longitud = strlen(cadena);
	int longitud_retorno;

	//======================================
	char *cad = base64_encode((const unsigned char *  )cadena, longitud, &longitud_retorno);

	string_discard(params[0]);

	int res;

	if (cad)
		res = string_new((char*)cad);
	else
		res = string_new("");

	string_use(res);
	free(cad);

	return res;

}



static  int  f_base64_decode_string(INSTANCE * my, int * params)
{

	const unsigned char *data = (const unsigned char *)string_get(params[0]);
	int longitud = strlen((char*)data);

	int longitud_retorno;

	//======================================
	char *cad = base64_decode( data, longitud, &longitud_retorno);

	string_discard(params[0]);


	memcpy(bufferBASE64Compartido, cad, longitud_retorno + 1);
 
	 
	int res;

	if (cad)
		res = string_new((char*)bufferBASE64Compartido);
	else
		res = string_new("");

	string_use(res);
	free(cad);

	return res;

}
//=====================================  fin + BASE64  =======================================


  

//=====================================   JSON =======================================

 


//create json with string
static  int  f_json_init_string(INSTANCE * my, int * params)
{
	const char *cadena = string_get(params[0]);


	json_t *root;
	json_error_t error;

	root = json_loads((char*)cadena, 0, &error);


	//liberar memoria
	string_discard(params[0]);

	return (int)root;
}



//finalize json
static  int  f_json_finalize(INSTANCE * my, int * params)
{
	 
	return 0;
}



//serialize json
static  int  f_json_serialize(INSTANCE * my, int * params)
{
	json_t *root = (json_t *)params[0];

	//----------
	char *resultado = json_dumps(root, JSON_ENCODE_ANY);

	int res;

	if (resultado)
		res = string_new((char*)resultado);
	else
		res = string_new("");

	string_use(res);

	return res;
}




// json string get
static  int  f_json_string_get(INSTANCE * my, int * params)
{

	//----------
	json_t *item = (json_t *)params[0];

	const char *cad = json_string_value(item);

	int res;

	if (cad)
		res = string_new((char*)cad);
	else
		res = string_new("");

	string_use(res);

	return res;
}



// json int get
static  int  f_json_int_get(INSTANCE * my, int * params)
{

	//----------
	
	json_t *item = (json_t *)params[0];

	int res = json_integer_value(item);

	return res;
}




// json float get
static  int  f_json_float_get(INSTANCE * my, int * params)
{

	//----------

	json_t *item = (json_t *)params[0];

	float res = json_real_value(item);

	return *(Sint32 *)&res;

}



// json string set
static  int  f_json_string_set(INSTANCE * my, int * params)
{

	json_t *item = (json_t *)params[0];
	char *valor = (char*)string_get(params[1]);

	int res = json_string_set(item, valor);

	//liberar memoria
	string_discard(params[0]);

	return res;
}


// json int set
static  int  f_json_int_set(INSTANCE * my, int * params)
{

	json_t *item = (json_t *)params[0];
	int valor = params[1];

	int res = json_integer_set(item, valor);


	return res;
}



// json float set
static  int  f_json_float_set(INSTANCE * my, int * params)
{

	json_t *item = (json_t *)params[0];
	float valor = *(float *)& params[1];

	int res = json_real_set(item, valor);


	return res;
}




// json new int
static  int  f_json_integer(INSTANCE * my, int * params)
{

	int valor = params[0];

	json_t *item = json_integer(valor);


	return (int)item;
}





// json new float
static  int  f_json_float(INSTANCE * my, int * params)
{

	float valor = *(float *)& params[0];

	json_t *item = json_real(valor);
	
	return (int)item;
}

// json new string
static  int  f_json_string(INSTANCE * my, int * params)
{

	char *valor = (char*)string_get(params[0]);

	json_t *item = json_string((char*)valor);

	//liberar memoria
	string_discard(params[0]);


	return (int)item;
}




//=====================================   JSON objects =======================================

// json new object
static  int  f_json_object_new(INSTANCE * my, int * params)
{
	json_t *objeto = json_object();

	return (int)objeto;
}



// json  object get
static  int  f_json_object_get(INSTANCE * my, int * params)
{

	json_t *root = (json_t *)params[0];
	char *llave = (char*)string_get(params[1]);
	
	json_t *objeto = json_object_get(root, llave);

	//liberar memoria
	string_discard(params[1]);


	return (int)objeto;
}


// json  object set
static  int  f_json_object_set(INSTANCE * my, int * params)
{

	json_t *root = (json_t *)params[0];
	char *llave = (char*)string_get(params[1]);
	json_t *valor = (json_t *)params[2];

	int res = json_object_set(root, llave, valor);

	//liberar memoria
	string_discard(params[1]);


	return res;
}



// json  object set new
static  int  f_json_object_set_new(INSTANCE * my, int * params)
{

	json_t *root = (json_t *)params[0];
	char *llave = (char*)string_get(params[1]);
	json_t *valor = (json_t *)params[2];

	int res = json_object_set_new(root, llave, valor);


	//liberar memoria
	string_discard(params[1]);


	return res;
}



// json  object delete
static  int  f_json_object_del(INSTANCE * my, int * params)
{
	json_t *root = (json_t *)params[0];
	char *llave = (char*)string_get(params[1]);
	
	int res = json_object_del(root, llave);


	//liberar memoria
	string_discard(params[1]);

	return res;
}



// json  object clear
static  int  f_json_object_clear(INSTANCE * my, int * params)
{
	json_t *root = (json_t *)params[0];

	int res = json_object_clear(root);

	return res;
}



// json  object size
static  int  f_json_object_size(INSTANCE * my, int * params)
{
	json_t *objeto = (json_t *)params[0];

	int tamano = json_object_size(objeto);

	return tamano;
}

//=====================================   fin + JSON objects =======================================


//-------------------------------- JSON  ARRAYS ------------------------------

// json  array new
static  int  f_json_array_new(INSTANCE * my, int * params)
{
	json_t *arreglo = json_array();

	return (int)arreglo;
}


// json  array get
static  int  f_json_array_get(INSTANCE * my, int * params)
{
	json_t *arreglo = (json_t *)params[0];
	int indice = params[1];


	json_t *item = json_array_get(arreglo, indice);

	return (int)item;
}


// json  array set
static  int  f_json_array_set(INSTANCE * my, int * params)
{
	json_t *arreglo = (json_t *)params[0];
	int indice = params[1];
	json_t *valor = (json_t *)params[2];
	
	int res = json_array_set(arreglo, indice, valor);

	return res;
}




// json  array append
static  int  f_json_array_append(INSTANCE * my, int * params)
{
	json_t *arreglo = (json_t *)params[0];
	json_t *valor = (json_t *)params[1];

	int res = json_array_append(arreglo, valor);

	return res;
}


// json  array insert
static  int  f_json_array_insert(INSTANCE * my, int * params)
{
	json_t *arreglo = (json_t *)params[0];
	int indice = params[1];
	json_t *valor = (json_t *)params[2];

	int res = json_array_insert(arreglo, indice, valor);

	return res;
}



// json  array remove
static  int  f_json_array_remove(INSTANCE * my, int * params)
{
	json_t *arreglo = (json_t *)params[0];
	int indice = params[1];

	int res = json_array_remove(arreglo, indice);

	return res;
}


// json  array clear
static  int  f_json_array_clear(INSTANCE * my, int * params)
{
	json_t *arreglo = (json_t *)params[0];

	int res = json_array_clear(arreglo);

	return res;
}


// json  array size
static  int  f_json_array_size(INSTANCE * my, int * params)
{
	json_t *arreglo = (json_t *)params[0];

	int tamano = json_array_size(arreglo);

	return tamano;
}


//-------------------------------- fin + JSON ARRAYS ------------------------------


//=====================================  fin + JSON =======================================






//=====================================   XML =======================================

// XML new
static  int  f_json_xml_new(INSTANCE * my, int * params)
{

	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;

	int res = (int)doc;

	return res;
}



// XML load string
static  int  f_json_xml_parse(INSTANCE * my, int * params)
{

	tinyxml2::XMLDocument  *doc = (tinyxml2::XMLDocument  *)params[0];
	const char *cadena = (const char*)string_get(params[1]);

	int xmlcoderror =(int) doc->Parse(cadena);

	if (xmlcoderror != 0) return 0; //carga de xml ? 

	int res = (int) doc; 

	//liberar memoria
	string_discard(params[1]);


	return res;
}



// XML new element
static  int  f_json_xml_new_element(INSTANCE * my, int * params)
{

	tinyxml2::XMLDocument  *doc = (tinyxml2::XMLDocument  *)params[0];
	const char *cadena = (const char*)string_get(params[1]);

	int res = (int)doc->NewElement(cadena);


	//liberar memoria
	string_discard(params[1]);


	return res;
}


//=====================================  fin + XML =======================================






/* ----------------------------------------------------------------- */
/* Declaracion de funciones                                          */

#define FENIX_export(a,b,c,d)  {a,b,c,(void*)d}


extern "C" DLLEXPORT DLSYSFUNCS  __bgdexport(bennuwebservices, functions_exports)[] = {

	//hashes encrypt	    
	FENIX_export("AES256_ENCRYPT", "SS", TYPE_STRING, f_encrypt_aes256),
	FENIX_export("AES256_DECRYPT", "SS", TYPE_STRING, f_decrypt_aes256),
	FENIX_export("SHA256_STRING", "S", TYPE_STRING, f_sha256_update),
	FENIX_export("CRC32", "PI", TYPE_FLOAT, f_crc32),
	FENIX_export("CRC32", "S", TYPE_FLOAT, f_crc32_string),


	//app protect

	FENIX_export("APP_HARDWARE_ID", "", TYPE_DWORD, f_app_hardware_id),
	FENIX_export("APP_EXISTS", "S", TYPE_DWORD, f_app_exists),

	//COMPRESS

	FENIX_export("DATA_COMPRESS", "PI", TYPE_STRING, f_data_compress),
	FENIX_export("DATA_UNCOMPRESS", "SP", TYPE_DWORD, f_data_uncompress),

	//BASE64
	FENIX_export("BASE64_ENCODE", "PI", TYPE_STRING, f_base64_encode),
	FENIX_export("BASE64_DECODE", "SP", TYPE_DWORD, f_base64_decode),
	FENIX_export("BASE64_ENCODE_STRING", "S", TYPE_STRING, f_base64_encode_string),
	FENIX_export("BASE64_DECODE_STRING", "S", TYPE_STRING, f_base64_decode_string),
	 

	//json
	
	FENIX_export("JSON_NEW", "S", TYPE_DWORD, f_json_init_string),
	FENIX_export("JSON_SERIALIZE", "I", TYPE_STRING, f_json_serialize),

	FENIX_export("JSON_STRING_GET", "I", TYPE_STRING, f_json_string_get),
	FENIX_export("JSON_INT_GET", "I", TYPE_DWORD, f_json_int_get),
	FENIX_export("JSON_FLOAT_GET", "I", TYPE_FLOAT, f_json_float_get),

	FENIX_export("JSON_STRING_SET", "IS", TYPE_DWORD, f_json_string_set),
	FENIX_export("JSON_INT_SET", "II", TYPE_DWORD, f_json_int_set),
	FENIX_export("JSON_FLOAT_SET", "IF", TYPE_DWORD, f_json_float_set),

	FENIX_export("JSON_INTEGER", "I", TYPE_DWORD, f_json_integer),
	FENIX_export("JSON_FLOAT", "F", TYPE_DWORD, f_json_float),
	FENIX_export("JSON_STRING", "S", TYPE_DWORD, f_json_string),

    //OBJECTS
	FENIX_export("JSON_OBJECT_NEW", "", TYPE_DWORD, f_json_object_new),
	FENIX_export("JSON_OBJECT_GET", "IS", TYPE_DWORD, f_json_object_get),
	FENIX_export("JSON_OBJECT_SET", "ISI", TYPE_DWORD, f_json_object_set),
	FENIX_export("JSON_OBJECT_SET_NEW", "ISI", TYPE_DWORD, f_json_object_set_new),
	FENIX_export("JSON_OBJECT_DEL", "IS", TYPE_DWORD, f_json_object_del),
	FENIX_export("JSON_OBJECT_CLEAR", "I", TYPE_DWORD, f_json_object_clear),
	FENIX_export("JSON_OBJECT_SIZE", "I", TYPE_DWORD, f_json_object_size),

	//ARRAYS
	FENIX_export("JSON_ARRAY_NEW", "", TYPE_DWORD, f_json_array_new),
	FENIX_export("JSON_ARRAY_GET", "II", TYPE_DWORD, f_json_array_get),
	FENIX_export("JSON_ARRAY_SET", "III", TYPE_DWORD, f_json_array_set),
	FENIX_export("JSON_ARRAY_APPEND", "II", TYPE_DWORD, f_json_array_append),
	FENIX_export("JSON_ARRAY_INSERT", "III", TYPE_DWORD, f_json_array_insert),
	FENIX_export("JSON_ARRAY_REMOVE", "II", TYPE_DWORD, f_json_array_remove),
	FENIX_export("JSON_ARRAY_CLEAR", "I", TYPE_DWORD, f_json_array_clear),
	FENIX_export("JSON_ARRAY_SIZE", "I", TYPE_DWORD, f_json_array_size),

	 

	{ 0            , 0     , 0         , 0 }
};




/* ----------------------------------------------------------------- */

/* Bigest priority first execute
Lowest priority last execute */

extern "C" DLLEXPORT char * __bgdexport(bennuwebservices, modules_dependency)[] = {
	"libsdlhandler",
	"mod_string",
	NULL
};

 
 

extern "C" DLLEXPORT void __bgdexport(bennuwebservices, module_initialize)()
{

	printf("Initialize Bennu Web Services . \n");
}


extern "C" DLLEXPORT void __bgdexport(bennuwebservices, module_finalize)()
{
 
	 

	printf("Unload Bennu Web Services .  \n");
}



 

