#include <jde.h>

#define b57odbc_c


/*****************************************************************************
 *    Source File:  b57odbc
 *
 *    Description:  Função para Trabalhar com ODBC Source File
 *
 *        History:
 *          Date        Programmer  SAR# - Description
 *          ----------  ----------  -------------------------------------------
 *   Author 20/01/11                Unknown  - Created  
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

#include <b57odbc.h>


/**************************************************************************
 *  Business Function:  ConectaBanco
 *
 *        Description:  Efetua Conexao ODBC
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC         lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI ConectaBanco (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
    int       rcode;
	//
	JCHAR      szBaseDados[300]       = {_J('\0')};
	JCHAR      *szDriver;
    SQLHENV   EnvironmentHandle    = _J('\0'); 
    SQLHSTMT  StatementHandle      = _J('\0');
    SQLHDBC   ConnectionHandle;
	HUSER     hUser                = (HUSER) NULL;
	JDEDB_RESULT  nJDBReturn;
	/*short teste = 0;*/
	//
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC )	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    rcode = JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//Seta Host
	if (IsStringBlank (lpDS->szStringConexao))
	{  
		//sprintf(szBaseDados,"Driver={SQL Server};Server=CANSWEB08;Database=AGCONETDB;Uid=usr_aol;Pwd=gk29*4b;");
	}
	else
	{
      jdeStrncpy(szBaseDados,(const JCHAR *)lpDS->szStringConexao,DIM(szBaseDados));
	};
	/* Pega um Enrinoment Handle pro ODBC */
    rcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &EnvironmentHandle);
    // 
    /* Atributos ODBC  */
    rcode =SQLSetEnvAttr(EnvironmentHandle,SQL_ATTR_ODBC_VERSION,(SQLPOINTER *)SQL_OV_ODBC3 ,0);
	//
    //* Pega um Handle de Conexao */
    rcode =SQLAllocHandle(SQL_HANDLE_DBC, EnvironmentHandle, &ConnectionHandle);
    /* Cria a Conexao */
	/*teste = jdeStrlen(szBaseDados); */ 
	rcode = SQLDriverConnect(ConnectionHandle, 
							 NULL, 
		                     (SQLWCHAR *) szBaseDados, 
							 (short) (jdeStrlen(szBaseDados)+1), 
							 NULL, 
							 300, 
							 NULL, 
							 SQL_DRIVER_NOPROMPT);
	/*rcode = SQLDriverConnect(ConnectionHandle, 
							 lpBhvrCom->hDlg, 
		                     NULL, 
							 300, 
							 NULL, 
							 300, 
							 NULL,
							 SQL_DRIVER_PROMPT);*/
    //
	if (rcode != SQL_SUCCESS) 
	{ 
		I57ODBC_LogErros(ConnectionHandle, SQL_HANDLE_DBC,(RETCODE) rcode); 
	}
	else 
	{
		rcode =SQLAllocHandle(SQL_HANDLE_STMT, ConnectionHandle, &StatementHandle);
		//
	    lpDS->idStatementHandle   = jdeStoreDataPtr(hUser, StatementHandle);
		lpDS->idConnectionHandle  = jdeStoreDataPtr(hUser, ConnectionHandle);
	    lpDS->idEnvironmentHandle = jdeStoreDataPtr(hUser, EnvironmentHandle);
		//
	}
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

   return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  ExecutaSQL
 *
 *        Description:  Executa Comando SQL
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC1        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI ExecutaSQL (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC1 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
	int rcode                      = 0;
	//
	HUSER     hUser                = (HUSER) NULL;
    SQLHSTMT  StatementHandle      = _J('\0');
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC1 )	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
	//
    rcode = JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
	//
    rcode =SQLExecDirect(StatementHandle,
                        (SQLWCHAR *) lpDS->szComandoSQL,
                        (SQLINTEGER) sizeof(lpDS->szComandoSQL));
	//
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

   return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  FechaConexao
 *
 *        Description:  Fecha Conexao ODBC
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC2        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI FechaConexao (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC2 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
    SQLHENV   EnvironmentHandle    = (_J('\0')); 
    SQLHSTMT  StatementHandle      = (_J('\0'));
    SQLHDBC   ConnectionHandle;
    HUSER hUser;
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC2 )	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//Remove os Handle da Memoria
	StatementHandle =(SQLHSTMT *) jdeRemoveDataPtr(hUser,lpDS->idStatementHandle);
	ConnectionHandle =(SQLHSTMT *) jdeRemoveDataPtr(hUser,lpDS->idConnectionHandle);
	EnvironmentHandle =(SQLHSTMT *) jdeRemoveDataPtr(hUser,lpDS->idEnvironmentHandle);
	//Disconecto do ODBC
    SQLFreeHandle(SQL_HANDLE_STMT,StatementHandle);
    SQLDisconnect(ConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_DBC,ConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_ENV,EnvironmentHandle);
	//Volto Zero em tudo
	lpDS->idStatementHandle=0;
	lpDS->idConnectionHandle=0;
	lpDS->idEnvironmentHandle=0;
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

   return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  FechaCursor
 *
 *        Description:  Fecha Cursor ODBC
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC8        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI FechaCursor (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC8 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
    SQLHSTMT   StatementHandle      = _J('\0');
    HUSER      hUser;
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
        (lpVoid    == (LPVOID)    NULL) ||
        (lpDS      == (LPDSD57ODBC8)	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }

   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
    //
    SQLCloseCursor(StatementHandle);
   /************************************************************************
    * Function Clean Up
    ************************************************************************/
    return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  Fetch
 *
 *        Description:  Executa Fetch
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC6        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI Fetch (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC6 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
    SQLHSTMT  StatementHandle      = (_J('\0'));
    HUSER     hUser;
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC6 )	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
	//
    lpDS->cCodigoErro = (JCHAR)SQLFetch(StatementHandle);  
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

   return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  RetornaData
 *
 *        Description:  Retorna Coluna Do Tipo Data
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC3        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI RetornaData (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC3 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
	int                  rcode                = 0;
    SQLHSTMT             StatementHandle      = (_J('\0'));
    SQL_TIMESTAMP_STRUCT jdValorData          = {0};	
    HUSER                hUser;
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC3)	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
    //
	rcode = SQLGetData(StatementHandle, 
		               (unsigned short)lpDS->nColuna,
					   SQL_C_TYPE_TIMESTAMP, 
					   &jdValorData, 
					   0, 
					   NULL);
	//
	lpDS->jdValorData.nYear = jdValorData.year;
	lpDS->jdValorData.nMonth = jdValorData.month;
	lpDS->jdValorData.nDay = jdValorData.day;
	//
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

   return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  RetornaMSG
 *
 *        Description:  Retorna Mensagem de Erro
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC7        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI RetornaMSG (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC7 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
    SQLHSTMT   StatementHandle      = _J('\0');
    HUSER      hUser;
	//
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
 	SQLSMALLINT   MsgLen;
    //
   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC7)	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
    //
    SQLGetDiagRec(SQL_HANDLE_STMT, 
	              StatementHandle, 
	 		      1, 
	 		      (SQLWCHAR *)SqlState, 
			      &NativeError, 
			      (SQLWCHAR *)Msg, 
			      sizeof(Msg), 
				  &MsgLen);
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

    return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  RetornaNumerico
 *
 *        Description:  Retorna Coluna Do Tipo Numerico
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC4        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI RetornaNumerico (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC4 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
	int        rcode                = 0;
    SQLHSTMT   StatementHandle      = (_J('\0'));
    SQLDOUBLE  dValorNumerico      = 0;
    HUSER      hUser;

   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC4)	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
    //
	rcode = SQLGetData(StatementHandle, 
		               (unsigned short)lpDS->nColuna, 
					   SQL_C_DOUBLE, 
					   &dValorNumerico, 
					   0, 
					   NULL);
	//
	DoubleToMathNumeric (dValorNumerico,&lpDS->mnValorNumerico);	
	//
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

   return (ER_SUCCESS);
}


