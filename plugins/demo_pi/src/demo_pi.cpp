/******************************************************************************
 * $Id: demo_pi.cpp,v 1.8 2010/06/21 01:54:37 bdbcat Exp $
 *
 * Project:  OpenCPN
 * Purpose:  DEMO Plugin
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
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
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers


#include "demo_pi.h"

#ifndef DECL_EXP
#ifdef __WXMSW__
#  define DECL_EXP     __declspec(dllexport)
#else
#  define DECL_EXP
#endif
#endif


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new demo_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}





//---------------------------------------------------------------------------------------------------------
//
//    Demo PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int demo_pi::Init(void)
{
//      printf("demo_pi Init()\n");

      m_pdemo_window = NULL;

      // Get a pointer to the opencpn display canvas, to use as a parent for windows created
      m_parent_window = GetOCPNCanvasWindow();

      // Create the Context Menu Items

      //    In order to avoid an ASSERT on msw debug builds,
      //    we need to create a dummy menu to act as a surrogate parent of the created MenuItems
      //    The Items will be re-parented when added to the real context meenu
      wxMenu dummy_menu;

      wxMenuItem *pmi = new wxMenuItem(&dummy_menu, -1, _("Show PlugIn DemoWindow"));
      m_show_id = AddCanvasContextMenuItem(pmi, this );
      SetCanvasContextMenuItemViz(m_show_id, true);

      wxMenuItem *pmih = new wxMenuItem(&dummy_menu, -1, _("Hide PlugIn DemoWindow"));
      m_hide_id = AddCanvasContextMenuItem(pmih, this );
      SetCanvasContextMenuItemViz(m_hide_id, false);

      return (
           INSTALLS_CONTEXTMENU_ITEMS     |
           WANTS_NMEA_SENTENCES
            );
}

bool demo_pi::DeInit(void)
{
//      printf("demo_pi DeInit()\n");
      if(m_pdemo_window)
      {
            m_pdemo_window->Close();
            m_pdemo_window->Destroy();
      }
      
      return true;
}

int demo_pi::GetAPIVersionMajor()
{
      return API_VERSION_MAJOR;
}

int demo_pi::GetAPIVersionMinor()
{
      return API_VERSION_MINOR;
}

int demo_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int demo_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}


wxString demo_pi::GetShortDescription()
{
      return _("Demo PlugIn for OpenCPN");
}

wxString demo_pi::GetLongDescription()
{
      return _("Demo PlugIn for OpenCPN\n\
Demonstrates PlugIn processing of NMEA messages.");

}

void demo_pi::SetNMEASentence(wxString &sentence)
{
//      printf("demo_pi::SetNMEASentence\n");
      if(m_pdemo_window)
      {
            m_pdemo_window->SetSentence(sentence);
      }
}


void demo_pi::OnContextMenuItemCallback(int id)
{
      wxLogMessage(_T("demo_pi OnContextMenuCallBack()"));
     ::wxBell();


      if(NULL == m_pdemo_window)
      {
            m_pdemo_window = new DemoWindow(m_parent_window, wxID_ANY);


            SetCanvasContextMenuItemViz(m_hide_id, true);
            SetCanvasContextMenuItemViz(m_show_id, false);
      }
      else
      {
            m_pdemo_window->Close();
            m_pdemo_window->Destroy();
            m_pdemo_window = NULL;

            SetCanvasContextMenuItemViz(m_hide_id, false);
            SetCanvasContextMenuItemViz(m_show_id, true);
      }      

}


//----------------------------------------------------------------
//
//    Demo Window Implementation
//
//----------------------------------------------------------------

BEGIN_EVENT_TABLE(DemoWindow, wxWindow)
  EVT_PAINT ( DemoWindow::OnPaint )
END_EVENT_TABLE()

DemoWindow::DemoWindow(wxWindow *pparent, wxWindowID id)
      :wxWindow(pparent, id, wxPoint(10,10), wxSize(200,200),
             wxSIMPLE_BORDER, _T("OpenCPN PlugIn"))
{
      mLat = 0.0;
      mLon = 1.0;
      mSog = 2.0;
      mCog = 3.0;
      mVar = 4.0;

}

DemoWindow::~DemoWindow()
{
}

void DemoWindow::SetSentence(wxString &sentence)
{
      m_NMEA0183 << sentence;

      bool bGoodData = false;

      if(m_NMEA0183.PreParse())
      {
            if(m_NMEA0183.LastSentenceIDReceived == _T("RMC"))
            {
                  if(m_NMEA0183.Parse())
                  {
                              if(m_NMEA0183.Rmc.IsDataValid == NTrue)
                              {
                                    float llt = m_NMEA0183.Rmc.Position.Latitude.Latitude;
                                    int lat_deg_int = (int)(llt / 100);
                                    float lat_deg = lat_deg_int;
                                    float lat_min = llt - (lat_deg * 100);
                                    mLat = lat_deg + (lat_min/60.);
                                    if(m_NMEA0183.Rmc.Position.Latitude.Northing == South)
                                          mLat = -mLat;

                                    float lln = m_NMEA0183.Rmc.Position.Longitude.Longitude;
                                    int lon_deg_int = (int)(lln / 100);
                                    float lon_deg = lon_deg_int;
                                    float lon_min = lln - (lon_deg * 100);
                                    mLon = lon_deg + (lon_min/60.);
                                    if(m_NMEA0183.Rmc.Position.Longitude.Easting == West)
                                          mLon = -mLon;

                                    mSog = m_NMEA0183.Rmc.SpeedOverGroundKnots;
                                    mCog = m_NMEA0183.Rmc.TrackMadeGoodDegreesTrue;

                                    if(m_NMEA0183.Rmc.MagneticVariationDirection == East)
                                          mVar =  m_NMEA0183.Rmc.MagneticVariation;
                                    else if(m_NMEA0183.Rmc.MagneticVariationDirection == West)
                                          mVar = -m_NMEA0183.Rmc.MagneticVariation;

                                    bGoodData = true;

                              }
                        }
                  }
        }

      //    Got the data, now do something with it

      if(bGoodData)
      {
            Refresh(false);
      }
}

void DemoWindow::OnPaint(wxPaintEvent& event)
{
      wxLogMessage(_T("demo_pi onpaint"));

      wxPaintDC dc ( this );

//      printf("onpaint\n");

      {
            dc.Clear();

            wxString data;
            data.Printf(_T("Lat: %g "), mLat);
            dc.DrawText(data, 10, 10);

            data.Printf(_T("Lon: %g"), mLon);
            dc.DrawText(data, 10, 40);

            data.Printf(_T("Sog: %g"), mSog);
            dc.DrawText(data, 10, 70);

            data.Printf(_T("Cog: %g"), mCog);
            dc.DrawText(data, 10, 100);
      }
}