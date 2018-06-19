/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveTAGMAPDialogReaction.h


========================================================================*/
#ifndef mqSaveTAGMAPDialogReaction_h
#define mqSaveTAGMAPDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqSaveTAGMAPDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveTAGMAPDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveTAGMAPDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqSaveTAGMAPDialogReaction)

};

#endif
