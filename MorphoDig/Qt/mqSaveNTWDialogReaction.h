/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveNTWDialogReaction.h


========================================================================*/
#ifndef mqSaveNTWDialogReaction_h
#define mqSaveNTWDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqSaveNTWDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveNTWDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveNTWDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqSaveNTWDialogReaction)

};

#endif
