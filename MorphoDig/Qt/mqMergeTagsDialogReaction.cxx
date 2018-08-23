/*=========================================================================

   Program: MorphoDig
   Module:    mqMergeTagsDialogReaction.cxx


========================================================================*/
#include "mqMergeTagsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMergeTagsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqMergeTagsDialogReaction::mqMergeTagsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	

	
}


//-----------------------------------------------------------------------------

void mqMergeTagsDialogReaction::showMergeTagsDialog()
{
	mqMergeTagsDialog mqMergeTags(mqCoreUtilities::mainWidget());
	mqMergeTags.exec();
  
  
}
