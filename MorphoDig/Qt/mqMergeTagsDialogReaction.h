/*=========================================================================

   Program: MorphoDig
   Module:    mqMergeTagsDialogReaction.h


========================================================================*/
#ifndef mqMergeTagsDialogReaction_h
#define mqMergeTagsDialogReaction_h

#include "mqReaction.h"
#include "mqMergeTagsDialog.h"
/**
* @ingroup Reactions
* mqMergeTagsDialogReaction 
* application.
*/
class  mqMergeTagsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqMergeTagsDialogReaction(QAction* parent);

  /**
  * Shows the merge tags dialog
  */  
  static void showMergeTagsDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqMergeTagsDialogReaction::showMergeTagsDialog(); }
  
	
	

private:
	Q_DISABLE_COPY(mqMergeTagsDialogReaction)
		
};

#endif
