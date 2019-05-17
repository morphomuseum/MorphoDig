/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveVTIDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveVTIDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveVTIDialogReaction::mqSaveVTIDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

void mqSaveVTIDialogReaction::onTriggered()
{
	
		vtkIdType num_selected_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
		if (num_selected_volumes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No volume selected. Please select at least one volume to use this option.");
			msgBox.exec();
			return;
		}
		else if (num_selected_volumes > 1)
		{
			QMessageBox msgBox;
			msgBox.setText("More than one volume are currently selected. Please only select one volume");
						
			msgBox.exec();
			 return; 

		}

		
		
		cout << "Save VTI Dialog Triggered!" << endl;
		QString fileName;		
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			 fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save VTI file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("VTI file (*.vti)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		

		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

		

		mqSaveVTIDialogReaction::showSaveVTIDialog(fileName);
}

//-----------------------------------------------------------------------------
void mqSaveVTIDialogReaction::showSaveVTIDialog(QString fileName)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE VTI, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveVTIDialog SaveVTI_dialog(mqCoreUtilities::mainWidget(), fileName);
  SaveVTI_dialog.exec();
}
