#include <jde.h>

#define b58re001_c


/*****************************************************************************
 *    Source File:  b58re001
 *
 *    Description:  Read File TXT Source File
 *
 *        History:
 *          Date        Programmer  SAR# - Description
 *          ----------  ----------  -------------------------------------------
 *   Author 04/01/2011  LEONARDO COLLOVINI        Unknown  - Created  
 *          26/01/2013  JULIO DONATI
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

#include <b58re001.h>


/**************************************************************************
 *  Business Function:  ReadNFETXT
 *
 *        Description:  Read NFE TXT
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD58RE001        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI ReadNFETXT (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD58RE001 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/

	int             i                             = 0;
	int             x                             = 0;
	int             iVetor                        = 0;
	int             posVetorProd                  = 0;
	int             posVetorLote                  = 0;
	int             posVetorDuplicata             = 0;
	int             posFimItem                    = 0;

	int             nTamCampoValor                = 0;
    char            szPath[301]                   = {'\0'};
	char            szLinhaTexto[MAXTAMLINHA]     = {'\0'};
	char            szValorNF[MAXTAMLINHA]        = {'\0'};
	char            szValor[MAXTAMLINHA]          = {'\0'};
	char            szTipoLinha[9]                = {'\0'};
	char            szCampoNF[11]                 = {'\0'};
	char            szTipoDadoNF[4]               = {'\0'};
	char            szValorNFAux[16]              = {'\0'};
    char            cErro                         = {'\0'};
	char            szLinhaItem[9]                = {'\0'};
	char            szLinhaLote[9]                = {'\0'};
	char            szLinhaDuplicata[9]           = {'\0'};
	char            szTipoNF[3]                   = {'\0'};  
	char            cFlagGravar                   = ' ';
	char            szSeparador[2]                = {'\0'};
	/* INICIO - JULIO DONATI - 30/09/2013 */
	char            szFCI_DESF[40]                = {'\0'};
	/* FIM    - JULIO DONATI - 30/09/2013 */

    MATH_NUMERIC    mnSegmento                    = { 0 };
	MATH_NUMERIC    mnUm                          = { 0 };
	MATH_NUMERIC    mnValorNF                     = { 0 };
	MATH_NUMERIC    mnN001                        = { 0 };
	MATH_NUMERIC    mnPosicao                     = { 0 };
	MATH_NUMERIC    mnUKID                        = { 0 };
	MATH_NUMERIC    mnOne                         = { 0 };
	MATH_NUMERIC    mnLinhaLote                   = { 0 };
    MATH_NUMERIC    mnLinhaDuplicata              = { 0 };
	MATH_NUMERIC    mnLnid                        = { 0 }; 
	MATH_NUMERIC    mnTamCampoValor               = { 0 };
	MATH_NUMERIC    mnZero                        = { 0 };
	JDEDATE         jdDateNF                      = { 0 };                       
	 

	ID		       idJDBReturn                   = JDEDB_PASSED;
	HREQUEST 	   hRequest	                     = (HREQUEST) NULL;
	ID             idReturnValue                 = ER_SUCCESS;    
	ID			   JDBResult		             = JDEDB_PASSED;
	NID			   szTableF58RE001               = NID_F58RE001;
	NID			   szTableF58REU01               = NID_F58REU01;	
	HUSER          hUser                         = (HUSER)NULL;
	HREQUEST       hRequestF58REU01              = (HREQUEST) NULL;
    SORTSTRUCT	   dsSort[2];	

   /************************************************************************
    * Declare structures
    ************************************************************************/
    F58RE001               dsStrF58RE001         = { 0 };
	F58REU01               dsInsertF58REU01      = { 0 };
	F58REU02               dsStrF58REU02         = { 0 }; 
	HREQUEST               hRequestF58REU02        = (HREQUEST) NULL;
    NID		               szTableF58REU02         = NID_F58REU02;
    SELECTSTRUCT           dsSelectF58RE001[1];
	DSD_DetNF              dsDetNF;
	DSD_CabNF              dsCabNF;
	DSD0000580             dSD0000580;
    DSDX00022A             dSDX00022A;  
    DSD_Linha              dSD_Linha             = { 0 };
    LPDS_Col_F58RE001      dsd_ColF58RE001       = NULL;
	DSD9800100	           dsD9800100;
    DSD58RE09A             dSD58RE09A ;
	DSD_Erro               dSD_Erro              = { 0 } ; 
    /*DSD58RE76A             dSD58RE76A;  */


	
   /************************************************************************
    * Declare pointers
    ************************************************************************/
    FILE            *fpFlat             = (FILE *)NULL;
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
   if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD58RE001)	NULL))
   {
     jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, "4363", (LPVOID) NULL);
     return ER_ERROR;
   }
   idReturnValue = JDB_InitBhvr((void *)lpBhvrCom, &hUser, (char *) NULL, 
                                                     JDEDB_COMMIT_AUTO);
	
	if (idJDBReturn != JDEDB_PASSED)
	{
		return ER_ERROR;;
	}

   /************************************************************************
    * Set pointers
    ************************************************************************/

   /************************************************************************
    * Main Processing
    ************************************************************************/    
	   cErro = '0';   
	   memset(&dSD_Erro, 0, sizeof(dSD_Erro));
       ParseNumericString(&mnZero,"0");
	   ParseNumericString(&mnOne,"1");
	   ParseNumericString(&mnLinhaLote,"0");
	   ParseNumericString(&mnLinhaDuplicata,"0");
       idJDBReturn = JDB_OpenTable(hUser, szTableF58RE001, ID_F58RE001_ID1,
                                (NULL), (ushort) 0, (char *)NULL,&hRequest);

	   if(idJDBReturn != JDEDB_PASSED )
	   {
		   cErro = '1';
           goto EOJReturn;
	   }
          memset((void *)(&dsStrF58REU02), (int)('\0'), sizeof(F58REU02));
		  

          memset(&dsStrF58RE001, 0, sizeof(dsStrF58RE001));
    	  memset(&dsSelectF58RE001, 0, sizeof(dsSelectF58RE001));
   	      i=0;	
   	      jdeNIDcpy(dsSelectF58RE001[i].Item1.szTable, NID_F58RE001);
	      jdeNIDcpy(dsSelectF58RE001[i].Item1.szDict, NID_DL01);
	      dsSelectF58RE001[i].nCmp = JDEDB_CMP_EQ;
	      dsSelectF58RE001[i].nAndOr = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	      dsSelectF58RE001[i].lpValue = (LPVOID) &lpDS->szLayout;
	      dsSelectF58RE001[i].nValues = 1;	    
          i++;
          idJDBReturn = JDB_SetSelection(hRequest, dsSelectF58RE001,(ushort)i, JDEDB_SET_REPLACE);
    
		  i = 0;
	      jdeNIDcpy( dsSort[i].Item.szTable, NID_F58RE001 );
	      jdeNIDcpy( dsSort[i].Item.szDict, NID_AA08 );
	      dsSort[i].Item.idInstance = 0;
	      dsSort[i].nSort = JDEDB_SORT_ASC;
          i++;
		  jdeNIDcpy( dsSort[i].Item.szTable, NID_F58RE001 );
	      jdeNIDcpy( dsSort[i].Item.szDict, NID_BDFBEP );
	      dsSort[i].Item.idInstance = 0;
	      dsSort[i].nSort = JDEDB_SORT_ASC;
          i++;
          idJDBReturn = JDB_SetSequencing( hRequest, dsSort, (ushort)i, JDEDB_SET_REPLACE);
	     
          idJDBReturn = JDB_SelectKeyed(hRequest, (ID)0, (void *)NULL, (short) 0);
	      idJDBReturn = JDB_Fetch(hRequest, (LPVOID)&dsStrF58RE001,(int)NULL);
		 
          
		  dsd_ColF58RE001 =jdeAlloc(COMMON_POOL,500 * sizeof (DSD_Col_F58RE001), MEM_ZEROINIT);
	
         
	    while(idJDBReturn == JDEDB_PASSED )
		{
			  memcpy(&dsd_ColF58RE001[iVetor],  &dsStrF58RE001, sizeof(F58RE001));  
			  iVetor++; 
              idJDBReturn = JDB_Fetch(hRequest, (LPVOID)&dsStrF58RE001,(int)NULL);
		}
	   
       strcpy(szTipoNF,"NS");

       memset((void *)(&dSDX00022A), (int)('\0'), sizeof(DSDX00022A));
	   strcpy(dSDX00022A.szObjectName,"B58RE001");
       idReturnValue = jdeCallObject("GetNextUniqueKeyID", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSDX00022A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
       MathCopy(&mnN001,&dSDX00022A.mnUniqueKeyID);

       memset((void *)(&dsD9800100), (int)('\0'), sizeof(DSD9800100));
	   idReturnValue = jdeCallObject("GetAuditInfo", NULL, lpBhvrCom,lpVoid, (LPVOID)&dsD9800100, (CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
	   strcpy(dSD_Linha.szUSER,dsD9800100.szUserName);
	   strcpy(dSD_Linha.szJOBN,dsD9800100.szWorkstation_UserId);
	   strcpy(dSD_Linha.szPID,"B58RE001");
	   dSD_Linha.cSeparadorDecimal = lpDS->cSeparadorDecimal;
	   dSD_Linha.cSeparadorTexto = lpDS->cSeparadorTexto; 
	   MathCopy(&dSD_Linha.mnTDAY,&dsD9800100.mnTime);
       dSD_Linha.jdUPMJ = dsD9800100.jdDate;
	   MathCopy(&dSD_Linha.mnN001,&mnN001);
       strcpy(dSD_Linha.szStatusInical,lpDS->szStatusInical);
	   MathCopy(&dSD_Linha.mnLoteControle,&lpDS->mnLoteControle);       
	   strcpy(szPath,lpDS->szPathFile);
	   strcpy(dSD_Linha.szDCT,szTipoNF);
	   strcpy(dSD_Linha.szOCTO,lpDS->szTipoDoc);
/* INICIO - JULIO DONATI - 30/07/2013 */
/*Código Comentado*/
	   /*strcpy(dSD_Linha.szListaPossibilidadeOC,lpDS->szListaPossibilidadeOC);*/
/*Novo Código*/
	   strcpy(dSD_Linha.szListaPossibilidadeOC,lpDS->szListaPossibOC_GDTA);
	   strcpy(dSD_Linha.szListaPossibilidadeFCI,lpDS->szListaPossibFCI_PRSTR);
/* FIM    - JULIO DONATI - 30/07/2013 */
   	   strcat(szPath,lpDS->szFileName);  
	   fpFlat = fopen(szPath, "a+");
	   if(fpFlat==NULL)
	   {
		   goto EOJReturn;
	   }

	   ParseNumericString(&mnUm,"1");
	   i = 0;
	   while (!feof(fpFlat) ) 
	   {
		  
          jdeStrcpy(szLinhaTexto ,(const char *) (" "),sizeof(szLinhaTexto)); 
		 
		  /* INICIO - JULIO DONATI - 27/05/2019 */
		  /*Código Comentado*/
          /*fgets(szLinhaTexto, 5001, fpFlat);*/
		  /*Novo Código*/
		  fgets(szLinhaTexto, 9001, fpFlat);
		  /* FIM - JULIO DONATI - 27/05/2019 */

          ParseNumericString(&mnSegmento,"0"); 
		  szSeparador[0] = lpDS->cSeparadorTexto;
         /* memset((void *)(&dSD58RE09A), (int)('\0'), sizeof(DSD58RE09A));
	      strcpy(dSD58RE09A.szStringIN,szLinhaTexto);
          dSD58RE09A.szSeparador[0] = lpDS->cSeparadorTexto;
		  MathCopy(&dSD58RE09A.mnSegmento,&mnSegmento);
          idReturnValue = jdeCallObject("ParseStringWithNullValues58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE09A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
          strcpy(szTipoLinha,dSD58RE09A.szStringOut); 
         */

        
		 IB58RE001ParseString(szTipoLinha, szLinhaTexto,mnSegmento,szSeparador);  

		  x = 0;
		  while(iVetor > x && x>=0)
		  {
              if(jdestrcmp(dsd_ColF58RE001[x].xmaa08,szTipoLinha)==0)
			  {
				  i = x;
				  x = -10;
              }
				  x++;
		  }

          IntToMathNumeric(dsd_ColF58RE001[i].xmint01, &mnPosicao);
           

		  if(jdestrcmp(szTipoLinha,szLinhaItem)==0)
		  {
		      i = posVetorProd;
			  cFlagGravar = '1';            
		  }
		  if(jdestrcmp(dsd_ColF58RE001[i].xmaa08,szTipoLinha)==0 && !jdestrcmp(szTipoLinha,szLinhaItem)==0 && jdestrcmp(dsd_ColF58RE001[i].xmaa10,"C")==0 && !IsStringBlank(szLinhaItem))
		  {       	     
				jdeStrcpy(szLinhaItem ,(const char *) (" "),sizeof(szLinhaItem));
				cFlagGravar = '1';			    
		  }
		  if(cFlagGravar == '1')
		  {
			    			
				MathCopy(&dsStrF58REU02.fdukid,&mnUKID);
			    idJDBReturn = JDB_OpenTable(hUser, szTableF58REU02, ID_F58REU02_NOTAFISCALDET, (NULL), (ushort) 0, (char *)NULL,&hRequestF58REU02);
	            idJDBReturn = JDB_InsertTable (hRequestF58REU02,szTableF58REU02, (ID) 0, &dsStrF58REU02);
				JDB_ClearColBuffer(hRequestF58REU02,(void *)(&dsStrF58REU02)); 
                if(hRequestF58REU02!=NULL)
				{
	              JDB_CloseTable(hRequestF58REU02);
				}
                cFlagGravar = '0'; 
                
		  }

		  if(jdestrcmp(szTipoLinha,szLinhaLote)==0)
		  {
			    i = posVetorLote;
			 
		  }

		  if(jdestrcmp(szTipoLinha,szLinhaDuplicata)==0)
		  {
			    i = posVetorDuplicata;
			 
		  }
		     
		  while(jdestrcmp(dsd_ColF58RE001[i].xmaa08,szTipoLinha)==0)
		  {
          
			if(jdestrcmp(dsd_ColF58RE001[i].xmaa10,"M")==0)
			{
			    IB58RE001_TagInfoAdic(szLinhaTexto,dsd_ColF58RE001, &dSD_Linha, i, lpVoid,  lpBhvrCom,  hUser,&dSD_Erro);
            }
           /*.................Item Inicio...............................*/
           			 
		     if(dsd_ColF58RE001[i].xmev01=='1' && jdestrcmp(dsd_ColF58RE001[i].xmaa10,"D")==0)
			 {
			   strcpy(szLinhaItem,dsd_ColF58RE001[i].xmaa08);
		       posVetorProd = i;
			   MathAdd(&mnLnid,&mnLnid,&mnOne);
			   MathCopy(&dSD_Linha.mnLnid,&mnLnid);
			   MathAdd(&mnUKID,&mnUKID,&mnOne);
			   MathCopy(&dSD_Linha.mnUKID,&mnUKID);
			 }
			 
             if(jdestrcmp(dsd_ColF58RE001[i].xmaa10,"D")==0)
			 {
			    posFimItem = IB58RE001_TagProdutos(szLinhaTexto,dsd_ColF58RE001,&dsStrF58REU02,posVetorProd,dSD_Linha,hUser,iVetor,lpVoid, lpBhvrCom, &dSD_Erro ,  szFCI_DESF  );	  		       
				if(!jdestrcmp(dSD_Erro.szCodErro," ")==0)
                      goto EOJReturn;
				
			 }
           		          
			 /*.........................Item Fim.........................*/

            /* INICIO - JULIO DONATI - 26/07/2013 */
			if(jdestrcmp(dsd_ColF58RE001[i].xmaa10,"I")==0 )
			{
			    IB58RE001_TagItemInfoAdic(szLinhaTexto,dsd_ColF58RE001, &dSD_Linha, i, lpVoid,  lpBhvrCom,  hUser,&dSD_Erro, szFCI_DESF);
            }
            /* FIM    - JULIO DONATI - 26/07/2013 */


			 /*........................Lote Inicio........................*/
				
			 if(dsd_ColF58RE001[i].xmev01=='2' && jdestrcmp(dsd_ColF58RE001[i].xmaa10,"L")==0)
			 {
				 strcpy(szLinhaLote,dsd_ColF58RE001[i].xmaa08);
				 posVetorLote = i;
				 mnLinhaLote = IB58RE001_TagLotes(szLinhaTexto, dsd_ColF58RE001, dSD_Linha, posVetorLote, mnLinhaLote, lpVoid,  lpBhvrCom,  hUser,&dSD_Erro );

			 }
			 /*............................Lote Fim.....................*/


	       /*........................Duplicata Inicio........................*/
				
			 if(dsd_ColF58RE001[i].xmev01=='3' && jdestrcmp(dsd_ColF58RE001[i].xmaa10,"F")==0 && jdestrcmp(dsd_ColF58RE001[i].xmdtai," ")!=0)
			 {
				 strcpy(szLinhaDuplicata,dsd_ColF58RE001[i].xmaa08);
				 posVetorDuplicata = i;
				 mnLinhaDuplicata = IB58RE001_TagDuplicata(szLinhaTexto, dsd_ColF58RE001, dSD_Linha, posVetorDuplicata, mnLinhaDuplicata, lpVoid,  lpBhvrCom,  hUser,&dSD_Erro );

			 }
			 /*....................   Duplicata Fim.........................*/


			 if(jdestrcmp(dsd_ColF58RE001[i].xmaa10,"C")==0 && jdestrcmp(dsd_ColF58RE001[i].xmdtai," ")!=0)
			 {
			       
				   jdeStrcpy(szValorNF ,(const char *) (" "),sizeof(szValorNF));
				   IntToMathNumeric(dsd_ColF58RE001[i].xmint01, &mnPosicao);
				   ParseNumericString(&mnSegmento,"1"); 
				   if(MathCompare(&mnPosicao,&mnSegmento)!=0){
					  
				     MathSubtract(&mnSegmento,&mnPosicao,&mnSegmento);
				   }
				   jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor)); 
				   memset((void *)(&dSD58RE09A), (int)('\0'), sizeof(DSD58RE09A));
	               strcpy(dSD58RE09A.szStringIN,szLinhaTexto);
                   dSD58RE09A.szSeparador[0] = lpDS->cSeparadorTexto;
		           MathCopy(&dSD58RE09A.mnSegmento,&mnSegmento);
                   idReturnValue = jdeCallObject("ParseStringWithNullValues58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE09A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
                   strcpy(szValor,dSD58RE09A.szStringOut);

				   

			   if(!IsStringBlank(szValor))
			   {
                IB58RE001_RemoveCaracterEspecial(szValor,szValorNF);
				strcpy(szCampoNF,"FH");
				strcat(szCampoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[i].xmdtai,' '),' '));	  
				strcpy(szTipoDadoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[i].xmowtp,' '),' '));

                nTamCampoValor = strlen(szValorNF);
				if(jdestrcmp(szTipoDadoNF,"2")==0 && nTamCampoValor>0)
				{ 					                    	 				    
				    IntToMathNumeric(nTamCampoValor, &mnTamCampoValor);
				     if(MathCompare(&mnTamCampoValor,&dsd_ColF58RE001[i].xmdtas)>0)
					 {					   
				        strcpy(dSD_Erro.szCodErro,"300");
                        strcpy(dSD_Erro.szMensagem,szCampoNF);		  
					    
					 }
				}

				if(jdestrcmp(szTipoDadoNF,"9")==0)
				{   
					if(nTamCampoValor>TAMNUMERICO){
					    strcpy(dSD_Erro.szCodErro,"300");
                        strcpy(dSD_Erro.szMensagem,szCampoNF);
					}
					else{
         	          mnValorNF=IB58RE001_FormataNumero( dsd_ColF58RE001[i].xmcdec, szValorNF,szCampoNF,lpDS->cSeparadorDecimal, lpVoid,lpBhvrCom,&dSD_Erro);			      
					}       
				}
	            if(jdestrcmp(szTipoDadoNF,"11")==0)
				{
                         jdeStrcpy(szValorNFAux ,(const char *) (" "),sizeof(szValorNFAux));   
				         strcpy(szValorNFAux,szValorNF);
	                     jdDateNF = IB58RE001_ConverteParaJDE_Date( szValorNFAux,szCampoNF, lpVoid,  lpBhvrCom,&dSD_Erro);
				}
				if(!jdestrcmp(dSD_Erro.szCodErro," ")==0){
                      goto EOJReturn;
				}
				
                if(jdestrcmp("FHBSER",szCampoNF)==0) {      
				     strcpy(dsInsertF58REU01.fhbser,szValorNF); 	
					 strcpy(dSD_Linha.szBSER,szValorNF);
				}              
                if(jdestrcmp("FHBNF0",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbnf0,&mnValorNF);   } 
                if(jdestrcmp("FHBSR0",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbsr0,szValorNF);   } 
                if(jdestrcmp("FHBNFS",szCampoNF)==0) {      dsInsertF58REU01.fhbnfs = szValorNF[0];   } 
                if(jdestrcmp("FHOCTO",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhocto,szValorNF);   } 
                if(jdestrcmp("FHMCU",szCampoNF)==0) {       strcpy(dsInsertF58REU01.fhmcu,szValorNF);   } 
                if(jdestrcmp("FHCO",szCampoNF)==0) {        strcpy(dsInsertF58REU01.fhco,szValorNF);   } 
                if(jdestrcmp("FHFCO",szCampoNF)==0) {       strcpy(dsInsertF58REU01.fhfco,szValorNF);   } 
                if(jdestrcmp("FHBCGT",szCampoNF)==0) {      
				    strcpy(dsInsertF58REU01.fhbcgt,szValorNF); 
			        strcpy(dSD_Linha.szTaxTo,szValorNF);
				    dsInsertF58REU01.fhshan = IB58RE0001_GetAN8(szValorNF, hUser);
				    MathCopy(&dSD_Linha.mnSHAN,&dsInsertF58REU01.fhshan);    
				}       
                if(jdestrcmp("FHBCPT",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbcpt , szValorNF);   } 
                if(jdestrcmp("FHSHST",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhshst,szValorNF);   } 
                if(jdestrcmp("FHSHZP",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhshzp,szValorNF);   } 
                if(jdestrcmp("FHVR01",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhvr01,szValorNF);   } 
                if(jdestrcmp("FHBSFH",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbsfh,&mnValorNF);   } 
                if(jdestrcmp("FHAN8V",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhan8v,&mnValorNF);   } 
                if(jdestrcmp("FHBCGF",szCampoNF)==0) {    
				   strcpy(dsInsertF58REU01.fhbcgf,szValorNF);   
			       strcpy(dSD_Linha.szTaxFrom,szValorNF);
				   dsInsertF58REU01.fhan8 = IB58RE0001_GetAN8(szValorNF, hUser);
				   MathCopy(&dSD_Linha.mnAN8,&dsInsertF58REU01.fhan8);
				} 
                if(jdestrcmp("FHBCPF",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbcpf , szValorNF);   } 
                if(jdestrcmp("FHADDS",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhadds,szValorNF);   } 
                if(jdestrcmp("FHBFRT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbfrt,&mnValorNF);   } 
                if(jdestrcmp("FHBSEG",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbseg,&mnValorNF);   } 
                if(jdestrcmp("FHBDFN",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbdfn,&mnValorNF);   } 
                if(jdestrcmp("FHBDES",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbdes,&mnValorNF);   } 
                if(jdestrcmp("FHBVTM",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvtm,&mnValorNF);   } 
                if(jdestrcmp("FHGDVL",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhgdvl,&mnValorNF);   } 
                if(jdestrcmp("FHBVTN",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvtn,&mnValorNF);   } 
                if(jdestrcmp("FHBICM",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbicm,&mnValorNF);   } 
                if(jdestrcmp("FHBBCL",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbbcl,&mnValorNF);   } 
                if(jdestrcmp("FHBVIS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvis,&mnValorNF);   } 
                if(jdestrcmp("FHBBIS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbbis,&mnValorNF);   } 
                if(jdestrcmp("FHBREP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbrep,&mnValorNF);   } 
                if(jdestrcmp("FHBDIZ",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbdiz,&mnValorNF);   } 
                if(jdestrcmp("FHBVII",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvii,&mnValorNF);   } 
                if(jdestrcmp("FHBVOI",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvoi,&mnValorNF);   } 
                if(jdestrcmp("FHBIPI",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbipi,&mnValorNF);   } 
                if(jdestrcmp("FHBBTP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbbtp,&mnValorNF);   } 
                if(jdestrcmp("FHBVOP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvop,&mnValorNF);   } 
                if(jdestrcmp("FHBVIP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvip,&mnValorNF);   } 
                if(jdestrcmp("FHBISS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbiss,&mnValorNF);   } 
                if(jdestrcmp("FHBBCS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbbcs,&mnValorNF);   } 
                if(jdestrcmp("FHBIRT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbirt,&mnValorNF);   
				/* INICIO - JULIO DONATI - 17/05/2018 */
				MathAdd(&dsInsertF58REU01.fhbvis,&dsInsertF58REU01.fhbvis,&dsInsertF58REU01.fhbirt);
				/* FIM    - JULIO DONATI - 17/05/2018 */
				}                 
				if(jdestrcmp("FHBBCR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbbcr,&mnValorNF);   } 
                if(jdestrcmp("FHBIIT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbiit,&mnValorNF);   
				/* INICIO - JULIO DONATI - 17/05/2018 */
				MathAdd(&dsInsertF58REU01.fhbicm,&dsInsertF58REU01.fhbicm,&dsInsertF58REU01.fhbiit);
				/* FIM    - JULIO DONATI - 17/05/2018 */
				} 
                if(jdestrcmp("FHBBCI",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbbci,&mnValorNF);   } 
                if(jdestrcmp("FHIRRI",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhirri,&mnValorNF);   } 
                if(jdestrcmp("FHISSU",szCampoNF)==0) {      
				 	    dsInsertF58REU01.fhissu = jdDateNF;   
				        dSD_Linha.jdISSU = jdDateNF;
				} 
                if(jdestrcmp("FHSHPJ",szCampoNF)==0) {      dsInsertF58REU01.fhshpj = jdDateNF;   } 
                if(jdestrcmp("FHADDJ",szCampoNF)==0) {      dsInsertF58REU01.fhaddj = jdDateNF;   } 
                if(jdestrcmp("FHCNDJ",szCampoNF)==0) {      dsInsertF58REU01.fhcndj = jdDateNF;   } 
                if(jdestrcmp("FHDETM",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhdetm,&mnValorNF);   } 
                if(jdestrcmp("FHBINM",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbinm,szValorNF);   } 
                if(jdestrcmp("FHLNID",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhlnid,&mnValorNF);   } 
                if(jdestrcmp("FHANCR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhancr,&mnValorNF);   } 
                if(jdestrcmp("FHBCGC",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbcgc,szValorNF);   } 
                if(jdestrcmp("FHCRTY",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhcrty,szValorNF);   } 
                if(jdestrcmp("FHFRTH",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhfrth,szValorNF);   } 
                if(jdestrcmp("FHLICP",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhlicp,szValorNF);   } 
                if(jdestrcmp("FHSTCD",szCampoNF)==0) {     
					strcpy(dsInsertF58REU01.fhstcd,szValorNF);
					
					if(strlen(szValorNF)>2){
                       jdeStrcpy(szValorNFAux ,(const char *) (" "),sizeof(szValorNFAux));
					   IB58RE001_SubStr(szValorNFAux,szValorNF,0,2);	
					   strcpy(dsInsertF58REU01.fhbser,szValorNFAux); 	
					   strcpy(dSD_Linha.szBSER,szValorNFAux);
					}else{

					  strcpy(dsInsertF58REU01.fhbser,szValorNF); 	
					  strcpy(dSD_Linha.szBSER,szValorNF);
					}
				} 
                if(jdestrcmp("FHTOQN",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhtoqn,&mnValorNF);   } 
                if(jdestrcmp("FHEQUP",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhequp,szValorNF);   } 
                if(jdestrcmp("FHR2",szCampoNF)==0) {        strcpy(dsInsertF58REU01.fhr2,szValorNF);   } 
                if(jdestrcmp("FHREF",szCampoNF)==0) {       strcpy(dsInsertF58REU01.fhref,szValorNF);   } 
                if(jdestrcmp("FHITWT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhitwt,&mnValorNF);   } 
                if(jdestrcmp("FHTOWT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhtowt,&mnValorNF);   } 
                if(jdestrcmp("FHBD01",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbd01,szValorNF);   } 
                if(jdestrcmp("FHAA1",szCampoNF)==0) {       MathCopy(&dsInsertF58REU01.fhaa1,&mnValorNF);   } 
                if(jdestrcmp("FHBD02",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbd02,szValorNF);   } 
                if(jdestrcmp("FHAA2",szCampoNF)==0) {       MathCopy(&dsInsertF58REU01.fhaa2,&mnValorNF);   } 
                if(jdestrcmp("FHBD03",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbd03,szValorNF);   } 
                if(jdestrcmp("FHBD04",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhbd04,szValorNF);   } 
                if(jdestrcmp("FHBVLF",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbvlf,&mnValorNF);   } 
                if(jdestrcmp("FHUSER",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhuser,szValorNF);   } 
                if(jdestrcmp("FHPID",szCampoNF)==0) {       strcpy(dsInsertF58REU01.fhpid,szValorNF);   } 
                if(jdestrcmp("FHJOBN",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhjobn,szValorNF);   } 
                if(jdestrcmp("FHUPMJ",szCampoNF)==0) {      dsInsertF58REU01.fhupmj = jdDateNF;   } 
                if(jdestrcmp("FHTDAY",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhtday,&mnValorNF);   } 
                if(jdestrcmp("FHBBIR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbbir,&mnValorNF);   } 
                if(jdestrcmp("FHBIPR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhbipr,&mnValorNF);   } 
                if(jdestrcmp("FHCRCD",szCampoNF)==0) {      strcpy(dsInsertF58REU01.fhcrcd,szValorNF);   } 
                if(jdestrcmp("FHFTR",szCampoNF)==0) {       MathCopy(&dsInsertF58REU01.fhftr,&mnValorNF);   } 
                if(jdestrcmp("FHF1T",szCampoNF)==0) {       strcpy(dsInsertF58REU01.fhf1t,szValorNF);   } 
                if(jdestrcmp("FHDS01",szCampoNF)==0) {  
                    nTamCampoValor = strlen(szValorNF);
                    nTamCampoValor = nTamCampoValor - 3;
                    IB58RE001_SubStr(szValorNF,szValorNF,3,nTamCampoValor);
					strcpy(dsInsertF58REU01.fhds01,szValorNF);   
				    strcpy(dSD_Linha.szChaveAcesso,szValorNF);  
				} 
                if(jdestrcmp("FHAN01",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhan01,&mnValorNF);   }
			    if(jdestrcmp("FHAN02",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhan02,&mnValorNF);   }
			    if(jdestrcmp("FHAN03",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhan03,&mnValorNF);   }
			    if(jdestrcmp("FHAN04",szCampoNF)==0) {      MathCopy(&dsInsertF58REU01.fhan04,&mnValorNF);   }
				if(jdestrcmp("FHSLN",szCampoNF)==0) 
				{  
					MathCopy(&dsInsertF58REU01.fhsln,&mnValorNF);
					MathCopy(&dSD_Linha.mnBNNF_LON,&mnValorNF);
			        MathCopy(&dsInsertF58REU01.fhn001,&mnN001);
					MathCopy(&dsInsertF58REU01.fhn002,&lpDS->mnLoteControle);
					strcpy(dsInsertF58REU01.fhdct,dSD_Linha.szDCT);
					strcpy(dsInsertF58REU01.fhocto,dSD_Linha.szOCTO);
                    if(strlen(szValorNF)>6)
					{
					  IB58RE001_SubStr(szValorNF,szValorNF,1,6);
					  ParseNumericString(&mnValorNF,"0");
					  mnValorNF = IB58RE001_FormataNumero( dsd_ColF58RE001[i].xmcdec, szValorNF,szCampoNF,lpDS->cSeparadorDecimal, lpVoid,  lpBhvrCom,&dSD_Erro);
					}
				    MathCopy(&dsInsertF58REU01.fhbnnf,&mnValorNF);   
					MathCopy(&dSD_Linha.mnBNNF,&mnValorNF);	     
				}
			    if(jdestrcmp("FHVERS",szCampoNF)==0) 
				{ 
					if(jdestrcmp(szValorNF,lpDS->szVersaoLayout)!=0)
					{
						strcpy(dSD_Erro.szCodErro,"304");
                        strcpy(dSD_Erro.szMensagem,szValorNF);	
						goto EOJReturn;
					}
					strcpy(dsInsertF58REU01.fhvers,szValorNF); 
				} 
				/*Thales Suzuki - 06-07-2015 - Inicio*/
				if(jdestrcmp("FHPDSC",szCampoNF)==0) {       strcpy(dsInsertF58REU01.fhpdsc,szValorNF);   }
				/*Thales Suzuki - 06-07-2015 - Fim*/
			 } 
			}		         			
			i++;
		 }
	   }
	   fclose(fpFlat);


	  
	 /*  if(jdestrcmp(dSD_Erro.szCodErro," ")==0)
	   {
             memset((void *)(&dSD58RE76A), (int)('\0'), sizeof(DSD58RE76A));
	         MathCopy(&dSD58RE76A.mnNotaFiscalNumber,&dSD_Linha.mnBNNF);
			 MathCopy(&dSD58RE76A.mnAddressNumber,&dSD_Linha.mnAN8);
			 strcpy(dSD58RE76A.szDocumentType,dSD_Linha.szDCT);
			 strcpy(dSD58RE76A.szNotaFiscalSeries,dSD_Linha.szBSER);
             dSD58RE76A.jdIssueDate = dSD_Linha.jdISSU; 
             idReturnValue = jdeCallObject("ConsultaNF58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE76A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
             if(!IsStringBlank(dSD58RE76A.szCodErro)){
			   strcpy(dSD_Erro.szCodErro,dSD58RE76A.szCodErro);
			 }
	   }*/
	   if(cErro =='0' && jdestrcmp(dSD_Erro.szCodErro," ")==0 )
	   {

	     JDBResult = JDB_OpenTable(hUser, szTableF58REU01, ID_F58REU01_NOTAFISCAL,
                                (NULL), (ushort) 0, (char *)NULL,&hRequestF58REU01);
	     strcpy(dsInsertF58REU01.fhuser,dSD_Linha.szUSER);
	     strcpy(dsInsertF58REU01.fhjobn,dSD_Linha.szJOBN);
	     MathCopy(&dsInsertF58REU01.fhtday,&dSD_Linha.mnTDAY);
	     strcpy(dsInsertF58REU01.fhpid,dSD_Linha.szPID);
         dsInsertF58REU01.fhupmj = dSD_Linha.jdUPMJ;
		 dsInsertF58REU01.fhusb1 = dSD_Linha.jdUPMJ;
         dsInsertF58REU01.fhflag = 'W';
		 strcpy(dsInsertF58REU01.fhaa03,dSD_Linha.szStatusInical);

	     idReturnValue = JDB_InsertTable (hRequestF58REU01,szTableF58REU01, (ID) 0, &dsInsertF58REU01);
		 if(hRequestF58REU01!=NULL)
		 {
	         JDB_CloseTable(hRequestF58REU01);
		 }
         if(idReturnValue == JDEDB_PASSED)
		 {
	      
		  IB58RE0001_InsertF58RE76B( dSD_Linha, lpVoid,  lpBhvrCom,  hUser);
		  IB58RE0001_InsertF58RE76C( dSD_Linha, lpVoid,  lpBhvrCom,  hUser);
		  IB58RE0001_InsertF58RE76D( dSD_Linha, lpVoid,  lpBhvrCom,  hUser);
		  IB58RE0001_InsertF58RE76A( dSD_Linha, lpVoid,  lpBhvrCom,  hUser);
		  IB58RE0001_InsertF58RE76G( dSD_Linha, lpVoid,  lpBhvrCom,  hUser);
		  /* INICIO - JULIO DONATI - 26/07/2013 */
		  IB58RE0001_InsertF58RE76H( dSD_Linha, lpVoid,  lpBhvrCom,  hUser);
		  /* FIM    - JULIO DONATI - 26/07/2013 */
		  cErro='N';
		 }
	   }
   
    EOJReturn:
    
	IB58RE0001_DeleteTableCache( dSD_Linha, lpVoid,  lpBhvrCom,  hUser);
    strcpy(lpDS->szCodErro,dSD_Erro.szCodErro);
	strcpy(lpDS->szMsgErro,dSD_Erro.szMensagem);
	lpDS->cErro = cErro;

	jdefree((void*)dsd_ColF58RE001); 
    JDB_FreeBhvr ( hUser );
    if(hRequest!=NULL)
    {
	 JDB_CloseTable(hRequest);
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
 *   Function:  IB58RE001_FormataNumero
 *      Notes:  FORMATA O NUMERO CONFORME PARAMETRIZADO 
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

 MATH_NUMERIC IB58RE001_FormataNumero(char decimal,char szValorNF[TAMNUMERICO],char szCampo[10],char cSeparadorDecimal,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,DSD_Erro *dSD_Erro )
 {
	 int            nRoundFlag   	     = 1;
	 int            nDecimals            = 0;
	 int            nTamParteDec         = 0;
	 int            x                    = 0;
	 MATH_NUMERIC   mnRemainder          = {0};
	 MATH_NUMERIC   mnHundred            = {0};
	 MATH_NUMERIC   mnValorNF            = {0};
	 char           cCaracter[2]         = {'\0'};
	 char           szValorFormatado[16] = {'\0'};
	 char           cCaracterTroca[2]    = {'\0'};
	 char           szDivisor[15]        = {'\0'};  
	 char           cErro     ;
	 char           szMensagem[81]       = {'\0'}; 
	 ID             idReturnValue        = ER_SUCCESS;  
     DSD0000580     dSD0000580;

	 cCaracter[0] = cSeparadorDecimal;
     cCaracterTroca[0] = ',';
     if(!IsStringBlank(szValorNF))
	 {
      jdeStripTrailingBlanks(szValorNF);
	 
	  if(decimal=='0')
	  {
			IB58RE001_RemoveParteDecimal(szValorNF, szValorFormatado, cCaracter);					  
	  }
	  else
	  {
		  
           nTamParteDec = IB58RE001_RemoveCaracterDecimal( szValorNF, szValorFormatado, cCaracter);	
		   szDivisor[x] = '1';
		   x++;
		   while(nTamParteDec > x){
		       szDivisor[x] = '0';
			   x++;
		   }
		
	  }
	  if(jdestrcmp(szValorNF,szValorFormatado)!='0')
		      strcpy(szValorNF,szValorFormatado); 

      memset((void *)(&dSD0000580), (int)('\0'), sizeof(DSD0000580));
	  strcpy(dSD0000580.szInputString,szValorNF);
      idReturnValue = jdeCallObject("ConvertStringToMathNumeric", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD0000580,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
	  if(dSD0000580.cReturnedErrorCode=='1')
	  {
	    MathCopy(&mnValorNF,&dSD0000580.mnOutputMathNumeric);
  	    if(nTamParteDec>0){
	       nDecimals = nTamParteDec;
           memset((void *)(&mnRemainder), (int)('\0'), sizeof(mnRemainder));
	       ParseNumericString(&mnHundred,szDivisor);
	       MathDivide(&mnValorNF,&mnRemainder, &mnValorNF, &mnHundred, &nDecimals, &nRoundFlag);
		}
	  }
	  else{ 
	     strcpy(dSD_Erro->szCodErro,"303");
		 strcpy(szMensagem," campo numerico: ");
		 strcat(szMensagem,szCampo);
		 strcat(szMensagem," - Revisar Layout");
		 strcpy(dSD_Erro->szMensagem,szMensagem);
	  }
	 }

	return(mnValorNF);
 
 }
 /**************************************************************************
 *   Function: IB58RE001_RemoveCaracterDecimal
 *      Notes: REMOVE UM CARACTER 
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 int IB58RE001_RemoveCaracterDecimal(char *StringIn,char *StringOut,char *caracter)
 {
	int nTamParteDec = 0;
	
	while(*StringIn )
	{
		if(*StringIn!=*caracter)
		{
		
		    	*StringOut++ = *StringIn++;
                if(nTamParteDec>0 && *StringIn!=' ' )
                  nTamParteDec++;
		} 
		else
		{
		    nTamParteDec++;
			*StringIn++;
		}
	}
   *StringOut = *StringIn;
   return nTamParteDec;
 }
 
/**************************************************************************
 *   Function: IB58RE001_TrocaCaracter
 *      Notes: Troca UM CARACTER ESPECIFICO
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB58RE001_TrocaCaracter(char *StringIn,char *StringOut,char *caracter, char *caracterTroca)
 {
	
	while(*StringIn )
	{
		if(*StringIn!=*caracter)
		{
		
		    	*StringOut++ = *StringIn++;		
		}
		else
		{
		
			*StringIn++ = *caracterTroca;
		}
	}
   *StringOut = *StringIn;	
 }

 /**************************************************************************
 *   Function:  IB58RE001_ConverteParaJDE_Date
 *      Notes:  CONVERTE UMA DATA GREGORIANA EM JULIANA
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

 JDEDATE IB58RE001_ConverteParaJDE_Date(char szValor[16],char szCampo[10],LPVOID lpVoid, LPBHVRCOM lpBhvrCom,DSD_Erro *dSD_Erro)
 {
      
	 DSD3401570 dSD3401570;
     DSD0800013 d0800013;  
     JDEDATE jdDate                   = { 0 };
     char           szSubStr[10]      = {'\0'};
     char           szSubAux[10]      = {'\0'};
	 char           szMensagem[81]    = {'\0'};
	 ID             idReturnValue     = ER_SUCCESS;  
	 

     if(!IsStringBlank(szValor))
	 {  
       
       IB58RE001_SubStr(szSubStr,szValor,5,2);  
	   strcpy(szSubAux,szSubStr);
	   strcat(szSubAux,"/");
	   IB58RE001_SubStr(szSubStr,szValor,8,2);
	   strcat(szSubAux,szSubStr);
	   strcat(szSubAux,"/");
	   IB58RE001_SubStr(szSubStr,szValor,0,4);
       strcat(szSubAux,szSubStr);
	   strcpy(szValor,szSubAux);
       memset((void *)(&d0800013), (int)('\0'), sizeof(DSD0800013));
	   strcpy(d0800013.szStringToConvert,szValor);
       idReturnValue = jdeCallObject("ConvertStringToDate", NULL, lpBhvrCom,lpVoid, (LPVOID)&d0800013,
				       (CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
	   jdDate = d0800013.jdDateConvertedDate;
	   if(jdDate.nMonth==0)
	   {
		 strcpy(dSD_Erro->szCodErro,"303");
		 strcpy(szMensagem," campo data: ");
		 strcat(szMensagem,szCampo);
		 strcat(szMensagem," - Revisar Layout");
		 strcpy(dSD_Erro->szMensagem,szMensagem);

	   }
	 }
      
   return(jdDate); 

 }

 /**************************************************************************
 *   Function:  IB58RE001LTRIM
 *      Notes:  REMOVE ESPACO A ESQUERDA
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

char *IB58RE0001LTRIM( char *string, char caracter )
{
    int i;
 
	for( i=0; (string[i] == caracter) && (i < (int)strlen(string)); i++ );

    return( &string[i] );
}

/**************************************************************************
 *   Function:  IB58RE001RTRIM
 *      Notes:  REMOVE ESPACO A DIREITA
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

char *IB58RE0001RTRIM( char *string, char caracter )
{
    int i;

    for( i=(strlen(string)-1); (string[i] == caracter) && (i>=0); i-- );
    string[++i] = '\0';

    return( string );
}

/**************************************************************************
 *   Function: IB58RE001_SubStr
 *      Notes:
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

void IB58RE001_SubStr(char *out,char in[MAXTAMLINHA],int posIni,int qtd)
{
  
	char letra[2]  = {'\0'};
	int cont       = 0;
	
	while (qtd>cont)
	{
     letra[0] = in[posIni]; 
     strcpy(out++,letra);
     cont++;
	 posIni++;
	}

}

/**************************************************************************
 *   Function: IB58RE001_RemoveCaracter
 *      Notes: REMOVE UM CARACTER ESPECIFICO
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB58RE001_RemoveCaracter(char *StringIn,char *StringOut,char *caracter)
 {
	
	while(*StringIn )
	{
		if(*StringIn!=*caracter)
		{
		
		    	*StringOut++ = *StringIn++;		
		}
		else
		{
		
			*StringIn++;
		}
	}
   *StringOut = *StringIn;	
 }

 /**************************************************************************
 *   Function:  IB58RE001_RemoveParteDecimal
 *      Notes:  
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

 void IB58RE001_RemoveParteDecimal(char *StringIn,char *StringOut,char *caracter)
 {
	int flag = 0; 
	
	while(*StringIn && flag==0 )
	{
		if(*StringIn!=*caracter)
		{
		  	*StringOut++ = *StringIn++;
		}
		else
		{
			flag = 1;
	
		}
	}

 }

 /**************************************************************************
 *   Function:  IB58RE0001_GetAN8
 *      Notes:  OBTEM AN8 PELO CNPJ
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

 MATH_NUMERIC IB58RE0001_GetAN8(char szCNPJ[21],HUSER hUser)
 {
   MATH_NUMERIC           mnAN8           = {0};
   BOOL                   bKeyExist;
   HREQUEST               hRequestF0101   = (HREQUEST)NULL;
   ID                     idJDBReturn;
   ID                     idIndex         = ID_F0101_TAX_ID;
   KEY4_F0101             dsKeySentStruct = {0};
   short                  nNumKeys        = 1;
   int                    nNotUsed = 0;
   F0101 dsKeyStructAll = {0};

     	  
	  memset ((void *)(&dsKeySentStruct), (int)('\0'), sizeof(KEY4_F0101));
      strcpy(dsKeySentStruct.abtax, szCNPJ);
	  idJDBReturn = JDB_OpenTable(hUser,NID_F0101,ID_F0101_TAX_ID,(NULL),(ushort) 0,(char *) NULL,&hRequestF0101); 
      bKeyExist = JDB_FetchKeyed(hRequestF0101, idIndex, & dsKeySentStruct, nNumKeys, & dsKeyStructAll, nNotUsed);

	   if (bKeyExist == JDEDB_PASSED)
      {
	     MathCopy(&mnAN8,&dsKeyStructAll.aban8);
	  }
	   if(hRequestF0101!=NULL){
	     JDB_CloseTable(hRequestF0101);
	   }
    return mnAN8;
}




 /* Internal function comment block */
/**************************************************************************
 *   Function:  IB58RE002_TagProdutos   
 *                          
 *    Notes:
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 int IB58RE001_TagProdutos(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,F58REU02 *dsInsertF58REU02,int posIniVetor,DSD_Linha dSD_Linha,HUSER  hUser,int posFimVetor,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,DSD_Erro *dSD_Erro , char  *lpszFCI_DESF ){
       
	   int          tamLinha                = 0; 
	   int          x                       = 0;	
	   int          nTamCampoValor          = 0;
	   char         szValor[TAMVALOR]       = {'\0'};
	   char         szValorNF[TAMVALOR]     = {'\0'};
	   char         szCampoNF[11]           = {'\0'};
	   char         szTipoDadoNF[3]         = {'\0'};
	   char         szTipoLinha[9]          = {'\0'};
	   int          posFimItem              = 0;
	   MATH_NUMERIC mnValorNF               = { 0 };
	   MATH_NUMERIC mnPosicao               = { 0 };
	   MATH_NUMERIC mnOne                   = { 0 };
	   MATH_NUMERIC mnTamCampoValor         = { 0 };
	   JDEDATE      jdDateNF                = { 0 };
	
	   DSD0000580   dSD0000580;
	   DSD58RE09A   dSD58RE09A ;
	   ID           idReturnValue;

	   x = posIniVetor; 
	   MathCopy(&dsInsertF58REU02->fdbnnf,&dSD_Linha.mnBNNF); 
	   MathCopy(&dsInsertF58REU02->fdsln,&dSD_Linha.mnBNNF_LON); 
	   MathCopy(&dsInsertF58REU02->fdn001,&dSD_Linha.mnN001);
	   MathCopy(&dsInsertF58REU02->fdukid,&dSD_Linha.mnUKID);
	   strcpy(dsInsertF58REU02->fdbser,dSD_Linha.szBSER);
	   strcpy(dsInsertF58REU02->fddct,dSD_Linha.szDCT);
       strcpy(dsInsertF58REU02->fdocto,dSD_Linha.szOCTO);
	   strcpy(dsInsertF58REU02->fduser,dSD_Linha.szUSER);
	   strcpy(dsInsertF58REU02->fdjobn,dSD_Linha.szJOBN);
	   MathCopy(&dsInsertF58REU02->fdtday,&dSD_Linha.mnTDAY);
	   strcpy(dsInsertF58REU02->fdpid,dSD_Linha.szPID);
       dsInsertF58REU02->fdupmj = dSD_Linha.jdUPMJ;
	   MathCopy(&dsInsertF58REU02->fdn002,&dSD_Linha.mnLoteControle);
	   MathCopy(&dsInsertF58REU02->fdan8,&dSD_Linha.mnAN8);
	   MathCopy(&dsInsertF58REU02->fdshan,&dSD_Linha.mnSHAN);
	   strcpy(dsInsertF58REU02->fdbcgt,dSD_Linha.szTaxTo);
	   strcpy(dsInsertF58REU02->fdbcgf,dSD_Linha.szTaxFrom);
        
	   ParseNumericString(&mnOne,"1");
       tamLinha = strlen(szLinha); 
       ParseNumericString(&mnPosicao,"0");
	   memset((void *)(&dSD58RE09A), (int)('\0'), sizeof(DSD58RE09A));
	   strcpy(dSD58RE09A.szStringIN,szLinha);
       dSD58RE09A.szSeparador[0] = dSD_Linha.cSeparadorTexto;
	   MathCopy(&dSD58RE09A.mnSegmento,&mnPosicao);
	   idReturnValue = jdeCallObject("ParseStringWithNullValues58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE09A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
       strcpy(szTipoLinha,dSD58RE09A.szStringOut); 		   
	   while(tamLinha>0 && x<posFimVetor)
	   {
          IntToMathNumeric(dsd_ColF58RE001[x].xmint01, &mnPosicao);
		  MathSubtract(&mnPosicao,&mnPosicao,&mnOne);    
		  strcpy( dsd_ColF58RE001[x].xmaa10,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmaa10,' '),' '));	
		  if(jdestrcmp(dsd_ColF58RE001[x].xmaa10,"C")==0 && posFimItem==0)
		  {
		       posFimItem = x-1;
		  }		
          jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor));  
          memset((void *)(&dSD58RE09A), (int)('\0'), sizeof(DSD58RE09A));
	      strcpy(dSD58RE09A.szStringIN,szLinha);
          dSD58RE09A.szSeparador[0] = dSD_Linha.cSeparadorTexto;
		  MathCopy(&dSD58RE09A.mnSegmento,&mnPosicao);
		  idReturnValue = jdeCallObject("ParseStringWithNullValues58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE09A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
          strcpy(szValor,dSD58RE09A.szStringOut); 
		  
         
		  if(jdestrcmp(dsd_ColF58RE001[x].xmaa10,"D")==0 && jdestrcmp(dsd_ColF58RE001[x].xmdtai," ")!=0 && jdestrcmp(dsd_ColF58RE001[x].xmaa08,szTipoLinha)==0 )
		  {   
			   strcpy(szTipoDadoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmowtp,' '),' '));
			   jdeStrcpy(szValorNF ,(const char *) (" "),sizeof(szValorNF));                                
			   IB58RE001_RemoveCaracterEspecial(szValor,szValorNF);
			   strcpy(szCampoNF,"FD");
			   strcat(szCampoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmdtai,' '),' '));
			   nTamCampoValor = strlen(szValorNF);
			   if(jdestrcmp(szTipoDadoNF,"2")==0 && nTamCampoValor>0)
			   { 					                    	 				    
			        IntToMathNumeric(nTamCampoValor, &mnTamCampoValor);
		 	        if(MathCompare(&mnTamCampoValor,&dsd_ColF58RE001[x].xmdtas)>0)
					{					   
			           strcpy(dSD_Erro->szCodErro,"300");
                       strcpy(dSD_Erro->szMensagem,szCampoNF);		  
			           goto EOJReturn;
					}
			   }
			   	  			    
				
               	if(jdestrcmp(szTipoDadoNF,"9")==0)
				{			                                         
  			       mnValorNF = IB58RE001_FormataNumero( dsd_ColF58RE001[x].xmcdec, szValorNF,szCampoNF,dSD_Linha.cSeparadorDecimal, lpVoid,  lpBhvrCom,dSD_Erro);
				}
				if(jdestrcmp(szTipoDadoNF,"11")==0)
				{
				   jdDateNF = IB58RE001_ConverteParaJDE_Date( szValorNF,szCampoNF, lpVoid,  lpBhvrCom,dSD_Erro);
				}	
	           	
				if(!jdestrcmp(dSD_Erro->szCodErro," ")==0){
                      goto EOJReturn;
				}

               if(jdestrcmp("FDSOS1",szCampoNF)==0) {      strcpy((char *)dsInsertF58REU02->fdsos1, (const char *) szValorNF);   } 
               if(jdestrcmp("FDBNF0",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbnf0,&mnValorNF);   } 
               if(jdestrcmp("FDBSR0",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbsr0,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBNFS",szCampoNF)==0) {      dsInsertF58REU02->fdbnfs = szValorNF[0];   } 
               if(jdestrcmp("FDOCTO",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdocto,(const char *) szValorNF);   } 
               if(jdestrcmp("FDMATC",szCampoNF)==0) {      dsInsertF58REU02->fdmatc = szValorNF[0];   } 
               if(jdestrcmp("FDDOCO",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fddoco,&mnValorNF);   } 
               if(jdestrcmp("FDPDCT",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdpdct,(const char *) szValorNF);   } 
               if(jdestrcmp("FDKCOO",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdkcoo,(const char *) szValorNF);   } 
               if(jdestrcmp("FDSFXO",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdsfxo,(const char *) szValorNF);   } 
               if(jdestrcmp("FDLNID",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdlnid,&mnValorNF);   } 
               if(jdestrcmp("FDNLIN",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdnlin,&mnValorNF);   } 
               if(jdestrcmp("FDDOC",szCampoNF)==0) {       MathCopy(&dsInsertF58REU02->fddoc,&mnValorNF);   } 
               if(jdestrcmp("FDDCTO",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fddcto,(const char *) szValorNF);   } 
               if(jdestrcmp("FDCO",szCampoNF)==0) {        strcpy((char *) dsInsertF58REU02->fdco,(const char *) szValorNF);   } 
               if(jdestrcmp("FDFCO",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdfco,(const char *) szValorNF);   } 
               if(jdestrcmp("FDITM",szCampoNF)==0) {       MathCopy(&dsInsertF58REU02->fditm,&mnValorNF);    } 
               if(jdestrcmp("FDLITM",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdlitm,(const char *) szValorNF);  strcpy(dSD_Linha.szLitm,szValorNF);  } 
               if(jdestrcmp("FDAITM",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdaitm,(const char *) szValorNF);   } 
               if(jdestrcmp("FDCITM",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdcitm,(const char *) szValorNF);   } 
               if(jdestrcmp("FDMCU",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdmcu,(const char *) szValorNF);   } 
               if(jdestrcmp("FDLOCN",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdlocn,(const char *) szValorNF);   } 
               if(jdestrcmp("FDLOTN",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdlotn,(const char *) szValorNF);   } 
               if(jdestrcmp("FDDSC1",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fddsc1,(const char *) szValorNF);   } 
               if(jdestrcmp("FDDSC2",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fddsc2,(const char *) szValorNF);   } 
               if(jdestrcmp("FDLNTY",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdlnty,(const char *) szValorNF);   } 
               if(jdestrcmp("FDUOM",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fduom,(const char *) szValorNF);   } 
               if(jdestrcmp("FDUORG",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fduorg,&mnValorNF);   } 
               if(jdestrcmp("FDUOM3",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fduom3,(const char *) szValorNF);   } 
               if(jdestrcmp("FDUOM4",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fduom4,(const char *) szValorNF);   } 
               if(jdestrcmp("FDUPRC",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fduprc,&mnValorNF);   } 
               if(jdestrcmp("FDAEXP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdaexp,&mnValorNF);   } 
               if(jdestrcmp("FDBDES",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbdes,&mnValorNF);   } 
               if(jdestrcmp("FDGLC",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdglc,(const char *) szValorNF);   } 
               if(jdestrcmp("FDSHAN",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdshan,&mnValorNF);   } 
               if(jdestrcmp("FDAN8",szCampoNF)==0) {       MathCopy(&dsInsertF58REU02->fdan8,&mnValorNF);   } 
               if(jdestrcmp("FDBCGT",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbcgt,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBCPT",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbcpt,(const char *)  szValorNF);   } 
               if(jdestrcmp("FDSHST",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdshst,(const char *) szValorNF);   } 
               if(jdestrcmp("FDSHZP",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdshzp,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBSFH",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbsfh,&mnValorNF);   } 
               if(jdestrcmp("FDAN8V",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan8v,&mnValorNF);   } 
               if(jdestrcmp("FDBCGF",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbcgf,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBCPF",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbcpf,(const char *) szValorNF);   } 
               if(jdestrcmp("FDADDS",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdadds,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBNOP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbnop,&mnValorNF);   } 
               if(jdestrcmp("FDBSOP",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbsop,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBDIP",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbdip,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBCLF",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbclf,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBCTF",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbctf,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBIST",szCampoNF)==0) {      dsInsertF58REU02->fdbist = szValorNF[0];   } 
               if(jdestrcmp("FDBORI",szCampoNF)==0) {      dsInsertF58REU02->fdbori = szValorNF[0];   } 
               if(jdestrcmp("FDBCFC",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbcfc,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBSTT",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbstt,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBFRT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbfrt,&mnValorNF);   } 
               if(jdestrcmp("FDBSEG",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbseg,&mnValorNF);   } 
               if(jdestrcmp("FDBDFN",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbdfn,&mnValorNF);   } 
               if(jdestrcmp("FDBICM",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbicm,&mnValorNF);   } 
               if(jdestrcmp("FDTXR1",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdtxr1,&mnValorNF);   } 
               if(jdestrcmp("FDBBCL",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbbcl,&mnValorNF);   } 
               if(jdestrcmp("FDBBIR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbbir,&mnValorNF);   } 
               if(jdestrcmp("FDBVIS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbvis,&mnValorNF);   } 
               if(jdestrcmp("FDBBIS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbbis,&mnValorNF);   } 
               if(jdestrcmp("FDBREP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbrep,&mnValorNF);   } 
               if(jdestrcmp("FDBDIZ",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbdiz,&mnValorNF);   } 
               if(jdestrcmp("FDBICP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbicp,&mnValorNF);   } 
               if(jdestrcmp("FDBFII",szCampoNF)==0) {      dsInsertF58REU02->fdbfii = szValorNF[0];   } 
               if(jdestrcmp("FDBIPR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbipr,&mnValorNF);   } 
               if(jdestrcmp("FDBIPI",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbipi,&mnValorNF);   } 
               if(jdestrcmp("FDTXR2",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdtxr2,&mnValorNF);   } 
               if(jdestrcmp("FDBBTP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbbtp,&mnValorNF);   } 
               if(jdestrcmp("FDBFIP",szCampoNF)==0) {      dsInsertF58REU02->fdbfip = szValorNF[0];   } 
               if(jdestrcmp("FDBISS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbiss,&mnValorNF);   } 
               if(jdestrcmp("FDTXR3",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdtxr3,&mnValorNF);   } 
               if(jdestrcmp("FDBBCS",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbbcs,&mnValorNF);   } 
               if(jdestrcmp("FDBIRT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbirt,&mnValorNF);   
			   /* INICIO - JULIO DONATI - 17/05/2018 */
			   MathAdd(&dsInsertF58REU02->fdbvis,&dsInsertF58REU02->fdbvis,&dsInsertF58REU02->fdbirt);
			   /* FIM    - JULIO DONATI - 17/05/2018 */
			   } 
               if(jdestrcmp("FDTXR4",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdtxr4,&mnValorNF);   
			   /* INICIO - JULIO DONATI - 17/05/2018 */
			   MathAdd(&dsInsertF58REU02->fdtxr1,&dsInsertF58REU02->fdtxr1,&dsInsertF58REU02->fdtxr4);
			   /* FIM    - JULIO DONATI - 17/05/2018 */			    
			   } 
               if(jdestrcmp("FDBBCR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbbcr,&mnValorNF);   } 
               if(jdestrcmp("FDBIIT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbiit,&mnValorNF);   
			   /* INICIO - JULIO DONATI - 17/05/2018 */
			   MathAdd(&dsInsertF58REU02->fdbicm,&dsInsertF58REU02->fdbicm,&dsInsertF58REU02->fdbiit);
			   /* FIM    - JULIO DONATI - 17/05/2018 */
			   } 
               if(jdestrcmp("FDTXR5",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdtxr5,&mnValorNF);   } 
               if(jdestrcmp("FDBBCI",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbbci,&mnValorNF);   } 
               if(jdestrcmp("FDANI",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdani,(const char *) szValorNF);   } 
               if(jdestrcmp("FDAID",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdaid,(const char *) szValorNF);   } 
               if(jdestrcmp("FDOMCU",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdomcu,(const char *) szValorNF);   } 
               if(jdestrcmp("FDOBJ",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdobj,(const char *) szValorNF);   } 
               if(jdestrcmp("FDSUB",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdsub,(const char *) szValorNF);   } 
               if(jdestrcmp("FDSBLT",szCampoNF)==0) {      dsInsertF58REU02->fdsblt = szValorNF[0];   } 
               if(jdestrcmp("FDSBL",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdsbl,(const char *) szValorNF);   } 
               if(jdestrcmp("FDBINM",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdbinm,(const char *) szValorNF);   } 
               if(jdestrcmp("FDISSU",szCampoNF)==0) {      dsInsertF58REU02->fdissu = jdDateNF;   } 
               if(jdestrcmp("FDBNIV",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbniv,&mnValorNF);   } 
               if(jdestrcmp("FDPGNO",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdpgno,&mnValorNF);   } 
               if(jdestrcmp("FDNXTR",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdnxtr,(const char *) szValorNF);   } 
               if(jdestrcmp("FDLTTR",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdlttr,(const char *) szValorNF);   } 
               if(jdestrcmp("FDUSER",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fduser,(const char *) szValorNF);   } 
               if(jdestrcmp("FDPID",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdpid,(const char *) szValorNF);   } 
               if(jdestrcmp("FDJOBN",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdjobn,(const char *) szValorNF);   } 
               if(jdestrcmp("FDUPMJ",szCampoNF)==0) {      dsInsertF58REU02->fdupmj = jdDateNF;   } 
               if(jdestrcmp("FDTDAY",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdtday,&mnValorNF);   } 
               if(jdestrcmp("FDCRCD",szCampoNF)==0) {      strcpy(dsInsertF58REU02->fdcrcd,szValorNF);   } 
               if(jdestrcmp("FDFTR",szCampoNF)==0) {       MathCopy(&dsInsertF58REU02->fdftr,&mnValorNF);   } 
               if(jdestrcmp("FDF1T",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdf1t,(const char *) szValorNF);   } 
               if(jdestrcmp("FDF2A",szCampoNF)==0) {       MathCopy(&dsInsertF58REU02->fdf2a,&mnValorNF);   } 
               if(jdestrcmp("FDF2T",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdf2t,(const char *) szValorNF);   } 
               if(jdestrcmp("FDUSB1",szCampoNF)==0) {      dsInsertF58REU02->fdusb1 = jdDateNF;   } 
               if(jdestrcmp("FDFLD3",szCampoNF)==0) {      dsInsertF58REU02->fdfld3 = szValorNF[0];   } 
               if(jdestrcmp("FDFLD4",szCampoNF)==0) {      dsInsertF58REU02->fdfld4 = szValorNF[0];   } 
			   if(jdestrcmp("FDAN01",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan01,&mnValorNF);   }
			   if(jdestrcmp("FDAN02",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan02,&mnValorNF);   }
			   if(jdestrcmp("FDASNP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdasnp,&mnValorNF);   }
			   if(jdestrcmp("FDAN03",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan03,&mnValorNF);   }
			   if(jdestrcmp("FDAN04",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan04,&mnValorNF);   }
			   if(jdestrcmp("FDRPCT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdrpct,&mnValorNF);   }
               if(jdestrcmp("FDBRNOP",szCampoNF)==0){      strcpy((char *) dsInsertF58REU02->fdbrnop,(const char *) szValorNF);   } 
               if(jdestrcmp("FDEXR1",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdexr1,(const char *) szValorNF);   } 
			   if(jdestrcmp("FDREF",szCampoNF)==0) {       strcpy((char *) dsInsertF58REU02->fdref,(const char *) szValorNF);   } 
               if(jdestrcmp("FDD200",szCampoNF)==0) {      strcpy((char *) dsInsertF58REU02->fdd200,(const char *) szValorNF);   } 
               if(jdestrcmp("FDAN06",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan06,&mnValorNF);   } 
               if(jdestrcmp("FDAN07",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan07,&mnValorNF);   } 
               if(jdestrcmp("FDAN08",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan08,&mnValorNF);   } 
               if(jdestrcmp("FDAN09",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan09,&mnValorNF);   } 
               if(jdestrcmp("FDAN10",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan10,&mnValorNF);   } 
               if(jdestrcmp("FDAN11",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan11,&mnValorNF);   } 
               if(jdestrcmp("FDAN12",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan12,&mnValorNF);   } 
               if(jdestrcmp("FDAN13",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan13,&mnValorNF);   } 
               if(jdestrcmp("FDAN14",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan14,&mnValorNF);   } 
               if(jdestrcmp("FDAN15",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdan15,&mnValorNF);   } 
               if(jdestrcmp("FDAMC3",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdamc3,&mnValorNF);   } 
               if(jdestrcmp("FDUPAX",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdupax,&mnValorNF);   } 
               if(jdestrcmp("FDUD01",szCampoNF)==0) {      strcpy(dsInsertF58REU02->fdud01,szValorNF);   } 
               if(jdestrcmp("FDUD02",szCampoNF)==0) {      strcpy(dsInsertF58REU02->fdud02,szValorNF);   } 
               if(jdestrcmp("FDDL03",szCampoNF)==0) {      strcpy(dsInsertF58REU02->fddl03,szValorNF);   } 
               if(jdestrcmp("FDDL04",szCampoNF)==0) {      strcpy(dsInsertF58REU02->fddl04,szValorNF);   } 
               if(jdestrcmp("FDCA10",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdca10,&mnValorNF);   } 
               if(jdestrcmp("FDCA11",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdca11,&mnValorNF);   } 
			   
			   /* INICIO - JULIO DONATI - 20/08/2019 */
			   if(jdestrcmp("FDBISF",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdbisf,&mnValorNF);   } 
			   if(jdestrcmp("FDAU01",szCampoNF)==0) {      strcpy(dsInsertF58REU02->fdau01,szValorNF);   } 
			   if(jdestrcmp("FDAA02",szCampoNF)==0) {      strcpy(dsInsertF58REU02->fdaa02,szValorNF);   } 
			   if(jdestrcmp("FDCEST",szCampoNF)==0) {      MathCopy(&dsInsertF58REU02->fdcest,&mnValorNF);   } 
			   /* FIM    - JULIO DONATI - 20/08/2019 */

			  
			}			   
		   /* INICIO - JULIO DONATI - 30/09/2013 */
		   else
		   {
			   if(jdestrcmp(dsd_ColF58RE001[x].xmaa10,"I")==0 && jdestrcmp(dsd_ColF58RE001[x].xmdtai," ")!=0 && jdestrcmp(dsd_ColF58RE001[x].xmaa08,szTipoLinha)==0 )
			   {
				   jdeStrcpy(szValorNF ,(const char *) (" "),sizeof(szValorNF));                                
				   IB58RE001_RemoveCaracterEspecial(szValor,szValorNF);
				   strcpy(szCampoNF,"FD");
				   strcat(szCampoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmdtai,' '),' '));
				   nTamCampoValor = strlen(szValorNF);
				
				   if(jdestrcmp("FDAPTA",szCampoNF)==0) {      strcpy((char *) lpszFCI_DESF,(const char *) szValorNF);   }			   			   
               }
		   }
		   /* FIM    - JULIO DONATI - 30/09/2013 */

		
        		
		tamLinha = strlen(szLinha);
	  
			x++; 
	
	   }
       if(jdestrcmp(dsInsertF58REU02->fdcitm," ")!=0)
	   {
         dsInsertF58REU02->fdflag = 'W';
        
         strcpy(dsInsertF58REU02->fdaa03,dSD_Linha.szStatusInical); 
		
	   }
     EOJReturn:

	  return posFimItem;	   	  
   }


   /**************************************************************************
 *   Function: IB58RE002_TagLotes
 *      Notes: GERA INFORMAÇÕES DE LOTES
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

  MATH_NUMERIC IB58RE001_TagLotes(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha dSD_Linha,int pVetorIni,MATH_NUMERIC mnLinha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro){
       
       int           tamLinha                = 0; 
	   int           nTamCampoValor          = 0;
	   int           x                       = 0;
	   char          szValor[TAMVALOR]             = {'\0'};   
	   char          szValorNF[TAMVALOR]           = {'\0'};
	   char          szCampoNF[11]           = {'\0'};
	   char          szTipoDadoNF[3]         = {'\0'};
	   char          cGrava                  = {'\0'};
	   MATH_NUMERIC  mnTamCampoValor         = { 0 };
	   MATH_NUMERIC  mnValorNF               = { 0 };
	   MATH_NUMERIC  mnOne                   = { 0 };
	   MATH_NUMERIC  mnPosicao               = { 0 };
	   JDEDATE       jdDateNF                = {0};
       ID		     idJDBReturn             = JDEDB_PASSED; 
	   ID            idReturnValue;
	   F58REU03      dsInsertF58REU03        = {0}; 
	   HREQUEST      hRequestF58REU03        = (HREQUEST) NULL;
       NID		     szTableF58REU03         = NID_F58REU03;
	   DSD58RE09A    dSD58RE09A ;

       idJDBReturn = JDB_OpenTable(hUser, szTableF58REU03, ID_F58REU03_PK, (NULL), (ushort) 0, (char *)NULL,&hRequestF58REU03);
	   
	   ParseNumericString( &mnOne, "1" );
	   tamLinha = strlen(szLinha);
	   x = pVetorIni;
	   while(jdestrcmp(dsd_ColF58RE001[x].xmaa10,"L")==0){
       
	              strcpy( dsd_ColF58RE001[x].xmaa10,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmaa10,' '),' '));
	              IntToMathNumeric(dsd_ColF58RE001[x].xmint01, &mnPosicao);
		          MathSubtract(&mnPosicao,&mnPosicao,&mnOne);      
			      jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor)); 
				  memset((void *)(&dSD58RE09A), (int)('\0'), sizeof(DSD58RE09A));
	              strcpy(dSD58RE09A.szStringIN,szLinha);
                  dSD58RE09A.szSeparador[0] = dSD_Linha.cSeparadorTexto;
		          MathCopy(&dSD58RE09A.mnSegmento,&mnPosicao);
		          idReturnValue = jdeCallObject("ParseStringWithNullValues58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE09A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
                  strcpy(szValor,dSD58RE09A.szStringOut);  
			      jdeStrcpy(szValorNF ,(const char *) (" "),sizeof(szValorNF));                     
                  
				  IB58RE001_RemoveCaracterEspecial(szValor,szValorNF);
			      strcpy(szCampoNF,"FL");
			      strcat(szCampoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmdtai,' '),' '));	  
			      strcpy(szTipoDadoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmowtp,' '),' '));	
				  
				  nTamCampoValor = strlen(szValorNF);
				  if(jdestrcmp(szTipoDadoNF,"2")==0 && nTamCampoValor>0)
				  { 					                    	 				    
			        IntToMathNumeric(nTamCampoValor, &mnTamCampoValor);
		 	        if(MathCompare(&mnTamCampoValor,&dsd_ColF58RE001[x].xmdtas)>0)
					{					   
			           strcpy(dSD_Erro->szCodErro,"300");
                       strcpy(dSD_Erro->szMensagem,szCampoNF);		  
			          
					}
				  }   
				  if(jdestrcmp(szTipoDadoNF,"9")==0)
				  {					   
  			           mnValorNF=IB58RE001_FormataNumero( dsd_ColF58RE001[x].xmcdec, szValorNF,szCampoNF,dSD_Linha.cSeparadorDecimal, lpVoid,  lpBhvrCom,dSD_Erro);					
				  }
				  if(jdestrcmp(szTipoDadoNF,"11")==0)
				  {
				       jdDateNF = IB58RE001_ConverteParaJDE_Date( szValorNF,szCampoNF, lpVoid,  lpBhvrCom,dSD_Erro);   
				  }
				  if(!jdestrcmp(dSD_Erro->szCodErro," ")==0)
				  {
                      goto EOJReturn;
				  }
				
			      /**************MEDICAMENTOS********************************************************/               
                  if(jdestrcmp("FLLOTN",szCampoNF)==0) {      strcpy(dsInsertF58REU03.fllotn,szValorNF);   } 
                  if(jdestrcmp("FLMMEJ",szCampoNF)==0) {      dsInsertF58REU03.flmmej = jdDateNF;   } 
                  if(jdestrcmp("FLUA01",szCampoNF)==0) {      dsInsertF58REU03.flua01 = jdDateNF;   } 
                  if(jdestrcmp("FLAEXP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flaexp,&mnValorNF);   } 
                  if(jdestrcmp("FLAN01",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flan01,&mnValorNF);   } 
                  if(jdestrcmp("FLUORG",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.fluorg,&mnValorNF);   } 
                  if(jdestrcmp("FLUOM",szCampoNF)==0) {       strcpy(dsInsertF58REU03.fluom,szValorNF);   } 
                 
				  if(jdestrcmp("FLIA01",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flia01,&mnValorNF);   } 
                  if(jdestrcmp("FLDL01",szCampoNF)==0) {      strcpy(dsInsertF58REU03.fldl01,szValorNF);   } 
                  if(jdestrcmp("FLAA05",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flaa05,szValorNF);   } 
                  if(jdestrcmp("FLDS40",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flds40,szValorNF);   } 
                  if(jdestrcmp("FLQ401",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flq401,szValorNF);   } 
                  if(jdestrcmp("FLQ402",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flq402,szValorNF);   } 
                  if(jdestrcmp("FLAA10",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flaa10,szValorNF);   } 
                  if(jdestrcmp("FLQ101",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flq101,szValorNF);   } 
                  if(jdestrcmp("FLQ102",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flq102,szValorNF);   } 
                  if(jdestrcmp("FLQ103",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flq103,szValorNF);   } 
                  if(jdestrcmp("FLFTA1",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flfta1,szValorNF);   } 
                  if(jdestrcmp("FLQ104",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flq104,szValorNF);   } 
                  if(jdestrcmp("FLQ403",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flq403,szValorNF);   } 
                  if(jdestrcmp("FLWG",szCampoNF)==0) {        MathCopy(&dsInsertF58REU03.flwg,&mnValorNF);   } 
                  if(jdestrcmp("FLYRM",szCampoNF)==0) {       MathCopy(&dsInsertF58REU03.flyrm,&mnValorNF);   } 
                  if(jdestrcmp("FLBLYR",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flblyr,&mnValorNF);   }           
				  if(jdestrcmp("FLAA01",szCampoNF)==0) {      dsInsertF58REU03.flaa01 = szValorNF[0];   } 
                  if(jdestrcmp("FLAA02",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flaa02,szValorNF);   } 
                  if(jdestrcmp("FLEF01",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flef01,&mnValorNF);   } 
                  if(jdestrcmp("FLHT01",szCampoNF)==0) {      strcpy(dsInsertF58REU03.flht01,szValorNF);   } 
                  if(jdestrcmp("FLEF10",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flef10,&mnValorNF);   } 
                  if(jdestrcmp("FLIA06",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flia06,&mnValorNF);   } 
                
				  if(jdestrcmp("FLEF11",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flef11,&mnValorNF);   } 
                  if(jdestrcmp("FLIA10",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flia10,&mnValorNF);   } 
                  if(jdestrcmp("FLFUF9",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flfuf9,&mnValorNF);   } 
                  if(jdestrcmp("FLDTA1",szCampoNF)==0) {      strcpy(dsInsertF58REU03.fldta1,szValorNF);   } 
                
				  if(jdestrcmp("FLQFX1",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flqfx1,&mnValorNF);   } 
                  if(jdestrcmp("FLPRL",szCampoNF)==0) {       MathCopy(&dsInsertF58REU03.flprl,&mnValorNF);   } 
                  if(jdestrcmp("FLROQT",szCampoNF)==0) {      MathCopy(&dsInsertF58REU03.flroqt,&mnValorNF);   } 
                    
				  if(!IsStringBlank(szValorNF))
				  {
                       cGrava = 'T';
				  }
		 x++;
	  }
      if(cGrava =='T')
	  {
		   MathAdd(&mnLinha,&mnLinha,&mnOne); 
		   MathCopy(&dsInsertF58REU03.flbnnf,&dSD_Linha.mnBNNF);
		   MathCopy(&dsInsertF58REU03.flsln,&dSD_Linha.mnBNNF_LON);
		   MathCopy(&dsInsertF58REU03.fln001,&dSD_Linha.mnN001);
		   MathCopy(&dsInsertF58REU03.flukid,&dSD_Linha.mnUKID);
		   MathCopy(&dsInsertF58REU03.fllinx,&mnLinha);
           strcpy(dsInsertF58REU03.flbser,dSD_Linha.szBSER);
		   strcpy(dsInsertF58REU03.fluser,dSD_Linha.szUSER);
	       strcpy(dsInsertF58REU03.fljobn,dSD_Linha.szJOBN);
	       MathCopy(&dsInsertF58REU03.flupmt,&dSD_Linha.mnTDAY);
	       strcpy(dsInsertF58REU03.flpid,dSD_Linha.szPID);
           dsInsertF58REU03.flupmj = dSD_Linha.jdUPMJ; 		  
	       idJDBReturn = JDB_InsertTable (hRequestF58REU03,szTableF58REU03, (ID) 0, &dsInsertF58REU03);
		 
	  }
     EOJReturn:

	 if(hRequestF58REU03!=NULL)
	 {
	    JDB_CloseTable(hRequestF58REU03);
	 }	
	 return mnLinha;
   }


   /**************************************************************************
 *   Function: IB58RE002_TagInfoAdic
 *      Notes: LEITURA DA TAG DE INFORMAÇÕES ADICIONAIS
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/ 
  void IB58RE001_TagInfoAdic(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha *dSD_Linha,int pVetorIni,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro)
  {
       int           cont                    = 0;
       int           tamLinha                = 0; 
	   int           x                       = 0;
	   int           pos                     = 0;
	   int           nTamCampoValor          = 0;
	   char          szValor[MAXTAMLINHA]    = {'\0'};
	   char          szValorNF[MAXTAMLINHA]  = {'\0'};
	   char          szCampoNF[11]           = {'\0'};
	   char          szTipoDadoNF[3]         = {'\0'};
	   char          cToken                  = {'\0'};
	   char          szSeparador[2]          = {'\0'};
	   MATH_NUMERIC  mnPosicao               = { 0 };
	   MATH_NUMERIC  mnValorNF               = { 0 };
	   MATH_NUMERIC  mnOne                   = { 0 };
	   MATH_NUMERIC  mnLinha                 = { 0 };
	   MATH_NUMERIC  mnZero                  = { 0 };
	   MATH_NUMERIC  mnTamCampoValor         = { 0 };
	   JDEDATE       jdDateNF                = {0};
       ID		     idJDBReturn             = JDEDB_PASSED; 
	   ID            idReturnValue;
	   F58REU04      dsInsertF58REU04        = {0}; 
	   HREQUEST      hRequestF58REU04        = (HREQUEST) NULL;
       NID		     szTableF58REU04         = NID_F58REU04;
	   DSD58RE09A   dSD58RE09A ;

	   ParseNumericString(&mnZero,"0");
       idJDBReturn = JDB_OpenTable(hUser, szTableF58REU04, ID_F58REU04_PK,
                                (NULL), (ushort) 0, (char *)NULL,&hRequestF58REU04);
	   ParseNumericString( &mnOne, "1" );
        cToken = ';';
	   if(MathCompare(&dSD_Linha->mnDOCO,&mnZero)==0)
	   {
	      IB5REU0001_DocumentSearch(szLinha,cToken,lpVoid, lpBhvrCom,hUser, dSD_Linha);   
	   }
	   x = pVetorIni;
	   while(jdestrcmp(dsd_ColF58RE001[x].xmaa10,"M")==0)
	   {
       		
			        jdeStrcpy(szValorNF ,(const char *) (" "),sizeof(szValorNF));
					IntToMathNumeric(dsd_ColF58RE001[x].xmint01, &mnPosicao);
		            MathSubtract(&mnPosicao,&mnPosicao,&mnOne);      
			        jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor)); 
				    /*
					memset((void *)(&dSD58RE09A), (int)('\0'), sizeof(DSD58RE09A));
	                strcpy(dSD58RE09A.szStringIN,szLinha);
                    dSD58RE09A.szSeparador[0] = dSD_Linha->cSeparadorTexto;
		            MathCopy(&dSD58RE09A.mnSegmento,&mnPosicao);
		            idReturnValue = jdeCallObject("ParseStringWithNullValues58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE09A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
                    strcpy(szValor,dSD58RE09A.szStringOut);  
					*/
					szSeparador[0] = dSD_Linha->cSeparadorTexto;
					IB58RE001ParseString(szValor, szLinha,mnPosicao,szSeparador);
					
			        IB58RE001_RemoveCaracterEspecial(szValor,szValorNF); 
					strcpy(szCampoNF,"FM");
			        strcat(szCampoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmdtai,' '),' '));	  
			        strcpy(szTipoDadoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmowtp,' '),' '));
					nTamCampoValor = strlen(szValorNF);
				    if(jdestrcmp(szTipoDadoNF,"2")==0 && nTamCampoValor>0)
					{ 					                    	 				    
			          IntToMathNumeric(nTamCampoValor, &mnTamCampoValor);
		 	         /* if(MathCompare(&mnTamCampoValor,&dsd_ColF58RE001[x].xmdtas)>0)
					  {					   
			           strcpy(dSD_Erro->szCodErro,"300");
                       strcpy(dSD_Erro->szMensagem,szCampoNF);		  
			          
					  }*/
					}   				    
                    if(jdestrcmp(szTipoDadoNF,"9")==0)			                                         
  			           mnValorNF=IB58RE001_FormataNumero( dsd_ColF58RE001[x].xmcdec, szValorNF,szCampoNF,dSD_Linha->cSeparadorDecimal, lpVoid,  lpBhvrCom,dSD_Erro);					
				    if(jdestrcmp(szTipoDadoNF,"11")==0)
				       jdDateNF = IB58RE001_ConverteParaJDE_Date( szValorNF,szCampoNF, lpVoid,  lpBhvrCom,dSD_Erro);
					
					if(!jdestrcmp(dSD_Erro->szCodErro," ")==0)
					{
                      goto EOJReturn;
					}
                    if(jdestrcmp("FMAPTA",szCampoNF)==0) 
					{      
						tamLinha = strlen(szValorNF);
						pos = 0;			
							while(tamLinha > pos)
							{
							   jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor)); 	 
                               IB58RE001_SubStr(szValor,szValorNF,pos,100);  
                               MathAdd(&mnLinha,&mnLinha,&mnOne); 
							   strcpy(dsInsertF58REU04.fmapta,szValor);
		                       MathCopy(&dsInsertF58REU04.fmbnnf,&dSD_Linha->mnBNNF);
							   MathCopy(&dsInsertF58REU04.fmsln,&dSD_Linha->mnBNNF_LON);
		                       MathCopy(&dsInsertF58REU04.fmlnid,&mnLinha);
							   MathCopy(&dsInsertF58REU04.fmn001,&dSD_Linha->mnN001);
		                       strcpy(dsInsertF58REU04.fmbser,dSD_Linha->szBSER);
		                       strcpy(dsInsertF58REU04.fmdct,dSD_Linha->szDCT);
		                       strcpy(dsInsertF58REU04.fmuser,dSD_Linha->szUSER);
	                           strcpy(dsInsertF58REU04.fmjobn,dSD_Linha->szJOBN);
	                           MathCopy(&dsInsertF58REU04.fmupmt,&dSD_Linha->mnTDAY);
	                           strcpy(dsInsertF58REU04.fmpid,dSD_Linha->szPID);
                               dsInsertF58REU04.fmupmj = dSD_Linha->jdUPMJ; 
	                           idJDBReturn = JDB_InsertTable (hRequestF58REU04,szTableF58REU04, (ID) 0, &dsInsertF58REU04);
							   pos = pos +100;
							}
		   x++;	       
		}      
        cont++;
	  }
 EOJReturn:

	 if(hRequestF58REU04!=NULL)
	 {
	    JDB_CloseTable(hRequestF58REU04);
	 }	
	 
 }



  /**************************************************************************
 *   Function: IB58RE002_TagItemInfoAdic
 *      Notes: LEITURA DA TAG DE INFORMAÇÕES ADICIONAIS
 *  Developer: JULIO DONATI - 26/07/2013
 *    Returns:
 *
 * Parameters:
 **************************************************************************/ 
  void IB58RE001_TagItemInfoAdic(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha *dSD_Linha,int pVetorIni,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro, char  *lpszFCI_DESF )
  {
       int           cont                    = 0;
       int           tamLinha                = 0; 
	   int           x                       = 0;
	   int           pos                     = 0;
	   char          szValor[MAXTAMLINHA]    = {'\0'};
	   char          szValorNF[MAXTAMLINHA]  = {'\0'};
	   char          szCampoNF[11]           = {'\0'};
	   char          szTipoDadoNF[3]         = {'\0'};
	   char          cToken                  = {'\0'};
	   char          szSeparador[2]          = {'\0'};

	   
	   MATH_NUMERIC  mnPosicao               = { 0 };
	   MATH_NUMERIC  mnOne                   = { 0 };
	   MATH_NUMERIC  mnZero                  = { 0 };	   
       ID		     idJDBReturn             = JDEDB_PASSED; 
	   ID            idReturnValue;
	   F58REU06      dsInsertF58REU06        = {0}; 
	   HREQUEST      hRequestF58REU06        = (HREQUEST) NULL;
       NID		     szTableF58REU06         = NID_F58REU06;
	   DSD58RE09A    dSD58RE09A ;

		/* INICIO - JULIO DONATI - 30/09/2013 */
	   KEY1_F58REU06 dsKeyF58RE002			 = {0};
	   ID			 idIndex				 = ID_F58REU06_PK;
	   short         nNumKeys			     = 6;
	   F58REU06      dsFetchF58REU06         = {0}; 
		/* FIM    - JULIO DONATI - 30/09/2013 */





 
	   ParseNumericString(&mnZero,"0");
       idJDBReturn = JDB_OpenTable(hUser, szTableF58REU06, ID_F58REU06_PK,
                                (NULL), (ushort) 0, (char *)NULL,&hRequestF58REU06);
	   ParseNumericString( &mnOne, "1" );
       cToken = ';';
	   IB58RE001_RemoveCaracterEspecial(szLinha,szLinha);
	   
	   /* INICIO - JULIO DONATI - 30/09/2013 */
	   
	   /* Codigo Comentado*/
	   /*IB5REU0001_FCISearch(szLinha,cToken,lpVoid, lpBhvrCom,hUser, dSD_Linha);   */
	   
	   /* Novo Codigo*/
	   if(jdestrcmp(lpszFCI_DESF," ")==0)
	   {
			IB5REU0001_FCISearch(szLinha,cToken,lpVoid, lpBhvrCom,hUser, dSD_Linha);  
	   }
	   /* INICIO - JULIO DONATI - 30/09/2013 */

	   
	   x = pVetorIni;
	   while(jdestrcmp(dsd_ColF58RE001[x].xmaa10,"I")==0)
	   {
       		
			        jdeStrcpy(szValorNF ,(const char *) (" "),sizeof(szValorNF));
					IntToMathNumeric(dsd_ColF58RE001[x].xmint01, &mnPosicao);
		            MathSubtract(&mnPosicao,&mnPosicao,&mnOne);      
			        jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor)); 
					szSeparador[0] = dSD_Linha->cSeparadorTexto;
					IB58RE001ParseString(szValor, szLinha,mnPosicao,szSeparador);
					
			        IB58RE001_RemoveCaracterEspecial(szValor,szValorNF); 
					strcpy(szCampoNF,"FI");
			        strcat(szCampoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmdtai,' '),' '));	  
			        strcpy(szTipoDadoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmowtp,' '),' '));
  				    
					
					if(!jdestrcmp(dSD_Erro->szCodErro," ")==0)
					{
                      goto EOJReturn;
					}
                    if(jdestrcmp("FIAPTA",szCampoNF)==0) 
					{      
						tamLinha = strlen(szValorNF);
						pos = 0;			
							
						jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor));
						strcpy(szValor,szValorNF);
						strcpy(dsInsertF58REU06.fiapta,szValor);
						
						/* INICIO - JULIO DONATI - 30/09/2013 */
						
						/* Codigo Comentado*/						
						/*strcpy(dsInsertF58REU06.fidesf,dSD_Linha->szFCI_DESF);*/
						
						/* Novo Codigo*/
						if(jdestrcmp(lpszFCI_DESF," ")==0)
						{							
							strcpy(dsInsertF58REU06.fidesf,dSD_Linha->szFCI_DESF);
						}
						else
						{						
							strcpy(dsInsertF58REU06.fidesf,lpszFCI_DESF);
						}
						/* FIM  - JULIO DONATI - 30/09/2013 */
						
		                MathCopy(&dsInsertF58REU06.fibnnf,&dSD_Linha->mnBNNF);
						MathCopy(&dsInsertF58REU06.fisln,&dSD_Linha->mnBNNF_LON);		                
						MathCopy(&dsInsertF58REU06.fin001,&dSD_Linha->mnN001);
		                strcpy(dsInsertF58REU06.fibser,dSD_Linha->szBSER);
		                strcpy(dsInsertF58REU06.fidct,dSD_Linha->szDCT);
						MathCopy(&dsInsertF58REU06.fiukid,&dSD_Linha->mnUKID);
		                strcpy(dsInsertF58REU06.fiuser,dSD_Linha->szUSER);
	                    strcpy(dsInsertF58REU06.fijobn,dSD_Linha->szJOBN);
	                    MathCopy(&dsInsertF58REU06.fiupmt,&dSD_Linha->mnTDAY);
	                    strcpy(dsInsertF58REU06.fipid,dSD_Linha->szPID);
                        dsInsertF58REU06.fiupmj = dSD_Linha->jdUPMJ; 


						/* INICIO - JULIO DONATI - 30/09/2013 */
						
						/* Codigo Comentado*/
						/*idJDBReturn = JDB_InsertTable (hRequestF58REU06,szTableF58REU06, (ID) 0, &dsInsertF58REU06);*/

						/* Novo Codigo*/
				        MathCopy(&dsKeyF58RE002.fibnnf,&dSD_Linha->mnBNNF);
						MathCopy(&dsKeyF58RE002.fisln,&dSD_Linha->mnBNNF_LON);		                
						MathCopy(&dsKeyF58RE002.fin001,&dSD_Linha->mnN001);
		                strcpy(dsKeyF58RE002.fibser,dSD_Linha->szBSER);
		                strcpy(dsKeyF58RE002.fidct,dSD_Linha->szDCT);
						MathCopy(&dsKeyF58RE002.fiukid,&dSD_Linha->mnUKID);

						idJDBReturn = JDB_FetchKeyed(hRequestF58REU06, idIndex, &dsKeyF58RE002, nNumKeys, &dsFetchF58REU06, FALSE);


						if (idJDBReturn == JDEDB_PASSED)
						{
							if (!IsStringBlank(dsFetchF58REU06.fidesf))
							{
								strcpy(dsInsertF58REU06.fidesf,dsFetchF58REU06.fidesf);
							}							
							JDB_UpdateTable(hRequestF58REU06, szTableF58REU06, (ID)0, idIndex,&dsKeyF58RE002, nNumKeys, &dsInsertF58REU06);
						}
						else
						{
						    idJDBReturn = JDB_InsertTable (hRequestF58REU06,szTableF58REU06, (ID) 0, &dsInsertF58REU06);
							jdeStrcpy(lpszFCI_DESF ,(const char *) (" "),sizeof(lpszFCI_DESF));
						}
						/* FIM  - JULIO DONATI - 30/09/2013 */
													

		   x++;	       
		}      
        cont++;
	  }
	jdeStrcpy(dSD_Linha->szFCI_DESF ,(const char *) (" "),sizeof(dSD_Linha->szFCI_DESF));

 EOJReturn:

	 if(hRequestF58REU06!=NULL)
	 {
	    JDB_CloseTable(hRequestF58REU06);
	 }	
	 
 }










   /**************************************************************************
 *   Function:  IB5REU001_TagDuplicata
 *      Notes:  Leitura dados duplicata
 *    26/06/12
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 MATH_NUMERIC IB58RE001_TagDuplicata(char *szLinha,LPDS_Col_F58RE001 dsd_ColF58RE001,DSD_Linha dSD_Linha,int pVetorIni,MATH_NUMERIC mnLinha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser,DSD_Erro *dSD_Erro){
       int           tamLinha                = 0; 
	   int           nTamCampoValor          = 0;
	   int           x                       = 0;
	   char          szValor[TAMVALOR]             = {'\0'};   
	   char          szValorNF[TAMVALOR]           = {'\0'};
	   char          szCampoNF[11]           = {'\0'};
	   char          szTipoDadoNF[3]         = {'\0'};
	   char          cGrava                  = {'\0'};
	   MATH_NUMERIC  mnTamCampoValor         = { 0 };
	   MATH_NUMERIC  mnValorNF               = { 0 };
	   MATH_NUMERIC  mnOne                   = { 0 };
	   MATH_NUMERIC  mnPosicao               = { 0 };
	   JDEDATE       jdDateNF                = {0};
       ID		     idJDBReturn             = JDEDB_PASSED; 
	   ID            idReturnValue;
	   F58REU05      dsInsertF58REU05        = {0}; 
	   HREQUEST      hRequestF58REU05        = (HREQUEST) NULL;
       NID		     szTableF58REU05         = NID_F58REU05;
	   DSD58RE09A    dSD58RE09A ;

       idJDBReturn = JDB_OpenTable(hUser, szTableF58REU05, ID_F58REU05_PK, (NULL), (ushort) 0, (char *)NULL,&hRequestF58REU05);
	   
	   ParseNumericString( &mnOne, "1" );
	   tamLinha = strlen(szLinha);
	   x = pVetorIni;

	   while(jdestrcmp(dsd_ColF58RE001[x].xmaa10,"F")==0){
       
	              strcpy( dsd_ColF58RE001[x].xmaa10,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmaa10,' '),' '));
	              IntToMathNumeric(dsd_ColF58RE001[x].xmint01, &mnPosicao);
		          MathSubtract(&mnPosicao,&mnPosicao,&mnOne);      
			      jdeStrcpy(szValor ,(const char *) (" "),sizeof(szValor)); 
				  memset((void *)(&dSD58RE09A), (int)('\0'), sizeof(DSD58RE09A));
	              strcpy(dSD58RE09A.szStringIN,szLinha);
                  dSD58RE09A.szSeparador[0] = dSD_Linha.cSeparadorTexto;
		          MathCopy(&dSD58RE09A.mnSegmento,&mnPosicao);
		          idReturnValue = jdeCallObject("ParseStringWithNullValues58", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD58RE09A,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
                  strcpy(szValor,dSD58RE09A.szStringOut);  
			      jdeStrcpy(szValorNF ,(const char *) (" "),sizeof(szValorNF));                     
                  
				  IB58RE001_RemoveCaracterEspecial(szValor,szValorNF);
			      strcpy(szCampoNF,"DC");
			      strcat(szCampoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmdtai,' '),' '));	  
			      strcpy(szTipoDadoNF,IB58RE0001LTRIM(IB58RE0001RTRIM(dsd_ColF58RE001[x].xmowtp,' '),' '));	
				  
				  nTamCampoValor = strlen(szValorNF);
				  if(jdestrcmp(szTipoDadoNF,"2")==0 && nTamCampoValor>0)
				  { 					                    	 				    
			        IntToMathNumeric(nTamCampoValor, &mnTamCampoValor);
		 	        if(MathCompare(&mnTamCampoValor,&dsd_ColF58RE001[x].xmdtas)>0)
					{					   
			           strcpy(dSD_Erro->szCodErro,"300");
                       strcpy(dSD_Erro->szMensagem,szCampoNF);		  
			          
					}
				  }   
				  if(jdestrcmp(szTipoDadoNF,"9")==0)
				  {					   
  			           mnValorNF=IB58RE001_FormataNumero( dsd_ColF58RE001[x].xmcdec, szValorNF,szCampoNF,dSD_Linha.cSeparadorDecimal, lpVoid,  lpBhvrCom,dSD_Erro);					
				  }
				  if(jdestrcmp(szTipoDadoNF,"11")==0)
				  {
				       jdDateNF = IB58RE001_ConverteParaJDE_Date( szValorNF,szCampoNF, lpVoid,  lpBhvrCom,dSD_Erro);   
				  }
				  if(!jdestrcmp(dSD_Erro->szCodErro," ")==0)
				  {
                      goto EOJReturn;
				  }


				  if(jdestrcmp("DCDS01",szCampoNF)==0) {      strcpy(dsInsertF58REU05.dcds01,szValorNF);   } 
                  if(jdestrcmp("DCDIVJ",szCampoNF)==0) {      dsInsertF58REU05.dcdivj = jdDateNF;   } 
                  if(jdestrcmp("DCAAP",szCampoNF)==0) {      MathCopy(&dsInsertF58REU05.dcaap,&mnValorNF);   } 
                 		
                    
				  if(!IsStringBlank(szValorNF))
				  {
                       cGrava = 'T';
				  }
		 x++;
	  }
      if(cGrava =='T')
	  {
		   MathAdd(&mnLinha,&mnLinha,&mnOne); 
		   MathCopy(&dsInsertF58REU05.dcbnnf,&dSD_Linha.mnBNNF);
		   MathCopy(&dsInsertF58REU05.dcsln,&dSD_Linha.mnBNNF_LON);
		   MathCopy(&dsInsertF58REU05.dcn001,&dSD_Linha.mnN001);
		   MathCopy(&dsInsertF58REU05.dcsfxe,&mnLinha);
           strcpy(dsInsertF58REU05.dcbser,dSD_Linha.szBSER);
		   strcpy(dsInsertF58REU05.dcuser,dSD_Linha.szUSER);
	       strcpy(dsInsertF58REU05.dcjobn,dSD_Linha.szJOBN);
	       MathCopy(&dsInsertF58REU05.dcupmt,&dSD_Linha.mnTDAY);
	       strcpy(dsInsertF58REU05.dcpid,dSD_Linha.szPID);
           dsInsertF58REU05.dcupmj = dSD_Linha.jdUPMJ; 		  
	       idJDBReturn = JDB_InsertTable (hRequestF58REU05,szTableF58REU05, (ID) 0, &dsInsertF58REU05);
		 
	  }
     EOJReturn:

	 if(hRequestF58REU05!=NULL)
	 {
	    JDB_CloseTable(hRequestF58REU05);
	 }	
	 return mnLinha;
}







   /**************************************************************************
 *   Function:  IB5REU001_DocumentSearch
 *      Notes:  LOCALIZA O NUMERO DA ORDEM DE COMPRA NA INFORMAÇÕES ADICIONAIS
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB5REU0001_DocumentSearch(char *szIn,char token,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER hUser,DSD_Linha *dSD_Linha)
 {
	MATH_NUMERIC  mnSegmento         = { 0};
	int           mnPosString        = 0;
	int           mnTamString        = 0;
	int           mnTamLinha         = 0;
	char          cEncontrouPedido   = {'\0'};
    char          szPalavra[200]     = {'\0'};  
	char          cFlagAux           = {'\0'};
	char          szSubStr[3]        = {'\0'};
	char          szPedido[15]       = {'\0'};
	MATH_NUMERIC  mnPedido           = { 0 };
	MATH_NUMERIC  mnOne              = { 0 };
	DSD0000580    dSD0000580;
	DSD0500690    dSD0500690         = { 0 }; 
	ID            idReturnValue      = ER_SUCCESS;  
	char          keys[]             = "1234567890";
	int           x                  = 0;
	char          keys2[]            = "qwertyuiopasdfghjklçzxcvbnmQWERTYUIOPASDFGHJKLÇZXCVBNM-,.:_";
/* INICIO - JULIO DONATI - 29/07/2013 */
/*Código Comentado*/
/*	char          ListaPalavras[301];*/
/*Novo Código*/
	char          ListaPalavras[1024];
/* FIM    - JULIO DONATI - 29/07/2013 */
	char          szCNPJ[26];

    strcpy(ListaPalavras,dSD_Linha->szListaPossibilidadeOC);
	strcpy(szCNPJ,dSD_Linha->szTaxFrom);

	ParseNumericString(&mnOne,"1");
	mnTamLinha = strlen(szIn);   
	memset((void *)(&dSD0500690), (int)('\0'), sizeof(dSD0500690));
	strcpy(dSD0500690.szInputText,ListaPalavras);
	dSD0500690.cDelimiter = token;
	MathCopy(&dSD0500690.mnSegmentNumber,&mnSegmento);
    idReturnValue = jdeCallObject("ParseTextString", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD0500690,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
    strcpy(szPalavra,dSD0500690.szOutputObjectSegment);
    cEncontrouPedido = 'F';
    while(!IsStringBlank(szPalavra) && cEncontrouPedido=='F' ) 
	{
      cFlagAux ='1';    
	   mnPosString = IB58RE0001_SearchStringPositionInString(szIn,szPalavra);
	   if( mnPosString>0 )
	   {
	        mnTamString  = strlen(szPalavra);
			mnPosString  = mnPosString + mnTamString;
			while(cFlagAux =='1' && mnTamLinha >0)
			{
			   IB58RE001_SubStr(szSubStr,szIn,mnPosString,1);
			   x = -1;
               x = strcspn (szSubStr,keys);
			   if (  x==0 )
			   {
                    strcat(szPedido,szSubStr);			      
			   }
			   else
			   {
				   x = -1;
                   x = strcspn (szSubStr,keys2);
				   if(x==0 || IsStringBlank(szSubStr))
				   {

					 IB58RE0001LTRIM(szPedido,'0');
                      if(!IsStringBlank(szPedido))
					  {
                            memset((void *)(&dSD0000580), (int)('\0'), sizeof(DSD0000580));
	                        strcpy(dSD0000580.szInputString,szPedido);
                            idReturnValue = jdeCallObject("ConvertStringToMathNumeric", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD0000580,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
	                        MathCopy(&mnPedido,&dSD0000580.mnOutputMathNumeric);
                            cEncontrouPedido = IB58RE0001_SearhPurchaseOrder(mnPedido, szCNPJ, hUser,dSD_Linha);
							jdeStrcpy(szPedido ,(const char *) (" "),sizeof(szPedido));  
                            cFlagAux = '2';
					  }
				   }    
			   }
			   mnPosString++;
			   mnTamLinha--;
			}
	   
	   }
      MathAdd(&mnSegmento,&mnSegmento,&mnOne);
      memset((void *)(&dSD0500690), (int)('\0'), sizeof(dSD0500690));
	  strcpy(dSD0500690.szInputText,ListaPalavras);
	  dSD0500690.cDelimiter = token;
	  MathCopy(&dSD0500690.mnSegmentNumber,&mnSegmento);
      idReturnValue = jdeCallObject("ParseTextString", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD0500690,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
      strcpy(szPalavra,dSD0500690.szOutputObjectSegment);
	}
	
 }



/**************************************************************************
 *   Function: IB58RE001_SearhPurchaseOrder
 *      Notes: VALIDA SE A ORDEM DE COMPRA EXISTE
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
char IB58RE0001_SearhPurchaseOrder(MATH_NUMERIC mnPedido,char *szCNPJ,HUSER hUser,DSD_Linha *dSD_Linha)
{

    HREQUEST               hRequestF4301        = (HREQUEST)NULL;   
    ID                     idJDBReturn;
	SELECTSTRUCT           dsSelectF4301[2];
	F4301                  dsStrF4301           = {0};
    NID				       szTableF4301         = NID_F4301;
    ID                     idJDBR;
	HREQUEST               hRequestF0101        = (HREQUEST)NULL;  
	SELECTSTRUCT           dsSelectF0101[1];
    F0101                  dsStrF0101           = {0};
    NID				       szTableF0101         = NID_F0101;
    MATH_NUMERIC           mnAN8                = { 0 };
	int                    i                    = 0;
    char                   cEncontrou           = {'\0'};

    
         idJDBReturn = JDB_OpenTable(hUser, szTableF0101, ID_F0101_TAX_ID,
                                (NULL), (ushort) 0, (char *)NULL,&hRequestF0101);
         memset(&dsStrF0101, 0, sizeof(dsStrF0101));
         memset(&dsSelectF0101, 0, sizeof(dsSelectF0101));
   	     i=0;	 
         jdeNIDcpy(dsSelectF0101[i].Item1.szTable, NID_F0101);
	     jdeNIDcpy(dsSelectF0101[i].Item1.szDict, NID_TAX);
	     dsSelectF0101[i].nCmp    = JDEDB_CMP_EQ;
	     dsSelectF0101[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	     dsSelectF0101[i].lpValue = (LPVOID) szCNPJ;
	     dsSelectF0101[i].nValues = 1;	 
		 i++;
         idJDBReturn = JDB_SetSelection(hRequestF0101, dsSelectF0101,(ushort)i, JDEDB_SET_REPLACE);	     
         idJDBReturn = JDB_SelectKeyed(hRequestF0101, (ID)0, (void *)NULL, (short) 0);
	     idJDBReturn = JDB_Fetch(hRequestF0101, (LPVOID)&dsStrF0101,(int)NULL);
         
		 idJDBR      = JDB_OpenTable(hUser, szTableF4301, ID_F4301_ADDRESS_NO__DOCT__DOC_ESU_ONLY,
                                (NULL), (ushort) 0, (char *)NULL,&hRequestF4301);

   	     cEncontrou = 'F';
         while(idJDBReturn == JDEDB_PASSED && cEncontrou =='F')
		 {           
           MathCopy(&mnAN8,&dsStrF0101.aban8);		   
           memset(&dsStrF4301, 0, sizeof(dsStrF4301));
    	   memset(&dsSelectF4301, 0, sizeof(dsSelectF4301));
   	       i=0;	
   	       jdeNIDcpy(dsSelectF4301[i].Item1.szTable, NID_F4301);
	       jdeNIDcpy(dsSelectF4301[i].Item1.szDict, NID_DOCO);
	       dsSelectF4301[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF4301[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF4301[i].lpValue = (LPVOID) &mnPedido;
	       dsSelectF4301[i].nValues = 1;	    
           i++;
		   jdeNIDcpy(dsSelectF4301[i].Item1.szTable, NID_F4301);
	       jdeNIDcpy(dsSelectF4301[i].Item1.szDict, NID_AN8);
	       dsSelectF4301[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF4301[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF4301[i].lpValue = (LPVOID) &mnAN8;
	       dsSelectF4301[i].nValues = 1;	    
           i++;
           idJDBR  = JDB_SetSelection(hRequestF4301, dsSelectF4301,(ushort)i, JDEDB_SET_REPLACE);	     
           idJDBR  = JDB_SelectKeyed(hRequestF4301, (ID)0, (void *)NULL, (short) 0);
	       idJDBR  = JDB_Fetch(hRequestF4301, (LPVOID)&dsStrF4301,(int)NULL);
		   if(idJDBR==JDEDB_PASSED)
		   {
              MathCopy(&dSD_Linha->mnAN8,&dsStrF4301.phan8);
			  MathCopy(&dSD_Linha->mnDOCO,&mnPedido); 
			  strcpy(dSD_Linha->szDCTO,dsStrF4301.phdcto);
			  strcpy(dSD_Linha->szMCU,dsStrF4301.phmcu);
			  strcpy(dSD_Linha->szKCOO,dsStrF4301.phkcoo);
			  cEncontrou ='T';  
		   }
		      		
            idJDBReturn = JDB_Fetch(hRequestF0101, (LPVOID)&dsStrF0101,(int)NULL);
            
		  }
		  if(hRequestF0101!=NULL)
              JDB_CloseTable(hRequestF0101);
		  if(hRequestF4301!=NULL)
			  JDB_CloseTable(hRequestF4301);

	return cEncontrou;
}


   /**************************************************************************
 *   Function:  IB5REU001_FCISearch
 *      Notes:  LOCALIZA O NUMERO DA ORDEM DE COMPRA NA INFORMAÇÕES ADICIONAIS
 *  Developer:  JULIO DONATI - 29/07/2013
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB5REU0001_FCISearch(char *szIn,char token,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER hUser,DSD_Linha *dSD_Linha)
 {
	MATH_NUMERIC  mnSegmento         = { 0};
	int           mnPosString        = 0;
	int           mnTamString        = 0;
	int           mnTamLinha         = 0;
	int           mnPosicaoFCI       = 0;
	char          cEncontrouFCI      = {'\0'};
    char          szPalavra[200]     = {'\0'};  
	char          cFlagAux           = {'\0'};
	char          szSubStr[3]        = {'\0'};
	char          szFCI[36]          = {'\0'};
	MATH_NUMERIC  mnOne              = { 0 };
	DSD0000580    dSD0000580;
	DSD0500690    dSD0500690         = { 0 }; 
	ID            idReturnValue      = ER_SUCCESS;  
	/*char          keys[]             = "qwertyuiopasdfghjklçzxcvbnm1234567890-QWERTYUIOPASDFGHJKLÇZXCVBNM";*/
	char          keys[]             = "abcdef1234567890-ABCDEF";
	int           x                  = 0;
	char          ListaPalavras[1024];

    strcpy(ListaPalavras,dSD_Linha->szListaPossibilidadeFCI);

	ParseNumericString(&mnOne,"1");
	mnTamLinha = strlen(szIn);   
	memset((void *)(&dSD0500690), (int)('\0'), sizeof(dSD0500690));
	strcpy(dSD0500690.szInputText,ListaPalavras);
	dSD0500690.cDelimiter = token;
	MathCopy(&dSD0500690.mnSegmentNumber,&mnSegmento);
    idReturnValue = jdeCallObject("ParseTextString", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD0500690,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
    strcpy(szPalavra,dSD0500690.szOutputObjectSegment);
    cEncontrouFCI = 'F';
    while(!IsStringBlank(szPalavra) && cEncontrouFCI=='F' ) 
	{
      cFlagAux ='1';    
	   mnPosString = IB58RE0001_SearchStringPositionInString(szIn,szPalavra);
	   if( mnPosString>0 )
	   {
	        mnTamString  = strlen(szPalavra);
			mnPosString  = mnPosString + mnTamString;
			while(cFlagAux =='1' && mnTamLinha >0 && mnPosicaoFCI<37)
			{
			   IB58RE001_SubStr(szSubStr,szIn,mnPosString,1);
			   x = -1;
               x = strcspn (szSubStr,keys);
			   if (  x==0 && mnPosicaoFCI<36)
			   {
                    if (szSubStr[0] == '-' ) 
                    {
						if (mnPosicaoFCI==8 || mnPosicaoFCI==13 || mnPosicaoFCI==18 || mnPosicaoFCI==23)
						{
						    strcat(szFCI,szSubStr);
							mnPosicaoFCI++;				
						}
						else
						{
						    mnPosicaoFCI = 37;				
						}

					}
					else
					{
						/* INICIO - JULIO DONATI - 19/11/2013 */
					    if ((IsStringBlank(szSubStr)) && (mnPosicaoFCI > 1))
						{
							mnPosicaoFCI = 37;
						}

						if ((mnPosicaoFCI==8 || mnPosicaoFCI==13 || mnPosicaoFCI==18 || mnPosicaoFCI==23) && (szSubStr[0] != '-' ))
						{
							mnPosicaoFCI = 37;
						}

						/* FIM    - JULIO DONATI - 19/11/2013 */
												
						strcat(szFCI,szSubStr);
						mnPosicaoFCI++;
					}
			   }
			   else
			   {
				   if (  x==1 && mnPosicaoFCI>1 && mnPosicaoFCI<36)
				   {
						mnPosicaoFCI = 37;
				   }

				   if(!IsStringBlank(szFCI) && mnPosicaoFCI==36)
				   {                            
						strcpy(dSD_Linha->szFCI_DESF,szFCI);
						jdeStrcpy(szFCI ,(const char *) (" "),sizeof(szFCI));  
                        cFlagAux = '2';	
						cEncontrouFCI = 'T';
				   }
				      
			   }
			   mnPosString++;
			   mnTamLinha--;
			}
	   
	   }
      MathAdd(&mnSegmento,&mnSegmento,&mnOne);
      memset((void *)(&dSD0500690), (int)('\0'), sizeof(dSD0500690));
	  strcpy(dSD0500690.szInputText,ListaPalavras);
	  dSD0500690.cDelimiter = token;
	  MathCopy(&dSD0500690.mnSegmentNumber,&mnSegmento);
      idReturnValue = jdeCallObject("ParseTextString", NULL, lpBhvrCom,lpVoid, (LPVOID)&dSD0500690,(CALLMAP *)NULL, (int)0, (char *)NULL,(char *)NULL, (int)0);
      strcpy(szPalavra,dSD0500690.szOutputObjectSegment);
	  mnPosicaoFCI = 0;
	}
	
 }







/**************************************************************************
 *   Function:  IB58RE001_SearchStringPositionInString
 *      Notes:  
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

int IB58RE0001_SearchStringPositionInString(char *szInputString,char *szStringToFind)
{

   char          *pPosicao;
   long	         lDeslocamento;
   MATH_NUMERIC  mnDeslocamento_Math;
   int           mnDeslocamento;

    pPosicao = strstr(szInputString, szStringToFind);
	if (pPosicao != NULL) 
	{
		lDeslocamento = pPosicao - szInputString;
	}
	else
	{
		lDeslocamento = -1;
	}

	LongToMathNumeric(lDeslocamento, &mnDeslocamento_Math);
	MathNumericToInt( &mnDeslocamento_Math, &mnDeslocamento);
 
   return mnDeslocamento;
}


 /**************************************************************************
 *   Function:  IB58RE001_InsertF58RE76A
 *      Notes:  INSERE NAS TABELA DE INTERFACE F58RE76A
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

 void IB58RE0001_InsertF58RE76A(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser)
 {
    
	HREQUEST               hRequest                = (HREQUEST)NULL;   
    ID                     idJDBReturn;
	SELECTSTRUCT           dsSelectF58REU01[2];
	F58REU01               dsStrF58REU01           = {0};
    NID				       szTableF58REU01         = NID_F58REU01;	
    HREQUEST               hRequestF58RE76A        = (HREQUEST)NULL;   
    NID				       szTableF58RE76A         = NID_F58RE76A;
    ID                     idJDBR;
    int                    i                       = 0;
	


           idJDBR  = JDB_OpenTable(hUser, szTableF58REU01, ID_F58REU01_BNNF_N001,(NULL), (ushort) 0, (char *)NULL,&hRequest);
		   
	       memset(&dsStrF58REU01, 0, sizeof(dsStrF58REU01));
    	   memset(&dsSelectF58REU01, 0, sizeof(dsSelectF58REU01));
   	       i=0;	
   	       jdeNIDcpy(dsSelectF58REU01[i].Item1.szTable, NID_F58REU01);
	       jdeNIDcpy(dsSelectF58REU01[i].Item1.szDict, NID_BNNF);
	       dsSelectF58REU01[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU01[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU01[i].lpValue = (LPVOID) &dSD_Linha.mnBNNF;
	       dsSelectF58REU01[i].nValues = 1;	    
           i++;
		   jdeNIDcpy(dsSelectF58REU01[i].Item1.szTable, NID_F58REU01);
	       jdeNIDcpy(dsSelectF58REU01[i].Item1.szDict, NID_N001);
	       dsSelectF58REU01[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU01[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU01[i].lpValue = (LPVOID) &dSD_Linha.mnN001;
	       dsSelectF58REU01[i].nValues = 1;	    
           i++;
           idJDBR  = JDB_SetSelection(hRequest, dsSelectF58REU01,(ushort)i, JDEDB_SET_REPLACE);	     
           idJDBR  = JDB_SelectKeyed(hRequest, (ID)0, (void *)NULL, (short) 0);
	       idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU01,(int)NULL);
           
		   if(idJDBR ==JDEDB_PASSED){
             
			  idJDBR  = JDB_OpenTable(hUser, szTableF58RE76A, ID_F58RE76A_NOTAFISCAL,(NULL), (ushort) 0, (char *)NULL,&hRequestF58RE76A);
              if(idJDBR ==JDEDB_PASSED)
			  {
                MathCopy(&dsStrF58REU01.fhan8, &dSD_Linha.mnAN8);  
			   idJDBR = JDB_InsertTable (hRequestF58RE76A,szTableF58RE76A, (ID) 0, &dsStrF58REU01);
			  } 
		   }

		 
		   
		   if(hRequest != NULL)
			   JDB_CloseTable(hRequest);
		   if(hRequestF58RE76A != NULL)
		       JDB_CloseTable(hRequestF58RE76A);
		   
 }
 /**************************************************************************
 *   Function:  IB58RE0001_InsertF58RE76C
 *      Notes:  INSERE NA TABELA DE INTERFACE F58RE76C
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB58RE0001_InsertF58RE76C(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser)
 {
     
	HREQUEST               hRequest                = (HREQUEST)NULL;   
    ID                     idJDBReturn;
	SELECTSTRUCT           dsSelectF58REU03[2];
	F58REU02               dsStrF58REU03           = {0};
    NID				       szTableF58REU03         = NID_F58REU03;      
    HREQUEST               hRequestF58RE76C        = (HREQUEST)NULL;   
    NID				       szTableF58RE76C         = NID_F58RE76C;
    ID                     idJDBR;
    int                    i                       = 0;


	       idJDBR  = JDB_OpenTable(hUser, szTableF58REU03, ID_F58REU03_BNNF_N001,(NULL), (ushort) 0, (char *)NULL,&hRequest);
		   idJDBR  = JDB_OpenTable(hUser, szTableF58RE76C, ID_F58RE76C_BNNF_N001,(NULL), (ushort) 0, (char *)NULL,&hRequestF58RE76C);

	       memset(&dsStrF58REU03, 0, sizeof(dsStrF58REU03));
    	   memset(&dsSelectF58REU03, 0, sizeof(dsSelectF58REU03));
   	       i=0;	
   	       jdeNIDcpy(dsSelectF58REU03[i].Item1.szTable, NID_F58REU03);
	       jdeNIDcpy(dsSelectF58REU03[i].Item1.szDict, NID_BNNF);
	       dsSelectF58REU03[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU03[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU03[i].lpValue = (LPVOID) &dSD_Linha.mnBNNF;
	       dsSelectF58REU03[i].nValues = 1;	    
           i++;
		   jdeNIDcpy(dsSelectF58REU03[i].Item1.szTable, NID_F58REU03);
	       jdeNIDcpy(dsSelectF58REU03[i].Item1.szDict, NID_N001);
	       dsSelectF58REU03[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU03[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU03[i].lpValue = (LPVOID) &dSD_Linha.mnN001;
	       dsSelectF58REU03[i].nValues = 1;	    
           i++;
           idJDBR  = JDB_SetSelection(hRequest, dsSelectF58REU03,(ushort)i, JDEDB_SET_REPLACE);	     
           idJDBR  = JDB_SelectKeyed(hRequest, (ID)0, (void *)NULL, (short) 0);
	       idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU03,(int)NULL);

		   while(idJDBR == JDEDB_PASSED) 
		   {             
             idJDBR = JDB_InsertTable (hRequestF58RE76C,szTableF58RE76C, (ID) 0, &dsStrF58REU03);
             idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU03,(int)NULL);
		   }
		  

		   if(hRequest != NULL)
			   JDB_CloseTable(hRequest);
		   if(hRequestF58RE76C != NULL)
		       JDB_CloseTable(hRequestF58RE76C);
 }

 /**************************************************************************
 *   Function:  IB58RE0001_InsertF58RE76B
 *      Notes:  INSERE NA TABELA DE INTERFACE F58RE76B
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB58RE0001_InsertF58RE76B(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser)
 {
     
	HREQUEST               hRequest                = (HREQUEST)NULL;   
    ID                     idJDBReturn;
	SELECTSTRUCT           dsSelectF58REU02[2];
	F58REU02               dsStrF58REU02           = {0};
    NID				       szTableF58REU02         = NID_F58REU02;      
    HREQUEST               hRequestF58RE76B        = (HREQUEST)NULL;   
    NID				       szTableF58RE76B         = NID_F58RE76B;
    ID                     idJDBR;
    int                    i                       = 0;
	MATH_NUMERIC           mnZero                  = { 0 };

           ParseNumericString(&mnZero,"0");

	       idJDBR  = JDB_OpenTable(hUser, szTableF58REU02, ID_F58REU02_BNNF_N001,(NULL), (ushort) 0, (char *)NULL,&hRequest);
		   idJDBR  = JDB_OpenTable(hUser, szTableF58RE76B, ID_F58RE76B_NOTAFISCALDET,(NULL), (ushort) 0, (char *)NULL,&hRequestF58RE76B);

	       memset(&dsStrF58REU02, 0, sizeof(dsStrF58REU02));
    	   memset(&dsSelectF58REU02, 0, sizeof(dsSelectF58REU02));
   	       i=0;	
   	       jdeNIDcpy(dsSelectF58REU02[i].Item1.szTable, NID_F58REU02);
	       jdeNIDcpy(dsSelectF58REU02[i].Item1.szDict, NID_BNNF);
	       dsSelectF58REU02[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU02[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU02[i].lpValue = (LPVOID) &dSD_Linha.mnBNNF;
	       dsSelectF58REU02[i].nValues = 1;	    
           i++;
		   jdeNIDcpy(dsSelectF58REU02[i].Item1.szTable, NID_F58REU02);
	       jdeNIDcpy(dsSelectF58REU02[i].Item1.szDict, NID_N001);
	       dsSelectF58REU02[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU02[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU02[i].lpValue = (LPVOID) &dSD_Linha.mnN001;
	       dsSelectF58REU02[i].nValues = 1;	    
           i++;
           idJDBR  = JDB_SetSelection(hRequest, dsSelectF58REU02,(ushort)i, JDEDB_SET_REPLACE);	     
           idJDBR  = JDB_SelectKeyed(hRequest, (ID)0, (void *)NULL, (short) 0);
	       idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU02,(int)NULL);

		   while(idJDBR == JDEDB_PASSED) 
		   {    
			  if(MathCompare(&dSD_Linha.mnDOCO,&mnZero)>0 && !MathCompare(&dsStrF58REU02.fddoco,&mnZero)>0)
			  { 
                 strcpy(dsStrF58REU02.fdkcoo, dSD_Linha.szKCOO); 
			     MathCopy(&dsStrF58REU02.fddoco, &dSD_Linha.mnDOCO);
			     strcpy(dsStrF58REU02.fdpdct,dSD_Linha.szDCTO);
			     strcpy(dsStrF58REU02.fdmcu,dSD_Linha.szMCU);
              }
			 MathCopy(&dsStrF58REU02.fdan8, &dSD_Linha.mnAN8);
             idJDBR = JDB_InsertTable (hRequestF58RE76B,szTableF58RE76B, (ID) 0, &dsStrF58REU02);
             idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU02,(int)NULL);
		   }
		  
		   if(hRequest != NULL)
			   JDB_CloseTable(hRequest);
		   if(hRequestF58RE76B != NULL)
		       JDB_CloseTable(hRequestF58RE76B);
 }

  /**************************************************************************
 *   Function:  IB58RE0001_InsertF58RE76D
 *      Notes:  INSERE NA TABELA DE INTERFACE F58RE76D
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB58RE0001_InsertF58RE76D(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser)
 {
     
	HREQUEST               hRequest                = (HREQUEST)NULL;   
    ID                     idJDBReturn;
	SELECTSTRUCT           dsSelectF58REU04[2];
	F58REU02               dsStrF58REU04           = {0};
    NID				       szTableF58REU04         = NID_F58REU04;      
    HREQUEST               hRequestF58RE76D        = (HREQUEST)NULL;   
    NID				       szTableF58RE76D         = NID_F58RE76D;
    ID                     idJDBR;
    int                    i                       = 0;


	       idJDBR  = JDB_OpenTable(hUser, szTableF58REU04, ID_F58REU04_PK,(NULL), (ushort) 0, (char *)NULL,&hRequest);
		   idJDBR  = JDB_OpenTable(hUser, szTableF58RE76D, ID_F58RE76D_PK,(NULL), (ushort) 0, (char *)NULL,&hRequestF58RE76D);

	       memset(&dsStrF58REU04, 0, sizeof(dsStrF58REU04));
    	   memset(&dsSelectF58REU04, 0, sizeof(dsSelectF58REU04));
   	       i=0;	
   	       jdeNIDcpy(dsSelectF58REU04[i].Item1.szTable, NID_F58REU04);
	       jdeNIDcpy(dsSelectF58REU04[i].Item1.szDict, NID_BNNF);
	       dsSelectF58REU04[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU04[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU04[i].lpValue = (LPVOID) &dSD_Linha.mnBNNF;
	       dsSelectF58REU04[i].nValues = 1;	    
           i++;
		   jdeNIDcpy(dsSelectF58REU04[i].Item1.szTable, NID_F58REU04);
	       jdeNIDcpy(dsSelectF58REU04[i].Item1.szDict, NID_N001);
	       dsSelectF58REU04[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU04[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU04[i].lpValue = (LPVOID) &dSD_Linha.mnN001;
	       dsSelectF58REU04[i].nValues = 1;	    
           i++;

           idJDBR  = JDB_SetSelection(hRequest, dsSelectF58REU04,(ushort)i, JDEDB_SET_REPLACE);	     
           idJDBR  = JDB_SelectKeyed(hRequest, (ID)0, (void *)NULL, (short) 0);
	       idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU04,(int)NULL);

		   while(idJDBR == JDEDB_PASSED) 
		   {             
             idJDBR = JDB_InsertTable (hRequestF58RE76D,szTableF58RE76D, (ID) 0, &dsStrF58REU04);
             idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU04,(int)NULL);
		   }
            
		   

		   if(hRequest != NULL)
			   JDB_CloseTable(hRequest);
		   if(hRequestF58RE76D != NULL)
		       JDB_CloseTable(hRequestF58RE76D);
 }



 /**************************************************************************
 *   Function:  IB58RE0001_InsertF58RE76G
 *      Notes:  INSERE NA TABELA DE INTERFACE F58RE76G
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB58RE0001_InsertF58RE76G(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser)
 {
     
	HREQUEST               hRequest                = (HREQUEST)NULL;   
    ID                     idJDBReturn;
	SELECTSTRUCT           dsSelectF58REU05[2];
	F58REU05               dsStrF58REU05           = {0};
    NID				       szTableF58REU05         = NID_F58REU05;      
    HREQUEST               hRequestF58RE76G        = (HREQUEST)NULL;   
    NID				       szTableF58RE76G         = NID_F58RE76G;
    ID                     idJDBR;
    int                    i                       = 0;


	       idJDBR  = JDB_OpenTable(hUser, szTableF58REU05, ID_F58REU05_ID1,(NULL), (ushort) 0, (char *)NULL,&hRequest);
		   idJDBR  = JDB_OpenTable(hUser, szTableF58RE76G, ID_F58RE76G_PK,(NULL), (ushort) 0, (char *)NULL,&hRequestF58RE76G);

	       memset(&dsStrF58REU05, 0, sizeof(dsStrF58REU05));
    	   memset(&dsSelectF58REU05, 0, sizeof(dsSelectF58REU05));
   	       i=0;	
   	       jdeNIDcpy(dsSelectF58REU05[i].Item1.szTable, NID_F58REU05);
	       jdeNIDcpy(dsSelectF58REU05[i].Item1.szDict, NID_BNNF);
	       dsSelectF58REU05[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU05[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU05[i].lpValue = (LPVOID) &dSD_Linha.mnBNNF;
	       dsSelectF58REU05[i].nValues = 1;	    
           i++;
		   jdeNIDcpy(dsSelectF58REU05[i].Item1.szTable, NID_F58REU05);
	       jdeNIDcpy(dsSelectF58REU05[i].Item1.szDict, NID_N001);
	       dsSelectF58REU05[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU05[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU05[i].lpValue = (LPVOID) &dSD_Linha.mnN001;
	       dsSelectF58REU05[i].nValues = 1;	    
           i++;
           idJDBR  = JDB_SetSelection(hRequest, dsSelectF58REU05,(ushort)i, JDEDB_SET_REPLACE);	     
           idJDBR  = JDB_SelectKeyed(hRequest, (ID)0, (void *)NULL, (short) 0);
	       idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU05,(int)NULL);

		   while(idJDBR == JDEDB_PASSED) 
		   {             
             idJDBR = JDB_InsertTable (hRequestF58RE76G,szTableF58RE76G, (ID) 0, &dsStrF58REU05);
             idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU05,(int)NULL);
		   }
		  

		   if(hRequest != NULL)
			   JDB_CloseTable(hRequest);
		   if(hRequestF58RE76G != NULL)
		       JDB_CloseTable(hRequestF58RE76G);
 }



 /**************************************************************************
 *   Function:  IB58RE0001_InsertF58RE76H
 *      Notes:  INSERE NA TABELA DE INTERFACE F58RE76H
 *  Developer:  JULIO DONATI - 26/07/2013
 *    Returns:
 *
 * Parameters:
 **************************************************************************/
 void IB58RE0001_InsertF58RE76H(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser)
 {
     
	HREQUEST               hRequest                = (HREQUEST)NULL;   
    ID                     idJDBReturn;
	SELECTSTRUCT           dsSelectF58REU06[2];
	F58REU06               dsStrF58REU06           = {0};
    NID				       szTableF58REU06         = NID_F58REU06;      
    HREQUEST               hRequestF58RE76H        = (HREQUEST)NULL;   
    NID				       szTableF58RE76H         = NID_F58RE76H;
    ID                     idJDBR;
    int                    i                       = 0;


	       idJDBR  = JDB_OpenTable(hUser, szTableF58REU06, ID_F58REU06_BNNF__N001,(NULL), (ushort) 0, (char *)NULL,&hRequest);
		   idJDBR  = JDB_OpenTable(hUser, szTableF58RE76H, ID_F58RE76H_PK,(NULL), (ushort) 0, (char *)NULL,&hRequestF58RE76H);

	       memset(&dsStrF58REU06, 0, sizeof(dsStrF58REU06));
    	   memset(&dsSelectF58REU06, 0, sizeof(dsSelectF58REU06));
   	       i=0;	
   	       jdeNIDcpy(dsSelectF58REU06[i].Item1.szTable, NID_F58REU06);
	       jdeNIDcpy(dsSelectF58REU06[i].Item1.szDict, NID_BNNF);
	       dsSelectF58REU06[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU06[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU06[i].lpValue = (LPVOID) &dSD_Linha.mnBNNF;
	       dsSelectF58REU06[i].nValues = 1;	    
           i++;
		   jdeNIDcpy(dsSelectF58REU06[i].Item1.szTable, NID_F58REU06);
	       jdeNIDcpy(dsSelectF58REU06[i].Item1.szDict, NID_N001);
	       dsSelectF58REU06[i].nCmp    = JDEDB_CMP_EQ;
	       dsSelectF58REU06[i].nAndOr  = (JDEDB_ANDOR) JDEDB_ANDOR_AND;
	       dsSelectF58REU06[i].lpValue = (LPVOID) &dSD_Linha.mnN001;
	       dsSelectF58REU06[i].nValues = 1;	    
           i++;
           idJDBR  = JDB_SetSelection(hRequest, dsSelectF58REU06,(ushort)i, JDEDB_SET_REPLACE);	     
           idJDBR  = JDB_SelectKeyed(hRequest, (ID)0, (void *)NULL, (short) 0);
	       idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU06,(int)NULL);

		   while(idJDBR == JDEDB_PASSED) 
		   {             
             idJDBR = JDB_InsertTable (hRequestF58RE76H,szTableF58RE76H, (ID) 0, &dsStrF58REU06);
             idJDBR  = JDB_Fetch(hRequest, (LPVOID)&dsStrF58REU06,(int)NULL);
		   }
		  

		   if(hRequest != NULL)
			   JDB_CloseTable(hRequest);
		   if(hRequestF58RE76H != NULL)
		       JDB_CloseTable(hRequestF58RE76H);
 }

  /**************************************************************************
 *   Function:  IB58RE001_RemoveCaracterEspecial
 *      Notes:  
 *
 *    Returns:
 *
 * Parameters:
 **************************************************************************/

 void IB58RE001_RemoveCaracterEspecial(char *pTextPtr,char *out)
  {

	  int			mnStringLength		        	= 0;
	  char		    szHoldTextString[MAXTAMLINHA]	= {'\0'};
      char          szAux[MAXTAMLINHA]              = {'\0'};

	  strcpy(szAux,pTextPtr);

 		while(*pTextPtr)
		{			
			if ((*pTextPtr == '\r')||(*pTextPtr == '\n'))
			{
				break;
			}
			pTextPtr++;
			mnStringLength++;
		}
		strncpy(szHoldTextString, szAux, mnStringLength);
		strncpy(out, szHoldTextString, mnStringLength);
		
  }
 
 void IB58RE001_jdeScrubDDText(char *lpName)
{
	char *szTmp = lpName;

	while (*szTmp)
	{
		
		if (*szTmp < 0)
		{
			*szTmp = ' ';
		}
		szTmp++;
	}
	return;
}

 

void IB58RE0001_DeleteTableCache(DSD_Linha dSD_Linha,LPVOID lpVoid, LPBHVRCOM lpBhvrCom,HUSER  hUser)
{
  NID				     szTableF58REU01         = NID_F58REU01;
  NID				     szTableF58REU02         = NID_F58REU02;
  NID				     szTableF58REU03         = NID_F58REU03; 
  NID				     szTableF58REU04         = NID_F58REU04; 
  NID				     szTableF58REU05         = NID_F58REU05;   
  KEY1_F58REU04          dsKeyF58REU04;
  KEY6_F58REU02          dsKeyF58REU02;
  KEY2_F58REU03          dsKeyF58REU03;
  KEY5_F58REU01          dsKeyF58REU01;
  KEY1_F58REU05          dsKeyF58REU05;
  ID                     idJDBR;
  HREQUEST               hRequestF58REU01        = (HREQUEST)NULL;
  HREQUEST               hRequestF58REU02        = (HREQUEST)NULL;
  HREQUEST               hRequestF58REU03        = (HREQUEST)NULL;
  HREQUEST               hRequestF58REU04        = (HREQUEST)NULL;
  HREQUEST               hRequestF58REU05        = (HREQUEST)NULL;
/* INICIO - JULIO DONATI - 26/07/2013 */
  NID				     szTableF58REU06         = NID_F58REU06; 
  KEY2_F58REU06		     dsKeyF58REU06;
  HREQUEST               hRequestF58REU06        = (HREQUEST)NULL;
/* FIM    - JULIO DONATI - 26/07/2013 */


  idJDBR  = JDB_OpenTable(hUser, szTableF58REU01, ID_F58REU01_BNNF_N001,(NULL), (ushort) 0, (char *)NULL,&hRequestF58REU01);
  idJDBR  = JDB_OpenTable(hUser, szTableF58REU02, ID_F58REU02_BNNF_N001,(NULL), (ushort) 0, (char *)NULL,&hRequestF58REU02);
  idJDBR  = JDB_OpenTable(hUser, szTableF58REU03, ID_F58REU03_BNNF_N001,(NULL), (ushort) 0, (char *)NULL,&hRequestF58REU03);
  idJDBR  = JDB_OpenTable(hUser, szTableF58REU04, ID_F58REU04_PK,(NULL), (ushort) 0, (char *)NULL,&hRequestF58REU04);
  idJDBR  = JDB_OpenTable(hUser, szTableF58REU05, ID_F58REU05_PK,(NULL), (ushort) 0, (char *)NULL,&hRequestF58REU05);
/* INICIO - JULIO DONATI - 26/07/2013 */
  idJDBR  = JDB_OpenTable(hUser, szTableF58REU06, ID_F58REU06_BNNF__N001,(NULL), (ushort) 0, (char *)NULL,&hRequestF58REU06);
/* FIM    - JULIO DONATI - 26/07/2013 */

  MathCopy(&dsKeyF58REU01.fhbnnf,&dSD_Linha.mnBNNF);
  MathCopy(&dsKeyF58REU01.fhn001,&dSD_Linha.mnN001);
  JDB_DeleteTable(hRequestF58REU01,szTableF58REU01,(ID)0, ID_F58REU01_BNNF_N001, (void *)&dsKeyF58REU01,
                  (short)2); 

  MathCopy(&dsKeyF58REU02.fdbnnf,&dSD_Linha.mnBNNF);
  MathCopy(&dsKeyF58REU02.fdn001,&dSD_Linha.mnN001);
  JDB_DeleteTable(hRequestF58REU02,szTableF58REU02,(ID)0, ID_F58REU02_BNNF_N001, (void *)&dsKeyF58REU02,
                  (short)2);  

  MathCopy(&dsKeyF58REU03.flbnnf,&dSD_Linha.mnBNNF);
  MathCopy(&dsKeyF58REU03.fln001,&dSD_Linha.mnN001);
  JDB_DeleteTable(hRequestF58REU03,szTableF58REU03,(ID)0, ID_F58REU03_BNNF_N001, (void *)&dsKeyF58REU03,
                  (short)2); 
  
  MathCopy(&dsKeyF58REU04.fmbnnf,&dSD_Linha.mnBNNF);
  MathCopy(&dsKeyF58REU04.fmn001,&dSD_Linha.mnN001);
  JDB_DeleteTable(hRequestF58REU04,szTableF58REU04,(ID)0, ID_F58REU04_PK, (void *)&dsKeyF58REU04,
                  (short)2); 


  MathCopy(&dsKeyF58REU05.dcbnnf,&dSD_Linha.mnBNNF);
  MathCopy(&dsKeyF58REU05.dcn001,&dSD_Linha.mnN001);
  JDB_DeleteTable(hRequestF58REU05,szTableF58REU05,(ID)0, ID_F58REU05_PK, (void *)&dsKeyF58REU05,
                  (short)2); 

/* INICIO - JULIO DONATI - 26/07/2013 */
  MathCopy(&dsKeyF58REU06.fibnnf,&dSD_Linha.mnBNNF);
  MathCopy(&dsKeyF58REU06.fin001,&dSD_Linha.mnN001);
  JDB_DeleteTable(hRequestF58REU06,szTableF58REU06,(ID)0, ID_F58REU06_BNNF__N001, (void *)&dsKeyF58REU06,
                  (short)2); 
/* FIM    - JULIO DONATI - 26/07/2013 */


   if(hRequestF58REU01!=NULL)
   {
	   JDB_CloseTable(hRequestF58REU01);
   }	
   if(hRequestF58REU02!=NULL)
   {
	   JDB_CloseTable(hRequestF58REU02);
   }
   if(hRequestF58REU03!=NULL)
   {
	   JDB_CloseTable(hRequestF58REU03);
   }
   if(hRequestF58REU04!=NULL)
   {
	   JDB_CloseTable(hRequestF58REU04);
   }
   if(hRequestF58REU05!=NULL)
   {
	   JDB_CloseTable(hRequestF58REU05);
   }
/* INICIO - JULIO DONATI - 26/07/2013 */
   if(hRequestF58REU06!=NULL)
   {
	   JDB_CloseTable(hRequestF58REU06);
   }
/* FIM    - JULIO DONATI - 26/07/2013 */

}


void IB58RE001ParseString(char *out,char *in, MATH_NUMERIC mnSegmento,char szSeparador[2]){
   MATH_NUMERIC       mnPosicao =  { 0 };
   int               mnTamStr  =  0;
   int               mnCont    =  0;
   char              szSub[3]  =  {'\0'};
   MATH_NUMERIC      mnUm      =  { 0 };
  
   mnTamStr = strlen(in); 
   ParseNumericString(&mnPosicao,"0");
   ParseNumericString(&mnUm,"1");
   while( MathCompare(&mnPosicao,&mnSegmento)<0 && mnCont<=mnTamStr ){ 
		jdeStrcpy(szSub ,(const char *) (" "),sizeof(szSub)); 
        IB58RE001_SubStr(szSub,in,mnCont,1);
		if(jdestrcmp(szSub,szSeparador)==0){
		   MathAdd(&mnPosicao,&mnPosicao,&mnUm);
		}
		mnCont++;
   }
    
   jdeStrcpy(szSub ,(const char *) (" "),sizeof(szSub));
   while( jdestrcmp(szSub,szSeparador)!=0 && mnCont<mnTamStr ){

		jdeStrcpy(szSub ,(const char *) (" "),sizeof(szSub));
		IB58RE001_SubStr(szSub,in,mnCont,1);
		if(jdestrcmp(szSub,szSeparador)!=0){
	
			strcpy(out++,szSub);
		}
       mnCont++;
   }

}


