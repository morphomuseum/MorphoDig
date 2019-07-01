/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqResliceDialog.h"
#include "ui_mqResliceDialog.h"
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
#include <QMessageBox>

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
mqResliceDialog::mqResliceDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqResliceDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqResliceDialog");	
	
	this->myVolume = NULL;
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotReslice()));
 
	
}




//-----------------------------------------------------------------------------
mqResliceDialog::~mqResliceDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

void mqResliceDialog::setVolume (vtkMDVolume *vol)
{
	this->myVolume = vol;
	QString myLabel(this->myVolume->GetName().c_str());
	this->Ui->VolumeName->setText(myLabel);
	
}

void mqResliceDialog::Reslice()
{
	if (this->myVolume != NULL)
	{
		cout << "Reslice dialog" << endl;

		
		int method = 0;
		if (this->Ui->linear->isChecked()) { method = 1; }
		if (this->Ui->neighbor->isChecked()) { method = 1; }
		if (this->Ui->cubic->isChecked()) { method = 2; }
		int extended = 0;
		if (this->Ui->extended->isChecked()) { extended = 1; }
		if (this->Ui->cropped->isChecked()) { extended =0; }
		
		this->myVolume->Reslice(extended, method);
		mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
		mqMorphoDigCore::instance()->Render();
	}

}


void mqResliceDialog::slotReslice()
{
	this->Reslice();
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

