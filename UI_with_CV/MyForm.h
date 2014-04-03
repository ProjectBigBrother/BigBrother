#using <mscorlib.dll>

#pragma once


using namespace std;
using namespace cv;
using namespace msclr::interop;


const cv::String imgdir = "./";

/** Function Headers */
std::vector<Rect> detectFaces(Mat frame);
void resizeFace(Mat &face);
void addToDataSet(Mat &data, vector<string> &labels, Mat &newData, vector<string> &newLabels); //combines two sets of samples for xml files
void automaticLearning(Mat frame, cv::String labelN, int i);
void guidedLearning();
void scanMode();
void detect();
void CameraController();
cv::Point ReturnLocation();
void Search();
void Move(int movePan, int moveTilt);
void Track(int coordX, int coordY);
void MoveRight(int move);
void MoveDown(int move);
void MoveUp(int move);
void MoveLeft(int move);
int isKnown(int label, Mat currentFace);
void DO(const char* comm);
void MoveToPos(int x, int y);
void PanToPos(int x);
void TiltToPos(int y);
void showFace_withColor(bool Known, System::Drawing::Graphics^ g, System::Drawing::Rectangle face, int prediction);


/* Variables for new recognition*/
cv::String face_cascade_name = "c:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml"; //path to haarcascade on comp may need to change comp to comp
string fn_csv = "C:\\Users\\Michelle\\Documents\\Visual Studio 2013\\Projects\\THE_db\\db.txt"; //location of csv file 
string num_faces_file = "C:\\Users\\Michelle\\Documents\\Visual Studio 2013\\Projects\\THE_db\\numFaces.txt"; //location of num_faces file
string newFolder = "C:\\Users\\Michelle\\Documents\\Visual Studio 2013\\Projects\\THE_db\\s"; //location to store new faces
string currentFolder; //for updated database
vector<Mat> images; 
vector<int> labels; //class labels for reading from CSV
vector<int> Glabels; //gender labels for reading from CSV
vector<string> Nlabels; //name labels for reading from CSV
vector<Rect> prevFaces;
CascadeClassifier face_cascade; //this is the classifier itself, initialized later
int im_width;
int im_height;
Ptr<FaceRecognizer> model;
Ptr<FaceRecognizer> gender_model;
int guided_NUM = 0;
bool guided_finished = 0;
char newNum[20];
vector<std::string> updateAT; // stores lines to add to at.txt file
vector<Mat> newFaces; // stores the faces for guided or unguided learning
int probability = 90;
bool unguided_start = 0;
int posX, posY;
int DoSearch;
int PanMAX = 60;
int TiltMAX = 25;
int imageHeight = 480;
int imageWidth = 640;
int currentHeight = 341;
int currentWidth = 353;
int tickCount = 0;
bool track = 0; //keeps track of whether or not tracking mode has been started 
bool searching = 0; // keeps track of whether or not search mode has been started
bool flag = 0;
bool MUTE = 0;
Mat face; //holds the current face


static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, vector<int>& Glabels, vector<string>& Nlabels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel, genderLabel, nameLabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel, separator);
		getline(liness, genderLabel, separator);
		getline(liness, nameLabel);

		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
			Glabels.push_back(atoi(genderLabel.c_str()));
			Nlabels.push_back(nameLabel);
		}
	}
}

static Mat norm_0_255(InputArray _src) {
	Mat src = _src.getMat();
	// Create and return normalized image:
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

namespace UI_with_CV {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	private:
		CvCapture* capture;

	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Timer^  timer2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::ComponentModel::IContainer^  components;
	protected:



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 3;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				87.46518F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.53482F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				223)));
			this->tableLayoutPanel1->Controls->Add(this->comboBox1, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->comboBox2, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox1, 2, 2);
			this->tableLayoutPanel1->Controls->Add(this->button1, 2, 3);
			this->tableLayoutPanel1->Controls->Add(this->label1, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label3, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->listBox1, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->button2, 2, 5);
			this->tableLayoutPanel1->Controls->Add(this->checkBox1, 1, 5);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 6;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(1020, 712);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// comboBox1
			// 
			this->comboBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(7) {
				L"Detect (Default)", L"Scan", L"Search", L"Tracking",
					L"Gender Recognition", L"Guided Learning", L"Unguided Learning"
			});
			this->comboBox1->Location = System::Drawing::Point(799, 3);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(218, 21);
			this->comboBox1->TabIndex = 1;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged_1);
			// 
			// comboBox2
			// 
			this->comboBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(799, 33);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(218, 21);
			this->comboBox2->TabIndex = 2;
			this->comboBox2->Visible = false;
			// 
			// textBox1
			// 
			this->textBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox1->Location = System::Drawing::Point(799, 63);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(218, 20);
			this->textBox1->TabIndex = 4;
			this->textBox1->Visible = false;
			// 
			// button1
			// 
			this->button1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->button1->Location = System::Drawing::Point(799, 93);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(218, 24);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Take Picture";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Visible = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Dock = System::Windows::Forms::DockStyle::Right;
			this->label1->Location = System::Drawing::Point(714, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(79, 30);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Select a Mode:";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Dock = System::Windows::Forms::DockStyle::Right;
			this->label3->Location = System::Drawing::Point(714, 30);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(79, 30);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Select Subject:";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->label3->Visible = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Dock = System::Windows::Forms::DockStyle::Right;
			this->label2->Location = System::Drawing::Point(704, 60);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(89, 30);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Name of Subject:";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->label2->Visible = false;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Location = System::Drawing::Point(3, 3);
			this->pictureBox1->Name = L"pictureBox1";
			this->tableLayoutPanel1->SetRowSpan(this->pictureBox1, 6);
			this->pictureBox1->Size = System::Drawing::Size(691, 706);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 8;
			this->pictureBox1->TabStop = false;
			// 
			// listBox1
			// 
			this->tableLayoutPanel1->SetColumnSpan(this->listBox1, 2);
			this->listBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listBox1->ForeColor = System::Drawing::Color::Red;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Statistics and error messages", L"will be displayed here" });
			this->listBox1->Location = System::Drawing::Point(700, 123);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(317, 556);
			this->listBox1->TabIndex = 6;
			// 
			// button2
			// 
			this->button2->Dock = System::Windows::Forms::DockStyle::Right;
			this->button2->Location = System::Drawing::Point(887, 685);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(130, 24);
			this->button2->TabIndex = 9;
			this->button2->Text = L"Retrain Database";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->checkBox1->Location = System::Drawing::Point(700, 685);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Padding = System::Windows::Forms::Padding(15, 0, 0, 0);
			this->checkBox1->Size = System::Drawing::Size(93, 24);
			this->checkBox1->TabIndex = 10;
			this->checkBox1->Text = L"Mute";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 40;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1020, 712);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Location = System::Drawing::Point(100, 100);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Big Brother";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}

