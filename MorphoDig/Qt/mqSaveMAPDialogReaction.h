/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveMAPDialogReaction.h


========================================================================*/
#ifndef mqSaveMAPDialogReaction_h
#define mqSaveMAPDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqSaveMAPDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveMAPDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveMAPDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqSaveMAPDialogReaction)

};

#endif
