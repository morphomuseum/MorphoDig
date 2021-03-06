/*=========================================================================

   Program: MorphoDig
   Module:   mqSaveDataReaction

========================================================================*/
#ifndef mqSaveDataReaction_h
#define mqSaveDataReaction_h

#include "mqReaction.h"
#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqSaveDataReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqSaveDataReaction(QAction* parent,  int _mode);

  //mode
  
  
  /* void SavePLY();
   void SaveSTL();
   void SaveVTP();*/
   void SavePOS(int mode);
   void SaveActiveScalarSummary();
   void SaveSelectedSurfaceScalars();
   void SaveCURInfos();
   void SaveShapeMeasures(int mode);
   void SaveMeshSize();
   void SaveORI();
   void ExportAvizoLandmarks();
   void ExportLandmarksAsFlags();
   
   
   
   
  
protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
	  if (this->Mode == 1)
	  {
		  this->ExportAvizoLandmarks();
	  }
	else if (this->Mode == 2)
	{
		this->ExportLandmarksAsFlags();
	}
	else if (this->Mode == 3)
	{
		
	}
	else if (this->Mode == 4)
	{
	
	}
	else if (this->Mode == 5)
	{
	
	}
	else if (this->Mode == 6)
	{
		
	}
	else if (this->Mode == 7)
	{
		this->SaveORI();
	}	
	else if (this->Mode == 9)
	{
		  this->SavePOS(1);//save pos for volumes
	}
	else if (this->Mode == 8)
	{
		this->SavePOS(0);//save pos for surfaces
	}	
	
	else if (this->Mode == 13)
	{
		
	}
	else if (this->Mode == 14)
	{
		
	}
	else if (this->Mode == 15)
	{
		
	}
	else if (this->Mode == 16)
	{
		
	}
	else if (this->Mode == 17)
	{
		this->SaveCURInfos();
	}
	else if (this->Mode == 18)
	{
		this->SaveShapeMeasures(1);
	}
	else if (this->Mode == 19)
	{
		this->SaveShapeMeasures(2);
	}
	else if (this->Mode == 20)
	{
		this->SaveShapeMeasures(3);
	}
	else if (this->Mode == 21)
	{
		this->SaveMeshSize();
	}
	else if (this->Mode == 22)
	{
		this->SaveActiveScalarSummary();
	}
	else if (this->Mode == 23)
	{
		this->SaveSelectedSurfaceScalars();
	}
	
  }

private:
	Q_DISABLE_COPY(mqSaveDataReaction)
	QMainWindow* MainWindow;
  int Mode;
};

#endif
