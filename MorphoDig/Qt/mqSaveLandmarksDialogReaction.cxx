/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveLandmarksDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveLandmarksDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveLandmarksDialogReaction::mqSaveLandmarksDialogReaction(QAction* parentObject, int lmktype)
  : Superclass(parentObject)
{
	this->m_lmktype = lmktype;
}

void mqSaveLandmarksDialogReaction::onTriggered()
{
	
	vtkIdType num_landmarks = 0;
	if (this->m_lmktype==0)// Normal landmarks
	{
		num_landmarks = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
	}
	else if (this->m_lmktype == 1) // Target landmarks
	{
		num_landmarks = mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();

	}
	else if (this->m_lmktype == 2) // Target landmarks
	{
		num_landmarks = mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
	}
	else // Handle landmarks
	{
		num_landmarks = mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	}
	
		if (num_landmarks == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark of this type exist...");
			msgBox.exec();
			return;
		}
		
		cout << "Save Landmark Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveLandmarksDialogReaction::showSaveLandmarksDialog(this->m_lmktype);
		
		
		
}

//-----------------------------------------------------------------------------
void mqSaveLandmarksDialogReaction::showSaveLandmarksDialog(int lmktype)
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE Landmark, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSaveLandmarksDialog SaveLandmark_dialog(mqCoreUtilities::mainWidget(), lmktype);
  SaveLandmark_dialog.exec();
}
