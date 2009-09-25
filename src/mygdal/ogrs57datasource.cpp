/******************************************************************************
 * $Id: ogrs57datasource.cpp,v 1.6 2009/09/25 15:22:05 bdbcat Exp $
 *
 * Project:  S-57 Translator
 * Purpose:  Implements OGRS57DataSource class
 * Author:   Frank Warmerdam, warmerda@home.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log: ogrs57datasource.cpp,v $
 * Revision 1.6  2009/09/25 15:22:05  bdbcat
 * Improve SENC creation progress dialog
 *
 * Revision 1.5  2009/03/30 19:08:43  bdbcat
 * Opencpn 1.3.0 Update
 *
 * Revision 1.4  2008/08/27 22:51:38  bdbcat
 * Add error returns to ENC update logic
 *
 * Revision 1.3  2008/04/10 01:10:54  bdbcat
 * Disallow option setup from environment
 *
 * Revision 1.2  2008/03/30 23:04:34  bdbcat
 * Add OpenMin method
 *
 * Revision 1.1.1.1  2006/08/21 05:52:20  dsr
 * Initial import as opencpn, GNU Automake compliant.
 *
 * Revision 1.1.1.1  2006/04/19 03:23:29  dsr
 * Rename/Import to OpenCPN
 *
 * Revision 1.25  2003/11/15 21:50:52  warmerda
 * Added limited creation support
 *
 * Revision 1.24  2003/11/12 21:24:12  warmerda
 * updates to new featuredefn generators
 *
 * Revision 1.23  2003/09/15 20:52:12  warmerda
 * added RETURN_LINKAGES
 *
 * Revision 1.22  2003/09/12 21:06:31  warmerda
 * use SetWellKnownGeogCS() to establish WGS84 SRS
 *
 * Revision 1.21  2003/09/05 19:12:05  warmerda
 * added RETURN_PRIMITIVES support to get low level prims
 *
 * Revision 1.20  2002/09/09 18:38:58  warmerda
 * Initialize module list.
 *
 * Revision 1.19  2002/09/09 18:20:58  warmerda
 * fixed serious leak ... S57Readers never destroyed!
 *
 * Revision 1.18  2002/05/14 21:33:30  warmerda
 * use macros for options, pass PRESERVE_EMPTY_NUMBERS opt
 *
 * Revision 1.17  2002/03/05 14:25:43  warmerda
 * expanded tabs
 *
 * Revision 1.16  2001/12/19 22:44:53  warmerda
 * added ADD_SOUNDG_DEPTH support
 *
 * Revision 1.15  2001/12/19 22:03:58  warmerda
 * iniitialize bExtentsSet
 *
 * Revision 1.14  2001/12/17 22:34:43  warmerda
 * ensure SPLIT_MULTIPOINT option is passed to S57Reader
 *
 * Revision 1.13  2001/12/14 19:40:18  warmerda
 * added optimized feature counting, and extents collection
 *
 * Revision 1.12  2001/11/21 14:35:44  warmerda
 * dont managed updates directly anymore
 *
 * Revision 1.11  2001/08/30 21:05:32  warmerda
 * added support for generic object if not recognised
 *
 * Revision 1.10  2001/08/30 03:48:43  warmerda
 * preliminary implementation of S57 Update Support
 *
 * Revision 1.9  2001/07/18 04:55:16  warmerda
 * added CPL_CSVID
 *
 * Revision 1.8  2000/06/16 18:10:05  warmerda
 * expanded tabs
 *
 * Revision 1.7  2000/06/07 20:50:58  warmerda
 * make CSV location configurable with env variable
 *
 * Revision 1.6  1999/11/29 14:04:53  warmerda
 * use LNAM_REFS
 *
 * Revision 1.5  1999/11/26 16:18:33  warmerda
 * OGRFeatureDefn generators no longer static
 *
 * Revision 1.4  1999/11/18 19:01:25  warmerda
 * expanded tabs
 *
 * Revision 1.3  1999/11/16 21:47:31  warmerda
 * updated class occurance collection
 *
 * Revision 1.2  1999/11/08 22:23:00  warmerda
 * added object class support
 *
 * Revision 1.1  1999/11/03 22:12:43  warmerda
 * New
 *
 */

#include "ogr_s57.h"
#include "cpl_conv.h"
#include "cpl_string.h"

CPL_CVSID("$Id: ogrs57datasource.cpp,v 1.6 2009/09/25 15:22:05 bdbcat Exp $");

S57ClassRegistrar *OGRS57DataSource::poRegistrar = NULL;

