/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqOpenRawDialog.h"
#include "ui_mqOpenRawDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QString>

#include <QHeaderView>


#include <sstream>

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


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
mqOpenRawDialog::mqOpenRawDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqOpenRawDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqOpenRawDialog");	
	
	this->myFileName = "";
  this->Ui->voxelSizeX->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeY->setButtonSymbols(QAbstractSpinBox::NoButtons);
  this->Ui->voxelSizeZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOpenRaw()));

}




//-----------------------------------------------------------------------------
mqOpenRawDialog::~mqOpenRawDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqOpenRawDialog::OpenRaw()
{
	cout << "OpenRaw dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes() ==1)
	{
		std::string action = "OpenRaw";
		
		//mqMorphoDigCore::instance()->OpenRawVolume(this->myFileName);
		
	}
}



void mqOpenRawDialog::setFileName(QString fileName)
{
	this->myFileName = fileName;
	QFileInfo fileInfo(fileName);
	QString onlyfilename(fileInfo.fileName());

	this->Ui->FileName->setText(onlyfilename);
	
		
	
}



void mqOpenRawDialog::slotOpenRaw()
{
	this->OpenRaw();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

