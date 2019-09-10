/*=========================================================================

   Program: MorphoDig
   Module:   mqOpenDataReaction

========================================================================*/
#ifndef mqOpenDataReaction_h
#define mqOpenDataReaction_h

#include "mqReaction.h"
#include <QMainWindow>


/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqOpenDataReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqOpenDataReaction(QAction* parent,  int _mode);

  //mode
   void OpenData();
   void OpenNTW();
   void OpenMesh();
   void OpenPOS(int mode);
   void OpenPOSTrans(int mode);
   void OpenLandmark(int mode);
   void OpenRAW();
   void OpenFLG();
   void OpenCUR();
   void OpenSTV();
   void OpenMAP();
   void OpenTAGMAP();
   void OpenORI();
   void OpenVolume();
   void ImportMask();
   void OpenTIFF3D();
   void OpenTIFF2DStack();
   void OpenDicom2DStack();
   void OpenDicomFolder();
   void ImportAvizoLandmarks();
   
  
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
    if (this->Mode ==0)
    {
      //mqOpenDataReaction::OpenData();
		this->OpenData();
    }
    else if (this->Mode == 1)
    {
     this->OpenNTW();
    }
	else if (this->Mode == 2)
	{
		this->OpenMesh();
	}
	else if (this->Mode == 3)
	{
		this->OpenLandmark(0);
	}
	else if (this->Mode == 4)
	{
		this->OpenLandmark(1);
	}
	else if (this->Mode == 5)
	{
		this->OpenCUR();
	}
	else if (this->Mode == 6)
	{
		this->OpenFLG();
	}
	else if (this->Mode == 7)
	{
		this->OpenORI();
	}	
	else if (this->Mode == 9)
	{
		this->OpenTAGMAP();
	}
	else if (this->Mode == 8)
	{
		this->OpenPOS(1);
	}
	else if (this->Mode == 10)
	{
		this->OpenPOSTrans(1);
	}
	else if (this->Mode == 11)
	{
		this->OpenPOS(2);
	}
	else if (this->Mode == 12)
	{
		this->OpenPOSTrans(2);
	}
	else if (this->Mode == 13)
	{
		this->OpenSTV();
	}
	else if (this->Mode == 14)
	{
		this->OpenLandmark(2);
	}
	else if (this->Mode == 15)
	{
		this->OpenLandmark(3);
	}
	else if (this->Mode == 16)
	{
		this->OpenSTV();
	}
	else if (this->Mode == 17)
	{
		this->OpenMAP();
	}
	else if (this->Mode == 18)
	{
		this->OpenVolume();
	}
	else if (this->Mode == 19)
	{
		this->ImportAvizoLandmarks();
	}
	else if (this->Mode == 20)
	{
		this->OpenRAW();
	}
	else if (this->Mode == 21)
	{
		this->OpenTIFF3D();
	}
	else if (this->Mode == 22)
	{
		this->OpenTIFF2DStack();
	}
	else if (this->Mode == 23)
	{
		this->OpenDicomFolder();
	}
	else if (this->Mode == 24)
	{
		this->OpenDicom2DStack();
	}
	else if (this->Mode == 25)
	{
	this->ImportMask();
	}
  }

private:
	Q_DISABLE_COPY(mqOpenDataReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
