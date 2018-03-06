/*=========================================================================

Program:   ParaView
Module:    vtkSMUndoStackBuilder.cxx

Copyright (c) Kitware, Inc.
All rights reserved.
See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkUndoStackBuilder.h"

#include "vtkCommand.h"
#include "vtkObjectFactory.h"
//#include "vtkSMMessage.h"
#include "vtkUndoStack.h"
#include "vtkUndoElement.h"
#include "vtkUndoSet.h"
#include "vtkUndoStackInternal.h"



vtkStandardNewMacro(vtkUndoStackBuilder);
vtkCxxSetObjectMacro(vtkUndoStackBuilder, UndoStack, vtkUndoStack);
//-----------------------------------------------------------------------------
vtkUndoStackBuilder::vtkUndoStackBuilder()
{
	this->UndoStack = 0;
	this->Label = NULL;
	this->Count = 0;
}

//-----------------------------------------------------------------------------
vtkUndoStackBuilder::~vtkUndoStackBuilder()
{
	
	this->SetLabel(NULL);
	this->SetUndoStack(0);
}
//-----------------------------------------------------------------------------
void vtkUndoStackBuilder::Begin(const char* label, int mCount)
{
	if (!this->Label)
	{
		this->SetLabel(label);
	}
	this->Count = mCount;
	
}

//-----------------------------------------------------------------------------
void vtkUndoStackBuilder::End()
{
	
}

//-----------------------------------------------------------------------------
void vtkUndoStackBuilder::PushToStack()
{	

	
	this->UndoStack->Push((this->Label ? this->Label : "Changes"), this->Count);
	
	this->InitializeUndoSet();
}

//-----------------------------------------------------------------------------
void vtkUndoStackBuilder::Clear()
{
	this->InitializeUndoSet();
}

//-----------------------------------------------------------------------------
void vtkUndoStackBuilder::InitializeUndoSet()
{
	this->SetLabel(NULL);
	this->Count = 0;
	
}


//-----------------------------------------------------------------------------
void vtkUndoStackBuilder::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
	os << indent << "UndoStack: " << this->UndoStack << endl;
}
