/*=========================================================================

Program:   MeshTools
Module:    vtkLM2Actor.cxx


=========================================================================*/
#include "vtkLM2Actor.h"

#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkMath.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkLine.h>
#include <vtkCellData.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>


vtkStandardNewMacro(vtkLM2Actor);


//----------------------------------------------------------------------------
vtkLM2Actor::vtkLM2Actor()
{
	this->PickableOn();
	this->LMBody =  vtkSmartPointer<vtkPolyData>::New();
	this->LMLabel = vtkCaptionActor2D::New();
	this->UndoRedo = new vtkLM2ActorUndoRedo;
	this->Selected=0;
	this->LMDrawLabel = 1;//Draws the label
	this->LMType = 0;	
	this->SetLMColor(); //Set color according to LMType
	this->LMBodyType = 0; //sphere by default
	this->LMSize = 0.1; // 0.1mm by default 
	this->LMNumber = 1; //1
	this->LMOrigin[0] = 0;
	this->LMOrigin[1] = 0;
	this->LMOrigin[2] = 0;

	this->LMOrientation[0] = 1;
	this->LMOrientation[1] = 0;
	this->LMOrientation[2] = 0;

	this->LMLabelText = NULL;

		
	
	


	
	this->UpdateProps();
}

//----------------------------------------------------------------------------
vtkLM2Actor::~vtkLM2Actor()
{
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	this->SetLMLabelText(NULL);
	//this->LMLabel->Delete();
	//this->LMBody->Delete();
}
void vtkLM2Actor::SetSelected(int selected)
{
	this->Selected = selected;
	vtkSmartPointer<vtkTextProperty> mproperty = vtkSmartPointer<vtkTextProperty>::New();
	
	if (selected == 1)
	{
		this->GetProperty()->SetColor(0.5, 0.5, 0.5);
		this->GetProperty()->SetOpacity(this->mColor[3]);
		if (this->mColor[3] < 0.75)
		{
			this->GetProperty()->SetOpacity(0.75);
		}
		mproperty->SetColor(0.5, 0.5,0.5);
		mproperty->SetFontFamilyToArial();
		this->LMLabel->SetCaptionTextProperty(mproperty);
	}
	else
	{
		this->GetProperty()->SetColor(this->mColor[0], this->mColor[1], this->mColor[2]);
		//cout << "mColor[3](alpha) =" << this->mColor[3] << endl;
		this->GetProperty()->SetOpacity(this->mColor[3]);
		double color[3] = { this->mColor[0], this->mColor[1], this->mColor[2] };
		mproperty->SetColor(color);
		mproperty->SetFontFamilyToArial();
		this->LMLabel->SetCaptionTextProperty(mproperty);
	}
}
//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkLM2Actor::ShallowCopy(vtkProp *prop)
{
	vtkMTActor *f = vtkMTActor::SafeDownCast(prop);


	// Now do superclass
	this->vtkMTActor::ShallowCopy(prop);
	
}



void vtkLM2Actor::SetLMOrigin(double x, double y, double z)
{
	double origin[3];
	origin[0] = x;
	origin[1] = y;
	origin[2] = z;
	this->SetLMOrigin(origin);
}
void vtkLM2Actor::SetLMOrigin(double origin[3])
{

	if (this->LMOrigin[0] != origin[0]
		|| this->LMOrigin[1] != origin[1]
		|| this->LMOrigin[2] != origin[2]

		)
	{
		this->LMOrigin[0] = origin[0];
		this->LMOrigin[1] = origin[1];
		this->LMOrigin[2] = origin[2];


		this->Modified();
		this->UpdateProps();
	}
}
void vtkLM2Actor::GetLMOrigin(double origin[3])
{
	double *org = this->GetLMOrigin();

	origin[0] = org[0];
	origin[1] = org[1];
	origin[2] = org[2];

}



double *vtkLM2Actor::GetLMOrigin()
{
	return this->LMOrigin;
}


void vtkLM2Actor::SetLMOrientation(double x, double y, double z)
{
	double orientation[3] = { x,y,z };
	
	this->SetLMOrientation(orientation);
}
void vtkLM2Actor::SetLMOrientation(double orientation[3])
{

	if (this->LMOrientation[0] != orientation[0]
		|| this->LMOrientation[1] != orientation[1]
		|| this->LMOrientation[2] != orientation[2]

		)
	{
		this->LMOrientation[0] = orientation[0];
		this->LMOrientation[1] = orientation[1];
		this->LMOrientation[2] = orientation[2];


		this->Modified();
		this->UpdateProps();
	}
}
void vtkLM2Actor::GetLMOrientation(double orientation[3])
{
	double *ori = this->GetLMOrientation();

	orientation[0] = ori[0];
	orientation[1] = ori[1];
	orientation[2] = ori[2];

}



double *vtkLM2Actor::GetLMOrientation()
{
	return this->LMOrientation;
}


