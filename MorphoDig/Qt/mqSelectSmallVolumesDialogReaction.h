/*=========================================================================

   Program: MorphoDig
   Module:    mqSelectSmallVolumesDialogReaction.h


========================================================================*/
#ifndef mqSelectSmallVolumesDialogReaction_h
#define mqSelectSmallVolumesDialogReaction_h

#include "mqReaction.h"
#include "mqSelectSmallVolumesDialog.h"
/**
* @ingroup Reactions
* mqSelectSmallVolumesDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqSelectSmallVolumesDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSelectSmallVolumesDialogReaction(QAction* parent);

  /**
  * Shows the select small volumes dialog for the application.
  */
  static void showSelectSmallVolumesDialog(mqSelectSmallVolumesDialog *SelectSmallVolumes_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqSelectSmallVolumesDialogReaction::showSelectSmallVolumesDialog(this->SelectSmallVolumes_dialog); }

private:
	Q_DISABLE_COPY(mqSelectSmallVolumesDialogReaction)
		mqSelectSmallVolumesDialog *SelectSmallVolumes_dialog;
};

#endif
