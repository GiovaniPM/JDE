
/*****************************************************************************
 *    Header File:  B57SSTR.h
 *
 *    Description:  Substitui string em string Header File
 *
 *        History:
 *          Date        Programmer  SAR# - Description
 *          ----------  ----------  -------------------------------------------
 *   Author 10/08/12                Unknown  - Created  
 *
 *
 * Copyright (c) J.D. Edwards World Source Company, 1996
 *
 * This unpublished material is proprietary to J.D. Edwards World Source 
 * Company.  All rights reserved.  The methods and techniques described 
 * herein are considered trade secrets and/or confidential.  Reproduction
 * or distribution, in whole or in part, is forbidden except by express
 * written permission of J.D. Edwards World Source Company.
 ****************************************************************************/

#ifndef __B57SSTR_H
#define __B57SSTR_H

/*****************************************************************************
 * Global Definitions
 ****************************************************************************/

/*****************************************************************************
 * DS Template Type Definitions
 ****************************************************************************/
/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados da função B57SSTR
 *    Template ID:   D57SSTR
 *    Generated:     Fri Aug 10 14:36:56 2012
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57SSTR
#define DATASTRUCTURE_D57SSTR

typedef struct tagDSD57SSTR
{
  JCHAR              szString[30001];                     
  JCHAR              szStringaSubstituir[301];            
  JCHAR              szStringSubstituta[301];             
} DSD57SSTR, *LPDSD57SSTR;

#define IDERRszString_1                           1L
#define IDERRszStringaSubstituir_2                2L
#define IDERRszStringSubstituta_3                 3L

#endif


/**
 * TYPEDEF for Data Structure
 * D57SSTRA : EStrutura de dados da bsfn B57SSTRA
 *
 * Copyright Oracle USA
 *
 * This is a JDEdwards EnterpriseOne generated file.
 * Do not modify this file.
 * Only re-generate with the appropriate tool.
 * Generation Date : 13/06/14
 */

#ifndef DATASTRUCTURE_D57SSTRA
#define DATASTRUCTURE_D57SSTRA

typedef struct tagDSD57SSTRA
{
  JCHAR          szLista[501];
  JCHAR          szItem[31];
  JCHAR          cAchou;
} DSD57SSTRA , *LPDSD57SSTRA;

#define IDERRszLista_1                            1L
#define IDERRszItem_2                             2L
#define IDERRcAchou_3                             3L

#endif /* DATASTRUCTURE_D57SSTRA */



/*****************************************************************************
 * Source Preprocessor Definitions
 ****************************************************************************/
#if defined (JDEBFRTN)
	#undef JDEBFRTN
#endif

#if defined (WIN32)
	#if defined (WIN32)
		#define JDEBFRTN(r) __declspec(dllexport) r
	#else
		#define JDEBFRTN(r) __declspec(dllimport) r
	#endif
#else
	#define JDEBFRTN(r) r
#endif

/*****************************************************************************
 * Business Function Prototypes
 ****************************************************************************/
JDEBFRTN (ID) JDEBFWINAPI SubstString                      (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57SSTR lpDS); 
 JDEBFRTN (ID) JDEBFWINAPI PesquisaStringLista              (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57SSTRA lpDS);


/*****************************************************************************
 * Internal Function Prototypes
 ****************************************************************************/

#endif    /* __B57SSTR_H */

