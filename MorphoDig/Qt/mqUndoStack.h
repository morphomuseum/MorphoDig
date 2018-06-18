/*=========================================================================

   Program: MorphoDig
   Module:    Modified and adapted from ParaView pqUndoStack.h

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

========================================================================*/
#ifndef mqUndoStack_h
#define mqUndoStack_h

#include "mqMorphoDigCore.h"
#include <QObject>

//class vtkUndoStack;
#include "vtkUndoStack.h"


class mqUndoStack : public QObject
{
  Q_OBJECT
public:
  
  mqUndoStack(QObject* parent = NULL);
  virtual ~mqUndoStack();

  /**
  * returns if it's possible to undo.
  */
  bool canUndo();
  int GetGlobalCount();

  /**
  * returns if it's possible to redo.
  */
  bool canRedo();

  /**
  * returns the undo label.
  */
  const QString undoLabel();

  /**
  * returns the redo label.
  */
  const QString redoLabel();

  

  

  
 

public slots:
void beginUndoSet(std::string &label);
  void endUndoSet();

  /**
  * triggers Undo.
  */
  void undo();

  /**
  * triggers Redo.
  */
  void redo();

  /**
  * Clears undo stack.
  */
  void clear();

  

signals:
  /**
  * Fired to notify interested parites that the stack has changed.
  * Has information to know the status of the top of the stack.
  */
  void stackChanged(bool canUndo, QString undoLabel, bool canRedo, QString redoLabel);

  void canUndoChanged(bool);
  void canRedoChanged(bool);
  void undoLabelChanged(const QString&);
  void redoLabelChanged(const QString&);

  // Fired after undo.
  void undone();
  // Fired after redo.
  void redone();

private slots:
  void onStackChanged();

private:
  class mqImplementation;
  mqImplementation* Implementation;
};



inline int BEGIN_UNDO_SET(std::string & name)
{
	mqUndoStack* usStack = mqMorphoDigCore::instance()->getUndoStack();
	if (usStack)
	{
		usStack->beginUndoSet(name);
		int Count = usStack->GetGlobalCount();
		return Count;
	}
	else
	{
		return 0;
	}
}

inline void END_UNDO_SET()
{
	mqUndoStack* usStack = mqMorphoDigCore::instance()->getUndoStack();
	if (usStack)
	{
		usStack->endUndoSet();
	}
}

/*
inline void BEGIN_UNDO_SET(const QString& name)
{
  pqUndoStack* usStack = pqApplicationCore::instance()->getUndoStack();
  if (usStack)
  {
    usStack->beginUndoSet(name);
  }
}

inline void END_UNDO_SET()
{
  pqUndoStack* usStack = pqApplicationCore::instance()->getUndoStack();
  if (usStack)
  {
    usStack->endUndoSet();
  }
}

*/

#endif
