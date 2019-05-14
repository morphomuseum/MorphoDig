/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveLandmarksDialog.h"
#include "ui_mqSaveLandmarksDialog.h"
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
mqSaveLandmarksDialog::mqSaveLandmarksDialog(QWidget* Parent, int lmtype)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveLandmarksDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveLandmarksDialog");
	
	this->m_lmtype = lmtype;
	// This is where we 
  //
 
 
 this->Ui->VER->setChecked(true);
 this->Ui->SaveAll->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveLandmarkFile()));

}




//-----------------------------------------------------------------------------
mqSaveLandmarksDialog::~mqSaveLandmarksDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveLandmarksDialog::slotSaveLandmarkFile()
{
	cout << "Landmark File Saved!" << endl;

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
	if (this->Ui->VER->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir()+ proposedName,
			tr("Landmark file (*.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	if (this->Ui->LMK->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir()+ proposedName,
			tr("Landmark file (*.lmk)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	if (this->Ui->TPS->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + proposedName,
			tr("Landmark file (*.tps)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	if (this->Ui->PTS->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save Landmark files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + proposedName,
			tr("Landmark file (*.pts)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
	}
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	int file_type = 0; // 0 VER 1 LMK 2 PTS 3 TPS 4 => to implement!
	int save_only_selected = 0; //0 no= save all landmark, 1 yes, save only selected landmarks
	
	if (this->Ui->VER->isChecked()) { file_type = 0; }
	else if (this->Ui->LMK->isChecked()) { file_type = 1; }
	else if (this->Ui->PTS->isChecked()) { file_type = 2; }
	else if (this->Ui->TPS->isChecked()) { file_type = 3; }
	

	if (this->Ui->SaveOnlySelected->isChecked()) { save_only_selected = 1; }
	else if (this->Ui->SaveAll->isChecked()) { save_only_selected = 0; }
	

	mqMorphoDigCore::instance()->SaveLandmarkFile(fileName, this->m_lmtype, file_type, save_only_selected);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

