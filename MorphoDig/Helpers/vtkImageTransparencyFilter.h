/*=========================================================================

  Program:   Taken from ParaView
  Module:    vtkImageTransparencyFilter.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

	 This software is distributed WITHOUT ANY WARRANTY; without even
	 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	 PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtkImageTransparencyFilter_h
#define vtkImageTransparencyFilter_h

#include <vtkImageAlgorithm.h>


class  vtkImageTransparencyFilter : public vtkImageAlgorithm
{
public:
	static vtkImageTransparencyFilter* New();
	vtkTypeMacro(vtkImageTransparencyFilter, vtkImageAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent) override;

	virtual int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
		vtkInformationVector* outputVector) override;

protected:
	// Override to set two inputs
	int FillInputPortInformation(int port, vtkInformation* info) override;

protected:
	vtkImageTransparencyFilter();
	~vtkImageTransparencyFilter();

private:
	vtkImageTransparencyFilter(const vtkImageTransparencyFilter&) = delete;
	void operator=(const vtkImageTransparencyFilter&) = delete;
};

#endif