#pragma endregion

	private: System::Void comboBox1_SelectedIndexChanged_1(System::Object^  sender, System::EventArgs^  e) {
				 searching = 0;
				 track = 0;
				 vector<std::string> names;
				 if (comboBox1->Text == L"Tracking"){
					 button1->Text = "Start";
					 comboBox2->Visible = true;
					 label3->Visible = true;
					 button1->Visible = true;
					 textBox1->Visible = false;
					 label2->Visible = false;
					 label3->Text = "Select Subject";
					 //listBox1->Items->Clear();
					 //fill comboBox2 with names from file
					 comboBox2->Items->Clear();
					 for (int i = 0; i < Nlabels.size(); i += 10)
					 {
						 System::String^ nameLabel = gcnew System::String(Nlabels[i].c_str());
						 comboBox2->Items->Add(nameLabel);
					 }
					 comboBox2->SelectedIndex = 0;


				 }
				 else if (comboBox1->Text == L"Search")
				 {
					 comboBox2->Visible = true;
					 label3->Visible = true;
					 button1->Visible = true;
					 button1->Text = "Start";
					 textBox1->Visible = false;
					 label2->Visible = false;
					 label3->Text = L"Select Subject";
					 //listBox1->Items->Clear();
					 //fill comboBox2 with names from file
					 comboBox2->Items->Clear();
					 for (int i = 0; i < Nlabels.size(); i += 10)
					 {
						 System::String^ nameLabel = gcnew System::String(Nlabels[i].c_str());
						 comboBox2->Items->Add(nameLabel);
					 }
					 comboBox2->SelectedIndex = 0;
				 }
				 else if (comboBox1->Text == L"Detect (Default)")
				 {
					 button1->Visible = false;
					 textBox1->Visible = false;
					 label2->Visible = false;
					 label3->Visible = false;
					 listBox1->Items->Clear();
					 comboBox2->Visible = false;
				 }
				 else if (comboBox1->Text == L"Scan"){
					 button1->Visible = false;
					 textBox1->Visible = false;
					 comboBox2->Visible = false;
					 label2->Visible = false;
					 label3->Visible = false;
					 //listBox1->Items->Clear();

				 }
				 else if (comboBox1->Text == L"Gender Recognition")
				 {
					 button1->Visible = false;
					 textBox1->Visible = false;
					 comboBox2->Visible = false;
					 label2->Visible = false;
					 label3->Visible = false;
				 }
				 else if (comboBox1->Text == L"Guided Learning")
				 {
					 label2->Visible = true;
					 label2->Text = L"Enter Name:";
					 button1->Visible = true;
					 textBox1->Visible = true;
					 listBox1->Visible = true;
					 label3->Visible = true;
					 label3->Text = L"Select Gender";
					 comboBox2->Items->Clear();
					 comboBox2->Visible = true;
					 comboBox2->Items->Add("Male");
					 comboBox2->Items->Add("Female");
					 listBox1->Items->Clear();
					 guidedLearning();
					 
					 
				 }
				 else if (comboBox1->Text == L"Unguided Learning")
				 {
					 label2->Visible = true;
					 label2->Text = L"Enter Name:";
					 button1->Visible = true;
					 textBox1->Visible = true;
					 listBox1->Visible = true;
					 label3->Visible = true;
					 label3->Text = L"Select Gender";
					 comboBox2->Items->Clear();
					 comboBox2->Visible = true;
					 comboBox2->Items->Add("Male");
					 comboBox2->Items->Add("Female");
					 listBox1->Items->Clear();
					 unguidedLearning();
				 }
				 //testBox2->Text = L"MODE: " + System::Convert::ToString(MODE);
	}

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
					 IplImage* nextFrame = cvRetrieveFrame(capture);
					 Bitmap^ display = IplImageToBitmap(nextFrame);
					 pictureBox1->Image = display;
					 tickCount++;
					 if (tickCount == 10)
						 tickCount = 0;

					 //mode select menu
					 if (comboBox1->Text == L"Tracking"){
						 if (track == 1) //run mode if start button has been clicked
							trackingMode();
					 }
					 else if (comboBox1->Text == L"Detect (Default)"){
						 detect();
					 }
					 else if (comboBox1->Text == L"Scan")
					 {
						 scanMode();
					 }
					 else if (comboBox1->Text == L"Search")
					 {
						 if (searching == 1)
							searchMode();
					 }
					 else if (comboBox1->Text == L"Gender Recognition")
					 {
						 genderMode();
					 }
					 else if (comboBox1->Text == L"Guided Learning")
					 {
						 //detect(); // if uncommented, will show box while 
					 }
					 else if (comboBox1->Text == L"Unguided Learning")
					 {
						 if (unguided_start == 1) //set by button click
						 {
							 System::String^ name = textBox1->Text;
							 std::string std_name(marshal_as<std::string>(name));

							 cv::String end = ".pgm";
							 

							 //define gender information
							 char gendString[20];
							 int gender = 66;
							 if (comboBox2->Text == L"Female")
								 gender = 99;
							 itoa(gender, gendString, 10);
							 //run unguided
							 if(guided_NUM <= 10)
							 {
								 IplImage* frame = cvRetrieveFrame(capture);
								 Bitmap^ display = IplImageToBitmap(frame);
								 pictureBox1->Image = display;

								 int dice = rand() % 100;
								 if (dice > probability)
								 {
									 Mat newImage = cvRetrieveFrame(capture);
									 Mat gray_image;
									 cvtColor(newImage, gray_image, CV_BGR2GRAY);
									 std::vector<Rect> faces = detectFaces(newImage);
									 int i = 0;


									 if (faces.size() == 1) //resize face for att database
									 {
										 cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
										 rectangle(newImage, faces[i].tl(), faces[i].br(), Scalar(255, 0, 0));
										 Mat faceROI = gray_image(faces[i]);
										 face = faceROI;
										 Mat resized(112, 112, CV_8UC1);
										 resize(face, resized, resized.size(), 0, 0, INTER_LINEAR);
										 Rect cropped(10, 0, 92, 112);
										 face = resized(cropped);

										 char numString[20];
										 guided_NUM++;
										 int n = guided_NUM;

										 int number = atoi(newNum);
										 number--;
										 char numString2[20];
										 itoa(number, numString2, 10);

										 //save face to database
										 if (guided_NUM <= 10)
										 {
											 newFaces.push_back(face);
											 itoa(n, numString, 10);

											 updateAT.push_back("\n" + currentFolder + "\\" + numString + end + "; " + numString2 + "; " + gendString + ";" + std_name);
											 listBox1->Items->Add(System::Convert::ToString(guided_NUM));
										 }

										 if (guided_NUM == 10)
										 {
											 unguided_start = 0;
											 for (int i = 0; i < 10; i++)
											 {
												 cv::String start = "..//..//THE_db//s";
												 start.append(newNum);
												 start.append("//");
												 itoa(i + 1, numString, 10);
												 start.append(numString);
												 start.append(end);

												 //save image to file
												 try {
													 imwrite(start, newFaces[i]);
												 }
												 catch (runtime_error& ex) {
													 listBox1->Items->Add("error");
												 }

												 //update at.txt
												 ofstream myfile;
												 myfile.open(fn_csv, ofstream::out | ofstream::app);
												 //System::String^ check = gcnew System::String(currentFolder.c_str());
												 //listBox1->Items->Add(check);

												 if (myfile.is_open())
												 {
													 myfile << updateAT[i];
													 myfile.close();
												 }

												 //updated numFaces file
												 myfile.open(num_faces_file);
												 if (myfile.is_open())
												 {
													 myfile << newNum;
													 myfile.close();
												 }
											 }
											 listBox1->Items->Add("Unguided Learning Complete!");
											 guided_NUM = 0;
											 guided_finished = 1;
										 }
									 }
								 }
						 }
					 }
				 }


	}

	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {

				 srand(time(NULL));

				 AllocConsole(); //open console so it doesn't pop up every time camera functions are run

				 int deviceId = 0; // set camera id
				 try {
					 read_csv(fn_csv, images, labels, Glabels, Nlabels);
				 }
				 catch (cv::Exception& e) {
					 cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
					 // nothing more we can do
					 exit(1);
				 }
				

				// listBox1->Items->Add(System::Convert::ToString(Glabels.size()));
				 //histogram equalization
				 for (int i = 0; i < labels.size(); i++)
				 {
					 equalizeHist(images[i], images[i]);
				 }
				 /*for (int i = 0; i < Nlabels.size(); i++)
				 {
					 System::String^ nameL = gcnew System::String(Nlabels[i].c_str());
					 listBox1->Items->Add(nameL);
				 }*/
				 // needed for reconstruction
				 im_width = images[0].cols;
				 im_height = images[0].rows;

				 model = createEigenFaceRecognizer();
				 gender_model = createFisherFaceRecognizer();
				 model->train(images, labels);
				 gender_model->train(images, Glabels);
				 //model->set("threshold", 100.0);

		
				 
				//start camera
				 CameraController();

				 face_cascade.load(face_cascade_name);
	 
				 // Read the video stream
				 capture = cvCaptureFromCAM(0);

	}

			 static Bitmap^ IplImageToBitmap(IplImage* ipl)
			 {
				 if (ipl->depth == 8)
				 {
					 if (ipl->nChannels == 3)
					 {
						 return gcnew System::Drawing::Bitmap(ipl->width, ipl->height, ipl->widthStep, System::Drawing::Imaging::PixelFormat::Format24bppRgb,
							 (System::IntPtr)ipl->imageData);
					 }

					 else
					 {
						 return gcnew System::Drawing::Bitmap(ipl->width, ipl->height, ipl->widthStep, System::Drawing::Imaging::PixelFormat::Format8bppIndexed,
							 (System::IntPtr)ipl->imageData);
					 }
				 }
			 }

			 // reads from the numFaces file and creates the new folder
			 void guidedLearning()
			 {
				 //get number of faces in database
				string line;
				ifstream myfile(num_faces_file);
				if (myfile.is_open())
				{
					getline(myfile, line);
					myfile.close();
				}
				int numFaces = atoi(line.c_str());
				//listBox1->Items->Add(System::Convert::ToString(numFaces));
				itoa(numFaces + 1, newNum, 10);
				//create new folder
				currentFolder = newFolder + newNum;
				//System::String^ name = gcnew System::String(folderName.c_str());
				//listBox1->Items->Add(name);

				//add checking to see if folder alread exists
				LPCSTR c_folderName = currentFolder.c_str();
				CreateDirectoryA(c_folderName, NULL);

				listBox1->Items->Add("Ready!");

				newFaces.clear();
				updateAT.clear();
				guided_finished = 0;
				guided_NUM = 0;
			 }
			 
			 //Called by the ticker timer when comboBox1 is set to Detect mode
			 // Detects faces and displays blue rectangles around the face
			 void detect()
			 {
				 IplImage* frame = cvRetrieveFrame(capture);
				 Bitmap^ display = IplImageToBitmap(frame);
				 listBox1->Items->Clear();

				 if (capture)
				 {
					 frame = cvQueryFrame(capture);

					 if (frame != NULL)
					 {
						 std::vector<Rect> faces = detectFaces(frame);
						 //create a graphics object
						 System::Drawing::Graphics^ g = Graphics::FromImage(display);
						 //draw boxes
						 for (int i = 0; i < faces.size(); i++){
							 System::Drawing::Rectangle rect = System::Drawing::Rectangle(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
							 drawBlueBox(g, rect);
						 }
						 pictureBox1->Image = display;
						 //dispose pen and graphics object
						 delete g;
					 }
				 }
				
			 }

			 void scanMode()
			 {
				 listBox1->Items->Clear();
				 listBox1->Items->Add("Scanning");
				 IplImage* frame = cvRetrieveFrame(capture);
				 Bitmap^ display = IplImageToBitmap(frame);
				 vector<int> allPredictions;


				 if (capture)
				 {
					 Mat matFrame = cvQueryFrame(capture);
					 Mat original = matFrame.clone(); 
					 Mat gray;
					 cvtColor(original, gray, CV_BGR2GRAY);

					 if (frame != NULL)
					 {
						 std::vector<Rect> faces = detectFaces(matFrame);
						 //create a graphics object
						 Graphics^ g = Graphics::FromImage(display);
						 System::Drawing::Rectangle rect;

						 for (int i = 0; i < faces.size(); i++){
							 rect = System::Drawing::Rectangle(faces[i].x, faces[i].y, faces[i].width, faces[i].height);

							 //Crop face from image
							 Rect face_i = faces[i];
							 Mat face = gray(face_i);

							 Mat face_resized;
							 equalizeHist(face_resized, face_resized);
							 cv::resize(face, face_resized, cv::Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
							 // Get the prediction and associated confidence from the model
							 int prediction = model->predict(face_resized);
						
							//check if prediction is already in frame
							//****if it IS in the frame, should check which is stronger match
							bool check = 1;
							for (int i = 0; i < allPredictions.size(); i++)
							{
								if (prediction == allPredictions[i])
									check = 0;
							}
							//add prediction to current list of predictions
							if (check == 1)
							{
								check = 0; //reset
								allPredictions.push_back(prediction); //add prediction to list

								bool Known = 1;
								if (isKnown(prediction, face) > 750000)
								{
									Known = 0;
								}
								//show face and label with color
								showFace_withColor(Known, g, rect, prediction);
							}
							else
								showFace_withColor(0, g, rect, prediction);
							
							allPredictions.clear();
						 }
						
						 //graphics object
						 delete g;
					 }
					 else
					 {
						 listBox1->Items->Clear();
						 listBox1->ForeColor = SystemColors::Control;
						 listBox1->Items->Add("--(!) No captured frame");
					 }
				 }
			 }

			 void searchMode()
			 {
				 listBox1->Items->Clear();
				 listBox1->Items->Add("Searching");
				 IplImage* frame = cvRetrieveFrame(capture);
				 Bitmap^ display = IplImageToBitmap(frame);

				 if (capture)
				 {
					 Mat matFrame = cvQueryFrame(capture);
					 Mat original = matFrame.clone();
					 Mat gray;
					 cvtColor(original, gray, CV_BGR2GRAY);

					 if (frame != NULL)
					 {
						 std::vector<Rect> faces = detectFaces(matFrame);
						 bool Found = 0;
						 //create a graphics object
						 Graphics^ g = Graphics::FromImage(display);
						 System::Drawing::Rectangle rect;
						 vector<int> allPredictions;
						 //draw line between  point p1 and p2
						 for (int i = 0; i < faces.size(); i++){

							 rect = System::Drawing::Rectangle(faces[i].x, faces[i].y, faces[i].width, faces[i].height);

							 //Crop face from image
							 Rect face_i = faces[i];
							 Mat face = gray(face_i);

							 Mat face_resized;
							 cv::resize(face, face_resized, cv::Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
							 int prediction = model->predict(face_resized);

							 //check if prediction is already in frame
							 //****if it IS in the frame, should check which is stronger match
							 bool check = 1;
							 for (int i = 0; i < allPredictions.size(); i++)
							 {
								 if (prediction == allPredictions[i])
									 check = 0;
							 }
							 if (check == 1)
							 {


								 System::String^ recog_name = gcnew System::String(Nlabels[prediction * 10].c_str());
								 if (System::String::Compare(recog_name, comboBox2->SelectedItem->ToString()) == 0)
								 {
									 showFace_withColor(1, g, rect, prediction);
									 listBox1->Items->Add("Subject Found");
									 tickCount = 0;
									 Found = 1;
								 }
								 else
								 {
									 drawBlueBox(g, rect);
								 }
								 if (Found == 0)
								 {
									 if (tickCount == 0)
									 {
										 Search();
									 }
								 }

							 }
							 if (faces.size() == 0){
								 if (tickCount == 0)
									 Search();
							 }
						 }
						 //dispose pen and graphics object
						 allPredictions.clear();
						 delete g;
					 }
					 else
					 {
						 listBox1->Items->Clear();
						 listBox1->ForeColor = SystemColors::Control;
						 listBox1->Items->Add("--(!) No captured frame");
					 }
				 }
			 }

			 void trackingMode()
			 {
				 listBox1->Items->Clear();
				 listBox1->Items->Add("Searching");
				 IplImage* frame = cvRetrieveFrame(capture);
				 Bitmap^ display = IplImageToBitmap(frame);

				 if (capture)
				 {
					 Mat matFrame = cvQueryFrame(capture);
					 Mat original = matFrame.clone();
					 Mat gray;
					 cvtColor(original, gray, CV_BGR2GRAY);

					 if (frame != NULL)
					 {
						 std::vector<Rect> faces = detectFaces(matFrame);
						 //create a graphics object
						 Graphics^ g = Graphics::FromImage(display);
						 System::Drawing::Rectangle rect;
						 Pen^ p = gcnew Pen(Color::Blue);
						 //draw line between  point p1 and p2
						 for (int i = 0; i < faces.size(); i++){

							 rect = System::Drawing::Rectangle(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
							 g->DrawRectangle(p, rect);
							 //listBox1->Items->Add(faces[i].x);
							 
							 //Crop face from image
							 Rect face_i = faces[i];
							 Mat face = gray(face_i);

							 Mat face_resized;
							 cv::resize(face, face_resized, cv::Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
							 int prediction = model->predict(face_resized);


							 System::String^ recog_name = gcnew System::String(Nlabels[prediction * 10].c_str());
							 if (System::String::Compare(recog_name, comboBox2->SelectedItem->ToString()) == 0)
							 //if subject has been found
							 {
								 cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
								 showFace_withColor(1, g, rect, prediction);
								
								 listBox1->Items->Add("Subject Found");
								 int x, y;
								 x = (currentWidth - center.x)/45;
								 y = (currentHeight - center.y)/30;
								 Track(x, y);
								 //listBox1->Items->Add(posX+":"+System::Convert::ToString(x) + ", "+ posY +":"+ System::Convert::ToString(y));
								 tickCount = 0;
							 }
							 else
							 {
								 if (tickCount == 5)
									 Search();
							 }

						 }
						 if (faces.size() == 0){
							 if (tickCount == 5)
								 Search();
						 }

						 //dispose pen and graphics object
						 delete p;
						 delete g;
					 }
					 else
					 {
						 listBox1->Items->Clear();
						 listBox1->ForeColor = SystemColors::Control;
						 listBox1->Items->Add("--(!) No captured frame");
					 }
				 }
			 }

			 void genderMode()
			 {
				 listBox1->Items->Clear();
				 listBox1->Items->Add("Scanning");
				 IplImage* frame = cvRetrieveFrame(capture);
				 Bitmap^ display = IplImageToBitmap(frame);

				 if (capture)
				 {
					 Mat matFrame = cvQueryFrame(capture);
					 Mat original = matFrame.clone();
					 Mat gray;
					 cvtColor(original, gray, CV_BGR2GRAY);

					 if (frame != NULL)
					 {
						 std::vector<Rect> faces = detectFaces(matFrame);
						 //create a graphics object
						 Graphics^ g = Graphics::FromImage(display);
						 System::Drawing::Rectangle rect;
						 //create a pen object and setting the color and width for the pen
						 Pen^ blue_p = gcnew Pen(Color::Blue);
						 Pen^ pink_p = gcnew Pen(Color::HotPink);

						 for (int i = 0; i < faces.size(); i++){
							 rect = System::Drawing::Rectangle(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
							 g->DrawRectangle(blue_p, rect);
							 //listBox1->Items->Add(faces[i].x);
							 System::Drawing::PointF point;
							 point.X = faces[i].x;
							 point.Y = (faces[i].y - 30);
							 System::Drawing::Font^ drawFont = gcnew System::Drawing::Font("Arial", 12);
							 SolidBrush^ drawBrush = gcnew SolidBrush(Color::Blue);
							 SolidBrush^ pinkBrush = gcnew SolidBrush(Color::HotPink);
							 //Crop face from image
							 Rect face_i = faces[i];
							 Mat face = gray(face_i);

							 Mat face_resized;
							 cv::resize(face, face_resized, cv::Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
							 int prediction = gender_model->predict(face_resized);

				
							 if (prediction == 66)
							 {
								 listBox1->Items->Add("Male");
								 g->DrawRectangle(blue_p, rect);
								 g->DrawString("Male", drawFont, drawBrush, point);
							 }
							 else
							 {
								 listBox1->Items->Add("Female");
								 g->DrawRectangle(pink_p, rect);
								 g->DrawString("Female", drawFont, pinkBrush, point);
							 }
							 

						 }

						 //dispose pen and graphics object
						 delete blue_p;
						 delete pink_p;
						 delete g;
					 }
					 else
					 {
						 listBox1->Items->Clear();
						 listBox1->ForeColor = SystemColors::Control;
						 listBox1->Items->Add("--(!) No captured frame");
					 }
				 }
			 }

			 //detect mode -> returns faces as rectangles to be displayed
			 std::vector<Rect> detectFaces(Mat frame)
			 {
				 std::vector<Rect> faces;
				 std::vector<Rect> keptFaces;
				 std::vector<Rect> profileFaces;
				 Mat frame_gray;

				 cvtColor(frame, frame_gray, CV_BGR2GRAY);
				 equalizeHist(frame_gray, frame_gray);

				 //-- Detect faces
				 face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
				 
				 for (int i = 0; i < faces.size(); i++)
				 {
					 cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);

					 for (int j = 0; j < prevFaces.size(); j++)
					 {
						 if ((prevFaces[j].tl().x < center.x && prevFaces[j].tl().y < center.y) && (center.x < prevFaces[j].br().x && center.y < prevFaces[j].br().y))
						 {
							 keptFaces.push_back(faces[i]);
						 }
					 }
				 }

				 prevFaces = faces;

				 int num = keptFaces.size();
				 if (num == 1)
					 listBox1->Items->Add("Detected 1 face");
				 else
					listBox1->Items->Add("Detected " + System::Convert::ToString(num) + " faces");
		
				 return keptFaces;


			 }

			 std::vector<Rect> detectFacesNoCheck(Mat frame)
			 {
				 std::vector<Rect> faces;
				 std::vector<Rect> keptFaces;
				 std::vector<Rect> profileFaces;
				 Mat frame_gray;

				 cvtColor(frame, frame_gray, CV_BGR2GRAY);
				 equalizeHist(frame_gray, frame_gray);

				 //-- Detect faces
				 face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

				 int num = faces.size();
				 if (num == 1)
					 listBox1->Items->Add("Detected 1 face");
				 else
					 listBox1->Items->Add("Detected " + System::Convert::ToString(num) + " faces");

				 return faces;
			 }

			 void unguidedLearning()
			 {

				 //get number of faces in database
				 string line;
				 ifstream myfile(num_faces_file);
				 if (myfile.is_open())
				 {
					 getline(myfile, line);
					 myfile.close();
				 }
				 int numFaces = atoi(line.c_str());
				 itoa(numFaces + 1, newNum, 10);

				 //create new folder
				 currentFolder = newFolder + newNum;
				 //System::String^ name = gcnew System::String(folderName.c_str());
				 //listBox1->Items->Add(name);

				 //add checking to see if folder alread exists
				 LPCSTR c_folderName = currentFolder.c_str();
				 CreateDirectoryA(c_folderName, NULL);

				 listBox1->Items->Add("Ready!");

				 newFaces.clear();
				 updateAT.clear();
				 guided_finished = 0;
				 guided_NUM = 0;
				 button1->Text = L"Start Learning";


			 }

			 void showFace_withColor(bool Known, Graphics^ g, System::Drawing::Rectangle face, int prediction)
			 {
				 Drawing::Color color;
				 if (!Known)
				 {
					color = Color::Blue;
				 }
				 else
				 {
					 System::Array^ colorsArray = Enum::GetValues(KnownColor::typeid);
					 array<Drawing::KnownColor>^allColors = gcnew array<Drawing::KnownColor>(colorsArray->Length);
					 Array::Copy(colorsArray, allColors, colorsArray->Length);
					 color = Drawing::Color::FromName(allColors[prediction*10 % allColors->Length].ToString());
				 }
			
				 Pen^ p = gcnew Pen(color);
				 g->DrawRectangle(p, face);
				 SolidBrush^ faceBrush = gcnew SolidBrush(color);
				 System::String^ recog_name = gcnew System::String(Nlabels[prediction * 10].c_str());
				 System::Drawing::PointF point;
				 point.X = face.X;
				 point.Y = (face.Y - 30);
				 System::Drawing::Font^ drawFont = gcnew System::Drawing::Font("Arial", 12);
			
				 if (!Known)
				 {
					 recog_name = "UNKNOWN";
				 }

				 listBox1->Items->Add(recog_name);
				 g->DrawString(recog_name, drawFont, faceBrush, point);

				 delete p;
				 delete faceBrush;
			 }

			 void drawBlueBox(Graphics^ g, System::Drawing::Rectangle face)
			 {

				 Pen^ p = gcnew Pen(Color::Blue);
				 g->DrawRectangle(p, face);

				 delete p;

			 }

			
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				// (void)system("C:\\cameraFunctions\\PTZDeviceConsole\\bin\\Release\\Left.exe 10");
				

				 int unguidedCount = 0;
				 //listBox1->Items->Clear();

				 System::String^ name = textBox1->Text;
				 std::string std_name(marshal_as<std::string>(name));

				 cv::String end = ".pgm";

				 //define gender information
				 char gendString[20];
				 int gender = 66;
				 if (comboBox2->Text == L"Female")
					 gender = 99;
				 itoa(gender, gendString, 10);

				 if (comboBox1->Text == L"Guided Learning")
				 {
					 Mat newImage = cvRetrieveFrame(capture);

					 Mat gray_image;
					 cvtColor(newImage, gray_image, CV_BGR2GRAY);
					 std::vector<Rect> faces = detectFacesNoCheck(newImage);
					 int i = 0;

					 if (faces.size() == 1)
					 {
						 cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
						 rectangle(newImage, faces[i].tl(), faces[i].br(), Scalar(255, 0, 0));
						 Mat faceROI = gray_image(faces[i]);
						 face = faceROI;
						 Mat resized(112, 112, CV_8UC1);
						 resize(face, resized, resized.size(), 0, 0, INTER_LINEAR);
						 Rect cropped(10, 0, 92, 112);
						 face = resized(cropped);
						 char numString[20];


						 guided_NUM++;
						 int n = guided_NUM;

						 int number = atoi(newNum);
						 number--;
						 char numString2[20];
						 itoa(number, numString2, 10);

						 //save face to database
						 if (guided_NUM <= 10)
						 {
							 newFaces.push_back(face);
							 itoa(n, numString, 10);

							 updateAT.push_back("\n" + currentFolder + "\\" + numString + end + "; " + numString2 + "; " + gendString + ";" + std_name);
							 listBox1->Items->Add(System::Convert::ToString(guided_NUM));
						 }

						 if (guided_NUM == 10)
						 {
							 for (int i = 0; i < 10; i++)
							 {
								 cv::String start = "..//..//THE_db//s";
								 start.append(newNum);
								 start.append("//");
								 itoa(i + 1, numString, 10);
								 start.append(numString);
								 start.append(end);

								 //save image to file
								 try {
									 imwrite(start, newFaces[i]);
								 }
								 catch (runtime_error& ex) {
									 listBox1->Items->Add("error");
								 }

								 //update at.txt
								 ofstream myfile;
								 myfile.open(fn_csv, ofstream::out | ofstream::app);
								 //System::String^ check = gcnew System::String(currentFolder.c_str());
								 //listBox1->Items->Add(check);

								 if (myfile.is_open())
								 {
									 myfile << updateAT[i];
									 myfile.close();
								 }

								 //updated numFaces file
								 myfile.open(num_faces_file);
								 if (myfile.is_open())
								 {
									 myfile << newNum;
									 myfile.close();
								 }
							 }
							 listBox1->Items->Add("Guided Learning Complete!");
							 guided_NUM = 0;
							 guided_finished = 1;

							 

						 }
					 }
					 else
						 listBox1->Items->Add("Too many faces or face not detected, please try again");
				 }
				 else if (comboBox1->Text == L"Unguided Learning")
				 {
					 unguided_start = 1;
				 }
				 else if (comboBox1->Text == L"Tracking")
				 {
					 track = 1;
				 }
				 else if (comboBox1->Text == L"Search")
				 {
					 searching = 1;
				 }
	}
					
				 
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	
				 images.clear();
				 labels.clear();
				 Glabels.clear();
				 Nlabels.clear();
				 read_csv(fn_csv, images, labels, Glabels, Nlabels);

				 model->train(images, labels);
				 gender_model->train(images, Glabels);
				 //model->set("threshold", 100);

	}


//For planned sound
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 MUTE = !MUTE;
}

};
}


//resize cropped image to 200 by 200 for storage
void resizeFace(Mat &face)
{
	Mat resized(200, 200, CV_8UC1);
	resize(face, resized, resized.size(), 0, 0, INTER_LINEAR);
	face = resized;

}

void CameraController()
{
	//Init bottom left and 
	//system("C:\\CameraController\\PTZDeviceConsole.exe left 60");
	//system("C:\\CameraController\\PTZDeviceConsole.exe down 25");

	//system("C:\\CameraController\\PTZDeviceConsole.exe right 30");
	//system("C:\\CameraController\\PTZDeviceConsole.exe up 12");
	posX = 30;
	posY = 15;
	DoSearch = 0;
}

cv::Point ReturnLocation(){
	cv::Point currentLocation = cv::Point(posX, posY);

	return currentLocation;
}
void MoveUp(int move)
{
	//move = checkValid('u', move);
	int temp = posY + move;
	if (temp < TiltMAX){
		posY = temp;
		//return move; //valid move
	}
	else if (posY < TiltMAX){
		temp = TiltMAX - posY;
		posY = TiltMAX;
		move = temp;
	}
	else
	{
		move = 0;
	}
	std::string tmp = "C:\\CameraController\\PTZDeviceConsole.exe up " + std::to_string(move);
	const char* command = tmp.c_str();
	DO(command);
	//WinExec(command, SW_HIDE);
}
void MoveDown(int move)
{
	int temp = posY - move;
	if (temp > 0){
		posY = temp;
	}
	else
	{
		move = posY;
		posY = 0;
	}
	std::string tmp =  "C:\\CameraController\\PTZDeviceConsole.exe down " + std::to_string(move);
	const char* command = tmp.c_str();
	DO(command);
	//WinExec(command, SW_HIDE);
}
void MoveLeft(int move)
{
	int temp = posX - move;
	if (temp > 0){
		posX = temp;
	}
	else
	{
		temp = posX;
		posX = 0;
		move = temp;
	}

	std::string tmp = "C:\\CameraController\\PTZDeviceConsole.exe left " + std::to_string(move);
	const char* command = tmp.c_str();
	DO(command);
	//WinExec(command, SW_HIDE);
}
void MoveRight(int move)
{
	int temp = posX + move;
	if (temp < PanMAX){
		posX = temp;
	}
	else if (posX < PanMAX){
		temp = PanMAX - posX;
		posX = PanMAX;
		move = temp;
	}
	std::string tmp = "C:\\CameraController\\PTZDeviceConsole.exe right " + std::to_string(move);
	const char* command = tmp.c_str();
	DO(command);
	//WinExec(command, SW_HIDE);
}

void Track(int coordX, int coordY)
{
	int movePan, moveTilt;
	movePan = coordX;//currentWidth - coordX;
	moveTilt = coordY;//currentHeight - coordY;

	//movePan = movePan / 45;//30 best estimate
	//moveTilt = moveTilt / 30;//35 best estimate

	if (movePan > 0){
		MoveRight(abs(movePan));
	}
	else{
		MoveLeft(abs(movePan));
	}
	///TILT??
	if (moveTilt > 0){
		MoveUp(abs(moveTilt));
	}
	else{
		MoveDown(abs(moveTilt));
	}
}

void Move(int movePan, int moveTilt)
{

	if (movePan > posX){
		MoveRight(movePan);
	}
	else{
		MoveLeft(movePan);
	}

	if (moveTilt > posY){
		MoveUp(abs(moveTilt));
	}
	else{
		MoveDown(moveTilt);
	}
}

void Search(){
	int temp, temp2;
	temp = (posX + 5);
	temp2 = (posX - 5);
	if (DoSearch == 1){
		if (temp > 60){
			DoSearch = 0;
			PanToPos(temp2);
			return;
		}
		PanToPos(temp);
	}
	if (DoSearch == 0){
		if (temp2 < 0){
			DoSearch = 1;
			PanToPos(temp);
			return;
		}
		PanToPos(temp2);
	}
	//TiltToPos(13);
	waitKey(2);
}

//returns the absolute difference between images - using average of all faces of given person
int isKnown(int label, Mat currentFace){
	
	//trying straigh pixel subtraction
	int diff;
	int sum = 0;
	int pixels = im_width*im_height;

	for (int k = 0; k < 10; k++)
	{
		Mat reconstruction = images[label * 10 + k];
		reconstruction.convertTo(reconstruction, CV_8UC1, 1, 0);
		int rows, cols;

		Mat resized(112, 112, CV_8UC1);
		resize(currentFace, resized, resized.size(), 0, 0, INTER_LINEAR);
		Rect cropped(10, 0, 92, 112);
		currentFace = resized(cropped);
		equalizeHist(currentFace, currentFace);
		for (int i = 0; i < im_height; i++){
			for (int j = 0; j < im_width; j++){
				diff = abs(currentFace.at<uchar>(i, j) - reconstruction.at<uchar>(i, j));
				sum += diff;
			}
		}
	}sum /= 10;

	return sum;
	/*if (sum < 670000)//number needs tweaking based on where we are going to run the software
	{
		return 1;
	}
	else
	{
		return 0;
	}

	int avg = 100*sum / pixels / 255;
	return sum;*/
	
}

void DO(const char* comm){

	wchar_t wtext[100];
	mbstowcs(wtext, comm, strlen(comm) + 1);
	LPWSTR command = wtext;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		command,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void MoveToPos(int x, int y){
	PanToPos(x);
	TiltToPos(y);
}
void PanToPos(int x){
	if (x >= 0 && x <= 60){
		if (x > posX){
			MoveRight(x - posX);
		}
		else{
			MoveLeft(posX - x);
		}
	}
}
void TiltToPos(int y){
	if (y >= 0 && y <= 24){
		if (y >= posY){
			MoveUp(y - posY);
		}
		else{
			MoveDown(posY - y);
		}
	}
}