void vtkLM2Actor::CreateLMLabelText()
{
	//instantiates the label

	// LMType : 
	// 0 : -x x -y y
	// 1 :  -x x -z z
	// 2 : -y y -z z

	double pos[3] = { this->LMOrigin[0] + 1.1*this->LMSize , this->LMOrigin[1], this->LMOrigin[2] };

	

	vtkSmartPointer<vtkTextProperty> mproperty = vtkSmartPointer<vtkTextProperty>::New();

	double color[3] = { this->mColor[0], this->mColor[1], this->mColor[2] };
	

	std::string myStrLabel;
	myStrLabel = std::to_string(this->LMNumber);	
	mproperty->SetColor(color);
	


	mproperty->SetFontFamilyToArial();

	this->LMLabel->SetCaptionTextProperty(mproperty);
	this->LMLabel->SetPosition(0, 0);
	this->LMLabel->SetAttachmentPoint(pos);
	this->LMLabel->SetHeight(0.03);
	this->LMLabel->BorderOff();
	this->LMLabel->LeaderOff();
	this->SetLMLabelText(myStrLabel.c_str());
	this->LMLabel->SetCaption(this->LMLabelText);


}

void vtkLM2Actor::CreateLMBody()
{


	vtkSmartPointer<vtkSphereSource> sphereSource =
	vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(this->LMOrigin[0], this->LMOrigin[1], this->LMOrigin[2]);
	sphereSource->SetRadius(this->LMSize);
	if (this->LMType == 1 || this->LMType == 3)
	{
		sphereSource->SetRadius(1.1*this->LMSize);
	}
	sphereSource->Update();
	this->LMBody = sphereSource->GetOutput();

		

}



void vtkLM2Actor::SetLMColor()
{
	// Create six colors - one for each line
	double red[4] = { 1, 0, 0, 1 }; // LMType=0
	double yellow[4] = { 1, 1, 0,0.5 }; // LMType = 1 (target LM)
	double darkred[4] = { 0.5, 0, 0, 1 }; // LMType = 2 (curve node: dark red)
	double orange[4] = { 1, 0.5, 0, 1 }; // LMType = 3 (curve handle : orange)
	double green[4] = { 0, 1, 0, 1 }; // LMType=4 (curve starting point)
	double blue[4] = { 0, 0, 1, 1 }; // LMType = 5 (curve milestone)	
	double cyan[4] = { 0, 1, 1, 1 }; // LMType = 6 (curve ending point)
	if (this->LMType == 0) { this->SetmColor(red); }
	if (this->LMType == 1) { this->SetmColor(yellow); }
	if (this->LMType == 2) { this->SetmColor(darkred); }
	if (this->LMType == 3) { this->SetmColor(orange); }
	if (this->LMType == 4) { this->SetmColor(green); }
	if (this->LMType == 5) { this->SetmColor(blue); }
	if (this->LMType == 6) { this->SetmColor(cyan); }
}

//----------------------------------------------------------------------------
void vtkLM2Actor::SetLMType(int type)
{
	if (this->LMType != type && type >= 0 && type <= 6)
	{
		

		this->SetLMColor(); // when type changes, color changes as well
		this->LMType = type;
		this->Modified();
		this->UpdateProps();
	}


}

void vtkLM2Actor::SetLMBodyType(int type)
{
	if (this->LMBodyType != type && type >= 0 && type <= 1)
	{
		this->LMBodyType = type;
		this->Modified();
		this->UpdateProps();
	}


}

void vtkLM2Actor::SetLMSize(double size)
{
	if (this->LMSize != size && size>0)
	{
		this->LMSize = size;
		this->Modified();
		this->UpdateProps();
	}

}


void vtkLM2Actor::SetLMNumber(int num)
{
	if (this->LMNumber != num && num>0)
	{
		this->LMNumber = num;
		this->Modified();
		this->UpdateProps();
	}

}



//----------------------------------------------------------------------------
void vtkLM2Actor::UpdateProps()
{
	//Recreates the text actor and the LMBody

	this->CreateLMBody();
	this->CreateLMLabelText();

}

void vtkLM2Actor::Undo(int mCount)
{

	//cout << "Inside actor Undo" << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		//cout << "Undo actor event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopUndoStack();
	}




}
void vtkLM2Actor::Redo(int mCount)
{
	//cout << "Inside actor Undo, try to undo " <<mCount<< endl;
	if (this->UndoRedo->RedoStack.empty())
	{
		cout << "Redo Stack empty!" << endl;
		return;
	}
	//cout << "Youngest redo count= " << this->UndoRedo->RedoStack.back().UndoCount<< endl;

	if (mCount == this->UndoRedo->RedoStack.back().UndoCount)
	{
		//cout << "Redo actor event " << this->UndoRedo->RedoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopRedoStack();
	}
}
void vtkLM2Actor::Erase(int mCount)
{
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	int oldestCount = this->UndoRedo->UndoStack.front().UndoCount;
	if (oldestCount <= mCount)
	{
		//cout << "ERASE actor event " << oldestCount << endl;
		this->UndoRedo->UndoStack.erase(this->UndoRedo->UndoStack.begin());
	}
}

