/*=========================================================================

   Program: MorphoDig
   Module:    mqTagFromRGBDialogReaction.h


========================================================================*/
#ifndef mqTagFromRGBDialogReaction_h
#define mqTagFromRGBDialogReaction_h

#include "mqReaction.h"
#include "mqTagFromRGBDialog.h"
#include "mqMorphoDigCore.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqTagFromRGBDialogReaction used to create a Tag array from currently displayed RGB colors of selected surfaces
*/
class  mqTagFromRGBDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqTagFromRGBDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showTagFromRGBDialog(mqTagFromRGBDialog *TagFromRGB_dialog);

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
	  mqTagFromRGBDialogReaction::showTagFromRGBDialog(this->TagFromRGB_dialog); 
  
  }

private:
	Q_DISABLE_COPY(mqTagFromRGBDialogReaction)
		mqTagFromRGBDialog *TagFromRGB_dialog;
};

#endif
