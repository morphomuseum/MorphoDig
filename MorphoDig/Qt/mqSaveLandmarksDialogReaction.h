/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveLandmarkDialogReaction.h


========================================================================*/
#ifndef mqSaveLandmarksDialogReaction_h
#define mqSaveLandmarksDialogReaction_h


#include "mqReaction.h"

/**
* @ingroup Reactions
* mqSaveLandmarkDialogReaction used to show the standard Landmark Save dialog for the
* application.
*/
class  mqSaveLandmarksDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSaveLandmarksDialogReaction(QAction* parent, int lmktype);

  /**
  * Shows the SaveLandmark dialog for the application.
  */
  static void showSaveLandmarksDialog(int lmktype);

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered();


private:
	int m_lmktype;
  Q_DISABLE_COPY(mqSaveLandmarksDialogReaction)

};

#endif
