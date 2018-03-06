/*=========================================================================

  Program:   ParaView
  Module:    vtkUndoStackInternal.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include <vtkSmartPointer.h>
#include "vtkUndoSet.h"

#include <string>
#include <vector>

class vtkUndoStackInternal
{
public:
  struct Element
  {
    std::string Label;    
	int UndoCount;
    Element(const char* label, int Count)
    {
	 this->UndoCount = Count;
     this->Label = label;
     
    }
  };
  typedef std::vector<Element> VectorOfElements;
  VectorOfElements UndoStack;
  VectorOfElements RedoStack;
};
//****************************************************************************
// VTK-HeaderTest-Exclude: vtkUndoStackInternal.h
