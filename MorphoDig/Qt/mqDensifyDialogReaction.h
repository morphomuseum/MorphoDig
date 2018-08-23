/*=========================================================================

   Program: MorphoDig
   Module:    mqDensifyDialogReaction.h


========================================================================*/
#ifndef mqDensifyDialogReaction_h
#define mqDensifyDialogReaction_h

#include "mqReaction.h"
#include "mqDensifyDialog.h"
#include "mqMorphoDigCore.h"
#include <QMessageBox>
/**
* @ingroup Reactions
* mqDensifyDialogReaction used to Densify selected surfaces
*/
class  mqDensifyDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqDensifyDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showDensifyDialog(mqDensifyDialog *Densify_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { 
	
	  vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	  if (num_selected_meshes == 0) {
		  QMessageBox msgBox;
		  msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		  msgBox.exec();
		  return;
	  }
	  mqDensifyDialogReaction::showDensifyDialog(this->Densify_dialog); 
  
  
  }

private:
	Q_DISABLE_COPY(mqDensifyDialogReaction)
		mqDensifyDialog *Densify_dialog;
};

#endif
