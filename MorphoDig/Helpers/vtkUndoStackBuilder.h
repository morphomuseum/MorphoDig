/*=========================================================================

Program:   ParaView
Module:    vtkSMUndoStackBuilder.h

Copyright (c) Kitware, Inc.
All rights reserved.
See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
* @class   vtkSMUndoStackBuilder
* @brief   builds server manager undo sets and
* pushes them on the undo stack.
*
* vtkSMUndoStackBuilder records Server Manager changes that are undo/redo
* able and collects them. To begin recording such changes one must call
* Begin(). To end recording use End(). One can have multiple blocks
* of Begin-End before pushing the changes on the Undo Stack. To push all
* collected changes onto the Undo Stack as a single undoable step,
* use PushToStack().
* Applications can subclass vtkSMUndoStackBuilder to record GUI related
* changes and add them to the undo stack.
*/

#ifndef vtkUndoStackBuilder_h
#define vtkUndoStackBuilder_h

//#include "vtkSMMessageMinimal.h"          // needed for vtkSMMessage
#include <vtkObject.h>

//class vtkSMProxy;
class vtkUndoStack;
class vtkUndoElement;
class vtkUndoSet;


class  vtkUndoStackBuilder : public vtkObject
{
public:
	static vtkUndoStackBuilder* New();
	vtkTypeMacro(vtkUndoStackBuilder, vtkObject);
	void PrintSelf(ostream& os, vtkIndent indent);

	virtual void Begin(const char* label, int mCount);

	virtual void End();

	//@{
	/**
	* Convenience method call End(); PushToStack(); in that order.
	*/
	void EndAndPushToStack()
	{
		this->End();
		this->PushToStack();
	}
	//@}

	virtual void PushToStack();

	/**
	* Discard all recorded changes that haven't been pushed on the UndoStack.
	*/
	virtual void Clear();



	//@{
	/**
	* Get/Set the undo stack that this builder will build.
	*/
	vtkGetObjectMacro(UndoStack, vtkUndoStack);
	virtual void SetUndoStack(vtkUndoStack*);
	//@}

protected:
	vtkUndoStackBuilder();
	~vtkUndoStackBuilder();

	vtkUndoStack* UndoStack;
	char* Label;
	int Count;
	
	vtkSetStringMacro(Label);


	void InitializeUndoSet();

	

private:
	vtkUndoStackBuilder(const vtkUndoStackBuilder&) ;
	void operator=(const vtkUndoStackBuilder&) ;
};

#endif
