

#ifndef VTK_BEZIER_CURVE_SOURCE_H
#define VTK_BEZIER_CURVE_SOURCE_H

#include <vtkPolyDataAlgorithm.h>
#include "vtkLMActorCollection.h"

class vtkImageData;
class vtkTransform;

class vtkBezierCurveSource : public vtkPolyDataAlgorithm
{
public:
  static vtkBezierCurveSource *New();
  vtkTypeMacro(vtkBezierCurveSource, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkPolyData* GetNodeHandleConnexionOutput();// Output 2
  vtkPolyData* GetSelectedBezierCurveOutput();// Output 1
  vtkPolyData* GetBezierCurveOutput(); // same as GetOutput()

  void SetNodes(vtkSmartPointer<vtkLMActorCollection> Nodes);
  void SetHandles(vtkSmartPointer<vtkLMActorCollection> Handles);
  int GetCurveSegmentNumber();
  int IsSegmentSelected(int seg);
  double GetCurveSegmentLength(int seg);
protected:

  vtkBezierCurveSource();
  ~vtkBezierCurveSource();
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkBezierCurveSource(const vtkBezierCurveSource&);  // Not implemented.
  void operator=(const vtkBezierCurveSource&);  // Not implemented.
  
  void UpdadeNodeHandleConnexionPolyData(vtkPolyData* pd);
  void UpdateBezierCurvePolyData(vtkPolyData* pd, int selected_segments);
  

private:
	vtkSmartPointer<vtkLMActorCollection> Nodes;
	vtkSmartPointer<vtkLMActorCollection> Handles;
  
};

#endif