/************************************************************************/
/*                          OGRS57DataSource()                          */
/************************************************************************/

OGRS57DataSource::OGRS57DataSource()

{
    nLayers = 0;
    papoLayers = NULL;

    nModules = 0;
    papoModules = NULL;
    poWriter = NULL;

    pszName = NULL;

//    poSpatialRef = new OGRSpatialReference();
//    poSpatialRef->SetWellKnownGeogCS( "WGS84" );

    bExtentsSet = FALSE;


/* -------------------------------------------------------------------- */
/*      Allow initialization of options from the environment.           */
/* -------------------------------------------------------------------- */
    papszOptions = NULL;

    /*
    if( CPLGetConfigOption("OGR_S57_OPTIONS",NULL) != NULL )
    {
        papszOptions =
            CSLTokenizeStringComplex( CPLGetConfigOption("OGR_S57_OPTIONS",""),
                                      ",", FALSE, FALSE );
    }
    */
}

/************************************************************************/
/*                         ~OGRS57DataSource()                          */
/************************************************************************/

OGRS57DataSource::~OGRS57DataSource()

{
    int         i;

    for( i = 0; i < nLayers; i++ )
        delete papoLayers[i];

    CPLFree( papoLayers );

    for( i = 0; i < nModules; i++ )
    {
          if(papoModules[i])
                papoModules[i]->Close();
          delete papoModules[i];
    }
    CPLFree( papoModules );

    CPLFree( pszName );

    CSLDestroy( papszOptions );

//    delete poSpatialRef;

/*
    if( poWriter != NULL )
    {
        poWriter->Close();
        delete poWriter;
    }
*/
}

/************************************************************************/
/*                           SetOptionList()                            */
/************************************************************************/

void OGRS57DataSource::SetOptionList( char ** papszNewOptions )

{
    CSLDestroy( papszOptions );
    papszOptions = CSLDuplicate( papszNewOptions );
}

/************************************************************************/
/*                             GetOption()                              */
/************************************************************************/

const char *OGRS57DataSource::GetOption( const char * pszOption )

{
    return CSLFetchNameValue( papszOptions, pszOption );
}

/************************************************************************/
/*                           TestCapability()                           */
/************************************************************************/

int OGRS57DataSource::TestCapability( const char * )

{
    return FALSE;
}

/************************************************************************/
/*                                Open()                                */
/************************************************************************/
int OGRS57DataSource::Open( const char * pszFilename, int bTestOpen, CallBackFunction pcallback )

{

    int         iModule;
    int         error_return = 0;

    pszName = CPLStrdup( pszFilename );

/* -------------------------------------------------------------------- */
/*      Check a few bits of the header to see if it looks like an       */
/*      S57 file (really, if it looks like an ISO8211 file).            */
/* -------------------------------------------------------------------- */
    if( bTestOpen )
    {
        FILE    *fp;
        char    pachLeader[10];

        fp = VSIFOpen( pszFilename, "rb" );
        if( fp == NULL )
            return BAD_FILE;

        if( VSIFRead( pachLeader, 1, 10, fp ) != 10
            || (pachLeader[5] != '1' && pachLeader[5] != '2'
                && pachLeader[5] != '3' )
            || pachLeader[6] != 'L'
            || (pachLeader[8] != '1' && pachLeader[8] != ' ') )
        {
            VSIFClose( fp );
            return BAD_HEADER;
        }

        VSIFClose( fp );
    }

/* -------------------------------------------------------------------- */
/*      Setup reader options.                                           */
/* -------------------------------------------------------------------- */
    char **papszReaderOptions = NULL;
    S57Reader   *poModule;

    poModule = new S57Reader( pszFilename );

    papszReaderOptions = CSLSetNameValue(papszReaderOptions,
                                         S57O_LNAM_REFS, "ON" );
    if( GetOption(S57O_UPDATES) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_UPDATES,
                             GetOption(S57O_UPDATES));

    if( GetOption(S57O_SPLIT_MULTIPOINT) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_SPLIT_MULTIPOINT,
                             GetOption(S57O_SPLIT_MULTIPOINT) );

    if( GetOption(S57O_ADD_SOUNDG_DEPTH) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_ADD_SOUNDG_DEPTH,
                             GetOption(S57O_ADD_SOUNDG_DEPTH));

    if( GetOption(S57O_PRESERVE_EMPTY_NUMBERS) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_PRESERVE_EMPTY_NUMBERS,
                             GetOption(S57O_PRESERVE_EMPTY_NUMBERS) );

    if( GetOption(S57O_RETURN_PRIMITIVES) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_RETURN_PRIMITIVES,
                             GetOption(S57O_RETURN_PRIMITIVES) );

    if( GetOption(S57O_RETURN_LINKAGES) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_RETURN_LINKAGES,
                             GetOption(S57O_RETURN_LINKAGES) );

    poModule->SetOptions( papszReaderOptions );
    CSLDestroy( papszReaderOptions );

/* -------------------------------------------------------------------- */
/*      Try opening.                                                    */
/*                                                                      */
/*      Eventually this should check for catalogs, and if found         */
/*      instantiate a whole series of modules.                          */
/* -------------------------------------------------------------------- */
    if( !poModule->Open( bTestOpen ) )
    {
        delete poModule;

        return BAD_OPEN;
    }

    nModules = 1;
    papoModules = (S57Reader **) CPLMalloc(sizeof(void*));
    papoModules[0] = poModule;

/* -------------------------------------------------------------------- */
/*      Instantiate the class registrar if possible.                    */
/* -------------------------------------------------------------------- */
    if( poRegistrar == NULL )
    {
        poRegistrar = new S57ClassRegistrar();

        if( !poRegistrar->LoadInfo( NULL, FALSE ) )
        {
            delete poRegistrar;
            poRegistrar = NULL;
        }
    }

/* -------------------------------------------------------------------- */
/*      Add the primitive layers if they are called for.                */
/* -------------------------------------------------------------------- */

    if( GetOption( S57O_RETURN_PRIMITIVES ) != NULL )
    {
        OGRFeatureDefn  *poDefn;

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VI, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VC, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VE, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VF, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );
    }

