/******************************************************************************
 * $Id: sfiiddef.h,v 1.1 2006/08/21 05:52:20 dsr Exp $
 *
 * Project:  OpenGIS Simple Features Reference Implementation
 * Purpose:  Various classid objects for ``well known'' SF COM classes.
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
 * $Log: sfiiddef.h,v $
 * Revision 1.1  2006/08/21 05:52:20  dsr
 * Initial revision
 *
 * Revision 1.1.1.1  2006/04/19 03:23:28  dsr
 * Rename/Import to OpenCPN
 *
 * Revision 1.1  1999/05/17 14:40:10  warmerda
 * New
 *
 */

#ifndef _SFIIDID_H_INCLUDED
#define _SFIIDID_H_INCLUDED

const IID IID_ISpatialReferenceFactory = {0x620600B1,0xFEA1,0x11d0,{0xB0,0x4B,0x00,0x80,0xC7,0xF7,0x94,0x81}};

const IID IID_IGeometry = 
 {0x6A124031,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_IGeometryFactory = 
 {0x6A124033,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_IPoint = 
 {0x6A124035,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_ICurve = 
 {0x6A124036,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_ILinearRing = 
 {0x6A124038,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_ILineString = 
 {0x6A124037,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_ISurface = 
 {0x6A124039,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_IGeometryCollection = 
 {0x6A12403A,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_IPolygon = 
 {0x6A12403C,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_IMultiCurve = 
 {0x6A12403D,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_IMultiSurface = 
 {0x6A12403F,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_ISpatialRelationship = 
 {0x6A124040,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_ISpatialRelationship2 = 
 {0x6A124041,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_ISpatialOperator = 
 {0x6A124042,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

const IID IID_IWks = 
 {0x6A124032,0xFE38,0x11d0,{0xBE,0xCE,0x00,0x80,0x5F,0x7C,0x42,0x68}};

#endif
