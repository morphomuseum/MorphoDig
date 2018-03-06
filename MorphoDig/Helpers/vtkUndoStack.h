/*=========================================================================

  Program:   ParaView
  Module:    vtkUndoStack.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/



#ifndef vtkUndoStack_h
#define vtkUndoStack_h



#include <vtkObject.h>
#include <vtkSmartPointer.h>
class vtkUndoStackInternal;
class vtkUndoStackBuilder;




class  vtkUndoStack : public vtkObject
{
public:
 

  static vtkUndoStack* New();
  vtkTypeMacro(vtkUndoStack, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  int GetGlobalCount();
  
  /**
   * Push an undo set on the Undo stack. This will clear
   * any sets in the Redo stack.
   */
  virtual void Push(const char* label, int mCount);

  /**
   * Returns the label for the set at the given Undo position.
   * 0 is the current undo set, 1 is the one preceding to the current one
   * and so on.
   * \returns NULL is no set exists at the given index, otherwise the label
   * for the change set.
   */
  const char* GetUndoSetLabel(unsigned int position);

  /**
   * Returns the label for the set at the given Redo position.
   * 0 is the next set to redo, 1 is the one after the next one
   * and so on.
   * \returns NULL is no set exists at the given index, otherwise the label
   * for the change set.
   */
  const char* GetRedoSetLabel(unsigned int position);

  /**
   * Returns the number of sets on the undo stack.
   */
  unsigned int GetNumberOfUndoSets();

  /**
   * Returns the number of sets on the undo stack.
   */
  unsigned int GetNumberOfRedoSets();

  /**
   * Returns if undo operation can be performed.
   */
  int CanUndo() { return (this->GetNumberOfUndoSets() > 0); }

  /**
   * Returns if redo operation can be performed.
   */
  int CanRedo() { return (this->GetNumberOfRedoSets() > 0); }


  /**
   * Performs an Undo using the set on the top of the undo stack. The set is poped from
   * the undo stack and pushed at the top of the redo stack.
   * Before undo begins, it fires vtkCommand::StartEvent and when undo completes,
   * it fires vtkCommand::EndEvent.
   * \returns the status of the operation.
   */
  virtual int Undo();

  /**
   * Performs a Redo using the set on the top of the redo stack. The set is poped from
   * the redo stack and pushed at the top of the undo stack.
   * Before redo begins, it fires vtkCommand::StartEvent and when redo completes,
   * it fires vtkCommand::EndEvent.
   * \returns the status of the operation.
   */
  virtual int Redo();

  /**
   * Pop the undo stack. The UndoElement on the top of the undo stack is popped from the
   * undo stack and pushed on the redo stack. This is same as Undo() except that the
   * vtkUndoElement::Undo() is not invoked.
   */
  void PopUndoStack();

  /**
   * Pop the redo stack. The UndoElement on the top of the redo stack is popped and then
   * pushed on the undo stack. This is same as Redo() except that vtkUndoElement::Redo()
   * is not invoked.
   */
  void PopRedoStack();

  /**
   * Clears all the undo/redo elements from the stack.
   */
  void Clear();

  //@{
  /**
   * Returns if the stack is currently being undone.
   */
  vtkGetMacro(InUndo, bool);
  //@}

  //@{
  /**
   * Returns if the stack is currently being redone.
   */
  vtkGetMacro(InRedo, bool);
  //@}

  //@{
  /**
   * Get set the maximum stack depth. As more entries are pushed on the stack,
   * if its size exceeds this limit then old entries will be removed.
   * Default is 10.
   */
  vtkSetClampMacro(StackDepth, int, 1, 100);
  vtkGetMacro(StackDepth, int);

  void beginUndoSet(std::string &label);
  void endUndoSet();

protected:
  vtkUndoStack();
  ~vtkUndoStack();
  //@}

  vtkUndoStackInternal* Internal;
  vtkSmartPointer<vtkUndoStackBuilder> UndoStackBuilder;
  int StackDepth;

private:
  vtkUndoStack(const vtkUndoStack&) ;
  void operator=(const vtkUndoStack&) ;
  int NestedCount;
  int GlobalCount;
  bool InUndo;
  bool InRedo;
};



/*inline int BEGIN_UNDO_SET(std::string & name)
{
	vtkUndoStack* usStack = vtkMorphoDigCore::instance()->getUndoStack();
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
	vtkUndoStack* usStack = vtkMorphoDigCore::instance()->getUndoStack();
	if (usStack)
	{
		usStack->endUndoSet();
	}
}*/
/*
inline void CLEAR_UNDO_STACK()
{
	vtkUndoStack* usStack = vtkMorphoDigCore::instance()->getUndoStack();
	if (usStack)
	{
		usStack->clear();
	}
}

inline void ADD_UNDO_ELEM(vtkUndoElement* elem)
{
	vtkUndoStack* usStack = vtkMorphoDigCore::instance()->getUndoStack();
	if (usStack)
	{
		usStack->addToActiveUndoSet(elem);
	}
}

inline void BEGIN_UNDO_EXCLUDE()
{
	vtkUndoStack* usStack = vtkMorphoDigCore::instance()->getUndoStack();
	if (usStack)
	{
		usStack->beginNonUndoableChanges();
	}
}

inline void END_UNDO_EXCLUDE()
{
	vtkUndoStack* usStack = vtkMorphoDigCore::instance()->getUndoStack();
	if (usStack)
	{
		usStack->endNonUndoableChanges();
	}
}*/
#endif
