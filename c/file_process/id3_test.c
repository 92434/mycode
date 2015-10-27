/***************************************************************************************************
NAME  =  id3v2.h ;
FUNC  =  ?????id3v2?? ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <memory.h> 
#include <windows.h> 

#define ARTISTS_ID      0
#define ALBUMS_ID       1

typedef struct {
  char        cFrame_ID[4];                                                                                                                                        /* frameID */
  char        cFrame_size[4];
  char        cFrame_Flag[2];
  int        iFrame_size;                                                                                                                                                /* frame size */
  int        IsTagalter;                                                                                                                                                        /* Tag alter preservation */
  int        IsFilealter;                                                                                                                                                /* File alter preservation */
  int        IsRO;                                                                                                                                                                                /* Read only */
  int        IsCompre;                                                                                                                                                                /* Compression */
  int        IsEncr;                                                                                                                                                                        /* Encryption */
  int        HasGroup;                                                                                                                                                                /* Grouping identity */
  int        pos;                                                                                                                                                                                /* ?frame???mp3??? */
} ID3V2_INFO_FRAME_HEADER;


typedef struct node{
  ID3V2_INFO_FRAME_HEADER frame_header;     /* frame header */
  char        *content;                                                                                                                                                        /* frame ??? */
  struct node *next;
} ID3V2_INFO_FRAME;

typedef struct {
  char  identifier[3];                                                                                                                                /* tag header?????? */
  struct {
    char        major_version;
    char        revision;
  } Version;                                                                                                                                                                        /* ?? */
  char        flag;
  int        IsUnsynchr;                                                                                                                                                        /* Unsynchronisation?? */
  int        HasExtHeader;                                                                                                                                                /* ??header */
  int        IsExper;                                                                                                                                                                /* Experimental indicator */
  char        cInfoSize[4];                                                                                                                                       
  int        iInfoSize;                                                                                                                                                        /* total tag size - 10 */       
  int HasTag;                                                                                                                                                       
} ID3V2_INFO_HEADER;


typedef struct {                                                                                                                                                        /* ID3V2?? */
  ID3V2_INFO_HEADER header;                                                                                                                /* tag header */
  ID3V2_INFO_FRAME *frame;                                                                                                                /* frame????????? */
  int count;                                                                                                                                                                        /* frame??? */
} ID3V2_INFO;



