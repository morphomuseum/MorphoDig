/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqIsosurfaceDialog.h"
#include "ui_mqIsosurfaceDialog.h"
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
mqIsosurfaceDialog::mqIsosurfaceDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqIsosurfaceDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqIsosurfaceDialog");	
	
	this->myVolume = NULL;
  this->Ui->threshold->setButtonSymbols(QAbstractSpinBox::NoButtons);
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotIsosurface()));

}




//-----------------------------------------------------------------------------
mqIsosurfaceDialog::~mqIsosurfaceDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqIsosurfaceDialog::Isosurface()
{
	cout << "Isosurface dialog" << endl;
	
	if (mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes() ==1)
	{
		std::string action = "Isosurface";
		
		mqMorphoDigCore::instance()->addIsosurface(this->Ui->flyingEdges->isChecked(), this->Ui->threshold->value());
		
	}
}



void mqIsosurfaceDialog::setVolume (vtkMDVolume *vol)
{
	this->myVolume = vol;
	QString myLabel(this->myVolume->GetName().c_str());
	this->Ui->VolumeName->setText(myLabel);
	double min = this->myVolume->GetRangeMin();
	double max = this->myVolume->GetRangeMax();
	double avg = (max + min) / 2;
	this->Ui->threshold->setMinimum(min);
	this->Ui->threshold->setMaximum(max);
	this->Ui->threshold->setValue(avg);
}



void mqIsosurfaceDialog::slotIsosurface()
{
	this->Isosurface();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

