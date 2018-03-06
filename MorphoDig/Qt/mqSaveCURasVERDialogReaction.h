/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveCURasVERDialogReaction.h


========================================================================*/
#ifndef mqSaveCURasVERDialogReaction_h
#define mqSaveCURasVERDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqSaveCURasVERDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveCURasVERDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveCURasVERDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqSaveCURasVERDialogReaction)

};

#endif