char * jump_pad(char *p)
{
    while(!*p)
    {
        p++;
    }
    return p;
}
//extern int Check_ID3V2_3(FILE *mp3, ID3V2_INFO *info);
//extern void Read_frame(FILE *mp3, ID3V2_INFO *info);
//extern void Frame_free(ID3V2_INFO *info);
//extern void Edit_frame(char Frame_ID, char *padding, FILE *mp3, ID3V2_INFO * info);
/***************************************************************************************************
NAME  =  id3v2.c ;
FUNC  =  ??id3v2 ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/


//#include "id3v2.h"

//static void getnbyte(FILE *mp3,int n, char *byte);
//static void Getpadding(FILE *mp3,ID3V2_INFO_FRAME *frame);
//static void Content_free (char *str);
/***************************************************************************************************
NAME  =  Check_ID3V2_3 ;
FUNC  =  ???????ID3V2.3 ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
static void getnbyte(FILE *mp3,int n, char *byte)
{
        int i = 0;
		char * temp_byte = byte;
        for ( ;i < n ;i++)
        {
                *temp_byte++ = fgetc(mp3);
        }
}



/***************************************************************************************************
NAME  =  Check_ID3V2_3 ;
FUNC  =  ???????ID3V2.3 ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
static void Content_free (char *str)
{
        if (NULL != str)\
                free(str);
}


/***************************************************************************************************
NAME  =  Check_ID3V2_3 ;
FUNC  =  ???????ID3V2.3 ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
void print(ID3V2_INFO *info)
{
		int iContent = 0;
		ID3V2_INFO_FRAME *pCurframe = NULL;
        if (NULL == info)
        {
                printf("parameter is NULL!\n");
                return ;
        }
       
        if(0 == info->header.HasTag)
        {
                printf("No tag for id3v2!\n");
                return ;
        }
       

       
        printf("TAG is %c%c%c""V2.""%d.%d\n",info->header.identifier[0],
                        info->header.identifier[1],
                        info->header.identifier[2],
                        (int)info->header.Version.major_version,
                        (int)info->header.Version.revision);
        printf("Unsynchronisation: %d\n", info->header.IsUnsynchr);
        printf("Extended header: %d\n", info->header.HasExtHeader);
        printf("Experimental indicator: %d\n", info->header.IsExper);
        printf("ID3v2 tag size: %d\n\n", info->header.iInfoSize);
       
        if(NULL != info->frame)
        {
                pCurframe = info->frame;
        }
       
        for(; NULL != pCurframe; )
        {
                iContent = 0;
                iContent += jump_pad(pCurframe->content) - pCurframe->content;
                printf("Frame ID: %s\n",pCurframe->frame_header.cFrame_ID);
                printf("Frame size: %d\n",pCurframe->frame_header.iFrame_size);
                printf("Tag alter preservation: %d\n",pCurframe->frame_header.IsTagalter);
                printf("File alter preservation: %d\n",pCurframe->frame_header.IsFilealter);
                printf("Read only: %d\n",pCurframe->frame_header.IsRO);
                printf("Compression: %d\n",pCurframe->frame_header.IsCompre);
                printf("Encryption: %d\n",pCurframe->frame_header.IsEncr);
                printf("Grouping identity: %d\n",pCurframe->frame_header.HasGroup);
                printf("frame start pos: %d\n",pCurframe->frame_header.pos);
               

                printf("content:");
                for( ;iContent < pCurframe->frame_header.iFrame_size; iContent++)
                {
                        printf("%c",pCurframe->content[iContent]);
                }
                printf("\n\n");
               
                pCurframe = pCurframe->next;
        }

}



/***************************************************************************************************
NAME  =  Check_ID3V2_3 ;
FUNC  =  ???????ID3V2.3 ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
void change_id3_info(ID3V2_INFO *info,char *buffer, int pid, int pid_index)
{
	int iContent = 0;
	ID3V2_INFO_FRAME *pCurframe = NULL;
	char pid_index_num[4];




    
    if (NULL == info)
    {
            printf("parameter is NULL!\n");
			system("pause");
            return ;
    }
   
    if(0 == info->header.HasTag)
    {
            printf("No tag for id3v2!\n");
			system("pause");
            return ;
    }
          
    if(NULL != info->frame)
    {
            pCurframe = info->frame;
    }
   
    for(; NULL != pCurframe; )
    {
        switch(pid)
        {
            case ARTISTS_ID:
                if(pCurframe->frame_header.cFrame_ID[0] == 'T' && pCurframe->frame_header.cFrame_ID[1] == 'P' && pCurframe->frame_header.cFrame_ID[2] == 'E'&& pCurframe->frame_header.cFrame_ID[3] == '1')
                {
                    char *p;
                    //int i;
                    p = &buffer[pCurframe->frame_header.pos + 10 + pCurframe->frame_header.iFrame_size];
                    //fseek(mp3, 0, SEEK_SET);
                    //i = pCurframe->frame_header.pos + pCurframe->frame_header.iFrame_size - 4;
                    //fread(pid_index_num, sizeof(char), 4, mp3);
                    pid_index_num[3] = pid_index % 10 +0x30;
                    pid_index_num[2] = (pid_index / 10) % 10 +0x30;
                    pid_index_num[1] = (pid_index / 100) % 10 +0x30;
                    pid_index_num[0] = (pid_index / 1000) % 10 +0x30;
                    *(--p) = pid_index_num[3];
                    *(--p) = pid_index_num[2];
                    *(--p) = pid_index_num[1];
                    *(--p) = pid_index_num[0];
					///system("@echo modified atirst >message.txt");
					///system("message.txt");
					///system("del message.txt");
                }
                break;
            case ALBUMS_ID:
                if(pCurframe->frame_header.cFrame_ID[0] == 'T' && pCurframe->frame_header.cFrame_ID[1] == 'A' && pCurframe->frame_header.cFrame_ID[2] == 'L'&& pCurframe->frame_header.cFrame_ID[3] == 'B')
                {
                    char *p;
                    //int i;
                    p = &buffer[pCurframe->frame_header.pos + 10 + pCurframe->frame_header.iFrame_size];
                    //fseek(mp3, 0, SEEK_SET);
                    //i = pCurframe->frame_header.pos + pCurframe->frame_header.iFrame_size - 4;
                    //fread(pid_index_num, sizeof(char), 4, mp3);
                    pid_index_num[3] = pid_index % 10 +0x30;
                    pid_index_num[2] = (pid_index / 10) % 10 +0x30;
                    pid_index_num[1] = (pid_index / 100) % 10 +0x30;
                    pid_index_num[0] = (pid_index / 1000) % 10 +0x30;
                    *(--p) = pid_index_num[3];
                    *(--p) = pid_index_num[2];
                    *(--p) = pid_index_num[1];
                    *(--p) = pid_index_num[0];
                }
                break;
			default:
				break;
        }
        
        pCurframe = pCurframe->next;
    }
}



/***************************************************************************************************
NAME  =  Frame_free ;
FUNC  =  ??info??frame?? ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
void Frame_free(ID3V2_INFO *info)
{
       
        if((NULL == info)||(NULL == info->frame))
                return ;
       
        if(0 == info->header.HasTag)
        {
                printf("No tag for id3v2!\n");
				system("pause");
                return ;
        }
       
        while(1)
        {
                        if((NULL!=info->frame)&&(NULL==info->frame->next))
                {
                        Content_free(info->frame->content);
                        free(info->frame);
                        info->frame = NULL;
                        return ;
                }
       
                if((NULL!=info->frame->next)&&(NULL==info->frame->next->next))
                {
                        Content_free(info->frame->next->content);
                        free(info->frame->next);
                        info->frame->next = NULL;
                }
       
                        if((NULL!=info->frame->next) && NULL!=info->frame->next->next)
                {
                        ID3V2_INFO_FRAME *p = info->frame->next;
                        info->frame->next = info->frame->next->next;
                        Content_free(p->content);
                        free(p);
                }
        }
}


/***************************************************************************************************
NAME  =  Edit_ID3V2 ;
FUNC  =  ??ID3V2?? ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
void Edit_ID3V2(char Frame_ID, char *padding, FILE *mp3, ID3V2_INFO * info)
{
}

/***************************************************************************************************
NAME  =  Getpadding ;
FUNC  =  ??frame?padding ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
static void Getpadding(FILE *mp3,ID3V2_INFO_FRAME *frame)
{

        int i = 0;
       
        frame->content = (char *)malloc(sizeof(char)*(frame->frame_header.iFrame_size));
        memset(frame->content, 0 , sizeof(char)*frame->frame_header.iFrame_size);
        fseek(mp3,(frame->frame_header.pos)+10,SEEK_SET);
       
        getnbyte(mp3, frame->frame_header.iFrame_size, frame->content);
       
        return ;

}


void Read_frame(FILE *mp3, ID3V2_INFO *info)
{
		ID3V2_INFO_FRAME *frame = NULL;
        ID3V2_INFO_FRAME *frame_cur =NULL;
        int pos = 10;
       
        if ((NULL == mp3)||(NULL == info))
        {
                printf("parameter is NULL!\n");
                return ;
        }
       
        if(0 == info->header.HasTag)
        {
                printf("No tag for id3v2!\n");
                return ;
        }
       
        for( ; pos < (info->header.iInfoSize) ; )
        {
                char frame_header[10];
               
                frame = (ID3V2_INFO_FRAME *) malloc (sizeof(ID3V2_INFO_FRAME));
                memset(frame, 0, sizeof(ID3V2_INFO_FRAME));
                       
                fseek(mp3,pos,SEEK_SET);
                getnbyte(mp3,10,frame_header);
                if(!frame_header[0])
                        return ;
               
                frame->frame_header.cFrame_ID[0] = frame_header[0];
                frame->frame_header.cFrame_ID[1] = frame_header[1];
                frame->frame_header.cFrame_ID[2] = frame_header[2];
                frame->frame_header.cFrame_ID[3] = frame_header[3];
                frame->frame_header.cFrame_size[0] = frame_header[4];
                frame->frame_header.cFrame_size[1] = frame_header[5];
                frame->frame_header.cFrame_size[2] = frame_header[6];
                frame->frame_header.cFrame_size[3] = frame_header[7];
                frame->frame_header.cFrame_Flag[0] = frame_header[8];
                frame->frame_header.cFrame_Flag[1] = frame_header[9];
               
                frame->frame_header.iFrame_size = frame->frame_header.cFrame_size[3] +
                                                                                                                                                        frame->frame_header.cFrame_size[2] * (1<<8) +
                                                                                                                                                        frame->frame_header.cFrame_size[1] * (1<<16) +
                                                                                                                                                        frame->frame_header.cFrame_size[0] * (1<<24);
               
                if (frame->frame_header.cFrame_size[0] & (1<<8))
                {
                        frame->frame_header.IsTagalter = 1;
                }
               
                if (frame->frame_header.cFrame_size[0] & (1<<7))
                {
                        frame->frame_header.IsFilealter = 1;
                }
               
                if (frame->frame_header.cFrame_size[0] & (1<<6))
                {
                        frame->frame_header.IsRO = 1;
                }
               
                if (frame->frame_header.cFrame_size[1] & (1<<8))
                {
                        frame->frame_header.IsCompre = 1;
                }
               
                if (frame->frame_header.cFrame_size[1] & (1<<7))
                {
                        frame->frame_header.IsEncr = 1;
                }
               
                if (frame->frame_header.cFrame_size[1] & (1<<6))
                {
                        frame->frame_header.HasGroup = 1;
                }
               
                frame->frame_header.pos = pos;
               
                pos += (10 + frame->frame_header.iFrame_size);
                       
                Getpadding(mp3,frame);
               
                frame->next = NULL;
               
                info->count++;
               
               
               
                if(1 == info->count)
                {
                        info->frame = frame;
                } else {
                        frame_cur->next = frame;
                }               
                frame_cur = frame;
        }
       
}
/***************************************************************************************************
NAME  =  Check_ID3V2_3 ;
FUNC  =  ???????ID3V2.3 ;
DATE  =  2008-03-03 ;
AUTH  =  ?? ;
HIST  =  ??_0100, 2008-03-03, ??, Original ;
***************************************************************************************************/
int Check_ID3V2_3(FILE *mp3, ID3V2_INFO *info)
{
		char cHeader[10];

        if ((NULL == mp3)||(NULL == info))
        {
                printf("parameter is NULL!");
                return 0;
        }

        info->header.HasTag = 0;
        info->header.IsUnsynchr = 0;
        info->header.HasExtHeader = 0;
        info->header.IsExper = 0;
       
        getnbyte(mp3,10,cHeader);
       
        if (('I' == cHeader[0]) && ('D' == cHeader[1]) && ('3' == cHeader[2]) && (3 == cHeader[3]))
        {
                info->header.HasTag = 1;
                info->header.identifier[0] = 'I';
                info->header.identifier[1] = 'D';
                info->header.identifier[2] = '3';
                info->header.Version.major_version = cHeader[3];
                info->header.Version.revision = cHeader[4];
                info->header.flag = cHeader[5];
                info->header.cInfoSize[0] = cHeader[6];
                info->header.cInfoSize[1] = cHeader[7];
                info->header.cInfoSize[2] = cHeader[8];
                info->header.cInfoSize[3] = cHeader[9];
        } else {
                return 0;
        }
       
        if (info->header.flag & (1<<8))
        {
                info->header.IsUnsynchr = 1;
        }
       
        if (info->header.flag & (1<<7))
        {
                info->header.HasExtHeader = 1;
        }
       
        if (info->header.flag & (1<<6))
        {
                info->header.IsExper = 1;
        }
       
        info->count = 0;
        info->frame = NULL;
       
        info->header.iInfoSize = (info->header.cInfoSize[3]&0x7F) + (info->header.cInfoSize[2]&0x7F) * (0x80) +
                        (info->header.cInfoSize[1]&0x7F) * (0x400) + (info->header.cInfoSize[0]&0x7F) * (0x200000);
       
        //        printf("test OK");
       
        return 1;
}
/*
int copyfile(char *src_file_name, char *dst_file_name)
{
    FILE *src_file;
    FILE *dst_file;
    char *temp_buffer;
    int filename_index = 0;

    for(filename_index = 0; filename_index < 5; filename_index++)
    {
        int file_size;
        int pos;
        char pad_num[4];
        
        pad_num[0] = filename_index % 10 + 0x30;
        pad_num[1] = (filename_index / 10) % 10 + 0x30;
        pad_num[2] = (filename_index / 100) % 10 + 0x30;
        pad_num[3] = (filename_index / 1000) % 10 + 0x30;
        pos = strlen(dst_file_name) - 5;
        dst_file_name[pos--] = pad_num[0];
        dst_file_name[pos--] = pad_num[1];
        dst_file_name[pos--] = pad_num[2];
        dst_file_name[pos--] = pad_num[3];
        
        src_file = fopen(src_file_name, "rb");        
        fseek(src_file, 0, SEEK_END);
        file_size = ftell(src_file);
        temp_buffer = malloc(file_size);
        fseek(src_file, 0, SEEK_SET);
        fread(temp_buffer, sizeof(char), file_size, src_file);
        fclose(src_file);

        
        if(dst_file = fopen(dst_file_name, "wb+"))
        {
            fwrite(temp_buffer, sizeof(char), file_size, dst_file);
        }
        fclose(dst_file);
        free(temp_buffer);
    }
    return 0;
}*/


