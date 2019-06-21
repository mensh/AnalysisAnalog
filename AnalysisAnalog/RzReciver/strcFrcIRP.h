#ifndef STRCFRCIRPH
#define STRCFRCIRPH

class cFrcIRP
{
private:
   unsigned char *pucBYTE;
   unsigned int uiSIZE;

public:

   cFrcIRP (void)
   {
      uiSIZE = 0;
      pucBYTE = NULL;
   };

   ~cFrcIRP (void)
   {
      ClearMem();
   }

   void ClearMem (void)
   {
      delete [] pucBYTE;
      pucBYTE = NULL;

      uiSIZE = 0;
   }

   bool SetSizeFrcIRP (unsigned int uiSize)
   {
      if (pucBYTE) ClearMem();

      pucBYTE = new unsigned char [uiSize];

      if (pucBYTE)
      {
         uiSIZE = uiSize;
         ClearFrcIRP();
      }

      return (pucBYTE != NULL);
   };

   unsigned int GetSizeFrcIRP (void)
   {
      return uiSIZE;
   };

   unsigned char &operator[] (unsigned int Index)
   {
      if (!pucBYTE || Index >= uiSIZE) throw;

      return pucBYTE[Index];
   };

   void ClearFrcIRP (void)
   {
      ZeroMemory(pucBYTE, uiSIZE);
   }

   unsigned char *GetPtrFrcIRP (void)
   {
      return pucBYTE;
   }

   void SetFrcIRPFromBuffer (unsigned char *Buffer)
   {
      if (!pucBYTE) throw;

      CopyMemory(pucBYTE, Buffer, uiSIZE);
   }
};

#endif