void vtkLM2Actor::PopUndoStack()
{
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}

	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	// Now put undo Matrix inside LM object : 
	Mat->DeepCopy(this->UndoRedo->UndoStack.back().Matrix);

	std::cout << "Old Matrix: " << endl << *SavedMat << std::endl;
	std::cout << "New Matrix: " << endl << *Mat << std::endl;

	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();


	this->GetMatrix(Mat);
	
	int mCurrentSelected = this->Selected;
	int mCurrentType = this->LMType;
	int mCurrentNumber = this->LMNumber;
	std::string mCurrentLabel = this->LMLabelText;
	this->SetLMType(this->UndoRedo->UndoStack.back().Type);
	this->SetSelected(this->UndoRedo->UndoStack.back().Selected);
	this->LMNumber = this->UndoRedo->UndoStack.back().Number;
	this->SetLMLabelText(this->UndoRedo->UndoStack.back().Label.c_str());
	cout << "PopUndoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->RedoStack.push_back(vtkLM2ActorUndoRedo::Element(SavedMat, mCurrentSelected, mCurrentNumber, mCurrentType, mCurrentLabel, this->UndoRedo->UndoStack.back().UndoCount));
	this->UndoRedo->UndoStack.pop_back();
	this->Modified();
}
void vtkLM2Actor::PopRedoStack()
{
	if (this->UndoRedo->RedoStack.empty())
	{
		return;
	}
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	// Now put redp Matrix inside object : 
	Mat->DeepCopy(this->UndoRedo->RedoStack.back().Matrix);
	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();

	int mCurrentSelected = this->Selected;
	int mCurrentType = this->LMType;
	int mCurrentNumber = this->LMNumber;
	std::string mCurrentLabel = this->LMLabelText;
	this->SetLMType(this->UndoRedo->RedoStack.back().Type);
	this->SetSelected(this->UndoRedo->RedoStack.back().Selected);
	this->LMNumber = this->UndoRedo->RedoStack.back().Number;
	this->SetLMLabelText(this->UndoRedo->RedoStack.back().Label.c_str());

	cout << "PopRedoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->UndoStack.push_back(vtkLM2ActorUndoRedo::Element(SavedMat, mCurrentSelected, mCurrentNumber, mCurrentType, mCurrentLabel, this->UndoRedo->RedoStack.back().UndoCount));
	this->UndoRedo->RedoStack.pop_back();
	this->Modified();
}

void vtkLM2Actor::SaveState(int mCount)
{
	this->UndoRedo->RedoStack.clear();
	int Count = mCount;


	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	int mSelected = this->Selected;
	int mType = this->LMType;
	int mNumber = this->LMNumber;
	std::string mLabel = this->LMLabelText;

	//std::cout << "Saved Matrix: " << endl << *Mat << std::endl;

	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	//std::cout << "Saved Matrix Copy: " << endl << *SavedMat << std::endl;
	this->UndoRedo->UndoStack.push_back(vtkLM2ActorUndoRedo::Element(SavedMat,  mSelected, mNumber, mType, mLabel, mCount));

}

//----------------------------------------------------------------------------
void vtkLM2Actor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "LMLabelText: ";
	os << this->LMLabelText << endl;
	//this->LMLabel->PrintSelf(os, indent);
	this->LMBody->PrintSelf(os, vtkIndent(5));



	os << indent << "LMType: ";
	if (this->LMType >= 0)
	{
		os << this->LMType << endl;
	}
	else
	{
		os << "(no LM Type)" << endl;
	}

	os << indent << "LM origin: ";
	if (this->LMOrigin[0])
	{
		os << this->LMOrigin[0] << "," << this->LMOrigin[1] << "," << this->LMOrigin[2] << endl;
	}
	else
	{
		os << "(no LM Origin)" << endl;
	}

	os << indent << "LM orientation: ";
	if (this->LMOrientation[0])
	{
		os << this->LMOrientation[0] << "," << this->LMOrientation[1] << "," << this->LMOrientation[2] << endl;
	}
	else
	{
		os << "(no LM Origin)" << endl;
	}

	os << indent << "LMSize: ";
	if (this->LMSize)
	{
		os << this->LMSize << endl;
	}
	else
	{
		os << "(no LM Size)" << endl;
	}



	os << indent << "LMLabelText: " << (this->LMLabelText ?
		this->LMLabelText : "(none)")
		<< endl;


	os << indent << "LMDrawLabel: " << (this->LMDrawLabel ? "On\n" : "Off\n");


}