/* -------------------------------------------------------------------- */
/*      Initialize a layer for each type of geometry.  Eventually       */
/*      we will do this by object class.                                */
/* -------------------------------------------------------------------- */
 /*
    if( poRegistrar == NULL )
    {
        OGRFeatureDefn  *poDefn;

        poDefn = S57GenerateGeomFeatureDefn( wkbPoint,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateGeomFeatureDefn( wkbLineString,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateGeomFeatureDefn( wkbPolygon,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateGeomFeatureDefn( wkbNone,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );
    }
 */
/* -------------------------------------------------------------------- */
/*      Initialize a feature definition for each class that actually    */
/*      occurs in the dataset.                                          */
/* -------------------------------------------------------------------- */
//    else
    {
        OGRFeatureDefn  *poDefn;
        int             *panClassCount;
        int             iClass, bGeneric = FALSE;

        for( iModule = 0; iModule < nModules; iModule++ )
        {
            papoModules[iModule]->SetClassBased( poRegistrar );
        }


        for( iModule = 0; iModule < nModules; iModule++ )
        {
              int ingest_error = papoModules[iModule]->Ingest( pcallback );
              if(ingest_error)
                    error_return = ingest_error;
        }

        panClassCount = (int *) CPLCalloc(sizeof(int),MAX_CLASSES);

        for( iModule = 0; iModule < nModules; iModule++ )
            papoModules[iModule]->CollectClassList(panClassCount,MAX_CLASSES);

        for( iClass = 0; iClass < MAX_CLASSES; iClass++ )
        {
              if(pcallback)
              {
                    if (!(*pcallback)())
                    {
                          break;;
                    }
              }

            if( panClassCount[iClass] > 0 )
            {
                poDefn =
                    S57GenerateObjectClassDefn( poRegistrar, iClass,
                                                poModule->GetOptionFlags() );


                if( poDefn != NULL )
                    AddLayer( new OGRS57Layer( this, poDefn,
                                               panClassCount[iClass] ) );
                else
                {
                    bGeneric = TRUE;
                    CPLDebug( "S57",
                              "Unable to find definition for OBJL=%d\n",
                              iClass );
                }

            }
        }
/*
        if( bGeneric )
        {
            poDefn = S57GenerateGeomFeatureDefn( wkbUnknown,
                                                 poModule->GetOptionFlags() );
            AddLayer( new OGRS57Layer( this, poDefn ) );
        }
  */
        CPLFree( panClassCount );
    }

/* -------------------------------------------------------------------- */
/*      Attach the layer definitions to each of the readers.            */
/* -------------------------------------------------------------------- */
    for( iModule = 0; iModule < nModules; iModule++ )
    {
        for( int iLayer = 0; iLayer < nLayers; iLayer++ )
        {
            papoModules[iModule]->AddFeatureDefn(
                papoLayers[iLayer]->GetLayerDefn() );
        }
    }

    return error_return;
}

/************************************************************************/
/*                                OpenMin()                             */
/************************************************************************/

