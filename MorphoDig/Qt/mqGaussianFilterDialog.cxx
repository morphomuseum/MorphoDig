/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqGaussianFilterDialog.h"
#include "ui_mqGaussianFilterDialog.h"
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
mqGaussianFilterDialog::mqGaussianFilterDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqGaussianFilterDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqGaussianFilterDialog");	
	
	this->myVolume = NULL;
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotGaussianFilter()));

}




//-----------------------------------------------------------------------------
mqGaussianFilterDialog::~mqGaussianFilterDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqGaussianFilterDialog::GaussianFilter()
{
	cout << "GaussianFilter dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes() ==1)
	{
		std::string action = "GaussianFilter";
		if (this->myVolume != NULL)
		{
			this->myVolume->GaussianFilter(this->Ui->std_x->value(), this->Ui->std_y->value(), this->Ui->std_z->value(), this->Ui->rad_x->value(), this->Ui->rad_y->value(), this->Ui->rad_z->value());
			mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
			mqMorphoDigCore::instance()->Render();
		}
		//mqMorphoDigCore::instance()->addGaussianFilter(this->Ui->flyingEdges->isChecked(), this->Ui->threshold->value());
		
	}
}



void mqGaussianFilterDialog::setVolume (vtkMDVolume *vol)
{
	this->myVolume = vol;
	QString myLabel(this->myVolume->GetName().c_str());
	this->Ui->VolumeName->setText(myLabel);
}



void mqGaussianFilterDialog::slotGaussianFilter()
{
	this->GaussianFilter();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

