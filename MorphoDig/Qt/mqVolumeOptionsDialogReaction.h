/*=========================================================================

   Program: MorphoDig
   Module:    mqVolumeOptionsDialogReaction.h


========================================================================*/
#ifndef mqVolumeOptionsDialogReaction_h
#define mqVolumeOptionsDialogReaction_h

#include "mqReaction.h"
#include "mqVolumeOptionsDialog.h"
/**
* @ingroup Reactions
* mqVolumeOptionsDialogReaction used to show the standard volume options
*/
class  mqVolumeOptionsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqVolumeOptionsDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showVolumeOptionsDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();

private:
	Q_DISABLE_COPY(mqVolumeOptionsDialogReaction)
	
};

#endif
