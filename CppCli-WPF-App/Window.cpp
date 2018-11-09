///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Demo for CSE 687 Project #4                                //
// Jingyi Yang-CSE687 - Object Oriented Design, Spring 2017              //
// Application: OOD Project #4                                           //
// Platform:    Visual Studio 2015, Dell XPS 13, Windows 10              //
///////////////////////////////////////////////////////////////////////////

#include "Window.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo(){
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pClient_ = pObjFact->createClient();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_, pClient_);
	pClient_->startClient();
	pChann_->start();
	pClient_->setSender(pSendr_);
	delete pObjFact;

	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();

	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	hShowItemsButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getItemsFromList);
	hCategory->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DisplayCategory);
	hDisCategory->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DisplayFiles);
	hUploadItemButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadFile);
	hDownload->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadFile);
	hNoparent->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::NoParent);
	hDisplayhtml->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DisplayHTML);
	hAnalysis->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DoAnlysiser);
    hDelete->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::Deletes);
	hDemo->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::StartDemo);

	this->Title = "WPF C++/CLI Demo";
	this->Width = 800;
	this->Height = 450;

	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);

	setUpTabControl();
	setUpSendMessageView();
	setUpFileListView();
	setUpRepoView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}

void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSendMessageTab->Header = "Demo";
	hFileListTab->Header = "File List";
	hRepoTab->Header = "Remote Repository";
	hTabControl->Items->Add(hSendMessageTab);
	hTabControl->Items->Add(hFileListTab);
	hTabControl->Items->Add(hRepoTab);
}

void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 16;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->Children->Add(hScrollViewer1);
}

void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hSendButton->Content = "Send Message";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClearButton->Content = "Clear";
	hBorder2->Child = hSendButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSendMessageGrid->SetRow(hStackPanel1, 1);
	hSendMessageGrid->Children->Add(hStackPanel1);
}

void WPFCppCliDemo::setUpSendMessageView()
{
	Console::Write("\n  setting up sendMessage view");
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hRepoGrid->RowDefinitions->Add(hRow1Def);

	hDemo->Content = "Click to Start Demo";
	hDemo->Height = 30;
	hDemo->Width = 150;
	hDemo->BorderThickness = Thickness(2);
	hDemo->BorderBrush = Brushes::Black;
	hSendMessageGrid->SetRow(hDemo, 0);
	hSendMessageGrid->Children->Add(hDemo);
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
	pSendr_->postMessage(toStdString(msgText));
	Console::Write("\n  sent message");
	hStatus->Text = "Sent message";
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
	for each(String^ item in msg->Split(','))
	{
		hRepoBox->Items->Add(item);
	}
}

void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		std::string msg = pRecvr_->getMessage();
		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;

		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	}
}

void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock1->Text = "";
}


void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}

	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
}



void WPFCppCliDemo::setUpFileListView()
{
	Console::Write("\n  setting up FileList view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(80);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hListBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->SetColumn(hBorder1, 0);
	hFileListGrid->SetColumnSpan(hBorder1, 2);
	hRepoGrid->SetRowSpan(hBorder1, 4);
	hFileListGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(80);
	RowDefinition^ hRow2Def2 = gcnew RowDefinition();
	hRow2Def2->Height = GridLength(20);
	RowDefinition^ hRow2Def3 = gcnew RowDefinition();
	hRow2Def3->Height = GridLength(60);
	RowDefinition^ hRow2Def4 = gcnew RowDefinition();
	hRow2Def4->Height = GridLength(60);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFileListGrid->RowDefinitions->Add(hRow2Def2);
	hFileListGrid->RowDefinitions->Add(hRow2Def3);
	hFileListGrid->RowDefinitions->Add(hRow2Def4);

	ColumnDefinition^ hCol1Def = gcnew ColumnDefinition();
	hCol1Def->Width = GridLength(300);
	ColumnDefinition^ hCol2Def = gcnew ColumnDefinition();
	ColumnDefinition^ hCol3Def = gcnew ColumnDefinition();

	hFileListGrid->ColumnDefinitions->Add(hCol1Def);
	hFileListGrid->ColumnDefinitions->Add(hCol2Def);
	hFileListGrid->ColumnDefinitions->Add(hCol3Def);

	hFolderBrowseButton->Content = "Browse";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 150;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 0);
	hFileListGrid->SetColumn(hFolderBrowseButton, 2);
	hFileListGrid->Children->Add(hFolderBrowseButton);

	part1();
}

