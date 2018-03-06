/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveFlagDialogReaction.h


========================================================================*/
#ifndef mqSaveFlagsDialogReaction_h
#define mqSaveFlagsDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqSaveFlagsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveFlagsDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveFlagsDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqSaveFlagsDialogReaction)

};

#endif
