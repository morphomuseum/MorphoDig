/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqSaveMHDMHADialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSaveMHDMHADialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSaveMHDMHADialogReaction::mqSaveMHDMHADialogReaction(QAction* parentObject, int _mode)
  : Superclass(parentObject)
{
	this->Mode = _mode; //0 save volume  //1 save mask
}

void mqSaveMHDMHADialogReaction::onTriggered()
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
		if (this->Mode == 1)
		{
			vtkMDVolume *myVolume = mqMorphoDigCore::instance()->GetFirstSelectedVolume();
			if (myVolume != NULL)
			{
				if (myVolume->MaskWorthSaving() == 0)
				{
					QMessageBox msgBox;
					msgBox.setText("Mask is empty or was not even initialized. Please select one volume with a mask worth being saved!");

					msgBox.exec();
					return;
				}
			}
		}
		
		
		cout << "Save MHDMHA Dialog Triggered!" << endl;
		QString fileName;
		if (this->Mode == 0)
		{

			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save MHD or MHA files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("MHD (*.mhd);; MHA (*.mha);; (*.*)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		}
		else
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save MHD or MHA files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str()+"_MSK",
				tr("MHD (*.mhd);; MHA (*.mha);; (*.*)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);
		}
		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

		

		mqSaveMHDMHADialogReaction::showSaveMHDMHADialog(fileName, this->Mode);
}

//-----------------------------------------------------------------------------
void mqSaveMHDMHADialogReaction::showSaveMHDMHADialog(QString fileName, int mode)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE MHDMHA, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSaveMHDMHADialog SaveMHDMHA_dialog(mqCoreUtilities::mainWidget(), fileName, mode);
  SaveMHDMHA_dialog.exec();
}
