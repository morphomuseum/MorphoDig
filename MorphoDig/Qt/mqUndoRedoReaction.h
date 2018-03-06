/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqUndoRedoReaction.h

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
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
#ifndef mqUndoRedoReaction_h
#define mqUndoRedoReaction_h

#include "mqReaction.h"

class mqUndoStack;

/**
* @ingroup Reactions
* Reaction for application undo-redo.
*/
class mqUndoRedoReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  /**
  * if \c undo is set to true, then this behaves as an undo-reaction otherwise
  * as a redo-reaction.
  */
  mqUndoRedoReaction(QAction* parent, bool undo);

  /**
  * undo.
  */
  static void undo();

  /**
  * redo.
  */
  static void redo();

  /**
  * Clear stack
  */
  static void clear();

protected slots:
  void enable(bool);
  void setLabel(const QString& label);
  //@@TODO
  void setUndoStack(mqUndoStack*);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered()
  {
    if (this->Undo)
    {
      mqUndoRedoReaction::undo();
    }
    else
    {
      mqUndoRedoReaction::redo();
    }
  }

private:
  Q_DISABLE_COPY(mqUndoRedoReaction)

  bool Undo;
};

#endif
