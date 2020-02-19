
/*****************************************************************************
 *    Header File:  B57ODBC.h
 *
 *    Description:  Função para Trabalhar com ODBC Header File
 *
 *        History:
 *          Date        Programmer  SAR# - Description
 *          ----------  ----------  -------------------------------------------
 *   Author 20/01/11                Unknown  - Created  
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

#ifndef __B57ODBC_H
#define __B57ODBC_H

/*****************************************************************************
 * Table Header Inclusions
 ****************************************************************************/

/*****************************************************************************
 * External Business Function Header Inclusions
 ****************************************************************************/

/*****************************************************************************
 * Global Definitions
 ****************************************************************************/

/*****************************************************************************
 * Structure Definitions
 ****************************************************************************/

/*****************************************************************************
 * DS Template Type Definitions
 ****************************************************************************/
/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC
 *    Template ID:   D57ODBC
 *    Generated:     Thu Jan 20 14:01:10 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC
#define DATASTRUCTURE_D57ODBC

typedef struct tagDSD57ODBC
{
  ID                idStatementHandle;                   
  ID                idConnectionHandle;                  
  ID                idEnvironmentHandle;                 
  JCHAR              szStringConexao[501];                
  JCHAR              cCodigoErro;                         
} DSD57ODBC, *LPDSD57ODBC;

#define IDERRidStatementHandle_1                  1L
#define IDERRidConnectionHandle_2                 2L
#define IDERRidEnvironmentHandle_3                3L
#define IDERRszStringConexao_5                    5L
#define IDERRcCodigoErro_6                        6L

#endif

/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Executa SQL
 *    Template ID:   D57ODBC1
 *    Generated:     Mon Aug 06 09:54:07 2012
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC1
#define DATASTRUCTURE_D57ODBC1

typedef struct tagDSD57ODBC1
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
  JCHAR              szComandoSQL[30001];                 
} DSD57ODBC1, *LPDSD57ODBC1;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L
#define IDERRszComandoSQL_9                       9L

#endif

/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Fecha Conexao
 *    Template ID:   D57ODBC2
 *    Generated:     Thu Jan 20 14:01:38 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC2
#define DATASTRUCTURE_D57ODBC2

typedef struct tagDSD57ODBC2
{
  ID                idStatementHandle;                   
  ID                idConnectionHandle;                  
  ID                idEnvironmentHandle;                 
  JCHAR              cCodigoErro;                         
} DSD57ODBC2, *LPDSD57ODBC2;

#define IDERRidStatementHandle_1                  1L
#define IDERRidConnectionHandle_2                 2L
#define IDERRidEnvironmentHandle_3                3L
#define IDERRcCodigoErro_6                        6L

#endif

/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Retorna Data
 *    Template ID:   D57ODBC3
 *    Generated:     Thu Jan 20 14:13:15 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC3
#define DATASTRUCTURE_D57ODBC3

typedef struct tagDSD57ODBC3
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
  int               nColuna;                             
  JDEDATE           jdValorData;                         
} DSD57ODBC3, *LPDSD57ODBC3;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L
#define IDERRnColuna_8                            8L
#define IDERRjdValorData_10                       10L

#endif


/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Retorna Numerico
 *    Template ID:   D57ODBC4
 *    Generated:     Thu Jan 20 14:01:55 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC4
#define DATASTRUCTURE_D57ODBC4

typedef struct tagDSD57ODBC4
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
  int               nColuna;                             
  MATH_NUMERIC      mnValorNumerico;                     
} DSD57ODBC4, *LPDSD57ODBC4;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L
#define IDERRnColuna_8                            8L
#define IDERRmnValorNumerico_10                   10L

#endif

/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Retorna String
 *    Template ID:   D57ODBC5
 *    Generated:     Thu Jan 20 14:14:19 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC5
#define DATASTRUCTURE_D57ODBC5

