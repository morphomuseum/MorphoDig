/*=========================================================================

   Program: MorphoDig
   Module:    mqScreenshotDialogReaction.cxx


========================================================================*/
#include "mqScreenshotDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqScreenshotDialog.h"
#include "mqReaction.h"
#include <QFileDialog>

//-----------------------------------------------------------------------------
mqScreenshotDialogReaction::mqScreenshotDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{

	

	
}


void mqScreenshotDialogReaction::onTriggered()
{

	/* {
	  vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	  if (num_selected_meshes == 0) {
		  QMessageBox msgBox;
		  msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		  msgBox.exec();
		  return;
	  }
	  mqScreenshotDialogReaction::showScreenshotDialog(this->Screenshot_dialog); 
  
  }*/
	QString fileName;


	mqMorphoDigCore::instance()->ComputeSelectedNamesLists();

	fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		tr("Take screenshot!"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + "Screenshot",
		tr("PNG (*.png)"), NULL
		//, QFileDialog::DontConfirmOverwrite
	);

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	mqScreenshotDialogReaction::showScreenshotDialog(fileName);
	

}


//-----------------------------------------------------------------------------
void mqScreenshotDialogReaction::showScreenshotDialog(QString fileName)
{
	mqScreenshotDialog Screenshot_dialog(mqCoreUtilities::mainWidget(), fileName);
	Screenshot_dialog.exec();
  
 
}
