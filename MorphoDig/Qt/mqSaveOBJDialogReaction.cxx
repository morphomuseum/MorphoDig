/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveOBJDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveOBJDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveOBJDialogReaction::mqSaveOBJDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

void mqSaveOBJDialogReaction::onTriggered()
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
		int cpt = mqMorphoDigCore::instance()->GetNumerOfNonRGBArraysOfSelectedObjects();
		if (cpt > 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Warning: the arrays contained in the selected surfaces (scalars, tags, and color arrays) can not be saved in OBJ files. Continue?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { return; }

		}

		cout << "Save OBJ Dialog Triggered!" << endl;
		QString fileName;
		if (num_selected_meshes == 1)
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();

			fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save OBJ files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("OBJ file (*.obj)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		}
		else
		{
			fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save OBJ files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
				tr("OBJ file (*.obj)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);

		}

		cout << fileName.toStdString();
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

		mqSaveOBJDialogReaction::showSaveOBJDialog(fileName);
}

//-----------------------------------------------------------------------------
void mqSaveOBJDialogReaction::showSaveOBJDialog(QString fileName)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE OBJ, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveOBJDialog SaveOBJ_dialog(mqCoreUtilities::mainWidget(), fileName);
  SaveOBJ_dialog.exec();
}
