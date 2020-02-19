	#include <jde.h>

#define b57sstr_c


/*****************************************************************************
 *    Source File:  b57sstr
 *
 *    Description:  Substitui string em string Source File
 *
 *        History:
 *          Date        Programmer  SAR# - Description
 *          ----------  ----------  -------------------------------------------
 *   Author 10/08/12                Unknown  - Created  
 *
 * Copyright (c) J.D. Edwards World Source Company, 1996
 *
 * This unpublished material is proprietary to J.D. Edwards World Source Company.
 * All rights reserved.  The methods and techniques described herein are
 * considered trade secrets and/or confidential.  Reproduction or
 * distribution, in whole or in part, is forbidden except by express
 * written permission of J.D. Edwards World Source Company.
 ****************************************************************************/
/**************************************************************************
 * Notes:
 *
 **************************************************************************/

#include <b57sstr.h>


/**************************************************************************
 *  Business Function:  SubstString
 *
 *        Description:  Substitui String em String
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57SSTR         lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI SubstString (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57SSTR lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
    JCHAR              szBuffer[30001];                     
	JCHAR              *p;
	JCHAR              cContinua = _J('S');
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
	if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57SSTR)	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
	while (cContinua == _J('S'))
	{
		 if(!(p = jdeStrstr(lpDS->szString, lpDS->szStringaSubstituir)))
		 {
			 //
			 cContinua = _J('N');
			 //
		 } else {
			//
			jdeStrncpy(szBuffer, lpDS->szString, p - lpDS->szString); 
			szBuffer[p-lpDS->szString] = _J('\0');
			jdeSprintf(szBuffer+(p-lpDS->szString), _J("%ls%ls"), lpDS->szStringSubstituta, p + jdeStrlen(lpDS->szStringaSubstituir));
			//
	    	jdeStrncpy(lpDS->szString, szBuffer, DIM(szBuffer));
		    //
		 }
		 //
	}      

   /************************************************************************
    * Function Clean Up
    ************************************************************************/

   return (ER_SUCCESS);
}

/* Internal function comment block */
/**************************************************************************
 *   Function:  Ixxxxxxx_a   // Replace "xxxxxxx" with source file number
 *                           // and "a" with the function name
 *      Notes:
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/



 
/************************************************************************** 
 *  Business Function:  PesquisaStringLista 
 * 
 *        Description:  Pesquisa String em Lista "," 
 * 
 *         Parameters: 
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications 
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE! 
 *           LPDSD57S            lpDS         Parameter Data Structure Pointer   
 * 
 *************************************************************************/ 
 
JDEBFRTN (ID) JDEBFWINAPI PesquisaStringLista (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57SSTRA lpDS)  
 
{ 
   /************************************************************************ 
    *  Variable declarations 
    ************************************************************************/ 
    int     nPosicao      = 0;
	int     nPosicaoSubString = 0;
    int     nTamanho      = 0;
	JCHAR   SubString[30] = {0};
 
   /************************************************************************ 
    * Declare structures 
    ************************************************************************/ 
 
   /************************************************************************ 
    * Declare pointers 
    ************************************************************************/ 
 
   /************************************************************************ 
    * Check for NULL pointers 
    ************************************************************************/ 
   if ((lpBhvrCom == (LPBHVRCOM) NULL) || 
       (lpVoid    == (LPVOID)    NULL) || 
       (lpDS      == (LPDSD57SSTRA)	NULL)) 
   { 
     jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL); 
     return ER_ERROR; 
   } 
 
   /************************************************************************ 
    * Set pointers 
    ************************************************************************/ 
 
   /************************************************************************ 
    * Main Processing 
    ************************************************************************/ 
    lpDS->cAchou = _J('N');
    nTamanho = jdeStrlen(lpDS->szLista);
	for (nPosicao = 0; nPosicao <= nTamanho; nPosicao++) {
		if ((lpDS->szLista[nPosicao] == _J(','))  || (nPosicao == nTamanho)) {
			//
			if (jdeStrcmp(SubString, lpDS->szItem) == 0) {
				lpDS->cAchou = _J('S');
				nPosicao += nTamanho;
			};
			//
			memset((void *)(SubString), (int)(_J('\0')), sizeof(SubString));
			nPosicaoSubString = 0;
		} 
		else {
			if (lpDS->szLista[nPosicao] != _J(' ')) {
				SubString[nPosicaoSubString] = lpDS->szLista[nPosicao];
				nPosicaoSubString++;
			}
		};
	};
   /************************************************************************ 
    * Function Clean Up 
    ************************************************************************/ 
 
   return (ER_SUCCESS); 
} 
 