int OGRS57DataSource::OpenMin( const char * pszFilename, int bTestOpen )

{
    pszName = CPLStrdup( pszFilename );

/* -------------------------------------------------------------------- */
/*      Check a few bits of the header to see if it looks like an       */
/*      S57 file (really, if it looks like an ISO8211 file).            */
/* -------------------------------------------------------------------- */
    if( bTestOpen )
    {
        FILE    *fp;
        char    pachLeader[10];

        fp = VSIFOpen( pszFilename, "rb" );
        if( fp == NULL )
            return FALSE;

        if( VSIFRead( pachLeader, 1, 10, fp ) != 10
            || (pachLeader[5] != '1' && pachLeader[5] != '2'
                && pachLeader[5] != '3' )
            || pachLeader[6] != 'L'
            || (pachLeader[8] != '1' && pachLeader[8] != ' ') )
        {
            VSIFClose( fp );
            return FALSE;
        }

        VSIFClose( fp );
    }

/* -------------------------------------------------------------------- */
/*      Setup reader options.                                           */
/* -------------------------------------------------------------------- */
    char **papszReaderOptions = NULL;
    S57Reader   *poModule;

    poModule = new S57Reader( pszFilename );

    papszReaderOptions = CSLSetNameValue(papszReaderOptions,
                                         S57O_LNAM_REFS, "ON" );
    if( GetOption(S57O_UPDATES) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_UPDATES,
                             GetOption(S57O_UPDATES));

    if( GetOption(S57O_SPLIT_MULTIPOINT) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_SPLIT_MULTIPOINT,
                             GetOption(S57O_SPLIT_MULTIPOINT) );

    if( GetOption(S57O_ADD_SOUNDG_DEPTH) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_ADD_SOUNDG_DEPTH,
                             GetOption(S57O_ADD_SOUNDG_DEPTH));

    if( GetOption(S57O_PRESERVE_EMPTY_NUMBERS) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_PRESERVE_EMPTY_NUMBERS,
                             GetOption(S57O_PRESERVE_EMPTY_NUMBERS) );

    if( GetOption(S57O_RETURN_PRIMITIVES) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_RETURN_PRIMITIVES,
                             GetOption(S57O_RETURN_PRIMITIVES) );

    if( GetOption(S57O_RETURN_LINKAGES) != NULL )
        papszReaderOptions =
            CSLSetNameValue( papszReaderOptions, S57O_RETURN_LINKAGES,
                             GetOption(S57O_RETURN_LINKAGES) );

    poModule->SetOptions( papszReaderOptions );
    CSLDestroy( papszReaderOptions );

/* -------------------------------------------------------------------- */
/*      Try opening.                                                    */
/*                                                                      */
/*      Eventually this should check for catalogs, and if found         */
/*      instantiate a whole series of modules.                          */
/* -------------------------------------------------------------------- */
    if( !poModule->Open( bTestOpen ) )
    {
        delete poModule;

        return FALSE;
    }

    nModules = 1;
    papoModules = (S57Reader **) CPLMalloc(sizeof(void*));
    papoModules[0] = poModule;

/* -------------------------------------------------------------------- */
/*      Instantiate the class registrar if possible.                    */
/* -------------------------------------------------------------------- */
/*
    if( poRegistrar == NULL )
    {
        poRegistrar = new S57ClassRegistrar();

        if( !poRegistrar->LoadInfo( NULL, FALSE ) )
        {
            delete poRegistrar;
            poRegistrar = NULL;
        }
    }
*/
/* -------------------------------------------------------------------- */
/*      Add the primitive layers if they are called for.                */
/* -------------------------------------------------------------------- */
/*
    if( GetOption( S57O_RETURN_PRIMITIVES ) != NULL )
    {
        OGRFeatureDefn  *poDefn;

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VI, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VC, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VE, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateVectorPrimitiveFeatureDefn( RCNM_VF, poModule->GetOptionFlags());
        AddLayer( new OGRS57Layer( this, poDefn ) );
    }
 */
/* -------------------------------------------------------------------- */
/*      Initialize a layer for each type of geometry.  Eventually       */
/*      we will do this by object class.                                */
/* -------------------------------------------------------------------- */
 /*
    if( poRegistrar == NULL )
    {
        OGRFeatureDefn  *poDefn;

        poDefn = S57GenerateGeomFeatureDefn( wkbPoint,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateGeomFeatureDefn( wkbLineString,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateGeomFeatureDefn( wkbPolygon,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );

        poDefn = S57GenerateGeomFeatureDefn( wkbNone,
                                             poModule->GetOptionFlags() );
        AddLayer( new OGRS57Layer( this, poDefn ) );
    }
 */
/* -------------------------------------------------------------------- */
/*      Initialize a feature definition for each class that actually    */
/*      occurs in the dataset.                                          */
/* -------------------------------------------------------------------- */
//    else
/*
    {
        OGRFeatureDefn  *poDefn;
        int             *panClassCount;
        int             iClass, bGeneric = FALSE;

        for( iModule = 0; iModule < nModules; iModule++ )
        {
            papoModules[iModule]->SetClassBased( poRegistrar );
        }

        panClassCount = (int *) CPLCalloc(sizeof(int),MAX_CLASSES);

        for( iModule = 0; iModule < nModules; iModule++ )
            papoModules[iModule]->CollectClassList(panClassCount,MAX_CLASSES);

        for( iClass = 0; iClass < MAX_CLASSES; iClass++ )
        {
            if( panClassCount[iClass] > 0 )
            {
                poDefn =
                    S57GenerateObjectClassDefn( poRegistrar, iClass,
                                                poModule->GetOptionFlags() );


                if( poDefn != NULL )
                    AddLayer( new OGRS57Layer( this, poDefn,
                                               panClassCount[iClass] ) );
                else
                {
                    bGeneric = TRUE;
                    CPLDebug( "S57",
                              "Unable to find definition for OBJL=%d\n",
                              iClass );
                }

            }
        }

        if( bGeneric )
        {
            poDefn = S57GenerateGeomFeatureDefn( wkbUnknown,
                                                 poModule->GetOptionFlags() );
            AddLayer( new OGRS57Layer( this, poDefn ) );
        }

        CPLFree( panClassCount );
    }
*/
/* -------------------------------------------------------------------- */
/*      Attach the layer definitions to each of the readers.            */
/* -------------------------------------------------------------------- */
/*
    for( iModule = 0; iModule < nModules; iModule++ )
    {
        for( int iLayer = 0; iLayer < nLayers; iLayer++ )
        {
            papoModules[iModule]->AddFeatureDefn(
                papoLayers[iLayer]->GetLayerDefn() );
        }
    }
*/
    return TRUE;
}

