/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveLandmarkDialogReaction.h


========================================================================*/
#ifndef mqSelectLandmarkRangeDialogReaction_h
#define mqSelectLandmarkRangeDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveLandmarkDialogReaction used to show the standard Landmark Save dialog for the
* application.
*/
class  mqSelectLandmarkRangeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSelectLandmarkRangeDialogReaction(QAction* parent);

  /**
  * Shows the SaveLandmark dialog for the application.
  */
  static void showSelectLandmarkRangeDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
	int m_lmktype;
  Q_DISABLE_COPY(mqSelectLandmarkRangeDialogReaction)

};

#endif
