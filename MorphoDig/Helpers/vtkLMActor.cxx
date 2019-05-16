/*=========================================================================

Program:   MorphoDig
Module:    vtkLMActor.cxx
=========================================================================*/
#include "vtkLMActor.h"
#include  "vtkProbeSource.h"
#include  "mqMorphoDigCore.h"

#include <vtkTextProperty.h>
#include <vtkSphereSource.h>

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <vtkTransform.h>
vtkStandardNewMacro(vtkLMActor);

#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3

#define SPHERE 0
#define ARROW 1



//----------------------------------------------------------------------------
vtkLMActor::vtkLMActor()
{
	this->UndoRedo = new vtkLMActorUndoRedo;
	this->Selected = 0;
	this->Changed = 0;
	this->LMBodyType = SPHERE;
	this->LMText = "Flag 0";
	this->LMBody = vtkSmartPointer<vtkPolyData>::New();
	this->LMLabel = vtkBillboardTextActor3D::New();
	
	this->LMDrawLabel = 1;//Draws the label
	this->LMType = NORMAL_LMK;
	this->LMNodeType = -1; //NOT a curve node ...
	this->LMSize = 0.1; // 0.1mm by default 
	this->LMNumber = 1; //1
	this->LMOrigin[0] = 0;
	this->LMOrigin[1] = 0;
	this->LMOrigin[2] = 0;

	this->LMOrientation[0] = 1;
	this->LMOrientation[1] = 0;
	this->LMOrientation[2] = 0;

	this->LMLabelText = NULL;
	this->ResetLMColor(); //Set color according to LMType if LMType !=2, otherwise according to LMNodeType	
	this->UpdateProps();
	
}

//----------------------------------------------------------------------------
vtkLMActor::~vtkLMActor()
{
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	

}

void vtkLMActor::SetLMOriginAndOrientation(double origin[3], double orientation[3])
{
	
		

	
	if (this->LMOrigin[0] != origin[0]
		|| this->LMOrigin[1] != origin[1]
		|| this->LMOrigin[2] != origin[2]
		|| this->LMOrientation[0] != orientation[0]
		|| this->LMOrientation[1] != orientation[1]
		|| this->LMOrientation[2] != orientation[2]

		)
	{
		this->LMOrigin[0] = origin[0];
		this->LMOrigin[1] = origin[1];
		this->LMOrigin[2] = origin[2];
		this->LMOrientation[0] = orientation[0];
		this->LMOrientation[1] = orientation[1];
		this->LMOrientation[2] = orientation[2];


		this->Modified();
		//cout << "Update proprs" << endl;
		this->UpdateProps();
	}
}

void vtkLMActor::SetLMText(std::string lm_text)
{
	this->LMText = lm_text;
	this->UpdateProps();
}
void vtkLMActor::SetLMOrigin(double x, double y, double z)
{
	double origin[3];
	origin[0] = x;
	origin[1] = y;
	origin[2] = z;
	this->SetLMOrigin(origin);
}
void vtkLMActor::SetLMOrigin(double origin[3])
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
		//cout << "Update proprs" << endl;
		this->UpdateProps();
	}
}
void vtkLMActor::GetLMOrigin(double origin[3])
{
	double *org = this->GetLMOrigin();

	origin[0] = org[0];
	origin[1] = org[1];
	origin[2] = org[2];

}



double *vtkLMActor::GetLMOrigin()
{
	return this->LMOrigin;
}


void vtkLMActor::SetLMOrientation(double x, double y, double z)
{
	double orientation[3] = { x,y,z };

	this->SetLMOrientation(orientation);
}
void vtkLMActor::SetLMOrientation(double orientation[3])
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
void vtkLMActor::SetLMSize(double size)
{
	//if (this->LMType != FLAG_LMK)
	//{
		this->LMSize = size;
	//}
	//else
	//{
	//	this->LMSize = size*0.33;
	//}
	this->Modified();
	this->UpdateProps();
	
}
void vtkLMActor::GetLMOrientation(double orientation[3])
{
	double *ori = this->GetLMOrientation();

	orientation[0] = ori[0];
	orientation[1] = ori[1];
	orientation[2] = ori[2];

}



