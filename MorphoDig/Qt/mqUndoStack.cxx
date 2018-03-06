/*=========================================================================

   Program: MorphoDig
   Module:    copied and adapted from Paraview pqUndoStack.cxx

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "mqUndoStack.h"

#include "mqMorphoDigCore.h"

#include "vtkUndoStack.h"
#include <vtkEventQtSlotConnect.h>
#include <QPointer>
#include <QtDebug>

//-----------------------------------------------------------------------------
class mqUndoStack::mqImplementation
{
public:
  mqImplementation() { this->NestedCount = 0; }
  vtkSmartPointer<vtkUndoStack> UndoStack;
  vtkSmartPointer<vtkEventQtSlotConnect> VTKConnector;
  
  int NestedCount;
};

//-----------------------------------------------------------------------------
mqUndoStack::mqUndoStack( QObject* _parent /*=null*/)
  : QObject(_parent)
{
  this->Implementation = new mqImplementation();
  this->Implementation->UndoStack = vtkSmartPointer<vtkUndoStack>::New();

 

  this->Implementation->VTKConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  this->Implementation->VTKConnector->Connect(this->Implementation->UndoStack,
    vtkCommand::ModifiedEvent, this, SLOT(onStackChanged()), NULL, 1.0);
}

//-----------------------------------------------------------------------------
mqUndoStack::~mqUndoStack()
{
  delete this->Implementation;
}


//-----------------------------------------------------------------------------
bool mqUndoStack::canUndo()
{
  return this->Implementation->UndoStack->CanUndo();
}

//-----------------------------------------------------------------------------
bool mqUndoStack::canRedo()
{
  return this->Implementation->UndoStack->CanRedo();
}

//-----------------------------------------------------------------------------
const QString mqUndoStack::undoLabel()
{
  return this->Implementation->UndoStack->CanUndo()
    ? this->Implementation->UndoStack->GetUndoSetLabel(0)
    : QString();
}

//-----------------------------------------------------------------------------
const QString mqUndoStack::redoLabel()
{
  return this->Implementation->UndoStack->CanRedo()
    ? this->Implementation->UndoStack->GetRedoSetLabel(0)
    : QString();
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void mqUndoStack::onStackChanged()
{
  bool can_undo = false;
  bool can_redo = false;
  QString undo_label;
  QString redo_label;
  cout << "on stack changed!" << endl;
  if (this->Implementation->UndoStack->CanUndo())
  {
    can_undo = true;
    undo_label = this->Implementation->UndoStack->GetUndoSetLabel(0);
  }
  if (this->Implementation->UndoStack->CanRedo())
  {
    can_redo = true;
    redo_label = this->Implementation->UndoStack->GetRedoSetLabel(0);
  }

  emit this->stackChanged(can_undo, undo_label, can_redo, redo_label);
  emit this->canUndoChanged(can_undo);
  emit this->canRedoChanged(can_redo);
  emit this->undoLabelChanged(undo_label);
  emit this->redoLabelChanged(redo_label);
}

int mqUndoStack::GetGlobalCount()
{
	
		return this->Implementation->UndoStack->GetGlobalCount();
	
	
}
//-----------------------------------------------------------------------------
void mqUndoStack::beginUndoSet(std::string &label)
{
  if (this->Implementation->NestedCount == 0)
  {
    this->Implementation->UndoStack->beginUndoSet(label);
  }

  this->Implementation->NestedCount++;
}

//-----------------------------------------------------------------------------
void mqUndoStack::endUndoSet()
{
  if (this->Implementation->NestedCount == 0)
  {
    qDebug() << "endUndoSet called without a beginUndoSet.";
    return;
  }

  this->Implementation->NestedCount--;
  if (this->Implementation->NestedCount == 0)
  {
    this->Implementation->UndoStack->endUndoSet();
  }
}

//-----------------------------------------------------------------------------
void mqUndoStack::undo()
{
  cout << "mqUndoStack undo call!" << endl;
  this->Implementation->UndoStack->Undo();

 mqMorphoDigCore::instance()->Render();

  emit this->undone();
  cout << "end mqUndoStack undo call!" << endl;
}

//-----------------------------------------------------------------------------
void mqUndoStack::redo()
{
  this->Implementation->UndoStack->Redo();
  mqMorphoDigCore::instance()->Render();
 

  emit this->redone();
}


//-----------------------------------------------------------------------------
void mqUndoStack::clear()
{
  this->Implementation->UndoStack->Clear();  
  
}

