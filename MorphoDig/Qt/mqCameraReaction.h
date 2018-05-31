/*=========================================================================

   Program: MorphoDig
   Module:   mqCameraReaction

========================================================================*/
#ifndef mqCameraReaction_h
#define mqCameraReaction_h

#include "mqReaction.h"
#include <QMainWindow>


/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqCameraReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqCameraReaction(QAction* parent,  int _mode);

  //mode
   void CameraFront();//0
   void CameraBack();//1
   void CameraLeft();//2
   void CameraRight();//3
   void CameraAbove();//4
  void  CameraBelow();//5

  
  void CameraCentreOfMassToggle();
  void CameraOrthoPerspectiveToggle();
  
   
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
    if (this->Mode ==0)
    {
      	this->CameraFront();
    }
    else if (this->Mode == 1)
    {
		this->CameraBack();
    }
	else if (this->Mode == 2)
	{
		this->CameraLeft();
	}
	else if (this->Mode == 3)
	{
		this->CameraRight();
	}
	else if (this->Mode == 4)
	{
		this->CameraAbove();
	}
	else if (this->Mode == 5)
	{
		this->CameraBelow();
	}
	else if (this->Mode == 6)
	{
		this->CameraCentreOfMassToggle();
	}
	else if (this->Mode == 7)
	{
		this->CameraOrthoPerspectiveToggle();
	}
	
  }

private:
	Q_DISABLE_COPY(mqCameraReaction)
	QMainWindow* MainWindow;
  int Mode;
  QAction *parentObject;
};

#endif