double *vtkLMActor::GetLMOrientation()
{
	return this->LMOrientation;
}



void vtkLMActor::CreateLMLabelText()
{
	//instantiates the label


	// Position of the label can be computed using this matrix!

	double init_pos[3] = { 0,0,0 };
	double mult = 1;
	if (this->LMType == TARGET_LMK || this->LMType == HANDLE_LMK)
	{
		mult = 1.4;
	}
	

	if (this->LMBodyType == SPHERE)
	{
		init_pos[0] += 0.5*1.1*mult*this->LMSize;
	}
	else //ARROW
	{
		if (this->LMType != FLAG_LMK)
		{
			init_pos[0] += 3 * 1.1*mult*this->LMSize;
		}
		else
		{	
			init_pos[0] += 1.1*mult*this->LMSize;
		}
	}

	
	double final_pos[3];

	

	mqMorphoDigCore::TransformPoint(this->GetMatrix(), init_pos, final_pos);


	vtkSmartPointer<vtkTextProperty> mproperty = vtkSmartPointer<vtkTextProperty>::New();

	double color[3] = { this->mColor[0], this->mColor[1], this->mColor[2] };


	std::string myStrLabel;
	if (this->GetLMType() != FLAG_LMK)
	{
		myStrLabel = std::to_string(this->LMNumber);
	}
	else
	{

		myStrLabel = this->LMText;
	}
		mproperty->SetColor(color);

	//mproperty->Set

	mproperty->SetFontFamilyToArial();
	mproperty->SetFontSize(mqMorphoDigCore::instance()->Getmui_FontSize());

	this->LMLabel->SetTextProperty(mproperty);
	this->LMLabel->SetPosition(final_pos);
	
	//this->LMLabel->
	this->SetLMLabelText(myStrLabel.c_str());
	this->LMLabel->SetInput(this->LMLabelText);

}

void vtkLMActor::UpdateDisplayText()
{
//mqMorphoDigCore::instance()->Getmui_FontSize()
	if (mqMorphoDigCore::instance()->Getmui_DisplayLandmarkText() == 1)
	{
		mqMorphoDigCore::instance()->getRenderer()->AddActor(this->GetLMLabelActor3D());
	}
	{
		mqMorphoDigCore::instance()->getRenderer()->RemoveActor(this->GetLMLabelActor3D());
	}
}
void vtkLMActor::CreateLMBody()
{

	
	vtkMatrix4x4 *Mat = this->GetMatrix();
	
	/*cout << "Initial matrix:" << endl;
	Mat->PrintSelf(cout, vtkIndent(1));*/
	Mat->SetElement(0, 3, this->LMOrigin[0]);
	Mat->SetElement(1, 3, this->LMOrigin[1]);
	Mat->SetElement(2, 3, this->LMOrigin[2]);

	vtkMath::Normalize(this->LMOrientation);
			
	// rotation 
	// Landmark object oriented along x axis : 1 0 0 
	// assume rot.  
	// 1) around z 
	// 2)    around x 
	// -> values sinz,cosz,sinx,cosx need to be computed

	float cosz = this->LMOrientation[0]; // nx
	float sinz = sqrt(1.0 - (cosz*cosz));
	float cosx, sinx;
	if (sinz == 0.0) // just in case we are in the speciale case
	{
		cosx = 1.0; sinx = 0.0;
	}
	else
	{
		cosx = this->LMOrientation[1] / sinz;

		sinx = this->LMOrientation[2] / sinz;
	}
	
	Mat->SetElement(0, 0, cosz); //=nx
	Mat->SetElement(1, 0, cosx*sinz); //=ny
	Mat->SetElement(2, 0, sinx *sinz);    //=nz

	Mat->SetElement(0, 1, -sinz);
	Mat->SetElement(1, 1, cosx*cosz);
	Mat->SetElement(2, 1, sinx*cosz);

	Mat->SetElement(0, 2, 0); 
	Mat->SetElement(1, 2, -sinx);
	Mat->SetElement(2, 2, cosx); 


	/*cout << "Modified matrix:" << endl;
	Mat->PrintSelf(cout, vtkIndent(1));*/

	
	this->SetUserMatrix(Mat);

	double pos[3];
	this->GetPosition(pos);
	
	
	if (this->LMBodyType == SPHERE)
	{
		vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();
		
		sphereSource->SetCenter(0, 0, 0);
		sphereSource->SetRadius(0.5*this->LMSize);
		if (this->LMType == TARGET_LMK || this->LMType == HANDLE_LMK)
		{
			sphereSource->SetRadius(0.5*1.4*this->LMSize);
		}
		sphereSource->Update();
		this->LMBody = sphereSource->GetOutput();
	}
	else
	{
		vtkSmartPointer<vtkProbeSource> probeSource =
			vtkSmartPointer<vtkProbeSource>::New();

		probeSource->SetInvert(true);		
		probeSource->SetArrowLength(3*this->LMSize);
		if (this->LMType == TARGET_LMK || this->LMType == HANDLE_LMK)
		{
			probeSource->SetArrowLength(3*1.4*this->LMSize);
		}
		else if (this->LMType == FLAG_LMK)
		{

			//cout << "Probe source arrow length: " << this->LMSize <<this->LMLabelText<<endl;
			probeSource->SetArrowLength(this->LMSize);
			
		}
		probeSource->Modified();
		probeSource->Update();
		
		this->LMBody = probeSource->GetOutput();
	}
	this->Modified();


}



