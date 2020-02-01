#include "mainapp.hpp"
#include "../mainframe/mainframe.hpp"

wxIMPLEMENT_APP(ExtfixGUI);

bool ExtfixGUI::OnInit() {
	MainFrame* frame = new MainFrame();
	frame->Show(true);

	return true;
}