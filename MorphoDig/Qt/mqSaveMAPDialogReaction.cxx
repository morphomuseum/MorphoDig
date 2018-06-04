/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveMAPDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveMAPDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveMAPDialogReaction::mqSaveMAPDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveMAPDialogReaction::onTriggered()
{
	
		
		cout << "Save MAP Dialog Triggered!" << endl;


		


		mqSaveMAPDialogReaction::showSaveMAPDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSaveMAPDialogReaction::showSaveMAPDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE MAP, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveMAPDialog SaveMAP_dialog(mqCoreUtilities::mainWidget());
  SaveMAP_dialog.exec();
}
