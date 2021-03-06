/*=========================================================================

   Program: MorphoDig
   Module:    mqChangeNodeReaction.cxx

========================================================================*/
#include "mqChangeNodeReaction.h"

#include "mqMorphoDigCore.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include "mqMoveCurveHandlesDialog.h"
#include "mqCoreUtilities.h"
#include "mqUndoStack.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <vtkPLYReader.h>
#include <vtkMath.h>
#include <vtkSTLReader.h>
#include <vtkCleanPolyData.h>
#include <vtkFloatArray.h>

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataNormals.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//-----------------------------------------------------------------------------
mqChangeNodeReaction::mqChangeNodeReaction(QAction* parentObject,  int _mode)
  : Superclass(parentObject)
{
  this->Mode = _mode;
  this->MainWindow = mqMorphoDigCore::instance()->GetMainWindow();
  
}

//@@TODO
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

void mqChangeNodeReaction::MoveCurveHandles()
{
	vtkIdType num_lmk = 0;
	num_lmk += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_lmk == 0) {
		QMessageBox msgBox;
		msgBox.setText("No curve handle selected. Please select at least one curve handle landmark to use this option.");
		msgBox.exec();
		return;
	}
	mqChangeNodeReaction::showMoveCurveHandlesDialog();
}

void mqChangeNodeReaction::ChangeSelectedNodes(int new_node_type)
{
	vtkIdType num_lmk = 0;
	num_lmk += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_lmk == 0) {
		QMessageBox msgBox;
		msgBox.setText("No curve node landmark selected. Please select at least one curve node landmark to use this option.");
		msgBox.exec();
		return;
	}
	vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
	myColl = mqMorphoDigCore::instance()->getNodeLandmarkCollection();


	myColl->InitTraversal();
	int coll_modified = 0;
	for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetLMNodeType(new_node_type);
			myActor->SetSelected(0);
			myActor->SetChanged(1);
			
			coll_modified = 1;
		}
	}
	if (coll_modified == 1)
	{
		myColl->Modified();
		mqMorphoDigCore::instance()->Render();
	}

}

void mqChangeNodeReaction::showMoveCurveHandlesDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur "Move curve handles", on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

	mqMoveCurveHandlesDialog MoveCurveHandles_dialog(mqCoreUtilities::mainWidget());
	MoveCurveHandles_dialog.exec();
}
