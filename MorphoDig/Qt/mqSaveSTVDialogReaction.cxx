/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveSTVDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveSTVDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveSTVDialogReaction::mqSaveSTVDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveSTVDialogReaction::onTriggered()
{
	vtkIdType num_Norm = 0;
	vtkIdType num_T = 0;
	vtkIdType num_Node = 0;
	vtkIdType num_H = 0;
	
		num_Norm = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
		num_H = mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();
		num_Node = mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
		num_H = mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	
	
		if (num_Norm==0&& num_T==0&&num_Node==0&&num_H == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark nor curve element  exist...");
			msgBox.exec();
			return;
		}
		
		cout << "Save STV Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveSTVDialogReaction::showSaveSTVDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSaveSTVDialogReaction::showSaveSTVDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE STV, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveSTVDialog SaveSTV_dialog(mqCoreUtilities::mainWidget());
  SaveSTV_dialog.exec();
}
