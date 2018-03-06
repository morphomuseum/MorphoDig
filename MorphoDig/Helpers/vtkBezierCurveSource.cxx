/****************************************************************************
 
 **
 ****************************************************************************/

/**
  
*/

#include "vtkBezierCurveSource.h"
#include "vtkLMActor.h"

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkExecutive.h>
#include <vtkPolyDataMapper.h>
#include <vtkOutputWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <cmath>

#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

vtkStandardNewMacro(vtkBezierCurveSource);

void vtkBezierCurveSource::SetNodes(vtkSmartPointer<vtkLMActorCollection> Nodes)
{
	this->Nodes = Nodes;


}
void vtkBezierCurveSource::SetHandles(vtkSmartPointer<vtkLMActorCollection> Handles)
{
	this->Handles = Handles;
}

vtkBezierCurveSource::vtkBezierCurveSource()
{
	this->Nodes = NULL;
	this->Handles = NULL;




	this->SetNumberOfInputPorts(0);
	this->SetNumberOfOutputPorts(3);
	
	/*
	A second output could be the selected curve segments.
	this->SetNumberOfOutputPorts(2);
	vtkPolyData *output2 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(1, output2);
	output2->Delete();*/
	
	vtkPolyData *output2 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(1, output2);
	output2->Delete();
	vtkPolyData *output3 = vtkPolyData::New();
	this->GetExecutive()->SetOutputData(2, output3);
	output3->Delete();

}




vtkBezierCurveSource::~vtkBezierCurveSource()
{
 
}

void vtkBezierCurveSource::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkPolyDataAlgorithm::PrintSelf(os, indent);

 
  int index = 0;
 
}


vtkPolyData* vtkBezierCurveSource::GetNodeHandleConnexionOutput()
{
  if (this->GetNumberOfOutputPorts() < 3)
    {
    return NULL;
    }
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(2) );
}


vtkPolyData* vtkBezierCurveSource::GetSelectedBezierCurveOutput()
{
	if (this->GetNumberOfOutputPorts() < 2)
	{
		return NULL;
	}
	return vtkPolyData::SafeDownCast(this->GetExecutive()->GetOutputData(1));
}
vtkPolyData* vtkBezierCurveSource::GetBezierCurveOutput()
{
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(0) );
}