void WPFCppCliDemo::part1() {
	// Show selected items button.
	hShowItemsButton->Content = "Selected Items";
	hShowItemsButton->Height = 30;
	hShowItemsButton->Width = 150;
	hShowItemsButton->BorderThickness = Thickness(2);
	hShowItemsButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hShowItemsButton, 1);
	hFileListGrid->SetColumn(hShowItemsButton, 2);
	hFileListGrid->Children->Add(hShowItemsButton);

	//category label
	CategoryLabel->Content = "Input Category:";
	CategoryLabel->Height = 30;
	CategoryLabel->Width = 150;
	hFileListGrid->SetRow(CategoryLabel, 2);
	hFileListGrid->SetColumn(CategoryLabel, 2);
	/*hFileListGrid->SetColumnSpan(CategoryLabel, 2);*/
	hFileListGrid->Children->Add(CategoryLabel);

	//category box
	Category->Height = 25;
	Category->Width = 150;
	hFileListGrid->SetRow(Category, 3);
	hFileListGrid->SetColumn(Category, 2);
	hFileListGrid->Children->Add(Category);

	// Upload selected items button.
	hUploadItemButton->Content = "Upload";
	hUploadItemButton->Height = 30;
	hUploadItemButton->Width = 150;
	hUploadItemButton->BorderThickness = Thickness(2);
	hUploadItemButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hUploadItemButton, 4);
	hFileListGrid->SetColumn(hUploadItemButton, 2);
	hFileListGrid->Children->Add(hUploadItemButton);

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();

}

void WPFCppCliDemo::setUpRepoView(){
	Console::Write("\n  setting up RepoList view");
	hRepoGrid->Margin = Thickness(20);
	hRepoTab->Content = hRepoGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hRepoGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = hRepoBox;
	hRepoGrid->SetRow(hBorder1, 0);
	hRepoGrid->SetColumn(hBorder1, 0);
	hRepoGrid->SetColumnSpan(hBorder1, 2);
	hRepoGrid->SetRowSpan(hBorder1, 6);
	hRepoGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(50);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(50);
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(50);
	RowDefinition^ hRow5Def = gcnew RowDefinition();
	hRow5Def->Height = GridLength(50);
	RowDefinition^ hRow6Def = gcnew RowDefinition();
	hRow6Def->Height = GridLength(50);
	RowDefinition^ hRow7Def = gcnew RowDefinition();
	hRow7Def->Height = GridLength(50);
	hRepoGrid->RowDefinitions->Add(hRow2Def);
	hRepoGrid->RowDefinitions->Add(hRow3Def);
	hRepoGrid->RowDefinitions->Add(hRow4Def);
	hRepoGrid->RowDefinitions->Add(hRow5Def);
	hRepoGrid->RowDefinitions->Add(hRow6Def);
	hRepoGrid->RowDefinitions->Add(hRow7Def);
	part2();
	part3();
}

void WPFCppCliDemo::part2() {

	ColumnDefinition^ hCol1Def = gcnew ColumnDefinition();
	hCol1Def->Width = GridLength(300);
	ColumnDefinition^ hCol2Def = gcnew ColumnDefinition();
	ColumnDefinition^ hCol3Def = gcnew ColumnDefinition();

	hRepoGrid->ColumnDefinitions->Add(hCol1Def);
	hRepoGrid->ColumnDefinitions->Add(hCol2Def);
	hRepoGrid->ColumnDefinitions->Add(hCol3Def);

	hCategory->Content = "Display Categories";
	hCategory->Height = 30;
	hCategory->Width = 150;
	hCategory->BorderThickness = Thickness(2);
	hCategory->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hCategory, 0);
	hRepoGrid->SetColumn(hCategory, 2);
	hRepoGrid->Children->Add(hCategory);

	hDisCategory->Content = "Display files in Category";
	hDisCategory->Height = 30;
	hDisCategory->Width = 150;
	hDisCategory->BorderThickness = Thickness(2);
	hDisCategory->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hDisCategory, 1);
	hRepoGrid->SetColumn(hDisCategory, 2);
	hRepoGrid->Children->Add(hDisCategory);

	hDisplayhtml->Content = "Display Web Page";
	hDisplayhtml->Height = 30;
	hDisplayhtml->Width = 150;
	hDisplayhtml->BorderThickness = Thickness(2);
	hDisplayhtml->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hDisplayhtml, 2);
	hRepoGrid->SetColumn(hDisplayhtml, 2);
	hRepoGrid->Children->Add(hDisplayhtml);
}

