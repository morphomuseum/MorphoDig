/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveVTIDialog.h"
#include "ui_mqSaveVTIDialog.h"
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
mqSaveVTIDialog::mqSaveVTIDialog(QWidget* Parent, QString fileName)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveVTIDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveVTIDialog");
	this->m_fileName = fileName;
	this->myVolume = NULL;
	// This is where we 
  //
 
 
 this->Ui->BigEndian->setChecked(true);
 this->Ui->CompressionZlib->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveVTIFile()));

}




//-----------------------------------------------------------------------------
mqSaveVTIDialog::~mqSaveVTIDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveVTIDialog::slotSaveVTIFile()
{
	cout << "VTI File Saved!" << endl;
	//int position_mode = 0; // 0 Original position , 1 Modified position
	int file_type = 2; // VTI

	int compressionOn = 1; //1comression, 0 non compression
	int compressionType = 1; //1 Zlib 2 LZ4 3 ZMA
	
	int bigendian = 1;

	
	if (this->Ui->CompressionOff->isChecked()) { compressionOn = 0; }
	
	if (this->Ui->CompressionZlib->isChecked()) { compressionType = 1; }
	if (this->Ui->CompressionLZ4->isChecked()) { compressionType = 2; }
	if (this->Ui->CompressionZMA->isChecked()) { compressionType = 3; }

	if (this->Ui->LittleEndian->isChecked()) { bigendian = 0; }

	this->myVolume = mqMorphoDigCore::instance()->GetFirstSelectedVolume();
	if (this->myVolume != NULL)
	{
		mqMorphoDigCore::instance()->SaveVolume(this->m_fileName, file_type, compressionOn, this->myVolume, compressionType, bigendian);
	}
	

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

