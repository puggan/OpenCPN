/******************************************************************************
 * $Id: cm93.h,v 1.3 2009/03/27 01:02:22 bdbcat Exp $
 *
 * Project:  OpenCPN
 * Purpose:  CM93 Chart Object
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) $YEAR$ by $AUTHOR$   *
 *   $EMAIL$   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *
 * $Log: cm93.h,v $
 * Revision 1.3  2009/03/27 01:02:22  bdbcat
 * No pragma pack()
 *
 * Revision 1.2  2009/03/26 22:53:00  bdbcat
 * Opencpn 1.3.0 Update
 *

 */

#ifndef __CM93CHART_H__
#define __CM93CHART_H__

#include    "s57chart.h"

//    Some constants
#define     INDEX_m_sor       217                // cm93 dictionary index for object type _m_sor


WX_DEFINE_ARRAY_INT(int, ArrayOfInts);


class M_COVR_Desc
{
   public:
      int         m_nvertices;
      double      *pvertices;
      int         m_npub_year;
};

WX_DECLARE_OBJARRAY(M_COVR_Desc, Array_Of_M_COVR_Desc);

//    Georeferencing constants

      //    This constant was developed empirically by looking at a
      //    representative cell, comparing the cm93 point transform coefficients
      //    to the stated lat/lon bounding box.
      //    This value corresponds to the semi-major axis for the "International 1924" geo-standard
      //    For WGS84, it should be 6378137.0......

static const double CM93_semimajor_axis_meters        = 6378388.0;           // CM93 semimajor axis
static const double WGS84_semimajor_axis_meters       = 6378137.0;           // WGS84 semimajor axis
static const double mercator_k0                       = 0.9996;

//    CM93 Data structures

//#pragma pack(push,1)

typedef struct{
      unsigned short    x;
      unsigned short    y;
}cm93_point;


typedef struct{
      unsigned short    x;
      unsigned short    y;
      unsigned short    z;
}cm93_point_3d;

//#pragma pack(pop)


//    This is the 128 byte cm93 cell header, found at offset 0x0a in the cell file
typedef struct{
      double                  lon_min;
      double                  lat_min;
      double                  lon_max;
      double                  lat_max;
                                          // Bounding Box, in Mercator transformed co-ordinates
      double                  easting_min;
      double                  northing_min;
      double                  easting_max;
      double                  northing_max;

      unsigned short          usn_vector_records;        //  number of spacial(vector) records
      int                     n_vector_record_points;    //  number of cm93 points in vector record block
      int                     m_46;
      int                     m_4a;
      unsigned short          usn_point3d_records;
      int                     m_50;
      int                     m_54;
      unsigned short          usn_point2d_records;             //m_58;
      unsigned short          m_5a;
      unsigned short          m_5c;
      unsigned short          usn_feature_records;            //m_5e, number of feature records

      int                     m_60;
      int                     m_64;
      unsigned short          m_68;
      unsigned short          m_6a;
      unsigned short          m_6c;
      int                     m_nrelated_object_pointers;

      int                     m_72;
      unsigned short          m_76;

      int                     m_78;
      int                     m_7c;

}header_struct;

typedef struct{
      unsigned short          n_points;
      unsigned short          x_min;
      unsigned short          y_min;
      unsigned short          x_max;
      unsigned short          y_max;
      int                     index;
      cm93_point              *p_points;
}geometry_descriptor;

typedef struct{
      geometry_descriptor     *pGeom_Description;
      unsigned char           segment_usage;
}vector_record_descriptor;



typedef struct{
      unsigned char     otype;
      unsigned char     geotype;
      unsigned short    n_geom_elements;
      void              *pGeometry;       // may be a (cm93_point*) or other geom;
      unsigned char     n_related_objects;
      void              *p_related_object_pointer_array;
      unsigned char     n_attributes;     // number of attributes
      unsigned char     *attributes_block;      // encoded attributes

}Object;


typedef struct{

      //          Georeferencing transform coefficients
      double                        transform_x_rate;
      double                        transform_y_rate;
      double                        transform_x_origin;
      double                        transform_y_origin;


      cm93_point                    *p2dpoint_array;
      Object                        **pprelated_object_block;
      unsigned char                 *attribute_block_top;               // attributes block
      geometry_descriptor           *edge_vector_descriptor_block;      // edge vector descriptor block
      geometry_descriptor           *point3d_descriptor_block;
      cm93_point                    *pvector_record_block_top;
      cm93_point_3d                 *p3dpoint_array;

      int                           m_nvector_records;
      int                           m_nfeature_records;
      int                           m_n_point3d_records;
      int                           m_n_point2d_records;

      //    Allocated working blocks
      vector_record_descriptor      *object_vector_record_descriptor_block;
      Object                        *pobject_block;

}Cell_Info_Block;





typedef struct{
      OGRGeometry       *pogrGeom;
      int               n_vector_indices;
      int               *pvector_index;
      int               n_contours;                          // parameters passed to trapezoid tesselator
      int               *contour_array;
      int               n_max_vertex;
      int               pointx;
      int               pointy;
      wxPoint2DDouble   *vertex_array;
      int               xmin, xmax, ymin, ymax;
}Extended_Geometry;

//----------------------------------------------------------------------------
// cm93_dictionary class
//    Encapsulating the conversion between binary cm_93 object class, attributes, etc
//    to standard S57 text conventions
//----------------------------------------------------------------------------

class cm93_dictionary
{
      public:

            cm93_dictionary();
            ~cm93_dictionary();

            bool LoadDictionary(wxString dictionary_dir);

            bool IsOk(void){ return m_ok; }

