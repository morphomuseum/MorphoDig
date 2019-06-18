/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveVTKDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveVTKDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveVTKDialogReaction::mqSaveVTKDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

void mqSaveVTKDialogReaction::onTriggered()
{
	
		vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}
		else if (num_selected_meshes > 1)
		{
			QMessageBox msgBox;
			msgBox.setText("More than one surface are currently selected. Do you want to save all selected surfaces into on single surface file?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { return; }

		}
		cout << "Save VTK Dialog Triggered!" << endl;
		
		
		
		QString fileName;
		if (num_selected_meshes == 1)
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			//+QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
			 fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save VTK PolyData files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("VTK XML PolyData file (*.vtp);; VTK PolyData file (*.vtk);; (*.*)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		}
		else
		{
			 fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save VTK PolyData files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
				tr("VTK XML PolyData file (*.vtp);; VTK PolyData file (*.vtk);; (*.*)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);

		}

		cout << "Filename chosen:"<< fileName.toStdString()<<endl;
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


		/*if (QFile::exists(fileName))
		{
		QMessageBox msgBox;
		msgBox.setText("Overwrite?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) { return; }



		}*/

		mqSaveVTKDialogReaction::showSaveVTKDialog(fileName);
}

//-----------------------------------------------------------------------------
void mqSaveVTKDialogReaction::showSaveVTKDialog(QString fileName)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE VTK, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveVTKDialog SaveVTK_dialog(mqCoreUtilities::mainWidget(), fileName);
  SaveVTK_dialog.exec();
}