void vtkLMActor::ResetLMColor()
{
	//cout << "ResetLMColor" << this->LMType << endl;
	// Create six colors - one for each line
	double red[4] = { 1, 0.4, 0.4, 1 };// LMType=0 VERT

	double yellow[4] = { 1, 1, 0,0.3 }; 
	double darkgreen[4] = { 0, 0.7, 0, 1 }; 
	double orange[4] = { 1, 0.5, 0, 0.3 }; 
	double green[4] = { 0.5, 1, 0, 1 };  
	double blue[4] = { 0, 0.5, 1, 1 }; 
	
	double cyan[4] = { 0, 1, 1, 1 }; 
	double violet[4] = { 0.7, 0, 1, 0.3 }; 
	double *flgcolor = mqMorphoDigCore::instance()->Getmui_FlagColor();
	double flagcolor[4] = { flgcolor[0], flgcolor[1], flgcolor[2], 0.5 };// LMType=0 VERT
	if (this->LMType == NORMAL_LMK) { this->SetmColor(green); }// LMType = 0 (normal LM) vert
	if (this->LMType == TARGET_LMK) { this->SetmColor(orange); }// LMType = 1 (target LM) jaune
	
	if (this->LMType == NODE_LMK) {
		if (this->LMNodeType == -1 || this->LMNodeType == NORMAL_NODE) {
			this->SetmColor(red);
		}
		if (this->LMNodeType == STARTING_NODE) {
			this->SetmColor(darkgreen);
		}
		if (this->LMNodeType == MILESTONE_NODE) {
			this->SetmColor(blue);
		}
		if (this->LMNodeType == CONNECT_NODE) {
			this->SetmColor(cyan);
		}
	}
	if (this->LMType == HANDLE_LMK) { this->SetmColor(violet); }

	if (this->LMType == FLAG_LMK) { 
		
		this->SetmColor(flagcolor); }


	
}

//----------------------------------------------------------------------------
void vtkLMActor::SetLMType(int type)
{
	this->LMType = type;
	if (type == FLAG_LMK)
	{	
		this->LMBodyType = ARROW; // only arrows for flags!
	}

	this->ResetLMColor();	
	this->Modified();
	this->UpdateProps();
	


}

void vtkLMActor::SetLMNodeType(int nodetype)
{
	if (nodetype < 0) { nodetype = NORMAL_NODE; }
	if (this->LMType == NODE_LMK)
	{
		this->LMNodeType = nodetype;
		this->ResetLMColor();
		this->Modified();
		this->UpdateProps();
	}
	


}

void vtkLMActor::SetLMBodyType(int type)
{
	if (this->LMType != FLAG_LMK)
	{
		if (this->LMBodyType != type && type >= 0 && type <= 1)
		{
			this->LMBodyType = type;
			this->Modified();
			this->UpdateProps();
		}
	}

}



