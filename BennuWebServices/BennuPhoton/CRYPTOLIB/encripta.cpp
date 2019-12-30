

#include "AES.h"

#define tamanoBufferEncriptar (1024*1000) //1mb

char bufferEncriptar[tamanoBufferEncriptar];




 
const char * encripta_cadena(const char *cadena, unsigned char *clave)
{ 	 
	CBm53AES_L hola(clave);

	hola.CipherStr(cadena, bufferEncriptar);

	return   bufferEncriptar;
}

 

const char * desencripta_cadena( char *cadena, unsigned char *clave)
{

	CBm53AES_L hola(clave);

	hola.InvCipherStr(cadena, bufferEncriptar);

	return bufferEncriptar;
}




//========================================================== SHA256


#include "sha256.h"
#include <iostream>
using std::string;
using std::cout;
using std::cin;
using std::endl;

 
 

const char * sha256_update(char *cadena)
{  
	std::string cadena2(cadena);

	return sha256(cadena2).c_str();
}