typedef struct tagDSD57ODBC5
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
  int               nColuna;                             
  JCHAR              szValorString[201];                  
} DSD57ODBC5, *LPDSD57ODBC5;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L
#define IDERRnColuna_8                            8L
#define IDERRszValorString_11                     11L

#endif


/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Fech
 *    Template ID:   D57ODBC6
 *    Generated:     Thu Jan 20 14:02:20 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC6
#define DATASTRUCTURE_D57ODBC6

typedef struct tagDSD57ODBC6
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
} DSD57ODBC6, *LPDSD57ODBC6;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L

#endif

/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Retorna Mensagem
 *    Template ID:   D57ODBC7
 *    Generated:     Thu Jan 20 14:02:28 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC7
#define DATASTRUCTURE_D57ODBC7

typedef struct tagDSD57ODBC7
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
  JCHAR              szMensagemErro[501];                 
} DSD57ODBC7, *LPDSD57ODBC7;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L
#define IDERRszMensagemErro_7                     7L

#endif

/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Fecha Cursor
 *    Template ID:   D57ODBC8
 *    Generated:     Thu Jan 20 14:02:36 2011
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC8
#define DATASTRUCTURE_D57ODBC8

typedef struct tagDSD57ODBC8
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
} DSD57ODBC8, *LPDSD57ODBC8;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L

#endif

/*****************************************
 * TYPEDEF for Data Structure
 *    Template Name: Estrutura de dados para função B57ODBC - Seta Parametro
 *    Template ID:   D57ODBC9
 *    Generated:     Wed Jul 18 15:19:16 2012
 *
 * DO NOT EDIT THE FOLLOWING TYPEDEF
 *    To make modifications, use the OneWorld Data Structure
 *    Tool to Generate a revised version, and paste from
 *    the clipboard.
 *
 **************************************/

#ifndef DATASTRUCTURE_D57ODBC9
#define DATASTRUCTURE_D57ODBC9

typedef struct tagDSD57ODBC9
{
  ID                idStatementHandle;                   
  JCHAR              cCodigoErro;                         
  JCHAR              szValorString[201];                  
  JCHAR              szTipoParametro[12];                 
  JCHAR              szParametro[49];                     
} DSD57ODBC9, *LPDSD57ODBC9;

#define IDERRidStatementHandle_1                  1L
#define IDERRcCodigoErro_6                        6L
#define IDERRszValorString_11                     11L
#define IDERRszTipoParametro_12                   12L
#define IDERRszParametro_13                       13L

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
JDEBFRTN (ID) JDEBFWINAPI ConectaBanco                     (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC lpDS);
JDEBFRTN (ID) JDEBFWINAPI ExecutaSQL                       (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC1 lpDS);
JDEBFRTN (ID) JDEBFWINAPI FechaConexao                     (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC2 lpDS);
JDEBFRTN (ID) JDEBFWINAPI FechaCursor                      (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC8 lpDS);
JDEBFRTN (ID) JDEBFWINAPI Fetch                            (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC6 lpDS);
JDEBFRTN (ID) JDEBFWINAPI RetornaData                      (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC3 lpDS);
JDEBFRTN (ID) JDEBFWINAPI RetornaMSG                       (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC7 lpDS);
JDEBFRTN (ID) JDEBFWINAPI RetornaNumerico                  (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC4 lpDS);
JDEBFRTN (ID) JDEBFWINAPI RetornaString                    (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC5 lpDS);
JDEBFRTN (ID) JDEBFWINAPI SetaParametro                    (LPBHVRCOM lpBhvrCom, LPVOID lpVoid, LPDSD57ODBC9 lpDS);

/*****************************************************************************
 * Internal Function Prototypes
 ****************************************************************************/
void I57ODBC_LogErros (SQLHANDLE      hHandle, SQLSMALLINT    hType, RETCODE        RetCode); 

#endif    /* __B57ODBC_H */

