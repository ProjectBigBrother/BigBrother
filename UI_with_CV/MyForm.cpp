#include "stdafx.h"
#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;


[STAThread]
void main(array<System::String^>^ args){
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	UI_with_CV::MyForm form;
	Application::Run(%form);
}


