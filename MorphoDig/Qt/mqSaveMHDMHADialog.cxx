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
mqSaveMHDMHADialog::mqSaveMHDMHADialog(QWidget* Parent, QString fileName)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveMHDMHADialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveMHDMHADialog");
	this->m_fileName = fileName;
	this->myVolume = NULL;
	// This is where we 
  //
 
 
 this->Ui->MHD->setChecked(true);
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
		mqMorphoDigCore::instance()->SaveVolume(this->m_fileName, file_type, compressionOn, this->myVolume);
	}
	

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