/************************************************************************/
/*                              GetLayer()                              */
/************************************************************************/

OGRLayer *OGRS57DataSource::GetLayer( int iLayer )

{
    if( iLayer < 0 || iLayer >= nLayers )
        return NULL;
    else
        return papoLayers[iLayer];
}

/************************************************************************/
/*                              AddLayer()                              */
/************************************************************************/

void OGRS57DataSource::AddLayer( OGRS57Layer * poNewLayer )

{
    papoLayers = (OGRS57Layer **)
        CPLRealloc( papoLayers, sizeof(void*) * ++nLayers );

    papoLayers[nLayers-1] = poNewLayer;
}

/************************************************************************/
/*                             GetModule()                              */
/************************************************************************/

S57Reader * OGRS57DataSource::GetModule( int i )

{
    if( i < 0 || i >= nModules )
        return NULL;
    else
        return papoModules[i];
}

/************************************************************************/
/*                            GetDSExtent()                             */
/************************************************************************/

OGRErr OGRS57DataSource::GetDSExtent( OGREnvelope *psExtent, int bForce )

{
/* -------------------------------------------------------------------- */
/*      If we have it, return it immediately.                           */
/* -------------------------------------------------------------------- */
    if( bExtentsSet )
    {
        *psExtent = oExtents;
        return OGRERR_NONE;
    }

    if( nModules == 0 )
        return OGRERR_FAILURE;

/* -------------------------------------------------------------------- */
/*      Otherwise try asking each of the readers for it.                */
/* -------------------------------------------------------------------- */
    for( int iModule=0; iModule < nModules; iModule++ )
    {
        OGREnvelope     oModuleEnvelope;
        OGRErr          eErr;

        eErr = papoModules[iModule]->GetExtent( &oModuleEnvelope, bForce );
        if( eErr != OGRERR_NONE )
            return eErr;

        if( iModule == 0 )
            oExtents = oModuleEnvelope;
        else
        {
            oExtents.MinX = MIN(oExtents.MinX,oModuleEnvelope.MinX);
            oExtents.MaxX = MAX(oExtents.MaxX,oModuleEnvelope.MaxX);
            oExtents.MinY = MIN(oExtents.MinY,oModuleEnvelope.MinY);
            oExtents.MaxX = MAX(oExtents.MaxY,oModuleEnvelope.MaxY);
        }
    }

    *psExtent = oExtents;
    bExtentsSet = TRUE;

    return OGRERR_NONE;
}