void vtkLMActor::SetLMNumber(int num)
{
	if (this->LMNumber != num && num>0)
	{
		this->LMNumber = num;
		this->Modified();
		this->UpdateProps();
	}

}

void vtkLMActor::ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat)
{
	double init_pos[3] = { this->LMOrigin[0], this->LMOrigin[1],this->LMOrigin[2] };
	double final_pos[3];
	mqMorphoDigCore::TransformPoint(Mat, init_pos, final_pos);

	double init_npos[3] =
	{ this->LMOrigin[0] + this->LMOrientation[0] ,
		this->LMOrigin[1] + this->LMOrientation[1] ,
		this->LMOrigin[2] + this->LMOrientation[2] ,
	};

	double final_npos[3];
	mqMorphoDigCore::TransformPoint(Mat, init_npos, final_npos);

	final_npos[0] = final_npos[0] - final_pos[0];
	final_npos[1] = final_npos[1] - final_pos[1];
	final_npos[2] = final_npos[2] - final_pos[2];
	this->SetLMOriginAndOrientation(final_pos, final_npos);

}

void vtkLMActor::Undo(int mCount)
{

	//cout << "Inside LM actor Undo" << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		//cout << "Undo actor event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		//cout << "Call Pop Undo Stack from LMActor" << endl;
		this->PopUndoStack();
	}
	
	
	

}
void vtkLMActor::Redo(int mCount)
{
	//cout << "Inside actor Undo, try to undo " <<mCount<< endl;
	if (this->UndoRedo->RedoStack.empty())
	{
		//cout << "Redo Stack empty!" << endl;
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
void vtkLMActor::Erase(int mCount)
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

void vtkLMActor::PopUndoStack()
{
	//cout << "Current LM PopUndoStack: " << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
		

	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);	
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	// Now put undo Matrix inside object : 
	Mat->DeepCopy(this->UndoRedo->UndoStack.back().Matrix);
	
	//std::cout << "Matrix about to be saved: " << endl << *SavedMat << std::endl;
	//std::cout << "Matrix retrieved in stack, about to be set: " << endl << *Mat << std::endl;
	// This part has to be done first otherwise matrix retrieval won't work!!!!
	double mcurrLength = this->LMSize;
	std::string mcurrLMText = this->GetLMText();
	if (this->LMType == FLAG_LMK)
	{
		this->LMSize = this->UndoRedo->UndoStack.back().FlagLength;
		this->SetLMText(this->UndoRedo->UndoStack.back().FlagLabel.c_str());
		//cout << "Undo flag label to :" << this->UndoRedo->UndoStack.back().FlagLabel.c_str() << endl;
		mqMorphoDigCore::instance()->UpdateLandmarkSettings(this);
	}

	/*
	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();
	*/
	this->SetUserMatrix(Mat);
	this->GetMatrix(Mat);
	
	//std::cout << "Matrix after setting : " << endl << *Mat << std::endl;

	
	
	double myCurrentColor[4];
	int mCurrentSelected = this->Selected;
	myCurrentColor[0] = this->mColor[0];
	myCurrentColor[1] = this->mColor[1];
	myCurrentColor[2] = this->mColor[2];
	myCurrentColor[3] = this->mColor[3];
	int myCurrentType = this->LMType;
	int myCurrentNodeType = this->LMNodeType;
	this->LMType = this->UndoRedo->UndoStack.back().Type;
	this->LMNodeType = this->UndoRedo->UndoStack.back().NodeType;
	this->mColor[0] = this->UndoRedo->UndoStack.back().Color[0];
	this->mColor[1] = this->UndoRedo->UndoStack.back().Color[1];
	this->mColor[2] = this->UndoRedo->UndoStack.back().Color[2];
	this->mColor[3] = this->UndoRedo->UndoStack.back().Color[3];
	this->SetSelected(this->UndoRedo->UndoStack.back().Selected);
	
//	cout << "PopUndoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->RedoStack.push_back(vtkLMActorUndoRedo::Element(SavedMat, myCurrentColor, mCurrentSelected,myCurrentType, myCurrentNodeType, this->UndoRedo->UndoStack.back().UndoCount, mcurrLength, mcurrLMText));
	this->UndoRedo->UndoStack.pop_back();
	this->CreateLMLabelText();
	this->Modified();
	//this->UpdateProps();
}
void vtkLMActor::PopRedoStack()
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

	//this part has to be done first (for some reason!!) otherwise matrix retrieval won't work!
	double mcurrLength = this->LMSize;
	std::string mcurrLMText = this->GetLMText();
	if (this->LMType == FLAG_LMK)
	{
		this->LMSize = this->UndoRedo->RedoStack.back().FlagLength;
		this->SetLMText(this->UndoRedo->RedoStack.back().FlagLabel.c_str());
		//cout << "Undo flag label to :" << this->UndoRedo->UndoStack.back().FlagLabel.c_str() << endl;
		mqMorphoDigCore::instance()->UpdateLandmarkSettings(this);

	}

	/*vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();*/
	this->SetUserMatrix(Mat);
	this->GetMatrix(Mat);

	double myCurrentColor[4];
	int mCurrentSelected = this->Selected;
	myCurrentColor[0] = this->mColor[0];
	myCurrentColor[1] = this->mColor[1];
	myCurrentColor[2] = this->mColor[2];
	myCurrentColor[3] = this->mColor[3];
	int myCurrentType = this->LMType;
	int myCurrentNodeType = this->LMNodeType;
	this->LMType = this->UndoRedo->RedoStack.back().Type;
	this->LMNodeType = this->UndoRedo->RedoStack.back().NodeType;
	this->mColor[0] = this->UndoRedo->RedoStack.back().Color[0];
	this->mColor[1] = this->UndoRedo->RedoStack.back().Color[1];
	this->mColor[2] = this->UndoRedo->RedoStack.back().Color[2];
	this->mColor[3] = this->UndoRedo->RedoStack.back().Color[3];
	this->SetSelected(this->UndoRedo->RedoStack.back().Selected);
	
	//cout << "PopRedoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->UndoStack.push_back(vtkLMActorUndoRedo::Element(SavedMat, myCurrentColor, mCurrentSelected, myCurrentType, myCurrentNodeType, this->UndoRedo->RedoStack.back().UndoCount, mcurrLength, mcurrLMText));
	this->UndoRedo->RedoStack.pop_back();
	this->CreateLMLabelText();
	this->Modified();
}

