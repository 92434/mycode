#include <stdio.h>
//#include <windows.h>
#include <string.h>
using namespace std;

char line[100] = {0};

FILE *OpenFile(char const *name)
{
	return fopen(name, "rb");
}

void CloseFile(FILE *file)
{
	fclose(file);
}
int FGetLine(FILE *filehandle, char *pBuffer, int BufferSize)
{
    unsigned char ch;
    int index = 0;

    memset(pBuffer, 0, BufferSize);
    while(fread(&ch, sizeof(char), 1, filehandle))
    {
        pBuffer[index] = ch;
        index++;

        if(pBuffer[index - 1] == 0x0a && pBuffer[index - 2] == 0x0d)
        {
            pBuffer[index - 1] = 0;
			pBuffer[index - 2] = 0;
            break;
        }
    }

    return index;
}

int main( int argc, char *argv[] )
{
	FILE *datFile    = NULL;
	datFile = OpenFile("123.txt");
	if(datFile){
		while(FGetLine(datFile, line, 100))
		{
			printf("%s\n",line);
		}
	}

	CloseFile(datFile);

	char * const pline = line;
    pline[0] = 'a';
    line[0] = 'a';
//    pline++;
	return 0;
}