/**************************************************************************
 *  Business Function:  RetornaString
 *
 *        Description:  Retorna Coluna Do Tipo String
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC5        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI RetornaString (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC5 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
	int        rcode                = 0;
    SQLHSTMT   StatementHandle      = _J('\0');
	SQLCHAR    szValorString[1000];
    HUSER      hUser;

   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC5)	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
    //
	rcode = SQLGetData(StatementHandle, 
		               (unsigned short)lpDS->nColuna, 
					   SQL_C_CHAR, 
					   &szValorString, 
					   1000, 
					   NULL);
    //
	jdeStrcpy (lpDS->szValorString, (const JCHAR *) szValorString);
   /************************************************************************
    * Function Clean Up
    ************************************************************************/

    return (ER_SUCCESS);
}

/**************************************************************************
 *  Business Function:  SetaParametro
 *
 *        Description:  Seta Parametro SQL
 *
 *         Parameters:
 *           LPBHVRCOM           lpBhvrCom    Business Function Communications
 *           LPVOID              lpVoid       Void Parameter - DO NOT USE!
 *           LPDSD57ODBC9        lpDS         Parameter Data Structure Pointer  
 *
 *************************************************************************/

JDEBFRTN (ID) JDEBFWINAPI SetaParametro (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC9 lpDS)  
{
   /************************************************************************
    *  Variable declarations
    ************************************************************************/
	int        rcode                = 0;
    SQLHSTMT   StatementHandle      = _J('\0');
	SQLCHAR    szValorString[1000];
    HUSER      hUser;

   /************************************************************************
    * Check for NULL pointers
    ************************************************************************/
    if ((lpBhvrCom == (LPBHVRCOM) NULL) ||
       (lpVoid    == (LPVOID)    NULL) ||
       (lpDS      == (LPDSD57ODBC9)	NULL))
    {
      jdeErrorSet (lpBhvrCom, lpVoid, (ID) 0, _J("4363"), (LPVOID) NULL);
      return ER_ERROR;
    }
   /************************************************************************
    * Main Processing
    ************************************************************************/
    JDB_InitBhvr(lpBhvrCom, &hUser, NULL, JDEDB_COMMIT_AUTO);
	//
	StatementHandle =(SQLHSTMT *) jdeRetrieveDataPtr(hUser, lpDS->idStatementHandle);
    //
	/*SQLBindParameter(hstmt1, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 10, 0,
                  SalesPerson, sizeof(SalesPerson), &SalesPersonLenOrInd);
	SQLBindParameter(hstmt1, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 10, 0,
                 &CustID, 0, &CustIDInd);*/
    //
	jdeStrcpy (lpDS->szValorString, (const JCHAR *) szValorString);
	//
   /************************************************************************
    * Function Clean Up
    ************************************************************************/
	//
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

void I57ODBC_LogErros (SQLHANDLE      hHandle,     
                             SQLSMALLINT    hType,   
                             RETCODE        RetCode) 
{ 
    SQLSMALLINT iRec = 0; 
    SQLINTEGER  iError; 
    SQLWCHAR     szMessage[3000]; 
    SQLWCHAR     szState[SQL_SQLSTATE_SIZE+1]; 
 	SQLSMALLINT   MsgLen;
 
 
    if (RetCode == SQL_INVALID_HANDLE) 
    { 
		jdeWriteLogEntry(JDELOG,_J("B57ODBC"), __FILE__, __LINE__, 0, _J("Invalid handle!"));
        return; 
    } 
 
    while (SQLGetDiagRec(hType, 
                         hHandle, 
                         ++iRec, 
                         szState, 
                         &iError, 
                         szMessage, 
                         (SQLSMALLINT)(sizeof(szMessage) / sizeof(SQLCHAR)), 
                         &MsgLen) == SQL_SUCCESS) 
    { 
		jdeWriteLogEntry(JDELOG,_J("B57ODBC"), __FILE__, __LINE__, 0, szMessage);
    } 
 
} 


