#include "mainapp/mainapp.hpp"
#include "mainapp/mainframe.hpp"

wxIMPLEMENT_APP(ExtfixGUI);

bool ExtfixGUI::OnInit() {
	MainFrame* frame = new MainFrame();
	frame->Show(true);

	return true;
}