void WPFCppCliDemo::part3() {
	hAnalysis->Content = "Analysiser";
	hAnalysis->Height = 30;
	hAnalysis->Width = 150;
	hAnalysis->BorderThickness = Thickness(2);
	hAnalysis->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hAnalysis, 3);
	hRepoGrid->SetColumn(hAnalysis, 2);
	hRepoGrid->Children->Add(hAnalysis);

	hDownload->Content = "Download Web Page";
	hDownload->Height = 30;
	hDownload->Width = 150;
	hDownload->BorderThickness = Thickness(2);
	hDownload->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hDownload, 4);
	hRepoGrid->SetColumn(hDownload, 2);
	hRepoGrid->Children->Add(hDownload);

	hNoparent->Content = "Display File No Parent";
	hNoparent->Height = 30;
	hNoparent->Width = 150;
	hNoparent->BorderThickness = Thickness(2);
	hNoparent->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hNoparent, 5);
	hRepoGrid->SetColumn(hNoparent, 2);
	hRepoGrid->Children->Add(hNoparent);

	hDelete->Content = "Delete";
	hDelete->Height = 30;
	hDelete->Width = 150;
	hDelete->BorderThickness = Thickness(2);
	hDelete->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hDelete, 6);
	hRepoGrid->SetColumn(hDelete, 2);
	hRepoGrid->Children->Add(hDelete);
}

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browse ";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  Opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
}


void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}
void WPFCppCliDemo::DisplayCategory(Object^ sender, RoutedEventArgs^ args) {
	hRepoBox->SelectionMode = SelectionMode::Single;
	pClient_->showCategories();
	hRepoBox->Items->Clear();
	statu = 0;
}
void WPFCppCliDemo::DisplayFiles(Object^ sender, RoutedEventArgs^ args) {
	if (hRepoBox->SelectedItem == nullptr)
		return;
	Cate = hRepoBox->SelectedItem->ToString();
	pClient_->showfiles(toStdString(hRepoBox->SelectedItem->ToString()));
	hRepoBox->Items->Clear();
	statu = 1;
}

void WPFCppCliDemo::DownloadFile(Object^ sender, RoutedEventArgs^ args) {
	if (hRepoBox->SelectedItem == nullptr)
		return;
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	for each(String^ item in hRepoBox->SelectedItems)
	{
		pClient_->downLoad(toStdString(item));
	}
}
void WPFCppCliDemo::NoParent(Object^ sender, RoutedEventArgs^ args) {
	if (hRepoBox->SelectedItem == nullptr)
		return;
	pClient_->NoParent(toStdString(hRepoBox->SelectedItem->ToString()));
	hRepoBox->Items->Clear();
	statu = 2;
}
void WPFCppCliDemo::DisplayHTML(Object^ sender, RoutedEventArgs^ args) {
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	pClient_->displayHTML();
	hRepoBox->Items->Clear();
	statu = 3;
}
void WPFCppCliDemo::UploadFile(Object^ sender, RoutedEventArgs^ args) {
	std::string cata = toStdString(Category->Text->ToString());
	for each(String^ item in hListBox->SelectedItems)
	{
		pClient_->sendfile(toStdString(item), cata);
	}
}

void WPFCppCliDemo::DoAnlysiser(Object^ sender, RoutedEventArgs^ args) {
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	pClient_->todoAnalysiser();
	hRepoBox->Items->Clear();
}

void WPFCppCliDemo::Deletes(Object^ sender, RoutedEventArgs^ args) {
	if (hRepoBox->SelectedItem == nullptr)
		return;
	switch (statu)
	{
	case(0):
		pClient_->deletefile("none", toStdString(hRepoBox->SelectedItem->ToString()));
		hRepoBox->Items->Clear();
		pClient_->showCategories();
		break;
	case(1):
		pClient_->deletefile(toStdString(hRepoBox->SelectedItem->ToString()), toStdString(Cate));
		hRepoBox->Items->Clear();
		pClient_->showfiles(toStdString(Cate));
		break;
	case(2):
		for each (String^ item in hRepoBox->SelectedItems) {
			pClient_->deletefile(toStdString(item), "none");
		}
		hRepoBox->Items->Clear();
		pClient_->NoParent(toStdString(Cate));
		break;
	case(3):
		for each (String^ item in hRepoBox->SelectedItems) {
			pClient_->deletefile(toStdString(item), "none");
		}
		hRepoBox->Items->Clear();
		pClient_->displayHTML();
		break;
	default:
		return;
	}
}

void WPFCppCliDemo::StartDemo(Object^ sender, RoutedEventArgs^ args) {
	pClient_->startdemo();
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}