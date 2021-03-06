/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSavePLYDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSavePLYDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSavePLYDialogReaction::mqSavePLYDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

void mqSavePLYDialogReaction::onTriggered()
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

		int cpt = mqMorphoDigCore::instance()->GetNumerOfNonRGBArraysOfSelectedObjects(1);
		if (cpt > 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Warning: the arrays contained in selected surfaces (scalars, tags, and color arrays except the \"RGB\" one) can not be saved in PLY files. Continue?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { return; }

		}

		cout << "Save PLY Dialog Triggered!" << endl;
		QString fileName;
		if (num_selected_meshes == 1)
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			 fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save PLY files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("Ply file (*.ply)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		}
		else
		{
			 fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save PLY files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
				tr("Ply file (*.ply)"), NULL
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

		mqSavePLYDialogReaction::showSavePLYDialog(fileName);
}

//-----------------------------------------------------------------------------
void mqSavePLYDialogReaction::showSavePLYDialog(QString fileName)
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE PLY, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSavePLYDialog SavePLY_dialog(mqCoreUtilities::mainWidget(), fileName);
  SavePLY_dialog.exec();
}
