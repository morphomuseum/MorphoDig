/*=========================================================================

   Program: MorphoDig
   Module:   mqDisplayReaction

========================================================================*/
#ifndef mqDisplayReaction_h
#define mqDisplayReaction_h

#include "mqReaction.h"
#include <QMainWindow>


/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqDisplayReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqDisplayReaction(QAction* parent,  int _mode);

  //mode
   void GridToggle();//0
   void OrientationHelperToggle();//1
   void RendererAnaglyphToggle();//2
   
  
   
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
    if (this->Mode ==0)
    {
      	this->GridToggle();
    }
    else if (this->Mode == 1)
    {
		this->OrientationHelperToggle();
    }
	else if (this->Mode == 2)
	{
		this->RendererAnaglyphToggle();
	}
	
	
  }

private:
	Q_DISABLE_COPY(mqDisplayReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
