
/*****************************************************************************
 *    Header File:  B58RE001.h
 *
 *    Description:  Read File TXT Header File
 *
 *        History:
 *          Date        Programmer  SAR# - Description
 *          ----------  ----------  -------------------------------------------
 *   Author 04/01/2011  DEMO        Unknown  - Created  
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

#ifndef __B58RE001_H
#define __B58RE001_H

/*****************************************************************************
 * Table Header Inclusions
 ****************************************************************************/
#include<f58re001.h>
#include <f58reu01.h>
#include <f58reu02.h>
#include <f58reu03.h>
#include <f58reu04.h>
#include <f58reu05.h>
#include <f4301.h>
#include <f0101.h>
#include <f58re76a.h>
#include <f58re76b.h>
#include <f58re76c.h>
#include <f58re76d.h>
#include <f58re76g.h>
/* INICIO - JULIO DONATI - 26/07/2013 */
#include <f58reu06.h>
#include <f58re76h.h>
/* FIM    - JULIO DONATI - 26/07/2013 */

/*****************************************************************************
 * External Business Function Header Inclusions
 ****************************************************************************/
#include <b0000580.h>
#include <n3401570.h>
#include <b0800013.h>
#include <x00022.h>
#include <b8000094.h>
#include <b0100015.h>
#include <b9800100.h>
#include <b0500690.h>
#include <b58re09a.h>

/*****************************************************************************
 * Global Definitions
 ****************************************************************************/

/* INICIO - JULIO DONATI - 19/02/2019 */
/*Código Comentado*/
/*#define MAXTAMLINHA 5001*/
/*Novo Código*/
#define MAXTAMLINHA 9001
/* FIM - JULIO DONATI - 19/02/2019 */

#define TAMNUMERICO 16
#define TAMVALOR    200  
/*****************************************************************************
 * Structure Definitions
 ****************************************************************************/
 typedef struct tag_DS_DetNF
	{
		
		char         szCampo[11] ;
		char         szValor[1024];
	    char         szTipoDado[3];
		MATH_NUMERIC mnNumDecimal;   

	}DSD_DetNF, *LPDS_DetNF;

   typedef struct tag_DS_CabNF
	{
		
		char         szCampo[11] ;
		char         szValor[1024];
		char         szTipoDado[3];
		MATH_NUMERIC mnNumDecimal;
       

	}DSD_CabNF, *LPDS_CabNF;

   typedef struct tag_DS_Erro
   {		
		char         szCodErro[7] ;
		char         szMensagem[81];       
   }DSD_Erro, *LPDS_Erro;
  
  typedef struct tag_DSLinha
  {
	 MATH_NUMERIC mnBNNF;
	 MATH_NUMERIC mnBNNF_LON; /*numero NF 9 digitos*/ 
	 MATH_NUMERIC mnLNID;
	 MATH_NUMERIC mnAN8;
	 MATH_NUMERIC mnSHAN;
	 char         szDCT[3];
	 char         szOCTO[3];
	 char         szKCOO[6];
	 MATH_NUMERIC mnUKID;
	 MATH_NUMERIC mnN001;
	 MATH_NUMERIC mnLnid;
	 char         szLitm[25];
	 char         szBSER[3];
	 char         szTaxTo[21];
	 char         szTaxFrom[21];
	 char         szMCU[13];  
     MATH_NUMERIC mnDOCO;
	 char         szDCTO[3];
	 char         szPID[11];
	 char         szUSER[11];
	 JDEDATE      jdUPMJ;
	 MATH_NUMERIC mnTDAY;
	 char         szJOBN[11];
	 MATH_NUMERIC mnLoteControle;
	 char         cSeparadorDecimal;
/* INICIO - JULIO DONATI - 29/07/2013 */
/*Código Comentado*/
/*	 char         szListaPossibilidadeOC[301];*/
/*Novo Código*/
	 char         szListaPossibilidadeOC[1025];
     char         szFCI_DESF[40];
	 MATH_NUMERIC mnVlrParcFCI_FEC;
	 MATH_NUMERIC mnPerParcFCI_PE01;
	 char         szListaPossibilidadeFCI[1025];
	 char         szListaPossibilidadeVlrFCI[1025];
	 char         szListaPossibilidadePercFCI[1025];
/* FIM    - JULIO DONATI - 29/07/2013 */
	 char         szTipoProduto[16];
	 char         cSeparadorTexto; 
	 char         szStatusInical[4]; 
	 JDEDATE      jdISSU;
	 char         szChaveAcesso[81];
   
  }DSD_Linha,*LPDS_Linha;

  typedef struct tag_DSD_Col_F58RE001{
   char           xmdl01[31];          
   char           xmaa08[9];
   MATH_NUMERIC   xmbdfbep;
   char           xmaa10[11];         
   char           xmdtai[11];          
   char           xmowtp[3];          
   int            xmint01;            
   char           xmcdec;             
   char           xmdsc1[31];          
   char           xmdsc2[31];         
   char           xmev01;    
   MATH_NUMERIC   xmdtas;
  }DSD_Col_F58RE001,*LPDS_Col_F58RE001;