void vtkLMActor::SaveState(int mCount)
{
	//cout << "myLMActor Save State"<<endl;
	//cout << "myLMActor Position: redostack clear." << mCount<< ","<<this->LMType<<endl;
	this->UndoRedo->RedoStack.clear();

	//int Count = 2;//MorphoDig::instance()->GetUndoCount()+1;
	int Count = mCount;

	
	// détruit ce qui est trop vieux dans le vector!
	//to do!
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	double myColor[4];
	int mSelected = this->Selected;
	myColor[0] = this->mColor[0];
	myColor[1] = this->mColor[1];
	myColor[2] = this->mColor[2];
	myColor[3] = this->mColor[3];
	//std::cout << "Saved Matrix: " << endl << *Mat << std::endl;
	int mType = this->LMType;
	int mNodeType = this->LMNodeType;
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	//cout << "Saved current LM state: " << endl;
	double length = this->LMSize;
	std::string flaglabel = this->GetLMText();
	//cout << "Save flag label:" << flaglabel << endl;
	this->UndoRedo->UndoStack.push_back(vtkLMActorUndoRedo::Element(SavedMat, myColor, mSelected, mType, mNodeType, mCount,length, flaglabel));

}
//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkLMActor::ShallowCopy(vtkProp *prop)
{
	vtkLMActor *f = vtkLMActor::SafeDownCast(prop);
	

	// Now do superclass
	this->vtkMDActor::ShallowCopy(prop);
}

void vtkLMActor::UpdateProps()
{
	//Recreates the text actor and the LMBody
	//cout << "LM update props" << endl;
	this->CreateLMBody();
	this->CreateLMLabelText();
	this->UpdateDisplayText();

}

//----------------------------------------------------------------------------
void vtkLMActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
// Actual actor render method.
void vtkLMActor::Render(vtkRenderer *ren, vtkMapper *mapper)
{
	this->Superclass::Render(ren, mapper);
}