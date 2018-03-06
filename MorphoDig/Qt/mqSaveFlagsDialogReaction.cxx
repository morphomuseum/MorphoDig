/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveFlagsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveFlagsDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveFlagsDialogReaction::mqSaveFlagsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveFlagsDialogReaction::onTriggered()
{
	
	vtkIdType num_Flags = 0;
	
		num_Flags = mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfItems();
	
	
		if (num_Flags == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Flag exist...");
			msgBox.exec();
			return;
		}
		
		cout << "Save Flag Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveFlagsDialogReaction::showSaveFlagsDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSaveFlagsDialogReaction::showSaveFlagsDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE Flag, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveFlagsDialog SaveFlag_dialog(mqCoreUtilities::mainWidget());
  SaveFlag_dialog.exec();
}
