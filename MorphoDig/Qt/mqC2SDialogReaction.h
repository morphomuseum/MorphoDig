/*=========================================================================

   Program: MorphoDig
   Module:    mqC2SDialogReaction.h


========================================================================*/
#ifndef mqC2SDialogReaction_h
#define mqC2SDialogReaction_h

#include "mqReaction.h"
#include "mqC2SDialog.h"
/**
* @ingroup Reactions
* mqC2SDialogReaction 
* application.
*/
class  mqC2SDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqC2SDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
   static void showC2SDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqC2SDialogReaction::showC2SDialog(); }
  
	//For an non modal dialog
	

private:
	Q_DISABLE_COPY(mqC2SDialogReaction)
		
};

#endif