            wxArrayString     *m_S57ClassArray;
            int               *m_GeomTypeArray;
            wxArrayString     *m_AttrArray;
            char              *m_ValTypeArray;

            bool              m_ok;
};


class cm93_attr_block
{
      public:
            cm93_attr_block(void * block, cm93_dictionary *pdict);
            unsigned char *GetNextAttr();

            int m_cptr;
            unsigned char *m_block;

            cm93_dictionary *m_pDict;
};


//----------------------------------------------------------------------------
// cm93 Chart Manager class
//----------------------------------------------------------------------------
class cm93manager
{
public:
    cm93manager();
    ~cm93manager();
    bool Loadcm93Dictionary(wxString name);
    cm93_dictionary *FindAndLoadDict(const wxString &file);


    cm93_dictionary   *m_pcm93Dict;

    //  Member variables used to record the calling of cm93chart::CreateHeaderDataFromCM93Cell()
    //  for each available scale value.  This allows that routine to return quickly with no error
    //  for all cells other than the first, at each scale....

    bool    m_bfoundA;
    bool    m_bfoundB;
    bool    m_bfoundC;
    bool    m_bfoundD;
    bool    m_bfoundE;
    bool    m_bfoundF;
    bool    m_bfoundG;
    bool    m_bfoundZ;


};

//----------------------------------------------------------------------------
// cm93 Chart object class
//----------------------------------------------------------------------------
class cm93chart : public s57chart
{
      public:
            cm93chart();
            ~cm93chart();

            cm93chart(int scale_index);
            InitReturn Init( const wxString& name, ChartInitFlag flags, ColorScheme cs );

            wxString GetName();

            double GetNormalScaleMin(double canvas_scale_factor);
            double GetNormalScaleMax(double canvas_scale_factor);

            void SetVPParms(ViewPort *vpt);
            void GetPointPix(ObjRazRules *rzRules, float northing, float easting, wxPoint *r);
            void GetPointPix(ObjRazRules *rzRules, wxPoint2DDouble *en, wxPoint *r, int nPoints);

            void SetCM93Dict(cm93_dictionary *pDict){m_pDict = pDict;}
            void SetCM93Prefix(wxString &prefix){m_prefix = prefix;}

            Array_Of_M_COVR_Desc    m_covr_array;

      private:
            InitReturn CreateHeaderDataFromCM93Cell(void);
            int read_header_and_populate_cib(header_struct *ph, Cell_Info_Block *pCIB);
            Extended_Geometry *BuildGeom(Object *pobject, wxFileOutputStream *postream, int iobject);

            S57Obj *CreateS57Obj( int iobject, Object *pobject, cm93_dictionary *pDict, Extended_Geometry *xgeom,
                                             double ref_lat, double ref_lon, double scale,
                                             bool bDeleteGeomInline);

            void translate_colmar(wxString &sclass, S57attVal *pattValTmp);

            int loadcell(int);
            int CreateObjChain(void);

            void Unload_CM93_Cell(void);

            //    cm93 point manipulation methods
            void Transform(cm93_point *s, double *lat, double *lon);


            Cell_Info_Block   m_CIB;


            cm93_dictionary   *m_pDict;
            wxString          m_prefix;

            double            m_sfactor;

            wxString          m_scalechar;
            ArrayOfInts       m_cells_loaded_array;

            bool              m_bref_is_set;

            int               m_current_cell_vearray_offset;
            int               *m_pcontour_array;
            int               m_ncontour_alloc;

};

//----------------------------------------------------------------------------
// cm93 Composite Chart object class
//----------------------------------------------------------------------------
class cm93compchart : public s57chart
{
      public:
            cm93compchart();
            ~cm93compchart();


            InitReturn Init( const wxString& name, ChartInitFlag flags, ColorScheme cs );

            double GetNormalScaleMin(double canvas_scale_factor);
            double GetNormalScaleMax(double canvas_scale_factor);
            wxString GetFullPath();
            wxString GetName();
            wxString GetPubDate();

            void SetVPParms(ViewPort *vpt);

            ThumbData *GetThumbData(int tnx, int tny, float lat, float lon);
            ThumbData *GetThumbData() {return (ThumbData *)NULL;}

            bool AdjustVP(ViewPort &vp_last, ViewPort &vp_proposed);
            bool IsRenderDelta(ViewPort &vp_last, ViewPort &vp_proposed);

            bool RenderViewOnDC(wxMemoryDC& dc, ViewPort& VPoint, ScaleTypeEnum scale_type);

            void GetPointPix(ObjRazRules *rzRules, float rlat, float rlon, wxPoint *r);
            void GetPixPoint(int pixx, int pixy, double *plat, double *plon, ViewPort *vpt);
            void GetPointPix(ObjRazRules *rzRules, wxPoint2DDouble *en, wxPoint *r, int nPoints);

            void InvalidateCache();

            ListOfS57Obj *GetObjListAtLatLon(float lat, float lon, float select_radius, ViewPort *VPoint);

            int Get_nve_elements(void);
            int Get_nvc_elements(void);

            VE_Element  **Get_pve_array(void);
            VC_Element  **Get_pvc_array(void);

            void UpdateLUPs(s57chart *pOwner);

      private:
            InitReturn CreateHeaderData();
            cm93_dictionary *FindAndLoadDict(const wxString &file);


            //    Data members
            cm93_dictionary   *m_pDict;

            cm93chart         *m_pcm93chart_array[8];
            cm93chart         *m_pcm93chart_current;
            int               m_cmscale;

            wxString          m_prefix;

            int               m_current_cell_pub_date;      // the (integer) publish date of the cell at the current VP
};


#endif