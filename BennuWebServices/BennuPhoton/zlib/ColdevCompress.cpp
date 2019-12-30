#include "zlib.h"
#include "string.h"
#include "malloc.h"
#include "ColdevCompress.h"
#include "stdio.h"

ColdevCompressData::ColdevCompressData()
{
	//pCompressedData =  NULL;
	//pUncompressedData =  NULL;
}


ColdevCompressData::~ColdevCompressData()
{
	//if (pCompressedData)		delete pCompressedData;

	//if (pUncompressedData)		delete pUncompressedData;
}


int ColdevCompressData::cd_compressBytes(const unsigned char *pData, unsigned long nDataSize, int *destbuffer, int *destlength)
{
 
	*destlength = 0;

//	printf("======================   \n");
//	printf("comp Initial size: %d\n", nDataSize);

	unsigned long nCompressedDataSize = nDataSize * 2;

	//if (pCompressedData) delete pCompressedData;
	
	//  pCompressedData = new unsigned char[nCompressedDataSize];

	  // if (!pCompressedData) return Z_BUF_ERROR; 



	int nResult = compress2(pCompressedData, &nCompressedDataSize, pData, nDataSize, 9);

	if (nResult == Z_OK)
	{
//		printf("Compressed size: %d\n", nCompressedDataSize);

		*destbuffer = (int)pCompressedData;
		*destlength = nCompressedDataSize;
	}

	return nResult;

}

 

int ColdevCompressData::cd_uncompressBytes(unsigned char * pCompressedData, unsigned long nCompressedDataSize, int *destbuffer, int *destlength)
{
	 
	unsigned long nDataSize = *destlength;

//	if (pUncompressedData) delete pUncompressedData;

 	//  pUncompressedData = new unsigned char[nDataSize];

	 

//	printf("Uncomp Initial size: %d  Original %d pointerD %d pointerO %d \n", nCompressedDataSize, nDataSize,	pUncompressedData, destbuffer);
	 // if (!pUncompressedData) return Z_BUF_ERROR;


	int	nResult = uncompress(pUncompressedData, &nDataSize, pCompressedData, nCompressedDataSize);

	if (nResult == Z_OK)
	{
	//	printf("Uncompressed size: %d\n", nDataSize);

		*destbuffer = (int)pUncompressedData;
		*destlength = nDataSize;
	}

//	printf("======================  %d \n", nResult);

	return nResult;

}



void ComprimeX()
{
	// original string len = 36
	char a[50] = "Hello Hello Hello Hello Hello Hello!";

	// placeholder for the compressed (deflated) version of "a" 
	char b[50];

	// placeholder for the UNcompressed (inflated) version of "b"
	char c[50];


	printf("Uncompressed size is: %lu\n", strlen(a));
	printf("Uncompressed string is: %s\n", a);


	// zlib struct
	z_stream defstream;
	defstream.zalloc = Z_NULL;
	defstream.zfree = Z_NULL;
	defstream.opaque = Z_NULL;

	// setup "a" as the input and "b" as the compressed output
	defstream.avail_in = (uInt)strlen(a) + 1; // size of input, string + terminator
	defstream.next_in = (Bytef *)a; // input char array
	defstream.avail_out = (uInt)sizeof(b); // size of output
	defstream.next_out = (Bytef *)b; // output char array

									 // the actual compression work.
	deflateInit(&defstream, Z_BEST_COMPRESSION);
	deflate(&defstream, Z_FINISH);
	deflateEnd(&defstream);

	// This is one way of getting the size of the output
	printf("Compressed size is: %lu\n", strlen(b));
	printf("Compressed string is: %s\n", b);
}



void DescomprimeX()
{
	// original string len = 36
	char a[50] = "Hello Hello Hello Hello Hello Hello!";

	// placeholder for the compressed (deflated) version of "a" 
	char b[50];

	// placeholder for the UNcompressed (inflated) version of "b"
	char c[50];


 
	// STEP 1.
	// deflate a into b. (that is, compress a into b)

	// zlib struct
	z_stream defstream;
	defstream.zalloc = Z_NULL;
	defstream.zfree = Z_NULL;
	defstream.opaque = Z_NULL;
	// setup "a" as the input and "b" as the compressed output
	defstream.avail_in = (uInt)strlen(a) + 1; // size of input, string + terminator
	defstream.next_in = (Bytef *)a; // input char array
	defstream.avail_out = (uInt)sizeof(b); // size of output
	defstream.next_out = (Bytef *)b; // output char array

									 // the actual compression work.
	deflateInit(&defstream, Z_BEST_COMPRESSION);
	deflate(&defstream, Z_FINISH);
	deflateEnd(&defstream);

	// This is one way of getting the size of the output
	printf("Compressed size is: %lu\n", strlen(b));
	printf("Compressed string is: %s\n", b);


	printf("\n----------\n\n");


	// STEP 2.
	// inflate b into c
	// zlib struct
	z_stream infstream;
	infstream.zalloc = Z_NULL;
	infstream.zfree = Z_NULL;
	infstream.opaque = Z_NULL;
	// setup "b" as the input and "c" as the compressed output
	infstream.avail_in = (uInt)((char*)defstream.next_out - b); // size of input
	infstream.next_in = (Bytef *)b; // input char array
	infstream.avail_out = (uInt)sizeof(c); // size of output
	infstream.next_out = (Bytef *)c; // output char array

									 // the actual DE-compression work.
	inflateInit(&infstream);
	inflate(&infstream, Z_NO_FLUSH);
	inflateEnd(&infstream);

	//printf("Uncompressed size is: %lu\n", strlen(c));
	printf("Uncompressed size is: %lu\n", infstream.total_out);
	printf("Uncompressed string is: %s\n", c);
}