int copyfile(char *src_file_name, char *dst_file_name)
{
    FILE *src_file;
    FILE *dst_file;
    char *temp_buffer;
    int file_size;

    src_file = fopen(src_file_name, "rb");        
    fseek(src_file, 0, SEEK_END);
    file_size = ftell(src_file);
    temp_buffer = malloc(file_size);
    fseek(src_file, 0, SEEK_SET);
    fread(temp_buffer, sizeof(char), file_size, src_file);
    fclose(src_file);

    
    if(dst_file = fopen(dst_file_name, "wb+"))
    {
        fwrite(temp_buffer, sizeof(char), file_size, dst_file);
    }
    fclose(dst_file);
    free(temp_buffer);
    return 0;
}

int change_id3(char *dst_file_name, int pid1, int pid_index1, int pid2, int pid_index2)
{
    ID3V2_INFO info;
    FILE *mp3;
    FILE *src_file;
    FILE *dst_file;
    char *temp_buffer;
    int file_size;

    mp3 = fopen("111.mp3","rb");
    if (NULL == mp3)
    {
            printf("mp3 open false\n");
			system("pause");
            return 0;
    }
    Check_ID3V2_3(mp3,&info);
    Read_frame(mp3,&info);
    fclose(mp3);
    print(&info);


    src_file = fopen("111.mp3", "rb");        
    fseek(src_file, 0, SEEK_END);
    file_size = ftell(src_file);
    temp_buffer = malloc(file_size);
    fseek(src_file, 0, SEEK_SET);
    fread(temp_buffer, sizeof(char), file_size, src_file);
    fclose(src_file);
    
    change_id3_info(&info, temp_buffer, pid1, pid_index1);
    change_id3_info(&info, temp_buffer, pid2, pid_index2);
    
    if(dst_file = fopen(dst_file_name, "wb+"))
    {
        fwrite(temp_buffer, sizeof(char), file_size, dst_file);
    }
    fclose(dst_file);
    free(temp_buffer);


    
    Frame_free(&info);
    return 0;
}
  

