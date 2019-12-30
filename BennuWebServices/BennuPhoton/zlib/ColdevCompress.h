#ifndef ColdevCompressLib
#define ColdevCompressLib



#include "zconf.h"

 

 



class ColdevCompressData
{
public:

	ColdevCompressData();
	~ColdevCompressData();

    int cd_compressBytes(const unsigned char *pData, unsigned long nDataSize, int *destbuffer, int *destlength);


	int cd_uncompressBytes(unsigned char * pCompressedData, unsigned long nCompressedDataSize, int *destbuffer, int *destlength);


	//vars
//	unsigned char * pCompressedData;
//	unsigned char * pUncompressedData;

#define tamanobuffercomprimir (1024 * 1000 ) //1mb 

	unsigned char  pCompressedData[tamanobuffercomprimir];
	unsigned char  pUncompressedData[tamanobuffercomprimir];

private:

	 
};

#endif // !1