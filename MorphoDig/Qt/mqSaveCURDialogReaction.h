/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveCURDialogReaction.h


========================================================================*/
#ifndef mqSaveCURDialogReaction_h
#define mqSaveCURDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveFlagDialogReaction used to show the standard Flag Save dialog for the
* application.
*/
class  mqSaveCURDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveCURDialogReaction(QAction* parent);

  /**
  * Shows the SaveFlag dialog for the application.
  */
  static void showSaveCURDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:

  Q_DISABLE_COPY(mqSaveCURDialogReaction)

};

#endif
