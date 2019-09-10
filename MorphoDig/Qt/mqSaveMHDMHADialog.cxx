/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveMHDMHADialog.h"
#include "ui_mqSaveMHDMHADialog.h"
#include "MorphoDigVersion.h"
#include "vtkMDVolume.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QCheckBox>
#include <QHeaderView>

#include <sstream>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

//-----------------------------------------------------------------------------
mqSaveMHDMHADialog::mqSaveMHDMHADialog(QWidget* Parent, QString fileName, int _mode)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveMHDMHADialog())
{

	this->Ui->setupUi(this);
	this->Mode = _mode;
	this->setObjectName("mqSaveMHDMHADialog");
	this->m_fileName = fileName;
	this->myVolume = NULL;
	// This is where we 
  //
 
	std::string MHAext(".mha");
	std::string MHAext2(".MHA");
	std::string MHDext(".mhd");
	std::string MHDext2(".MHD");

	int extension_found = 0;
	std::size_t found = fileName.toStdString().find(MHAext);
	std::size_t found2 = fileName.toStdString().find(MHAext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		//VTK
		this->Ui->MHD->setChecked(false);
		this->Ui->MHA->setChecked(true);
		this->Ui->MHD->setEnabled(false);
		this->Ui->MHA->setEnabled(false);
				
		cout << "extension MHA found!" << endl;
		extension_found = 1;
	}
	std::size_t found3 = fileName.toStdString().find(MHDext);
	std::size_t found4 = fileName.toStdString().find(MHDext2);
	if (found3 != std::string::npos || found4 != std::string::npos)
	{

		//VTP
		this->Ui->MHD->setChecked(true);
		this->Ui->MHA->setChecked(false);
		this->Ui->MHD->setEnabled(false);
		this->Ui->MHA->setEnabled(false);				
		cout << "extension MHD found!" << endl;
		extension_found = 1;
	}

	if (extension_found == 0)
	{
		cout << "extension not found!" << endl;
		this->Ui->MHD->setChecked(true);
		this->Ui->MHD->setEnabled(true);
		this->Ui->MHA->setEnabled(true);
	}

 
 
 this->Ui->CompressionOn->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveMHDMHAFile()));

}




//-----------------------------------------------------------------------------
mqSaveMHDMHADialog::~mqSaveMHDMHADialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveMHDMHADialog::slotSaveMHDMHAFile()
{
	cout << "MHDMHA File Saved!" << endl;
	//int position_mode = 0; // 0 Original position , 1 Modified position
	int file_type = 0; // 0 MHD 1 MHA
	int compressionOn = 1; //1comression, 0 non compression
	
	if (this->Ui->MHA->isChecked()) {file_type = 1; }		
	if (this->Ui->CompressionOff->isChecked()) { compressionOn = 0; }
	
	this->myVolume = mqMorphoDigCore::instance()->GetFirstSelectedVolume();
	if (this->myVolume != NULL)
	{
		if (this->Mode == 0)
		{
			//normal volume
			mqMorphoDigCore::instance()->SaveVolume(this->m_fileName, file_type, compressionOn, this->myVolume);
		}
		else
		{
			//mask
			mqMorphoDigCore::instance()->SaveVolume(this->m_fileName, file_type, compressionOn, this->myVolume,1,1,1);
		}
	}
	

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

