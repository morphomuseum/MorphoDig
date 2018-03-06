/*=========================================================================

  Program:   ParaView
  Module:    vtkUndoStack.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkUndoStack.h"
#include "vtkUndoStackBuilder.h"

#include "vtkUndoStackInternal.h"
#include "mqMorphoDigCore.h"
#include <vtkCommand.h>
#include <vtkObjectFactory.h>


vtkStandardNewMacro(vtkUndoStack);
//-----------------------------------------------------------------------------
vtkUndoStack::vtkUndoStack()
{
  this->Internal = new vtkUndoStackInternal;
  this->InUndo = false;
  this->InRedo = false;
  this->StackDepth = 20;
  this->NestedCount = 0;
  this->GlobalCount = 0;
  this->UndoStackBuilder = vtkSmartPointer<vtkUndoStackBuilder>::New();
  this->UndoStackBuilder->SetUndoStack(this);
}

//-----------------------------------------------------------------------------
vtkUndoStack::~vtkUndoStack()
{
  delete this->Internal;
  
}

//-----------------------------------------------------------------------------
void vtkUndoStack::Clear()
{
  this->Internal->UndoStack.clear();
  this->Internal->RedoStack.clear();  
  this->Modified();
}

int vtkUndoStack::GetGlobalCount()
{
	return this->GlobalCount;
}
void vtkUndoStack::beginUndoSet(std::string &label)
{
	this->GlobalCount++;
	
	if (this->NestedCount == 0)
	{
		this->UndoStackBuilder->Begin(label.c_str(), this->GlobalCount);
	}

	this->NestedCount++;
}

//-----------------------------------------------------------------------------
void vtkUndoStack::endUndoSet()
{
	if (this->NestedCount == 0)
	{
		cout << "endUndoSet called without a beginUndoSet." << endl;
		return;
	}

	this->NestedCount--;
	if (this->NestedCount == 0)
	{
		this->UndoStackBuilder->EndAndPushToStack();
	}
	
}

//-----------------------------------------------------------------------------
void vtkUndoStack::Push(const char* label, int mCount)
{
  this->Internal->RedoStack.clear();
  
  //int Count = 2;//MorphoDig::instance()->GetUndoCount()+1;
  int Count = mCount;

  // détruit ce qui est trop vieux dans le vector!
  while (this->Internal->UndoStack.size() >= static_cast<unsigned int>(this->StackDepth) &&
    this->StackDepth > 0)
  {
	  
	 int Count = this->Internal->UndoStack.front().UndoCount;	  
	 cout << "ERASE COUNT " << Count << endl;
	 mqMorphoDigCore::instance()->Erase(Count);
	 this->Internal->UndoStack.erase(this->Internal->UndoStack.begin());

	// to do : récupérer l'ID et faire un appel pour virer dans les objets les choses correspondant à ces ids!
    
  }
  this->Internal->UndoStack.push_back(vtkUndoStackInternal::Element(label, Count));
  
  this->Modified();
}

//-----------------------------------------------------------------------------
unsigned int vtkUndoStack::GetNumberOfUndoSets()
{
  return static_cast<unsigned int>(this->Internal->UndoStack.size());
}

//-----------------------------------------------------------------------------
unsigned int vtkUndoStack::GetNumberOfRedoSets()
{
  return static_cast<unsigned int>(this->Internal->RedoStack.size());
}

//-----------------------------------------------------------------------------
const char* vtkUndoStack::GetUndoSetLabel(unsigned int position)
{
  if (position >= this->Internal->UndoStack.size())
  {
    return NULL;
  }
  position = (static_cast<unsigned int>(this->Internal->UndoStack.size()) - position) - 1;
  return this->Internal->UndoStack[position].Label.c_str();
}

//-----------------------------------------------------------------------------
const char* vtkUndoStack::GetRedoSetLabel(unsigned int position)
{
  if (position >= this->Internal->RedoStack.size())
  {
    return NULL;
  }
  position = (static_cast<unsigned int>(this->Internal->RedoStack.size()) - position) - 1;
  return this->Internal->RedoStack[position].Label.c_str();
}

//-----------------------------------------------------------------------------
int vtkUndoStack::Undo()
{
  cout << "Inside vtkUndoStack Undo" << endl;
  if (this->Internal->UndoStack.empty())
  {
    return 0;
  }
  this->InUndo = true;
  //this->InvokeEvent(vtkCommand::StartEvent);
  //int status = this->Internal->UndoStack.back().UndoSet.GetPointer()->Undo();
  cout << "Undo " << this->Internal->UndoStack.back().UndoCount << ": "<< this->Internal->UndoStack.back().Label.c_str() << endl;
  // ici : faire l'appel global à undo de ce count là!!  
  mqMorphoDigCore::instance()->Undo(this->Internal->UndoStack.back().UndoCount);
  this->PopUndoStack();
  
  this->InvokeEvent(vtkCommand::EndEvent);
  this->InUndo = false;
  return 1;
}

//-----------------------------------------------------------------------------
int vtkUndoStack::Redo()
{
  if (this->Internal->RedoStack.empty())
  {
    return 0;
  }
  this->InRedo = true;
  this->InvokeEvent(vtkCommand::StartEvent);
  //int status = this->Internal->RedoStack.back().UndoSet.GetPointer()->Redo();
  
  // récupérer le count de this->Internal->RedoStack.back() et son label
  // ici : faire l'appel global à redo de ce count là!!
  mqMorphoDigCore::instance()->Redo(this->Internal->RedoStack.back().UndoCount);
  this->PopRedoStack();
  
  this->InvokeEvent(vtkCommand::EndEvent);
  this->InRedo = false;
  return 1;
}

//-----------------------------------------------------------------------------
void vtkUndoStack::PopUndoStack()
{
  if (this->Internal->UndoStack.empty())
  {
    return;
  }
  this->Internal->RedoStack.push_back(this->Internal->UndoStack.back());
  this->Internal->UndoStack.pop_back();
  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkUndoStack::PopRedoStack()
{
  if (this->Internal->RedoStack.empty())
  {
    return;
  }
  this->Internal->UndoStack.push_back(this->Internal->RedoStack.back());
  this->Internal->RedoStack.pop_back();
  this->Modified();
}



//-----------------------------------------------------------------------------
void vtkUndoStack::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "InUndo: " << this->InUndo << endl;
  os << indent << "InRedo: " << this->InRedo << endl;
  os << indent << "StackDepth: " << this->StackDepth << endl;
}