/* test */
int main()
{
    int artist_index_loop, album_index_loop, song_index_loop = 0;
    char src_file_name[] = "111.mp3";
    char dst_file_name[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz0000.mp3";



    //copyfile(src_file_name, dst_file_name);
    for(artist_index_loop = 0; artist_index_loop < 4; artist_index_loop++)
    {
        for(album_index_loop = 0; album_index_loop < 1; album_index_loop++)
        {
            //for(; song_index_loop < 4; song_index_loop++)
            {
				int pos;
                char pad_num[4];
                pad_num[3] = song_index_loop % 10 + 0x30;
                pad_num[2] = (song_index_loop / 10) % 10 + 0x30;
                pad_num[1] = (song_index_loop / 100) % 10 + 0x30;
                pad_num[0] = (song_index_loop / 1000) % 10 + 0x30;
                pos = strlen(dst_file_name) - 5;
                dst_file_name[pos--] = pad_num[3];
                dst_file_name[pos--] = pad_num[2];
                dst_file_name[pos--] = pad_num[1];
                dst_file_name[pos--] = pad_num[0];
                change_id3(dst_file_name, ARTISTS_ID, artist_index_loop,ALBUMS_ID, album_index_loop);
				song_index_loop++;
            }
        }
    }
    return 1;
}
