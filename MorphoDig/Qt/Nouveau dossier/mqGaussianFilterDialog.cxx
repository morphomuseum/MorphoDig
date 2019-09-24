/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqMedianFilterDialog.h"
#include "ui_mqMedianFilterDialog.h"
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
mqMedianFilterDialog::mqMedianFilterDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqMedianFilterDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqMedianFilterDialog");	
	
	this->myVolume = NULL;
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotMedianFilter()));

}




//-----------------------------------------------------------------------------
mqMedianFilterDialog::~mqMedianFilterDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqMedianFilterDialog::MedianFilter()
{
	cout << "MedianFilter dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes() ==1)
	{
		std::string action = "MedianFilter";
		if (this->myVolume != NULL)
		{
			this->myVolume->MedianFilter(this->Ui->x->value(), this->Ui->y->value(), this->Ui->z->value());
			mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
			mqMorphoDigCore::instance()->Render();
		}
		//mqMorphoDigCore::instance()->addMedianFilter(this->Ui->flyingEdges->isChecked(), this->Ui->threshold->value());
		
	}
}



void mqMedianFilterDialog::setVolume (vtkMDVolume *vol)
{
	this->myVolume = vol;
	QString myLabel(this->myVolume->GetName().c_str());
	this->Ui->VolumeName->setText(myLabel);
}



void mqMedianFilterDialog::slotMedianFilter()
{
	this->MedianFilter();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

