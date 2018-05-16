/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveMAPDialog.h"
#include "ui_mqSaveMAPDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
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
mqSaveMAPDialog::mqSaveMAPDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveMAPDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveMAPDialog");
	
	
	// This is where we 
  //
 
 this->Ui->SaveAll->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveMAPFile()));

}




//-----------------------------------------------------------------------------
mqSaveMAPDialog::~mqSaveMAPDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveMAPDialog::slotSaveMAPFile()
{
	cout << "MAP File Saved!" << endl;
	
	QString fileName;
	QString proposedName = "";
	vtkIdType num_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_meshes == 1)
	{
		mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
		proposedName += QDir::separator();
		proposedName += myActor->GetName().c_str();
	}
	else
	{
		vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 1)
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			proposedName += QDir::separator();
			proposedName += +mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str();
		}
	}

		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save MAP files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + proposedName,
			tr("MAP file (*.map)"), NULL
			//, QFileDialog::DontConfirmOverwrite
			);
	

	
	
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	int save_only_selected = 0; //0 no= save all landmark, 1 yes, save only selected landmarks
	
	

	if (this->Ui->SaveOnlySelected->isChecked()) { save_only_selected = 1; }
	else if (this->Ui->SaveAll->isChecked()) { save_only_selected = 0; }
	

	//mqMorphoDigCore::instance()->SaveMAPFile(fileName, save_only_selected);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