int vtkBezierCurveSource::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **vtkNotUsed(inputVector),
	vtkInformationVector *outputVector)
{


	//nhc= node handles connexion for selected bezier curve...
	vtkInformation *nhcOutInfo = outputVector->GetInformationObject(2);
	if (nhcOutInfo)
	{
		vtkPolyData *nhcOutput = vtkPolyData::SafeDownCast(nhcOutInfo->Get(vtkDataObject::DATA_OBJECT()));
		this->UpdadeNodeHandleConnexionPolyData(nhcOutput);
	}
	else
	{
		//cout << "no information object 2!" << endl;
	}


  //sbc for selected bezier curve...
  vtkInformation *sbcOutInfo = outputVector->GetInformationObject(1);
  if(sbcOutInfo)
    {
    vtkPolyData *sbcOutput = vtkPolyData::SafeDownCast(sbcOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateBezierCurvePolyData(sbcOutput, 1);
    }
	

  vtkInformation *bcOutInfo = outputVector->GetInformationObject(0);
  if(bcOutInfo)
    {
    vtkPolyData *bcOutput = vtkPolyData::SafeDownCast(bcOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateBezierCurvePolyData(bcOutput, 0);
    }

  return 1;
}


void vtkBezierCurveSource::UpdadeNodeHandleConnexionPolyData(vtkPolyData* pd)
{
	if (!pd)
	{
		return;
	}

	//cout << "update Node Handle Connection!" << endl;
	// First a rigid line between node landmarks.
	if (this->Nodes == NULL || this->Handles ==NULL)
	{
		return;
	}
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	
	int nrN = this->Nodes->GetNumberOfItems();
	int nrH = this->Handles->GetNumberOfItems();

	//cout << "nr of nodes:" << nrN << endl;
	//cout << "nr of Handles:" << nrH << endl;	
	
	
	int nrMin = std::min(nrH, nrN);
	//cout << "nr Min:" << nrH << endl;
	points->SetNumberOfPoints(2*nrMin);
	this->Nodes->InitTraversal();
	this->Handles->InitTraversal();

	if (nrN == nrH && nrH > 0)
	{
		for (int i = 0; i < nrN; i++)
		{
			vtkLMActor *MyNode = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
			vtkMatrix4x4 *Mat = MyNode->GetMatrix();
			double lmorigin[3] = { Mat->GetElement(0, 3), Mat->GetElement(1, 3), Mat->GetElement(2, 3) };
			double pt[4];
			pt[0] = lmorigin[0];
			pt[1] = lmorigin[1];
			pt[2] = lmorigin[2];
			pt[3] = 1.0;

			points->SetPoint(2 * i, pt[0], pt[1], pt[2]);
			vtkLMActor *MyHandle = vtkLMActor::SafeDownCast(this->Handles->GetNextActor());
			Mat = MyHandle->GetMatrix();
			lmorigin[0] = Mat->GetElement(0, 3);
			lmorigin[1] = Mat->GetElement(1, 3);
			lmorigin[2] = Mat->GetElement(2, 3);
			double pt2[4];
			pt2[0] = lmorigin[0];
			pt2[1] = lmorigin[1];
			pt2[2] = lmorigin[2];
			pt2[3] = 1.0;
			points->SetPoint(2 * i + 1, pt2[0], pt2[1], pt2[2]);

		}


	pd->SetPoints(points);

	vtkSmartPointer<vtkCellArray> cells =
		vtkSmartPointer<vtkCellArray>::New();

	

		for (int i = 0; i < nrN; i++)

		{

			vtkIdType ls[2];
			

			
				ls[0] = 2*i; ls[1] = 2*i + 1;
				cells->InsertNextCell(2, ls);					

		}
		pd->SetLines(cells);

	}
	
	//cout << "End update handle node connexion!" << endl;

	
}


void vtkBezierCurveSource::UpdateBezierCurvePolyData(vtkPolyData* pd, int selected_segments)
{
  if(!pd)
    {
    return;
    }

  //cout << "update bezier curve poly data!" << endl;
  // First a rigid line between node landmarks.
  if (this->Nodes == NULL)
  {
	  return;
  }
 


  vtkLMActor  * ob_H1, *ob_H2, *ob_N1, *ob_N2, *ob_Nstart, *ob_Hstart, *ob_HA, *ob_HB, *ob_NA, *ob_NB;
  ob_H1 = NULL;
  ob_N1 = NULL;
  ob_H2 = NULL;
  ob_N2 = NULL;
  ob_Hstart = NULL;
  ob_Nstart = NULL;
  ob_NA = NULL;
  ob_NB = NULL;
  ob_HA = NULL;
  ob_HB = NULL;


  int k, m, num_seg, ind, ind2, indh, indh2;
  int nint = 98; // will iterate nint , to draw nint+1 lines, that is nint+2 points
  double t;

  int num_landmark_H = this->Handles->GetNumberOfItems(); 
  int num_landmark_N = this->Nodes->GetNumberOfItems();
  int nbp = num_landmark_H;
  double  nn1[3], nn2[3], hh1[3], hh2[3], intvv[3], intvv2[3];

  int print = 0;
  num_seg = 0;
  //if (num_landmark_T == num_landmark_N && num_landmark_T >0)
  if (num_landmark_N >1 && num_landmark_N != num_landmark_H)
  {	// in this case, just draw segments
	  ob_N1 = this->Nodes->GetLandmarkAfter(0);
	  ob_N2 = this->Nodes->GetLandmarkAfter(ob_N1->GetLMNumber());
	  ob_Nstart = this->Nodes->GetLandmarkAfter(0);
	  

	  k = 0;
	  vtkSmartPointer<vtkPoints> points =
		  vtkSmartPointer<vtkPoints>::New();

	  int nrPoints = nrPoints = num_landmark_N;
	  //cout << "nr of nodes:" << nrPoints << endl;
	  points->SetNumberOfPoints(nrPoints);
	  this->Nodes->InitTraversal();
	  
	  for (int i = 0; i<nrPoints; i++)
	  {
		  vtkLMActor *MyActor = vtkLMActor::SafeDownCast(this->Nodes->GetNextActor());
		  vtkMatrix4x4 *Mat = MyActor->GetMatrix();
		  double lmorigin[3] = { Mat->GetElement(0, 3), Mat->GetElement(1, 3), Mat->GetElement(2, 3) };
		  double pt[4];
		  pt[0] = lmorigin[0];
		  pt[1] = lmorigin[1];
		  pt[2] = lmorigin[2];
		  pt[3] = 1.0;
		  points->SetPoint(i, pt[0], pt[1], pt[2]);
	  }
	  pd->SetPoints(points);
	  vtkSmartPointer<vtkCellArray> cells =
		  vtkSmartPointer<vtkCellArray>::New();
	  int ia= 0;	 
	  int istart=0;
	  int selected = 0;
	  while (ob_N1 != NULL)
	  {
		  
		  if (ob_N1->GetLMNodeType() == STARTING_NODE) {
			  num_seg++; selected = this->IsSegmentSelected(num_seg); 
			//  cout << "num_seg:" << num_seg << ", selected:" << selected << endl;
		  }
		  if (ob_N1->GetLMNodeType() == MILESTONE_NODE) { num_seg++; 
		  num_seg++; selected = this->IsSegmentSelected(num_seg); 
		  //cout << "num_seg:" << num_seg << ", selected:" << selected << endl;
		  }
		  
		  
		  if (ob_N1->GetLMNodeType() == CONNECT_NODE) // in that case we should connect that node to the preceding starting point
		  {
			  ob_NA = ob_N1; 			  
			  ob_NB = ob_Nstart;			  

		  }
		  else
		  {
			  ob_NA = ob_N1;
			  ob_NB = ob_N2;				  			
		  }
		  if (ob_NB != NULL)
		  {
			  if (! (ob_NA->GetLMNodeType() == NORMAL_NODE && ob_NB->GetLMNodeType() == STARTING_NODE))
			  {
				  if (selected == selected_segments)
				  {
					  vtkIdType ls[2];
					  ls[0] = ia;
					  if (ob_NA->GetLMNodeType() == CONNECT_NODE) { ls[1] = istart; }
					  else { ls[1] = ia + 1; }
					  // cout << "connect:" << ls[0] << "," << ls[1] << endl;
					  cells->InsertNextCell(2, ls);
				  }


			  }
			  

		  }
		  if (ob_N2 != NULL && ob_N2->GetLMNodeType() == STARTING_NODE) { ob_Nstart = ob_N2; istart = ia + 1; }

		  ind = ob_N1->GetLMNumber();
		  ob_N1 = this->Nodes->GetLandmarkAfter(ind);
		  if (ob_N1 != NULL)
		  {
			  ind2 = ob_N1->GetLMNumber();
			  ob_N2 = this->Nodes->GetLandmarkAfter(ind2);
			  
		  }
		  ia++; 
	  }
	  
	  pd->SetLines(cells);
  }
    
  else if (num_landmark_N >1 && num_landmark_N == num_landmark_H)
  {
	  vtkSmartPointer<vtkPoints> points =
		  vtkSmartPointer<vtkPoints>::New();
	  vtkSmartPointer<vtkCellArray> cells =
		  vtkSmartPointer<vtkCellArray>::New();
	  int cpt = 0;
	  //cout << "here" << endl;
	  ob_H1 = this->Handles->GetLandmarkAfter(0);
	  ob_Hstart = ob_H1;
	  ob_H2 = this->Handles->GetLandmarkAfter(ob_H1->GetLMNumber());
	  ob_N1 = this->Nodes->GetLandmarkAfter(0);
	  ob_Nstart = ob_N1;
	  ob_N2 = this->Nodes->GetLandmarkAfter(ob_N1->GetLMNumber());
	  k = 0;
	  int selected = 0;
	//  cout << "there" << endl;
	  while (ob_N1 != NULL && ob_H1 != NULL)
	  {

		  //stop drawing if the second point is a "start" of a new curve
		  if (ob_N1->GetLMNodeType() == STARTING_NODE) { num_seg++; selected = this->IsSegmentSelected(num_seg);
		//  cout << "num_seg:" << num_seg << ", selected:" << selected << endl;
		  }
		  if (ob_N1->GetLMNodeType() == MILESTONE_NODE) { num_seg++; selected = this->IsSegmentSelected(num_seg);
		//  cout << "num_seg:" << num_seg << ", selected:" << selected << endl;
		  }
		   

		  if (ob_N1->GetLMNodeType() == CONNECT_NODE) // in that case we should connect that node to the preceding starting point
		  {
			  ob_NA = ob_N1;
			  ob_NB = ob_Nstart;
			  ob_HA = ob_H1;
			  ob_HB = ob_Hstart;

		  }	
		  else
		  {
			  ob_NA = ob_N1;
			  ob_HA = ob_H1;
			  ob_NB = ob_N2;
			  ob_HB = ob_H2;
		  }

		  if (ob_NB != NULL && ob_HB != NULL)
		  {
			  vtkMatrix4x4 *MatNA = ob_NA->GetMatrix();
			  nn1[0] = MatNA->GetElement(0, 3);
			  nn1[1] = MatNA->GetElement(1, 3);
			  nn1[2] = MatNA->GetElement(2, 3);			  			  

			  vtkMatrix4x4 *MatNB = ob_NB->GetMatrix();
			  nn2[0] = MatNB->GetElement(0, 3);
			  nn2[1] = MatNB->GetElement(1, 3);
			  nn2[2] = MatNB->GetElement(2, 3);

			  vtkMatrix4x4 *MatHA = ob_HA->GetMatrix();
			  hh1[0] = MatHA->GetElement(0, 3);
			  hh1[1] = MatHA->GetElement(1, 3);
			  hh1[2] = MatHA->GetElement(2, 3);

			  vtkMatrix4x4 *MatHB = ob_HB->GetMatrix();
			  hh2[0] = MatHB->GetElement(0, 3);
			  hh2[1] = MatHB->GetElement(1, 3);
			  hh2[2] = MatHB->GetElement(2, 3);			  			  			  
			  			  			  			 
			  //trick : second handle is mirrored relative to the second point!
			  hh2[0] = nn2[0] - (hh2[0] - nn2[0]);
			  hh2[1] = nn2[1] - (hh2[1] - nn2[1]);
			  hh2[2] = nn2[2] - (hh2[2] - nn2[2]);

			  // At this stage : we have all the input we need!
			  // Just draw the Bezier curve between nn1 and nn2

			  int compute = 1;

			  if (ob_NA->GetLMNodeType() == NORMAL_NODE && ob_NB->GetLMNodeType() == STARTING_NODE) { compute = 0; }
			  if (selected != selected_segments) { compute = 0; }
			  if (compute == 1)
			  {
				  
				  for (m = 0; m <= nint; m++)
				  {
					  // t is [0.. 1]
					  t = (((double)m)) / (((double)nint + 1));
					 // glBegin(GL_LINES);
					  intvv[0] = (1 - t)*(1 - t)*(1 - t)*nn1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*nn2[0];
					  intvv[1] = (1 - t)*(1 - t)*(1 - t)*nn1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*nn2[1];
					  intvv[2] = (1 - t)*(1 - t)*(1 - t)*nn1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*nn2[2];
					 // glVertex3d(intvv[0], intvv[1], intvv[2]);


					  t = (((double)m + 1)) / (((double)nint + 1));
					  intvv2[0] = (1 - t)*(1 - t)*(1 - t)*nn1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*nn2[0];
					  intvv2[1] = (1 - t)*(1 - t)*(1 - t)*nn1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*nn2[1];
					  intvv2[2] = (1 - t)*(1 - t)*(1 - t)*nn1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*nn2[2];
					  points->InsertNextPoint(intvv);
					  points->InsertNextPoint(intvv2);
					  vtkIdType ls[2];
					  ls[0] = cpt;
					  ls[1] = cpt+1;					  					 
					  cells->InsertNextCell(2,ls);
					  cpt += 2;
					  //glVertex3d(intvv2[0], intvv2[1], intvv2[2]);
					 // glEnd();
				  }

			  }

		  }

		 
		  

		  ind = ob_N1->GetLMNumber();
		  indh = ob_H1->GetLMNumber();

		  ob_H1 = this->Handles->GetLandmarkAfter(indh);
		  ob_N1 = this->Nodes->GetLandmarkAfter(ind); 
		  if (ob_N1 != NULL)
		  {
			  ind2 = ob_N1->GetLMNumber();
			  ob_N2 = this->Nodes->GetLandmarkAfter(ind2);
		  }
		  if (ob_H1 != NULL)
		  {
			  indh2 = ob_H1->GetLMNumber();
			  ob_H2 = this->Handles->GetLandmarkAfter(indh2);
		  }
		  if (ob_N1 != NULL&& ob_N1->GetLMNodeType() == STARTING_NODE) {
			  ob_Hstart = ob_H1;
			  ob_Nstart = ob_N1;

		  }
		  
		  k++;
	  }
	  pd->SetPoints(points);
	  pd->SetLines(cells);
  }
  


}

int vtkBezierCurveSource::GetCurveSegmentNumber()
{

	int num_seg, ind;
	vtkLMActor  *ob_N1;
	ob_N1=NULL;

	int num_landmark_N = this->Nodes->GetNumberOfItems(); 	//number of node landmarks

	int print = 0;
	if (print == 1)
	{
		//std::cout<<std::endl<<"Start count segments curve";
	}
	num_seg=0;
	//if (num_landmark_T == num_landmark_N && num_landmark_T >0)
	if (num_landmark_N >1)
	{
		ob_N1 = this->Nodes->GetLandmarkAfter(0);
		while ( ob_N1 != NULL)
		{
			if (ob_N1->GetLMNodeType() ==STARTING_NODE || ob_N1->GetLMNodeType() == MILESTONE_NODE )
			{
				num_seg++;
			}
			ind = ob_N1->GetLMNumber();
			ob_N1 = this->Nodes->GetLandmarkAfter(ind);

		}
	}
	return num_seg;
}

int vtkBezierCurveSource::IsSegmentSelected(int seg)
{
	// Ici, on cherche à savoir si il y a au moins un node/handle
	// sélectionnée pour le segment donné.

	vtkLMActor *ob_H1, *ob_H2, *ob_N1, *ob_N2;
	ob_H1 = NULL;
	ob_N1 = NULL;
	ob_H2 = NULL;
	ob_N2 = NULL;


	int seg_selected = 0;
	int k, num_seg, ind, ind2, indh, indh2;


	int num_landmark_H = this->Handles->GetNumberOfItems();  //handle number
	int num_landmark_N = this->Nodes->GetNumberOfItems(); 	//node number
	int nbp = num_landmark_H;
	
	

	int print = 0;
	num_seg = 0;
	//if (num_landmark_T == num_landmark_N && num_landmark_T >0)
	if (num_landmark_N >1 && num_landmark_N != num_landmark_H)
	{	// in this case only check nodes
		num_seg=1;
		ob_N1 = this->Nodes->GetLandmarkAfter(0);
		ob_N2 = this->Nodes->GetLandmarkAfter(ob_N1->GetLMNumber()); 
		int current_seg_selected = 0;
		while (ob_N1 != NULL )
		{

			
			if (ob_N1 != NULL && ob_N1->GetSelected() == 1 ) { current_seg_selected =1; }

			if (ob_N1->GetLMNodeType() == MILESTONE_NODE) {
				if (seg == num_seg ) { return current_seg_selected; }//milestone involves 2 segments current+next
				num_seg++;				
				current_seg_selected = ob_N1->GetSelected();

			}

			if (ob_N2 != NULL && ob_N2->GetLMNodeType() == STARTING_NODE)
			{
				if (seg == num_seg) { return current_seg_selected; }
				num_seg++;
				current_seg_selected = 0;
			}
			ind = ob_N1->GetLMNumber();

			ob_N1 = this->Nodes->GetLandmarkAfter(ind); 
			if (ob_N1 != NULL)
			{
				ind2 = ob_N1->GetLMNumber();
				ob_N2 = this->Nodes->GetLandmarkAfter(ind2);
			}

		}
		if (num_seg == seg)
		{
			return current_seg_selected;
		}
		else
		{
			return 0;
		}


	}
	else if (num_landmark_N >1 && num_landmark_N == num_landmark_H)
	{
		
		ob_H1 = this->Handles->GetLandmarkAfter(0);
		ob_H2 = this->Handles->GetLandmarkAfter(ob_H1->GetLMNumber());
		ob_N1 = this->Nodes->GetLandmarkAfter(0);
		ob_N2 = this->Nodes->GetLandmarkAfter(ob_N1->GetLMNumber());
		k = 0;
		int current_seg_selected = 0;
		num_seg = 1;
		while (ob_N1 != NULL && ob_H1 != NULL)
		{

			if (ob_H1->GetSelected() == 1 || ob_N1->GetSelected() == 1)
			{
				current_seg_selected = 1;
			}						
			if (ob_N1->GetLMNodeType() == MILESTONE_NODE) {
				if (seg == num_seg) { return current_seg_selected; }//milestone involves 2 segments current+next
				num_seg++;
				current_seg_selected = ob_N1->GetSelected();
				if (ob_H1->GetSelected() == 1) { current_seg_selected = 1; }

			}

			if (ob_N2 != NULL && ob_N2->GetLMNodeType() == STARTING_NODE)
			{
				if (seg == num_seg) { return current_seg_selected; }
				num_seg++;
				current_seg_selected = ob_N2->GetSelected();
			}
		

			
			
			ind = ob_N1->GetLMNumber();
			indh = ob_H1->GetLMNumber();

			ob_H1 = this->Handles->GetLandmarkAfter(indh);
			ob_N1 = this->Nodes->GetLandmarkAfter(ind);
			if (ob_N1 != NULL)
			{
				ind2 = ob_N1->GetLMNumber();
				ob_N2 = this->Nodes->GetLandmarkAfter(ind2);
			}
			if (ob_H1 != NULL)
			{
				indh2 = ob_H1->GetLMNumber();
				ob_H2 = this->Handles->GetLandmarkAfter(indh2);
			}

		}
		if (num_seg == seg)
		{
			return current_seg_selected;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}


double vtkBezierCurveSource::GetCurveSegmentLength(int seg)
{
	double length = 0;
	

	//cout << "curve length!" << endl;
	// First a rigid line between node landmarks.
	if (this->Nodes == NULL)
	{
		return 0;
	}



	vtkLMActor  * ob_H1, *ob_H2, *ob_N1, *ob_N2, *ob_Nstart, *ob_Hstart, *ob_HA, *ob_HB, *ob_NA, *ob_NB;
	ob_H1 = NULL;
	ob_N1 = NULL;
	ob_H2 = NULL;
	ob_N2 = NULL;
	ob_Hstart = NULL;
	ob_Nstart = NULL;
	ob_NA = NULL;
	ob_NB = NULL;
	ob_HA = NULL;
	ob_HB = NULL;


	int k, m, num_seg, ind, ind2, indh, indh2;
	int nint = 98; // will iterate nint , to draw nint+1 lines, that is nint+2 points
	double t;

	int num_landmark_H = this->Handles->GetNumberOfItems();
	int num_landmark_N = this->Nodes->GetNumberOfItems();
	int nbp = num_landmark_H;
	double  nn1[3], nn2[3], hh1[3], hh2[3], intvv[3], intvv2[3];

	int print = 0;
	num_seg = 0;
	//if (num_landmark_T == num_landmark_N && num_landmark_T >0)
	if (num_landmark_N >1 && num_landmark_N != num_landmark_H)
	{	// in this case, just draw segments
		ob_N1 = this->Nodes->GetLandmarkAfter(0);
		ob_N2 = this->Nodes->GetLandmarkAfter(ob_N1->GetLMNumber());
		ob_Nstart = this->Nodes->GetLandmarkAfter(0);


		k = 0;
	

		while (ob_N1 != NULL)
		{

			if (ob_N1->GetLMNodeType() == STARTING_NODE) {
				num_seg++; 
				//cout << "num_seg:" << num_seg  << endl;
			}
			if (ob_N1->GetLMNodeType() == MILESTONE_NODE) {
				num_seg++;
				num_seg++; 
				//cout << "num_seg:" << num_seg<< endl;
			}


			if (ob_N1->GetLMNodeType() == CONNECT_NODE) // in that case we should connect that node to the preceding starting point
			{
				ob_NA = ob_N1;
				ob_NB = ob_Nstart;

			}
			else
			{
				ob_NA = ob_N1;
				ob_NB = ob_N2;
			}
			if (ob_NB != NULL)
			{
				if (!(ob_NA->GetLMNodeType() == NORMAL_NODE && ob_NB->GetLMNodeType() == STARTING_NODE))
				{
					if (num_seg == seg)
					{
						double len = 0;
						vtkMatrix4x4 *MatNA = ob_NA->GetMatrix();
						nn1[0] = MatNA->GetElement(0, 3);
						nn1[1] = MatNA->GetElement(1, 3);
						nn1[2] = MatNA->GetElement(2, 3);

						vtkMatrix4x4 *MatNB = ob_NB->GetMatrix();
						nn2[0] = MatNB->GetElement(0, 3);
						nn2[1] = MatNB->GetElement(1, 3);
						nn2[2] = MatNB->GetElement(2, 3);
						len = sqrt((nn1[0] - nn2[0])*(nn1[0] - nn2[0]) + (nn1[1] - nn2[1])*(nn1[1] - nn2[1]) + (nn1[2] - nn2[2])*(nn1[2] - nn2[2]));
						
						// cout << "connect:" << ls[0] << "," << ls[1] << endl;
						length += len;
					}


				}


			}
			if (ob_N2 != NULL && ob_N2->GetLMNodeType() == STARTING_NODE) { ob_Nstart = ob_N2; }

			ind = ob_N1->GetLMNumber();
			ob_N1 = this->Nodes->GetLandmarkAfter(ind);
			if (ob_N1 != NULL)
			{
				ind2 = ob_N1->GetLMNumber();
				ob_N2 = this->Nodes->GetLandmarkAfter(ind2);

			}
			
		}

		
	}

	else if (num_landmark_N >1 && num_landmark_N == num_landmark_H)
	{
			int cpt = 0;
		//cout << "here" << endl;
		ob_H1 = this->Handles->GetLandmarkAfter(0);
		ob_Hstart = ob_H1;
		ob_H2 = this->Handles->GetLandmarkAfter(ob_H1->GetLMNumber());
		ob_N1 = this->Nodes->GetLandmarkAfter(0);
		ob_Nstart = ob_N1;
		ob_N2 = this->Nodes->GetLandmarkAfter(ob_N1->GetLMNumber());
		k = 0;
		
		while (ob_N1 != NULL && ob_H1 != NULL)
		{

			//stop drawing if the second point is a "start" of a new curve
			if (ob_N1->GetLMNodeType() == STARTING_NODE) {
				num_seg++;
				//cout << "num_seg:" << num_seg  << endl;
			}
			if (ob_N1->GetLMNodeType() == MILESTONE_NODE) {
				num_seg++;
				//cout << "num_seg:" << num_seg  << endl;
			}


			if (ob_N1->GetLMNodeType() == CONNECT_NODE) // in that case we should connect that node to the preceding starting point
			{
				ob_NA = ob_N1;
				ob_NB = ob_Nstart;
				ob_HA = ob_H1;
				ob_HB = ob_Hstart;

			}
			else
			{
				ob_NA = ob_N1;
				ob_HA = ob_H1;
				ob_NB = ob_N2;
				ob_HB = ob_H2;
			}

			if (ob_NB != NULL && ob_HB != NULL)
			{
				vtkMatrix4x4 *MatNA = ob_NA->GetMatrix();
				nn1[0] = MatNA->GetElement(0, 3);
				nn1[1] = MatNA->GetElement(1, 3);
				nn1[2] = MatNA->GetElement(2, 3);

				vtkMatrix4x4 *MatNB = ob_NB->GetMatrix();
				nn2[0] = MatNB->GetElement(0, 3);
				nn2[1] = MatNB->GetElement(1, 3);
				nn2[2] = MatNB->GetElement(2, 3);

				vtkMatrix4x4 *MatHA = ob_HA->GetMatrix();
				hh1[0] = MatHA->GetElement(0, 3);
				hh1[1] = MatHA->GetElement(1, 3);
				hh1[2] = MatHA->GetElement(2, 3);

				vtkMatrix4x4 *MatHB = ob_HB->GetMatrix();
				hh2[0] = MatHB->GetElement(0, 3);
				hh2[1] = MatHB->GetElement(1, 3);
				hh2[2] = MatHB->GetElement(2, 3);

				//trick : second handle is mirrored relative to the second point!
				hh2[0] = nn2[0] - (hh2[0] - nn2[0]);
				hh2[1] = nn2[1] - (hh2[1] - nn2[1]);
				hh2[2] = nn2[2] - (hh2[2] - nn2[2]);

				// At this stage : we have all the input we need!
				// Just draw the Bezier curve between nn1 and nn2

				int compute = 1;

				if (ob_NA->GetLMNodeType() == NORMAL_NODE && ob_NB->GetLMNodeType() == STARTING_NODE) { compute = 0; }
				if (seg != num_seg) { compute = 0; }
				if (compute == 1)
				{

					for (m = 0; m <= nint; m++)
					{
						// t is [0.. 1]
						t = (((double)m)) / (((double)nint + 1));
						// glBegin(GL_LINES);
						intvv[0] = (1 - t)*(1 - t)*(1 - t)*nn1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*nn2[0];
						intvv[1] = (1 - t)*(1 - t)*(1 - t)*nn1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*nn2[1];
						intvv[2] = (1 - t)*(1 - t)*(1 - t)*nn1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*nn2[2];
						// glVertex3d(intvv[0], intvv[1], intvv[2]);


						t = (((double)m + 1)) / (((double)nint + 1));
						intvv2[0] = (1 - t)*(1 - t)*(1 - t)*nn1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*nn2[0];
						intvv2[1] = (1 - t)*(1 - t)*(1 - t)*nn1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*nn2[1];
						intvv2[2] = (1 - t)*(1 - t)*(1 - t)*nn1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*nn2[2];
						
						double len = sqrt((intvv[0] - intvv2[0])*(intvv[0] - intvv2[0]) + (intvv[1] - intvv2[1])*(intvv[1] - intvv2[1]) + (intvv[2] - intvv2[2])*(intvv[2] - intvv2[2]));

						// cout << "connect:" << ls[0] << "," << ls[1] << endl;
						length += len;
						//glVertex3d(intvv2[0], intvv2[1], intvv2[2]);
						// glEnd();
					}

				}

			}




			ind = ob_N1->GetLMNumber();
			indh = ob_H1->GetLMNumber();

			ob_H1 = this->Handles->GetLandmarkAfter(indh);
			ob_N1 = this->Nodes->GetLandmarkAfter(ind);
			if (ob_N1 != NULL)
			{
				ind2 = ob_N1->GetLMNumber();
				ob_N2 = this->Nodes->GetLandmarkAfter(ind2);
			}
			if (ob_H1 != NULL)
			{
				indh2 = ob_H1->GetLMNumber();
				ob_H2 = this->Handles->GetLandmarkAfter(indh2);
			}
			if (ob_N1 != NULL&& ob_N1->GetLMNodeType() == STARTING_NODE) {
				ob_Hstart = ob_H1;
				ob_Nstart = ob_N1;

			}

			k++;
		}
	
	}
	return length;
}




