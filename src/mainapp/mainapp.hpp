#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../mainframe/mainframe.hpp"

class ExtfixGUI : public wxApp
{
public:
	virtual bool OnInit();
};