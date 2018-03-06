/*=========================================================================

   Program: MorphoDig
   Module:   mqChangeNodeReaction

========================================================================*/
#ifndef mqChangeNodeReaction_h
#define mqChangeNodeReaction_h

#include "mqReaction.h"
#include <QMainWindow>


/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqChangeNodeReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqChangeNodeReaction(QAction* parent,  int _mode);

  //mode
  
   
   void ChangeSelectedNodes(int new_node_type);
   void MoveCurveHandles();
   static void showMoveCurveHandlesDialog();
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
	  if (this->Mode < 4)
	  {
		  int new_node_type = this->Mode;
		  this->ChangeSelectedNodes(new_node_type);
	  }
	  else if (this->Mode == 4)
	  {
		  this->MoveCurveHandles();
	  }
  }

private:
	Q_DISABLE_COPY(mqChangeNodeReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
