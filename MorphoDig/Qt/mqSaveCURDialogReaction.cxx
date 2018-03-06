/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveCURDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveCURDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveCURDialogReaction::mqSaveCURDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

}

void mqSaveCURDialogReaction::onTriggered()
{
	
	vtkIdType num_CURN = 0;
	vtkIdType num_CURH = 0;
	
		num_CURN = mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
		num_CURH = mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	
	
		if (num_CURN == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Curve Node exist...");
			msgBox.exec();
			return;
		}
		if (num_CURH == 0) {
			QMessageBox msgBox;
			msgBox.setText("No Curve Handle exist...");
			msgBox.exec();
			return;
		}
		if (num_CURH != num_CURN) {
			QMessageBox msgBox;
			msgBox.setText("Number of curve nodes differs from number of curve handles... impossible to save .CUR file!");
			msgBox.exec();
			return;
		}
		cout << "Save CUR Dialog Triggered!" << endl;


		

		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveCURDialogReaction::showSaveCURDialog();
		
		/*QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save CUR files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
			tr("CUR file (*.lmk *.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);


		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

		*/
		
}

//-----------------------------------------------------------------------------
void mqSaveCURDialogReaction::showSaveCURDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE CUR, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveCURDialog SaveCUR_dialog(mqCoreUtilities::mainWidget());
  SaveCUR_dialog.exec();
}