/*****************************************************************************
 * DS Template Type Definitions
 ****************************************************************************/
/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Read File TXT
 *    Template ID:   D58RE001
 *    Generated:     Tue Jan 04 17:52:23 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/
#ifndef DATASTRUCTURE_D58RE001
#define DATASTRUCTURE_D58RE001

typedef struct tagDSD58RE001
{
  char              szPathFile[121];                     
  char              szFileName[301];                     
  char              szLayout[31];                        
  char              cErro;                               
  char              szTipoDoc[3];                        
  char              cSeparadorDecimal;                   
  MATH_NUMERIC      mnLoteControle;                      
  char              szListaPossibilidadeOC[301];         
  char              cSeparadorTexto;                     
  char              szVersaoLayout[11];                  
  char              szStatusInical[4];   
  char              szCodErro[7];
  char              szMsgErro[81]; 
/* INICIO - JULIO DONATI - 30/07/2013 */
  char              szListaPossibOC_GDTA[1025];          
  char              szListaPossibFCI_PRSTR[1025];              
  char              szListaPossibPercFCI_W2CRM[1025];
  char              szListaPossibVlrFCI_Y5AFWF01[1025];
/* FIM    - JULIO DONATI - 30/07/2013 */
} DSD58RE001, *LPDSD58RE001;

#define IDERRszPathFile_1                         1L
#define IDERRszFileName_2                         2L
#define IDERRszLayout_3                           3L
#define IDERRcErro_4                              4L
#define IDERRszTipoDoc_5                          5L
#define IDERRcSeparadorDecimal_6                  6L
#define IDERRmnLoteControle_7                     7L
#define IDERRszListaPossibilidadeOC_8             8L
#define IDERRcSeparadorTexto_9                    9L
#define IDERRszVersaoLayout_10                    10L
#define IDERRszStatusInical_11                    11L
#define IDERRszCodErro_12                         12L
#define IDERRszMsgErro_13                         13L
/* INICIO - JULIO DONATI - 30/07/2013 */
#define IDERRszListaPossibOC_GDTA_14              14L
#define IDERRszListaPossibFCI_PRSTR_15            15L
#define IDERRszListaPossibPercFCI_W2CRM_17        17L
#define IDERRszListaPossibVlrFCI_Y5AFWF01_19      19L
/* FIM    - JULIO DONATI - 30/07/2013 */

#endif


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
JDEBFRTN (ID) JDEBFWINAPI ReadNFETXT                       (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD58RE001 lpDS);


/*****************************************************************************
 * Internal Function Prototypes
 ****************************************************************************/
