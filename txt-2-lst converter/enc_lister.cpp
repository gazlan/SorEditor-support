/* ******************************************************************** **
** @@ ENC Lister
** @  Copyrt : 
** @  Author : 
** @  Update :
** @  Update :
** @  Notes  :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "..\shared\file_find.h"

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif 

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ MakePrintable()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BYTE __fastcall MakePrintable(BYTE byChr)
{
   if (byChr < 0x20)
   {
      return 0x2E;
   }

   if (byChr == 0x7F)
   {
      return 0x2E;
   }

   return byChr;
}

/* ******************************************************************** **
** @@ Proceed()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Proceed(const char* const pszFileName)
{
   char     pszListName[_MAX_PATH];
   char     pszDrive   [_MAX_DRIVE];
   char     pszDir     [_MAX_DIR];
   char     pszFName   [_MAX_FNAME];
   char     pszExt     [_MAX_EXT];

   _splitpath(pszFileName,pszDrive,pszDir,pszFName,pszExt);
   _makepath( pszListName, pszDrive,pszDir,pszFName,"lst");

   FILE*    pIn = fopen(pszFileName,"rt");

   if (!pIn)
   {
      perror("\a\nOpen Input File Error !\n");
      return;
   }

   FILE*    pOut = fopen(pszListName,"wt");

   if (!pOut)
   {
      perror("\a\nOpen Output File Error !\n");
      fclose(pIn);
      pIn = NULL;
      return;
   }

   const DWORD    BUF_SIZE = (0x01 << 20);   // About 1 Mb !

   char*    pBuf = new char[BUF_SIZE];

   if (!pBuf)
   {
      fclose(pIn);
      pIn = NULL;
      fclose(pOut);
      pOut = NULL;
      return;
   }

   while (fgets(pBuf,BUF_SIZE,pIn))
   {
      pBuf[BUF_SIZE - 1] = 0; // ASCIIZ !

      DWORD    dwEOL = strcspn(pBuf,"\r\n");

      pBuf[dwEOL] = 0;  // Remove EOL chars

      int   iValue = atoi(pBuf);

      fprintf(pOut,"%12d\t %02X\t %c \t ; %s\r\n",iValue,iValue,MakePrintable(iValue),pBuf);
   }

   delete[] pBuf;
   pBuf = NULL;

   fclose(pIn);
   pIn = NULL;

   fclose(pOut);
   pOut = NULL;
}

/* ******************************************************************** **
** @@ ShowHelp()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void ShowHelp()
{
   const char  pszCopyright[] = "-*-   ENC Lister   *   (c) Gazlan, 2017   -*-";
   const char  pszDescript [] = "SorEditor ENC Lister";
   const char  pszE_Mail   [] = "complains_n_suggestions direct to gazlan@yandex.ru";

   printf("%s\n\n",pszCopyright);
   printf("%s\n\n",pszDescript);
   printf("Usage: enc_lister.com wildcards\n\n");
   printf("%s\n",pszE_Mail);
}

/* ******************************************************************** **
** @@ main()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

int main(int argc,char** argv)
{
   if ((argc < 2) || (argc > 4))
   {
      ShowHelp();
      return 0;
   }

   if (argc == 2 && ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h"))))
   {
      ShowHelp();
      return 0;
   }

   FindFile   FF;

   FF.SetMask(argv[1]);

   while (FF.Fetch())
   {
      if ((FF._w32fd.dwFileAttributes | FILE_ATTRIBUTE_NORMAL) && !(FF._w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
         Proceed(FF._w32fd.cFileName);
      }
   }

   return 0;
}

/* ******************************************************************** **
** @@                   The End
** ******************************************************************** */