MATH_NUMERIC IB58RE001_FormataNumero(char decimal,char szValorNF[TAMNUMERICO],char szCampo[10],char cSeparadorDecimal,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,DSD_Erro *dSD_Erro);
JDEDATE IB58RE001_ConverteParaJDE_Date(char szValor[16],char szCampo[10],LPVOID lpVoid, LPBHVRCOM lpBhvrCom,DSD_Erro *dSD_Erro);
char *IB58RE0001RTRIM( char *string, char caracter );
char *IB58RE0001LTRIM( char *string, char caracter );
void IB58RE001_RemoveCaracter(char *StringIn,char *StringOut,char *caracter);
void IB58RE001_RemoveParteDecimal(char *StringIn,char *StringOut,char *caracter);
void IB58RE001_SubStr(char *out,char in[MAXTAMLINHA],int posIni,int qtd);
MATH_NUMERIC IB58RE0001_GetAN8(char szCNPJ[21],HUSER hUser);
void IB58RE0001_InsertF58RE76A(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser);

/* INICIO - JULIO DONATI - 30/09/2013 */
/*Codigo Comentado*/
/*int IB58RE001_TagProdutos(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,F58REU02 *dsInsertF58REU02,int posIniVetor,DSD_Linha dSD_Linha,HUSER  hUser,int posFimVetor,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,DSD_Erro *dSD_Erro  );*/

/*Novo Codigo*/
int IB58RE001_TagProdutos(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,F58REU02 *dsInsertF58REU02,int posIniVetor,DSD_Linha dSD_Linha,HUSER  hUser,int posFimVetor,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,DSD_Erro *dSD_Erro,  char  *lpszFCI_DESF );
/* FIM    - JULIO DONATI - 30/09/2013 */

MATH_NUMERIC IB58RE001_TagLotes(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha dSD_Linha,int pVetorIni,MATH_NUMERIC mnLinha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro);
void IB58RE001_TagInfoAdic(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha *dSD_Linha,int pVetorIni,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro);
void IB5REU0001_DocumentSearch(char *szIn,char token,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER hUser,DSD_Linha *dSD_Linha);
char IB58RE0001_SearhPurchaseOrder(MATH_NUMERIC mnPedido,char *szCNPJ,HUSER hUser,DSD_Linha *dSD_Linha);
int IB58RE0001_SearchStringPositionInString(char *szInputString,char *szStringToFind);
void IB58RE0001_InsertF58RE76B(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser);
void IB58RE0001_InsertF58RE76D(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser);
void IB58RE0001_InsertF58RE76C(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser);
void IB58RE001_TrocaCaracter(char *StringIn,char *StringOut,char *caracter, char *caracterTroca);
int IB58RE001_RemoveCaracterDecimal(char *StringIn,char *StringOut,char *caracter);
void IB58RE001_jdeScrubDDText(char *lpName);
void IB58RE001_RemoveCaracterEspecial(char *pTextPtr,char *out);

void IB58RE0001_DeleteTableCache(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser);
void IB58RE001ParseString(char *out,char *in, MATH_NUMERIC mnSegmento,char szSeparador[2]);
MATH_NUMERIC IB58RE001_TagDuplicata(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha dSD_Linha,int pVetorIni,MATH_NUMERIC mnLinha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro);
void IB58RE0001_InsertF58RE76G(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser);
/* INICIO - JULIO DONATI - 26/07/2013 */
void IB58RE0001_InsertF58RE76H(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser);

/* INICIO - JULIO DONATI - 30/09/2013 */
/*Codigo Comentado*/
/*void IB58RE001_TagItemInfoAdic(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha *dSD_Linha,int pVetorIni,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro);*/
/*Novo Codigo*/
void IB58RE001_TagItemInfoAdic(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha *dSD_Linha,int pVetorIni,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro, char  *lpszFCI_DESF );
/* FIM    - JULIO DONATI - 30/09/2013 */

void IB5REU0001_FCISearch(char *szIn,char token,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER hUser,DSD_Linha *dSD_Linha);
/* FIM    - JULIO DONATI - 26/07/2013 */
#endif    /* __B58RE001_H */

