/*=========================================================================

  Program:   Visualization Toolkti
  Inspired from Module:    vtkInteractorStyleRubberBandPick.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
	 
=========================================================================*/
#include "vtkMDInteractorStyle.h"
#include "vtkMDActor.h"
#include "vtkMDVolume.h"
#include "vtkLMActor.h"
#include "vtkMDVolume.h"
#include "mqUndoStack.h"
#include "mqMorphoDigCore.h"
#include "mqOpenRawDialog.h"
#include "mqSaveNTWDialog.h"
#include "mqCoreUtilities.h"
#include "mqOpenTiff3DDialog.h"
#include "mqOpenDicomStackDialog.h"
#include <vtkBoxWidget.h>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <vtkImageAppend.h>
#include <vtkDICOMImageReader.h>
#include <vtkTIFFReader.h>
#include <vtkPNGReader.h>
#include <vtkBMPReader.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkIndent.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnsignedCharArray.h>
#include <vtkAbstractPropPicker.h>
#include <vtkAssemblyPath.h>
//#include <vtkAreaPicker.h>
#include <vtkRenderedAreaPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkSmartPointer.h>
#include <vtkPropPicker.h>
#include <vtkCellPicker.h>
#include <vtkVolumePicker.h>
#include <vtk3DWidget.h>
#include <vtkPolyDataMapper.h>
vtkStandardNewMacro(vtkMDInteractorStyle);


#define MOVECAM_CURSOR 0
#define MOVEOBJ_CURSOR 1
#define LMK_CURSOR 2
#define SELECT_CURSOR 3
#define LASSO_CURSOR 4
#define PENCIL_CURSOR 5
#define MAGICWAND_CURSOR 6
#define PAINTBUCKET_CURSOR 7

#define CAM 0
#define OBJ 1

#define MOVELMK_MODE 0
#define MOVECAM_MODE 1
#define MOVEOBJ_MODE 2



#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4

#define SPHERE 0
#define ARROW 1

#define LBUTTON_DOWN 0
#define LBUTTON_UP 1

#define RBUTTON_DOWN 0
#define RBUTTON_UP 1


#define VTKISMT_ORIENT 0
#define VTKISMD_SELECT 1
#define VTKISMD_TAGMASKPENCIL 2
#define CTRL_RELEASED 0
#define CTRL_PRESSED 1
#define ALT_PRESSED 4
#define ALT_RELEASED 5
#define L_PRESSED 2
#define L_RELEASED 3
#define T_PRESSED 7
#define T_RELEASED 8
#define M_PRESSED 7
#define M_RELEASED 8
#define K_PRESSED 11
#define K_RELEASED 12
#define J_PRESSED 13
#define J_RELEASED 14


#define SHIFT_PRESSED 9
#define SHIFT_RELEASED 10

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//--------------------------------------------------------------------------

vtkMDInteractorStyle::vtkMDInteractorStyle()
{
	this->CurrentMode = VTKISMT_ORIENT;
	this->Alt = ALT_RELEASED;
	//this->MoveMode = MOVECAM_MODE;
	this->L = L_RELEASED;
	this->J = J_RELEASED;
	this->K = K_RELEASED;
	this->T = T_RELEASED;
	this->M = M_RELEASED;
	this->MoveWhat = CAM;
	this->Ctrl = CTRL_RELEASED;
	this->Shift = SHIFT_RELEASED;
	this->LM_Button = LBUTTON_UP;
	this->RM_Button = RBUTTON_UP;
	this->StartPosition[0] = this->StartPosition[1] = 0;
	this->EndPosition[0] = this->EndPosition[1] = 0;
	this->TagMaskPencilCenterPosition[0] = this->TagMaskPencilCenterPosition[1] = 0;
	this->TagMaskPencilStarted = 0;
	this->Moving = 0;
	this->PixelArray = vtkUnsignedCharArray::New();
	this->PixelArray2 = vtkUnsignedCharArray::New();
	this->ActorCollection = vtkSmartPointer<vtkMDActorCollection>::New();
	this->VolumeCollection = vtkSmartPointer<vtkMDVolumeCollection>::New();
	this->NormalLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->TargetLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->NodeLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->HandleLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->FlagLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->ActorsPositionsSaved = 0;
	this->NumberOfSelectedActors = 0;
}
void vtkMDInteractorStyle::SetVolumeCollection(vtkSmartPointer<vtkMDVolumeCollection> VolColl)
{
	this->VolumeCollection = VolColl;
}
void vtkMDInteractorStyle::SetActorCollection(vtkSmartPointer<vtkMDActorCollection> ActColl)
{
	this->ActorCollection = ActColl;
}
void vtkMDInteractorStyle::SetNormalLandmarkCollection(vtkSmartPointer<vtkLMActorCollection> LmkColl)
{
	this->NormalLandmarkCollection = LmkColl;
}
void vtkMDInteractorStyle::SetTargetLandmarkCollection(vtkSmartPointer<vtkLMActorCollection> LmkColl)
{
	this->TargetLandmarkCollection = LmkColl;
}
void vtkMDInteractorStyle::SetNodeLandmarkCollection(vtkSmartPointer<vtkLMActorCollection> LmkColl)
{
	this->NodeLandmarkCollection = LmkColl;
}
void vtkMDInteractorStyle::SetHandleLandmarkCollection(vtkSmartPointer<vtkLMActorCollection> LmkColl)
{
	this->HandleLandmarkCollection = LmkColl;
}
void vtkMDInteractorStyle::SetFlagLandmarkCollection(vtkSmartPointer<vtkLMActorCollection> LmkColl)
{
	this->FlagLandmarkCollection = LmkColl;
}
//--------------------------------------------------------------------------
vtkMDInteractorStyle::~vtkMDInteractorStyle()
{
  this->PixelArray->Delete();
  this->PixelArray2->Delete();
}

void vtkMDInteractorStyle::EndRotate()
{
	this->EndLandmarkMovements();
	this->Superclass::EndRotate();

}
void vtkMDInteractorStyle::EndSpin()
{
	this->EndLandmarkMovements();
	this->Superclass::EndSpin();

}
void vtkMDInteractorStyle::EndPan()
{
	this->EndLandmarkMovements();
	this->Superclass::EndPan();

}

void vtkMDInteractorStyle::EndLandmarkMovements(vtkLMActor *myActor)
{
	vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
	
	if (myActor->GetSelected() == 1)
	{
		// do something
		vtkMatrix4x4 *Mat = myActor->GetMatrix();

		/*cout << "Initial matrix:" << endl;
		Mat->PrintSelf(cout, vtkIndent(1));*/
		double lmorigin[3] = { Mat->GetElement(0, 3), Mat->GetElement(1, 3), Mat->GetElement(2, 3) };
		//double lmorientation[3] = { 0,0,1 };
		double a[3] = { 0,0,0 };
		double at[3];
		double b[3] = { 1,0,0 };
		double bt[3];
		this->TransformPoint(Mat, a, at);
		this->TransformPoint(Mat, b, bt);
		double lmorientation[3] = { bt[0] - at[0],bt[1] - at[1] ,bt[2] - at[2] };
		vtkMath::Normalize(lmorientation);
		//myActor->SetLMOrigin(lmorigin);
		myActor->SetLMOriginAndOrientation(lmorigin, lmorientation);

		//Mat->SetElement(1, 3, this->LMOrigin[1]);
		//Mat->SetElement(2, 3, this->LMOrigin[2]);

	}
}
void vtkMDInteractorStyle::EndLandmarkMovements()
{

	//we reset LMOrigin and LMOrientation
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		this->EndLandmarkMovements(myActor);
	}
	//we reset LMOrigin and LMOrientation
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		this->EndLandmarkMovements(myActor);
	}
	//we reset LMOrigin and LMOrientation
	int node_modified = 0;
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			this->EndLandmarkMovements(myActor);
			node_modified = 1;
		}

	}
	if (node_modified == 1) { this->NodeLandmarkCollection->Modified(); }
	//we reset LMOrigin and LMOrientation
	
	int handle_modified = 0;
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			this->EndLandmarkMovements(myActor);
			handle_modified = 1;
		}

	}
	
	if (handle_modified == 1) { this->HandleLandmarkCollection->Modified(); }
	
	
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		this->EndLandmarkMovements(myActor);
	}

}


//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
  void vtkMDInteractorStyle::OnKeyPress()
  {
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	
	// Output the key that was pressed
	cout << key << endl;
	if (key.compare("Escape") == 0)
	{
		mqMorphoDigCore::instance()->SwitchMoveMode();
	}
	if (key.compare("space") == 0)
	{
		mqMorphoDigCore::instance()->SwitchLandmarkMode();
	}
	if (key.compare("Alt_L")==0)
	{
		this->Alt = ALT_PRESSED;
	}
	if (key.compare("l") == 0 || key.compare("L") == 0)
	{
		//cout << "l pressed" << endl;
		this->L = L_PRESSED;
	}
	if (key.compare("k") == 0 || key.compare("K") == 0)
	{
		//cout << "l pressed" << endl;
		this->L = L_PRESSED;
		this->K = K_PRESSED;

	}
	if (key.compare("j") == 0 || key.compare("J") == 0)
	{
		//cout << "l pressed" << endl;
		this->L = L_PRESSED;
		this->J = J_PRESSED;
	}
	if (key.compare("t") == 0 || key.compare("T") == 0 && this->CurrentMode != VTKISMD_TAGMASKPENCIL)
	{
		//cout << "l pressed" << endl;
		this->T = T_PRESSED;
		
		
		if (mqMorphoDigCore::instance()->Getmui_TagModeActivated())
		{
			
			int tool = mqMorphoDigCore::instance()->Getmui_TagTool();
			if (tool ==0)// pencil
			{
				this->CurrentMode = VTKISMD_TAGMASKPENCIL;
				mqMorphoDigCore::instance()->setCurrentCursor(5);
				//cout << "Tag Pencil start" << endl;
				
				this->TagMaskPencilStart();
				this->RedrawTagMaskPencilCircle();
				cout << "T pressed" << endl;
			}
			else // paint buckett
			{
				mqMorphoDigCore::instance()->setCurrentCursor(6);
			}
		}
	}
	if ((key.compare("m") == 0 || key.compare("M") == 0)&& this->CurrentMode != VTKISMD_TAGMASKPENCIL)
	{
		cout << "this->CurrentMode="<< this->CurrentMode<< endl;
		this->M = M_PRESSED;
		if (mqMorphoDigCore::instance()->Getmui_TagModeActivated())
		{
			int tool = mqMorphoDigCore::instance()->Getmui_TagTool();
			if (tool == 0)// pencil
			{


				this->CurrentMode = VTKISMD_TAGMASKPENCIL;
				mqMorphoDigCore::instance()->setCurrentCursor(5);
				//cout << "Tag Pencil start" << endl;
				this->TagMaskPencilStart();
				this->RedrawTagMaskPencilCircle();
				cout << "M pressed" << endl;
			}
		}
		
	}
	if (key.compare("Shift_L") == 0)
	{
		this->Shift = SHIFT_PRESSED;
		this->L = L_PRESSED;
		/*QPixmap cursor_pixmap = QPixmap(":/Cursors/move3.png");
		QCursor projectCursor = QCursor(cursor_pixmap, 0, 0);
		//std::cout << key<< "Pressed" << '\n';*/
	}
	if (key.compare("Control_L") == 0)
	{
		this->Ctrl = CTRL_PRESSED;
		//this->L = L_PRESSED;
		/*QPixmap cursor_pixmap = QPixmap(":/Cursors/move3.png");
		QCursor projectCursor = QCursor(cursor_pixmap, 0, 0);
		//std::cout << key<< "Pressed" << '\n';*/
	}
	//cout << this->Interactor->GetKeyCode() << endl;
	//cout << rwi->GetKeySym() << endl;
	if (key.compare("Delete") == 0)
	{
		//std::cout << "Pressed " << key << std::endl;
		this->DeleteSelectedActors();
		
		
	}
	if (key.compare("a") == 0)
	{		
			//cout << "a pressed!" << endl;
		int node_handle_collections_changed = 0;
			if (this->Ctrl == CTRL_PRESSED)
			{
				std::string action = "Select all actors";
				int Count = BEGIN_UNDO_SET(action);
				this->VolumeCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
				{
					vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
					if (myVolume->GetSelected() == 0)
					{
						myVolume->SaveState(Count);
					}
				}

				this->ActorCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
				{
					vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
					}
				}

				this->NormalLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
					}
				}
				this->TargetLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
					}
				}
				this->NodeLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
						node_handle_collections_changed = 1;
					}
				}
				this->HandleLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
						node_handle_collections_changed = 1;
					}
				}
				this->FlagLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
					}
				}
				END_UNDO_SET();
				//cout << "a and CTRL pressed!" << endl;
				this->ActorCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
				{
					vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SetSelected(1);
						myActor->SetisVisible(1);
						myActor->SetChanged(1);
						
					}


				}
				this->VolumeCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
				{
					vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
					if (myVolume->GetSelected() == 0)
					{
						myVolume->SetisVisible(1);
						myVolume->SetSelected(1);						
						myVolume->SetChanged(1);

					}


				}
				this->NormalLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SetSelected(1);
						myActor->SetChanged(1);
					}
				}
				this->TargetLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SetSelected(1);
						myActor->SetChanged(1);
					}
				}
				this->NodeLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SetSelected(1);
						myActor->SetChanged(1);
					}
				}
				this->HandleLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SetSelected(1);
						myActor->SetChanged(1);
					}
				}
				this->FlagLandmarkCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
					if (myActor->GetSelected() == 0)
					{
						myActor->SetSelected(1);
						myActor->SetChanged(1);
					}
				}
				vtkRenderWindowInteractor *rwi = this->Interactor;
				if (node_handle_collections_changed == 1)
				{
					this->NodeLandmarkCollection->Modified();
					this->HandleLandmarkCollection->Modified();
				}
				mqMorphoDigCore::instance()->signal_actorSelectionChanged();
				mqMorphoDigCore::instance()->signal_volumeSelectionChanged();
				cout << "Render from onKeyPress" << endl;
				rwi->Render();
			}
			

	}
	if (key.compare("d") == 0)
		{
		
			if (this->Ctrl == CTRL_PRESSED)
			{


				std::string action = "Unselect all actors";
				int Count = BEGIN_UNDO_SET(action);
				mqMorphoDigCore::instance()->UnselectAll(Count);

				
			
				END_UNDO_SET();
				vtkRenderWindowInteractor *rwi = this->Interactor;
				cout << "Render from onKeyPress2" << endl;
				rwi->Render();
			}
			
	}
	if (key.compare("o") == 0)
	{

		if (this->Ctrl == CTRL_PRESSED)
		{
			this->Ctrl = CTRL_RELEASED;
			cout << "Open Data!" << endl;
			//mqMorphoDigCore::instance()->getUndoStack();
			cout << "Open Data!" << endl;

			
			QStringList filenames = QFileDialog::getOpenFileNames(mqCoreUtilities::mainWidget(),
				QObject::tr("Load data"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
				QObject::tr("MorphoDig data or project (*.ntw *.ver *.cur *.stv *.tag *.tgp *.pos *.ori *.flg *.lmk *.tps *.pts *.ply *.stl *.vtk *.obj *.vtp *.mha *.mhd *.vti *.raw *.tif *.tiff *.bmp *.png *.dcm *.ima )"));
			int cpt_tiff = 0;
			int tiff_3D = 1;
			int cpt_bmp = 0;
			int cpt_png = 0;
			if (!filenames.isEmpty())
			{

				
				for (int i = 0; i < filenames.count(); i++)
				{
					QString fileName = filenames.at(i);
					
					std::string PNGext(".png");
					std::string PNGext2(".PNG");

					std::size_t found = fileName.toStdString().find(PNGext);
					std::size_t found2 = fileName.toStdString().find(PNGext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						cpt_png++;
						tiff_3D = 0;

					}

				}
				
				for (int i = 0; i < filenames.count(); i++)
				{
					QString fileName = filenames.at(i);
					std::string BMPext(".bmp");
					std::string BMPext2(".BMP");

					std::size_t found = fileName.toStdString().find(BMPext);
					std::size_t found2 = fileName.toStdString().find(BMPext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						cpt_bmp++;
						tiff_3D = 0;

					}

				}

				for (int i = 0; i < filenames.count(); i++)
				{
					QString fileName = filenames.at(i);
					std::string TIFext(".tif");
					std::string TIFext2(".TIF");
					std::string TIFext3(".tiff");
					std::string TIFext4(".TIFF");
					std::size_t found = fileName.toStdString().find(TIFext);
					std::size_t found2 = fileName.toStdString().find(TIFext2);
					std::size_t found3 = fileName.toStdString().find(TIFext3);
					std::size_t found4 = fileName.toStdString().find(TIFext4);
					if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
					{
						cpt_tiff++;
						//Tif TIFF

					}

				}
				if (cpt_tiff > 1)
				{
					tiff_3D = 0;
					cout << "Should not open 3D tiff!" << endl;

				}

				int cpt_dcm = 0;
				for (int i = 0; i < filenames.count(); i++)
				{
					QString fileName = filenames.at(i);
					std::string DCMext(".dcm");
					std::string DCMext2(".DCM");
					std::string DCMext3(".ima");
					std::string DCMext4(".IMA");
					std::size_t found = fileName.toStdString().find(DCMext);
					std::size_t found2 = fileName.toStdString().find(DCMext2);
					std::size_t found3 = fileName.toStdString().find(DCMext3);
					std::size_t found4 = fileName.toStdString().find(DCMext4);
					if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
					{
						cpt_dcm++;
						//dcm

					}

				}

				for (int i = 0; i < filenames.count(); i++)
				{
					QString fileName = filenames.at(i);
					cout << fileName.toStdString() << endl;
					if (fileName.isEmpty()) return;
					QFileInfo fileInfo(fileName);
					mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

					std::string STLext(".stl");
					std::string STLext2(".STL");
					std::string VTKext(".vtk");
					std::string VTKext2(".VTK");
					std::string VTKext3(".vtp");
					std::string VTKext4(".VTP");
					std::string PLYext(".ply");
					std::string PLYext2(".PLY");
					std::string OBJext(".obj");
					std::string OBJext2(".OBJ");
					std::string NTWext(".ntw");
					std::string NTWext2(".NTW");
					std::string VERext(".ver");
					std::string VERext2(".VER");
					std::string TPSext(".tps");
					std::string TPSext2(".TPS");
					std::string PTSext(".pts");
					std::string PTSext2(".PTS");
					std::string CURext(".cur");
					std::string CURext2(".CUR");
					std::string FLGext(".flg");
					std::string FLGext2(".FLG");
					std::string LMKext(".lmk");
					std::string LMKext2(".LMK");
					std::string TAGext(".tag");
					std::string TAGext2(".TAG");
					std::string TAGext3(".tgp");
					std::string TAGext4(".TGP");
					std::string STVext(".stv");
					std::string STVext2(".STV");
					std::string ORIext(".ori");
					std::string ORIext2(".ORI");
					std::string POSext(".pos");
					std::string POSext2(".POS");
					std::string MHAext(".mha");
					std::string MHAext2(".MHA");
					std::string MHDext(".mhd");
					std::string MHDext2(".MHD");
					std::string VTIext(".vti");
					std::string VTIext2(".VTI");
					std::string RAWext(".raw");
					std::string RAWext2(".RAW");
					std::string TIFext(".tif");
					std::string TIFext2(".TIF");
					std::string TIFext3(".tiff");
					std::string TIFext4(".TIFF");
					std::string BMPext(".bmp");
					std::string BMPext2(".BMP");
					std::string PNGext(".png");
					std::string PNGext2(".PNG");
					std::string DCMext(".dcm");
					std::string DCMext2(".DCM");
					std::string DCMext3(".ima");
					std::string DCMext4(".IMA");

					int type = 0; //0 = stl, 1 = vtk,  2 = ply, 3 = ntw, 4 ver, 5 cur, 6 flg, 7 lmk, 8 tag, 9 stv, 10 ori, 11 pos 13 MHA MHD VTI
					std::size_t found = fileName.toStdString().find(STLext);
					std::size_t found2 = fileName.toStdString().find(STLext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 0;
						//STL
					}

					found = fileName.toStdString().find(VTKext);
					found2 = fileName.toStdString().find(VTKext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 1; //VTK
					}
					std::size_t found3 = fileName.toStdString().find(VTKext3);
					std::size_t found4 = fileName.toStdString().find(VTKext4);
					if (found3 != std::string::npos || found4 != std::string::npos)
					{
						type = 4; //VTK
					}
					//std::cout << "2Type= " <<type<< std::endl;
					found = fileName.toStdString().find(PLYext);
					found2 = fileName.toStdString().find(PLYext2);

					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 2; //PLY
					}

					//std::cout << "2Type= " <<type<< std::endl;
					found = fileName.toStdString().find(OBJext);
					found2 = fileName.toStdString().find(OBJext2);

					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 3; //OBJ
					}

					//0 = stl, 1 = vtk,  2 = ply, 3 = ntw, 4 ver, 5 cur, 6 flg, 7 lmk
					found = fileName.toStdString().find(NTWext);
					found2 = fileName.toStdString().find(NTWext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 4; //NTW
					}

					//4 ver, 5 cur, 6 flg, 7 lmk
					found = fileName.toStdString().find(VERext);
					found2 = fileName.toStdString().find(VERext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 5; //VER
					}

					found = fileName.toStdString().find(CURext);
					found2 = fileName.toStdString().find(CURext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 6; //CUR
					}
					found = fileName.toStdString().find(FLGext);
					found2 = fileName.toStdString().find(FLGext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 7; //FLG
					}
					found = fileName.toStdString().find(LMKext);
					found2 = fileName.toStdString().find(LMKext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 8; //LMK
					}
					found = fileName.toStdString().find(TAGext);
					found2 = fileName.toStdString().find(TAGext2);
					found3 = fileName.toStdString().find(TAGext3);
					found4 = fileName.toStdString().find(TAGext4);
					if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
					{
						type = 9; //TAG
					}
					found = fileName.toStdString().find(STVext);
					found2 = fileName.toStdString().find(STVext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 10; //STV
					}
					//8 tag, 9 stv, 10 ori, 11 pos
					found = fileName.toStdString().find(ORIext);
					found2 = fileName.toStdString().find(ORIext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 11; //ORI
					}
					found = fileName.toStdString().find(POSext);
					found2 = fileName.toStdString().find(POSext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 12; //POS
					}
					found = fileName.toStdString().find(MHAext);
					found2 = fileName.toStdString().find(MHAext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 13; //MHA MHD VTI
					}
					found = fileName.toStdString().find(MHDext);
					found2 = fileName.toStdString().find(MHDext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 13; //MHA MHD VTI
					}
					found = fileName.toStdString().find(VTIext);
					found2 = fileName.toStdString().find(VTIext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 13; //MHA MHD VTI
					}

					found = fileName.toStdString().find(PTSext);
					found2 = fileName.toStdString().find(PTSext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 14; //PTS
					}

					found = fileName.toStdString().find(TPSext);
					found2 = fileName.toStdString().find(TPSext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						type = 15; //TPS
					}
					found = fileName.toStdString().find(RAWext);
					found2 = fileName.toStdString().find(RAWext2);
					if (found != std::string::npos || found2 != std::string::npos)
					{
						cout << "RAW" << endl;
						type = 16; //RAW
					}
					found = fileName.toStdString().find(TIFext);
					found2 = fileName.toStdString().find(TIFext2);
					found3 = fileName.toStdString().find(TIFext3);
					found4 = fileName.toStdString().find(TIFext4);
					std::size_t found5 = fileName.toStdString().find(BMPext);
					std::size_t found6 = fileName.toStdString().find(BMPext2);
					std::size_t found7 = fileName.toStdString().find(PNGext);
					std::size_t found8 = fileName.toStdString().find(PNGext2);
					if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos
						|| found5 != std::string::npos || found6 != std::string::npos || found7 != std::string::npos || found8 != std::string::npos
						)
					{
						
						type = 17;// TIFF PNG BMP
						cout << "CTRL+0 found tiff png bmp" << endl;
						
					}
					found = fileName.toStdString().find(DCMext);
					found2 = fileName.toStdString().find(DCMext2);
					found3 = fileName.toStdString().find(DCMext3);
					found4 = fileName.toStdString().find(DCMext4);
					if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
					{
						type = 18;


					}

					if (type < 4)
					{
						int ok=mqMorphoDigCore::instance()->OpenMesh(fileName);
						if (ok == 1) { mqMorphoDigCore::instance()->CreateSurfaceUndoSet(1); }
					}
					else if (type == 4)
					{
						mqMorphoDigCore::instance()->OpenNTW(fileName);
					}
					else if (type == 5)
					{
						mqMorphoDigCore::instance()->OpenVER(fileName, 0);
					}
					else if (type == 6)
					{
						mqMorphoDigCore::instance()->OpenCUR(fileName);
					}
					else if (type == 7)
					{
						mqMorphoDigCore::instance()->OpenFLG(fileName);
					}
					else if (type == 8)
					{
						mqMorphoDigCore::instance()->OpenLMK(fileName, 0);
					}
					else if (type == 9)
					{
						mqMorphoDigCore::instance()->OpenTAGMAP(fileName);
					}
					else if (type == 10)
					{
						mqMorphoDigCore::instance()->OpenSTV(fileName);
					}
					else if (type == 11)
					{
						mqMorphoDigCore::instance()->OpenORI(fileName);
					}
					else if (type == 12)
					{
						mqMorphoDigCore::instance()->OpenPOS(fileName, 1);
					}
					else if (type == 13)
					{
						mqMorphoDigCore::instance()->OpenVolume(fileName);
					}
					else if (type == 14)
					{
						mqMorphoDigCore::instance()->OpenPTS(fileName, 0);
					}
					else if (type == 15)
					{
						mqMorphoDigCore::instance()->OpenTPS(fileName, 0);
					}
					else if (type == 16)
					{
						//mqMorphoDigCore::instance()->OpenTPS(fileName, 0);
						mqOpenRawDialog OpenRaw_dialog(mqCoreUtilities::mainWidget());
						OpenRaw_dialog.setFileName(fileName);
						OpenRaw_dialog.exec();
					}
					else if (type == 17)
					{
						if (tiff_3D == 1)
						{

							mqOpenTiff3DDialog OpenTiff3D_dialog(mqCoreUtilities::mainWidget());
							OpenTiff3D_dialog.setFileName(fileName);
							OpenTiff3D_dialog.exec();
						}
						else
						{
							//do nothing!
						}
					}
					else if (type == 18)
					{

						//do nothing!

					}



				}
				if (cpt_tiff > 1 || cpt_png> 1 || cpt_bmp>1)
				{
					int first_image = 1;
					int found_3Dtiff = 0;
					int wrong_dims_msg = 0;
					int dimX = 0;
					int dimY = 0;
					int dimZ = 0;
					//QString stackName = "";
					QString firstFileName = "";
					vtkSmartPointer<vtkImageAppend> imageAppend = vtkSmartPointer<vtkImageAppend>::New();
					imageAppend->SetAppendAxis(2);

					for (int i = 0; i < filenames.count(); i++)
					{
						QString fileName = filenames.at(i);
						std::string TIFext(".tif");
						std::string TIFext2(".TIF");
						std::string TIFext3(".tiff");
						std::string TIFext4(".TIFF");
						std::string BMPext(".bmp");
						std::string BMPext2(".BMP");
						std::string PNGext(".png");
						std::string PNGext2(".PNG");

						std::size_t found = fileName.toStdString().find(TIFext);
						std::size_t found2 = fileName.toStdString().find(TIFext2);
						std::size_t found3 = fileName.toStdString().find(TIFext3);
						std::size_t found4 = fileName.toStdString().find(TIFext4);
						std::size_t found5 = fileName.toStdString().find(BMPext);
						std::size_t found6 = fileName.toStdString().find(BMPext2);
						std::size_t found7 = fileName.toStdString().find(PNGext);
						std::size_t found8 = fileName.toStdString().find(PNGext2);
						int file_format = 0; // tiff
						if (found5 != std::string::npos || found6 != std::string::npos)
						{
							file_format = 1; //bmp

						}
						if (found7 != std::string::npos || found8 != std::string::npos)
						{
							file_format = 2; //png

						}

						if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos
							|| found5 != std::string::npos || found6 != std::string::npos || found7 != std::string::npos || found8 != std::string::npos)
						{

							QFile file(fileName);
							QString name = "";
							if (file.exists()) {
								dimZ++;

								name = file.fileName(); // Return only a file name		
								vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
								if (file_format == 0)
								{
									vtkSmartPointer <vtkTIFFReader> tiffReader = vtkSmartPointer<vtkTIFFReader>::New();
									tiffReader->SetFileName(fileName.toLocal8Bit());
									//tiffReader->GetF
									tiffReader->Update();
									input = tiffReader->GetOutput();
								}
								else if (file_format == 1)
								{
									vtkSmartPointer <vtkBMPReader> bmpReader = vtkSmartPointer<vtkBMPReader>::New();
									bmpReader->SetAllow8BitBMP(true);
									bmpReader->SetFileName(fileName.toLocal8Bit());
									//tiffReader->GetF
									bmpReader->Update();
									input = bmpReader->GetOutput();
								}
								else
								{
									vtkSmartPointer <vtkPNGReader> pngReader = vtkSmartPointer<vtkPNGReader>::New();
									pngReader->SetFileName(fileName.toLocal8Bit());
									//tiffReader->GetF
									pngReader->Update();
									input = pngReader->GetOutput();
								}
							
								int dim[3];
								input->GetDimensions(dim);

								if (dim[2] != 1)
								{
									found_3Dtiff = 1;
									//mettre un message : pas possible de mettre un 3D tiff 
									QMessageBox msgBox;
									msgBox.setText("Error: please do not mix 2D and 3D TIFF files nor open several 3D TIFF files at once.");
									msgBox.exec();
									return;
								}
								if (first_image == 1)
								{
									firstFileName = fileName;
									// now try to set dimX and dimY based on the first image.																							
									cout << "First image dimensions: " << dim[0] << "," << dim[1] << "," << dim[2] << endl;

									dimX = dim[0];
									dimY = dim[1];
									imageAppend->AddInputData(input);
									first_image = 0;
								}
								else
								{
									if (dimX != dim[0] || dimY != dim[1])
									{
										cout << "found an image of wrong dimensions" << endl;
										if (wrong_dims_msg == 0)
										{
											wrong_dims_msg = 1;
											QMessageBox msgBox;
											QString msg = "At least one 2D image differs in dimensions from those of the first opened image(" + QString(dimX) + "," + QString(dimY) + "). These files will be ignored.";
											msgBox.setText(msg);
											msgBox.exec();
										}

									}
									else
									{
										imageAppend->AddInputData(input);
									}

								}
								file.close();
							}
							//Tif TIFF

						}

					}// end foreach
					imageAppend->Update();
					mqOpenTiff3DDialog OpenTiff3D_dialog(mqCoreUtilities::mainWidget());
					OpenTiff3D_dialog.setInputAsStack();
					OpenTiff3D_dialog.set2DStackInput(imageAppend->GetOutput());
					int dim[3];
					imageAppend->GetOutput()->GetDimensions(dim);
					OpenTiff3D_dialog.setDimensions(dim[0], dim[1], dim[2]);
					OpenTiff3D_dialog.setDataType(imageAppend->GetOutput()->GetScalarType());
					OpenTiff3D_dialog.setFileName(firstFileName);
					OpenTiff3D_dialog.exec();
				}

				// now the DICOM case

				if (cpt_dcm > 1)
				{
					//spacingZ est obtenu en ouvrant au moins 2 coupes. Soit �a marche avec getOrigin... sinon il faut arriver � ouvrir 2 coupes d'un coup!
					int first_image = 1;
					int second_image = 1;
					float origin_first[3];
					float origin_second[3];

					int found_3DDCM = 0;
					int wrong_dims_msg = 0;
					int dimX = 0;
					int dimY = 0;
					int dimZ = 0;
					double spacingX = 1;
					double spacingY = 1;
					double spacingZ = 1;
					//QString stackName = "";
					QString firstFileName = "";
					QString patientName = "";
					int frontToBack = 1;
					vtkSmartPointer<vtkImageAppend> imageAppend = vtkSmartPointer<vtkImageAppend>::New();
					imageAppend->SetAppendAxis(2);

					for (int i = 0; i < filenames.count(); i++)
					{
						QString fileName = filenames.at(i);
						std::string DCMext(".dcm");
						std::string DCMext2(".DCM");
						std::string DCMext3(".ima");
						std::string DCMext4(".IMA");

						std::size_t found = fileName.toStdString().find(DCMext);
						std::size_t found2 = fileName.toStdString().find(DCMext2);
						std::size_t found3 = fileName.toStdString().find(DCMext3);
						std::size_t found4 = fileName.toStdString().find(DCMext4);

						if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
						{

							QFile file(fileName);
							QString name = "";
							if (file.exists()) {
								dimZ++;

								name = file.fileName(); // Return only a file name		
								vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
								vtkSmartPointer <vtkDICOMImageReader> dcmReader = vtkSmartPointer<vtkDICOMImageReader>::New();
								dcmReader->SetFileName(fileName.toLocal8Bit());
								//tiffReader->GetF
								dcmReader->Update();
								input = dcmReader->GetOutput();
								int dim[3];
								double spacing[3];
								input->GetDimensions(dim);

								input->GetSpacing(spacing);

								if (dim[2] != 1 && dimZ > 1)
								{
									found_3DDCM = 1;
									//mettre un message : pas possible de mettre un 3D tiff 
									QMessageBox msgBox;
									msgBox.setText("Error: please only drag and drop several 2D DICOM files or one single 3D DICOM volume. Do not mix 2D and 3D DICOM files.");
									msgBox.exec();
									return;
								}
								else if (dim[2] == 1 && found_3DDCM == 1)
								{
									QMessageBox msgBox;
									msgBox.setText("Error: please only drag and drop several 2D DICOM files or one single 3D DICOM volume. Do not mix 2D and 3D DICOM files.");
									msgBox.exec();
									return;
								}
								if (first_image == 1)
								{
									patientName = dcmReader->GetPatientName();
									firstFileName = fileName;
									// now try to set dimX and dimY based on the first image.																							
									cout << "First image dimensions: " << dim[0] << "," << dim[1] << "," << dim[2] << "," << spacing[0] << "," << spacing[1] << "," << spacing[2] << endl;
									float *pos;
									pos = dcmReader->GetImagePositionPatient();
									origin_first[0] = pos[0];
									origin_first[1] = pos[1];
									origin_first[2] = pos[2];
									cout << "First image origin" << origin_first[0] << "," << origin_first[1] << "," << origin_first[2] << endl;
									dimX = dim[0];
									dimY = dim[1];
									spacingX = spacing[0];
									spacingY = spacing[1];
									//spacingZ = spacing[2];

									imageAppend->AddInputData(input);
									first_image = 0;
								}
								else
								{
									if (first_image == 0 && second_image == 1) {
										second_image = 0;
										//input->GetOrigin(origin_second);
										float *pos;
										pos = dcmReader->GetImagePositionPatient();
										origin_second[0] = pos[0];
										origin_second[1] = pos[1];
										origin_second[2] = pos[2];
										cout << "Second image origin" << origin_second[0] << "," << origin_second[1] << "," << origin_second[2] << endl;
										double diff = (double)(origin_first[2] - origin_second[2]);
										if (diff < 0) { frontToBack = 1; }
										else { frontToBack = 0; }
										spacingZ = abs(diff);


									}
									if (dimX != dim[0] || dimY != dim[1]
										//||spacingX != spacing[0] || spacingY != spacing[1] || spacingZ != spacing[2]
										)
									{
										cout << "found an image of wrong dimensions" << endl;
										if (wrong_dims_msg == 0)
										{
											wrong_dims_msg = 1;
											QMessageBox msgBox;
											QString msg = "At least one DICOM image differs in dimensions or voxel size from those of the first opened DICOM image(" + QString(dimX) + "," + QString(dimY) + "). These files will be ignored.";
											msgBox.setText(msg);
											msgBox.exec();
										}

									}
									else
									{
										imageAppend->AddInputData(input);
									}

								}
								file.close();
							}
							//Tif TIFF

						}

					}// end foreach
					imageAppend->Update();
					mqOpenDicomStackDialog OpenDicomStack_dialog(mqCoreUtilities::mainWidget());
					OpenDicomStack_dialog.setInputAsStack();
					OpenDicomStack_dialog.set2DStackInput(imageAppend->GetOutput());
					int dim[3];
					imageAppend->GetOutput()->GetDimensions(dim);

					OpenDicomStack_dialog.setDimensions(dim[0], dim[1], dim[2]);
					OpenDicomStack_dialog.setSpacing(spacingX, spacingY, spacingZ);
					cout << "frontToBack=" << frontToBack << endl;
					OpenDicomStack_dialog.setFrontToBack(frontToBack);
					OpenDicomStack_dialog.setDataType(imageAppend->GetOutput()->GetScalarType());
					OpenDicomStack_dialog.setFileName(firstFileName);
					OpenDicomStack_dialog.setPatientName(patientName);
					OpenDicomStack_dialog.exec();
				}


			}

		
			//  @@ to do!



		}

	}
	if (key.compare("s") == 0)
	{

		if (this->Ctrl == CTRL_PRESSED)
		{

			this->Ctrl = CTRL_RELEASED;
			//  @@ to do!
			vtkIdType num_sel_Actors = 0;
			vtkIdType num_sel_Volumes = 0;
			vtkIdType num_sel_Fags = 0;
			vtkIdType num_sel_Normal_Landmarks = 0;
			vtkIdType num_sel_Target_Landmarks = 0;
			vtkIdType num_sel_Node_Landmarks = 0;
			vtkIdType num_sel_Handle_Landmarks = 0;

			vtkIdType num_Actors = 0;
			vtkIdType num_Volumes = 0;

			vtkIdType num_Fags = 0;
			vtkIdType num_Normal_Landmarks = 0;
			vtkIdType num_Target_Landmarks = 0;
			vtkIdType num_Node_Landmarks = 0;
			vtkIdType num_Handle_Landmarks = 0;


			num_sel_Actors = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
			num_sel_Volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
			num_sel_Fags = mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
			num_sel_Normal_Landmarks = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
			num_sel_Target_Landmarks = mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
			num_sel_Node_Landmarks = mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
			num_sel_Handle_Landmarks = mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();

			num_Actors = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
			num_Volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfItems();
			num_Fags = mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfItems();
			num_Normal_Landmarks = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
			num_Target_Landmarks = mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();
			num_Node_Landmarks = mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
			num_Handle_Landmarks = mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();


			if (num_sel_Actors == 0
				&& num_sel_Volumes == 0
				&& num_sel_Fags == 0
				&& num_sel_Normal_Landmarks == 0
				&& num_sel_Target_Landmarks == 0
				&& num_sel_Node_Landmarks == 0
				&& num_sel_Handle_Landmarks == 0

				) {
				QMessageBox msgBox;
				msgBox.setText("Select at least one surface/flag/landmark/curve/volume element to use this function!");
				msgBox.exec();
				return;
			}

			if (num_sel_Actors != num_Actors
				|| num_sel_Volumes != num_Volumes
				|| num_sel_Fags != num_Fags
				|| num_sel_Normal_Landmarks != num_Normal_Landmarks
				|| num_sel_Target_Landmarks != num_Target_Landmarks
				|| num_sel_Node_Landmarks != num_Node_Landmarks
				|| num_sel_Handle_Landmarks != num_Handle_Landmarks


				) {
				QMessageBox msgBox;
				msgBox.setText("Some elements are unselected and will not be saved. Continue ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				msgBox.setDefaultButton(QMessageBox::No);
				int ret = msgBox.exec();
				if (ret == QMessageBox::No) { return; }


			}

			//cout << "Save NTW Dialog Triggered!" << endl;


			mqSaveNTWDialog SaveNTW_dialog(mqCoreUtilities::mainWidget());
			SaveNTW_dialog.exec();

		}

	}
	if (key.compare("z") == 0)
	{
		if (this->Ctrl == CTRL_PRESSED)
		{
			//cout << "CTRL-Z detected!" << endl;
			mqMorphoDigCore::instance()->Undo();
			cout << "Render from onKeyPress 3 z" << endl;
			rwi->Render();
		}

	}
	if (key.compare("y") == 0)
	{

		if (this->Ctrl == CTRL_PRESSED)
		{
			//cout << "CTRL-Y detected!" << endl;
			mqMorphoDigCore::instance()->Redo();
			cout << "Render from onKeyPress 4 y" << endl;
			rwi->Render();
		}

	}

	// Forward events
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}
  void vtkMDInteractorStyle::OnKeyRelease()
  {
	  // Get the keypress
	  vtkRenderWindowInteractor *rwi = this->Interactor;
	  std::string key = rwi->GetKeySym();

	  // Output the key that was pressed
	  //std::cout << "Pressed " << key << std::endl;
	  if (key.compare("Control_L") == 0)
	  {
		  this->Ctrl = CTRL_RELEASED;
		  this->L = L_RELEASED;
	  }

	  if (key.compare("Shift") == 0)
	  {
		  this->Shift = SHIFT_RELEASED;
		  this->L = L_RELEASED;
		  // std::cout << key << "Released" << '\n';
	  }

	  if (key.compare("Alt_L") == 0)
	  {
		  this->Alt = ALT_RELEASED;
	  }

	  if (key.compare("l") == 0 || key.compare("L") == 0)
	  {
		  this->L = L_RELEASED;
		  // std::cout << key << "Released" << '\n';
	  }
	  if (key.compare("j") == 0 || key.compare("J") == 0)
	  {
		  this->L = L_RELEASED;
		  this->J = J_RELEASED;
		  // std::cout << key << "Released" << '\n';
	  }
	  if (key.compare("k") == 0 || key.compare("K") == 0)
	  {
		  this->L = L_RELEASED;
		  this->K = K_RELEASED;
		  // std::cout << key << "Released" << '\n';
	  }
	  if (key.compare("t") == 0 || key.compare("T") == 0)
	  {
		  this->T = T_RELEASED;
		  this->TagMaskPencilStop();
		  cout << "Reset mode to 0 onKeyRelease" << endl;
		  this->CurrentMode = VTKISMT_ORIENT;
		  //mqMorphoDigCore::instance()->resetCursor();
		  cout << "Render from onKeyRelease" << endl;
		  this->Interactor->Render();
		  // std::cout << key << "Released" << '\n';
	  }
	  if (key.compare("m") == 0 || key.compare("M") == 0)
	  {
		  this->M = M_RELEASED;
		  this->TagMaskPencilStop();
		  cout << "Reset mode to 0 onKeyRelease" << endl;

		  this->CurrentMode = VTKISMT_ORIENT;
		  cout << "Render from onKeyRelease 2" << endl;
		  this->Interactor->Render();
		  
	  }
	  // Forward events
	  vtkInteractorStyleTrackballCamera::OnKeyRelease();
  }
//--------------------------------------------------------------------------
void vtkMDInteractorStyle::OnChar()
{
  switch (this->Interactor->GetKeyCode())
  {
	
    case 'r':
    case 'R':
	{
		//r toggles the rubber band selection mode for mouse button 1
		if (this->CurrentMode == VTKISMT_ORIENT)
		{
			this->CurrentMode = VTKISMD_SELECT;
		}
		else
		{
			cout << "Reset mode to 0 onChar" << endl;

			this->CurrentMode = VTKISMT_ORIENT;
		}
		break;
	}
    case 'p' :
    case 'P' :
    {
      vtkRenderWindowInteractor *rwi = this->Interactor;
      int *eventPos = rwi->GetEventPosition();
      this->FindPokedRenderer(eventPos[0], eventPos[1]);
      this->StartPosition[0] = eventPos[0];
      this->StartPosition[1] = eventPos[1];
      this->EndPosition[0] = eventPos[0];
      this->EndPosition[1] = eventPos[1];
      this->Pick();
      break;
    }
    default:
      this->Superclass::OnChar();
  }
}

void vtkMDInteractorStyle::Dolly(double factor)
{
	if (this->CurrentRenderer == NULL)
	{
		return;
	}
	if (this->T == T_PRESSED || this->M == M_PRESSED)
	{
		//cout << "T Dolly" << factor << endl;
		int tool = mqMorphoDigCore::instance()->Getmui_TagTool();
		if (mqMorphoDigCore::instance()->Getmui_TagModeActivated()&&tool == 0)// pencil
		{
			int pencil_size = mqMorphoDigCore::instance()->Getmui_PencilSize();
			if (factor < 1)
			{
				if (pencil_size > 1)
				{
					pencil_size-=5;
					if (pencil_size <= 0) { pencil_size = 1; }
				}
			}
			else
			{
				pencil_size+=5;
			}
				mqMorphoDigCore::instance()->Setmui_PencilSize(pencil_size,1);
				this->RedrawTagMaskPencilCircle();					
			
		}
	}
	else
	{
		vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
		//if (camera->GetParallelProjection())
		//{
		camera->SetParallelScale(camera->GetParallelScale() / factor);
		//important because this is used sometimes in perspective mode to check the distance between camera and grid center.
		//cout << "Dolly camera parallell" << endl;
		camera->Dolly(factor);
		//}
		//else
		//{
		//	camera->Dolly(factor);

		//}
		if (this->AutoAdjustCameraClippingRange)
		{
			this->CurrentRenderer->ResetCameraClippingRange();
			mqMorphoDigCore::instance()->ActivateClippingPlane();
		}

		if (this->Interactor->GetLightFollowCamera())
		{
			this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
		}

		mqMorphoDigCore::instance()->signal_zoomChanged();
	}
	cout << "Render from dolly" << endl;
	this->Interactor->Render();


}
void vtkMDInteractorStyle::RubberStart()
{
	//cout << "RubberStart()" << endl;
	mqMorphoDigCore::instance()->setCurrentCursor(4);
	if (!this->Interactor)
	{
		return;
	}

	//otherwise record the rubber band starting coordinate

	this->Moving = 1;

	vtkRenderWindow *renWin = this->Interactor->GetRenderWindow();

	this->StartPosition[0] = this->Interactor->GetEventPosition()[0];
	this->StartPosition[1] = this->Interactor->GetEventPosition()[1];
	this->EndPosition[0] = this->StartPosition[0];
	this->EndPosition[1] = this->StartPosition[1];

	this->PixelArray->Initialize();
	this->PixelArray->SetNumberOfComponents(4);
	int *size = renWin->GetSize();
	this->PixelArray->SetNumberOfTuples(size[0] * size[1]);

	renWin->GetRGBACharPixelData(0, 0, size[0] - 1, size[1] - 1, 1, this->PixelArray);

	this->FindPokedRenderer(this->StartPosition[0], this->StartPosition[1]);
}
void vtkMDInteractorStyle::TagMaskPencilStart()
{
	if (this->TagMaskPencilStarted == 0)
	{
		//cout << "PencilStart()" << endl;
		//mqMorphoDigCore::instance()->setCurrentCursor(4);
		//here we save 
		if (!this->Interactor)
		{
			return;
		}

		//otherwise record the rubber band starting coordinate

		this->Moving = 1;

		vtkRenderWindow *renWin = this->Interactor->GetRenderWindow();

		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];
		TagMaskPencilCenterPosition[0] = x;
		TagMaskPencilCenterPosition[1] = y;


		this->PixelArray2->Initialize();
		this->PixelArray2->SetNumberOfComponents(4);
		int *size = renWin->GetSize();
		this->PixelArray2->SetNumberOfTuples(size[0] * size[1]);

		renWin->GetRGBACharPixelData(0, 0, size[0] - 1, size[1] - 1, 1, this->PixelArray2);

		this->FindPokedRenderer(this->TagMaskPencilCenterPosition[0], this->TagMaskPencilCenterPosition[1]);
		this->TagMaskPencilStarted = 1;
	}
	

}
void vtkMDInteractorStyle::Tag(int mode)
{
	cout << "Tag" << mode << endl;
	//int* clickPos = this->GetInteractor()->GetEventPosition();
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	TagMaskPencilCenterPosition[0] = x;
	TagMaskPencilCenterPosition[1] = y;
	//std::cout << "Clicked at "
	//	<< x << " " << y << std::endl;
	if (this->CurrentRenderer == NULL) { cout << "Current renderer null" << endl; }
	if (this->CurrentRenderer != NULL)
	{
		//std::cout << "Current renderer:" << this->CurrentRenderer << endl;
		// Pick from this location.
		/* vtkSmartPointer<vtkPropPicker>  picker =
		vtkSmartPointer<vtkPropPicker>::New();*/

		vtkSmartPointer<vtkCellPicker> picker =
			vtkSmartPointer<vtkCellPicker>::New();

		picker->Pick(x, y, 0, this->CurrentRenderer);



		vtkIdType pickid = picker->GetPointId();


		if (picker->GetActor() == NULL) {
			cout << "Picked Null actor" << endl;
		}
		else
		{
			vtkMDActor * myActor = vtkMDActor::SafeDownCast(picker->GetActor());
			if (myActor != NULL)
			{
				//right button down : no override
				mqMorphoDigCore::instance()->TagAt(pickid, myActor, mode);
			}
			cout << "Render from Tag" << endl;
			mqMorphoDigCore::instance()->Render();
		}

	}
}
void vtkMDInteractorStyle::Mask()
{
	//cout << "Mask" << mode << endl;
	//int* clickPos = this->GetInteractor()->GetEventPosition();
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	TagMaskPencilCenterPosition[0] = x;
	TagMaskPencilCenterPosition[1] = y;
	//std::cout << "Clicked at "
	//	<< x << " " << y << std::endl;
	if (this->CurrentRenderer == NULL) { cout << "Current renderer null" << endl; }
	if (this->CurrentRenderer != NULL)
	{
		//std::cout << "Current renderer:" << this->CurrentRenderer << endl;
		// Pick from this location.
		/* vtkSmartPointer<vtkPropPicker>  picker =
		vtkSmartPointer<vtkPropPicker>::New();*/

		vtkSmartPointer<vtkVolumePicker> picker =
			vtkSmartPointer<vtkVolumePicker>::New();
		//picker->mask
		picker->Pick(x, y, 0, this->CurrentRenderer);



		vtkIdType pickid = picker->GetPointId();


		if (picker->GetVolume() == NULL ) {
			cout << "Picked Null volume" << endl;
		}
		else
		{
			vtkMDVolume * myVolume = vtkMDVolume::SafeDownCast(picker->GetVolume());
			if (myVolume != NULL)
			{
				//right button down : no override
				mqMorphoDigCore::instance()->MaskAt(pickid, x, y,myVolume);
				cout << "Render From Mask" << endl;
				mqMorphoDigCore::instance()->Render();
			}
			
		}

	}
}
//--------------------------------------------------------------------------
void vtkMDInteractorStyle::OnRightButtonDown()
{
	cout << "Right button down" << endl;
	this->RM_Button = RBUTTON_DOWN;
	if (this->T == T_PRESSED)
	{
		this->Tag(0);
		this->Mask();
		//this->GetInthis->Tag(0);teractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);
		//this->T = T_RELEASED;

	}
	if (this->M == M_PRESSED)
	{
		this->Mask();

		//this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);
		//this->T = T_RELEASED;

	}
	//Special case: move landmark
	if (this->L == L_PRESSED)
	{
		//int* clickPos = this->GetInteractor()->GetEventPosition();
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];
		//std::cout << "Clicked at "
		//	<< x << " " << y << std::endl;
		if (this->CurrentRenderer == NULL) { cout << "Current renderer null" << endl; }
		if (this->CurrentRenderer != NULL)
		{
			//std::cout << "Current renderer:" << this->CurrentRenderer << endl;
			// Pick from this location.
			/* vtkSmartPointer<vtkPropPicker>  picker =
			vtkSmartPointer<vtkPropPicker>::New();*/

			vtkSmartPointer<vtkVolumePicker> picker =
				vtkSmartPointer<vtkVolumePicker>::New();

			picker->Pick(x, y, 0, this->CurrentRenderer);



			double* pos = picker->GetPickPosition();
			//std::cout << "Pick position (world coordinates) is: "
			//	<< pos[0] << " " << pos[1]
			//	<< " " << pos[2] << std::endl;
			double* norm = picker->GetPickNormal();
			//std::cout << "Pick normal : "
			//	<< norm[0] << " " << norm[1]
			//	<< " " << norm[2] << std::endl;

			//std::cout << "Picked actor: " << picker->GetActor() << std::endl;
			if (picker->GetActor() == NULL && picker->GetVolume() == NULL)
			{
				cout << "No actor nor volume was picked" << endl;

			}			
			else
			{
				mqMorphoDigCore::instance()->UpdateFirstSelectedLandmark(pos, norm);
				cout << "Render From OnRightButtonDown" << endl;
				mqMorphoDigCore::instance()->Render();
			}

		}
		//this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);
		this->L = L_RELEASED;

	}
	else
	{
		if (this->T == T_RELEASED || this->M == M_RELEASED)
		{
			this->ResetMoveWhat();
		}
		//Right drag:
		// CAM mode: draw rubber band
		// OBJ mode: draw rubber band if alt is not pressed
		// OBJ mode : if alt is pressed, spin rotate actors 
		if (this->MoveWhat == CAM || this->Alt != ALT_PRESSED)
		
		{
			
			if (this->Alt == ALT_PRESSED)
			{
				this->StartSpin();
				this->Superclass::OnRightButtonDown();
			}
			else
			{
				this->CurrentMode = VTKISMD_SELECT;

				this->RubberStart();
			}
		}
		else // spin actors
		{
			this->ActorsPositionsSaved = 0;//allow to save position
			int x = this->Interactor->GetEventPosition()[0];
			int y = this->Interactor->GetEventPosition()[1];

			this->FindPokedRenderer(x, y);
			if (this->CurrentRenderer == NULL)
			{
				return;
			}

			this->GrabFocus(this->EventCallbackCommand);
			this->NumberOfSelectedActors = this->getNumberOfSelectedActors();
			this->StartSpin();
		}

	}
}
void vtkMDInteractorStyle::ResetMoveWhat()
{
	int movemode = mqMorphoDigCore::instance()->Getmui_MoveMode();
	if (this->Ctrl != CTRL_PRESSED)
	{
		if (movemode != MOVEOBJ_MODE)
		{
			this->MoveWhat = CAM;
		}
		else
		{
			this->MoveWhat = OBJ;
		}
	}
	else
	{
		if (movemode != MOVEOBJ_MODE)
		{
			this->MoveWhat = OBJ;
		}
		else
		{
			this->MoveWhat = CAM;
		}

	}
	if (this->MoveWhat == CAM)
	{
		mqMorphoDigCore::instance()->setCurrentCursor(CAM);
	}
	else
	{
		mqMorphoDigCore::instance()->setCurrentCursor(OBJ);
	}
	
}
void vtkMDInteractorStyle::OnLeftButtonDown()
{
	//cout << "Left button down!" << endl;
	this->LM_Button = LBUTTON_DOWN;
	
  if (this->CurrentMode != VTKISMD_SELECT)
  {
    //if not in rubber band mode, let the parent class handle it
	  //this->Interactor->GetControlKey()

	  //special case : landmark setting!
	  if (this->L == L_PRESSED && this->Ctrl != CTRL_PRESSED)
	  //if (this->L == L_PRESSED )
	  {


		  //int* clickPos = this->GetInteractor()->GetEventPosition();
		  int x = this->Interactor->GetEventPosition()[0];
		  int y = this->Interactor->GetEventPosition()[1];
		  //std::cout << "Clicked at "
		  //	  << x << " " << y   << std::endl;
		  if (this->CurrentRenderer == NULL) { cout << "Current renderer null" << endl; }
		  if (this->CurrentRenderer != NULL)
		  {
			  //	  std::cout << "Current renderer:" << this->CurrentRenderer << endl;
			  // Pick from this location.
			  /* vtkSmartPointer<vtkPropPicker>  picker =
			  vtkSmartPointer<vtkPropPicker>::New();*/

			  vtkSmartPointer<vtkVolumePicker> picker =
				  vtkSmartPointer<vtkVolumePicker>::New();

			  picker->Pick(x, y, 0, this->CurrentRenderer);



			  double* pos = picker->GetPickPosition();
			  
			    std::cout << "Pick position (world coordinates) is: "
			  	  << pos[0] << " " << pos[1]
			  	  << " " << pos[2] << std::endl;
			  double* norm = picker->GetPickNormal();
			   std::cout << "Pick normal : "
			  	  << norm[0] << " " << norm[1]
			  	  << " " << norm[2] << std::endl;

			  //  std::cout << "Picked actor: " << picker->GetActor() << std::endl;
			   
			   if (picker->GetActor() != NULL || picker->GetVolume() != NULL || picker->GetViewProp() !=NULL)
			   {
				   cout << "An actor, a ViewProp or a volume was picked" << endl;
			   
				   // else
				  //  {
				   //if curve
				   int node_type = 0;
				   if (mqMorphoDigCore::instance()->Getmui_LandmarkMode()==2)
				   { 
					int num_nodes= mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
					if (num_nodes == 0)
					{
						node_type = 1; // first node is mandatory a starting point
					}

					if (this->Shift==SHIFT_PRESSED)
					{
						node_type = 1; // starting point
					}
					else if (this->J == J_PRESSED)
					{ 
						node_type = 2; //curve milestone
					
					}
					else if (this->K == K_PRESSED)
					{
						node_type = 3;// connect to preceding starting point
					}
				   }
					   /*//node_type: only used if mode ==2, curve node
	//node_type: 1 curve starting point 
	//node_type: 0 normal node
	//node_type: 2 curve milestone
	//node_type: 3 connect to preceding starting point*/
				   mqMorphoDigCore::instance()->CreateLandmark(pos, norm, mqMorphoDigCore::instance()->Getmui_LandmarkMode(), node_type);
				   mqMorphoDigCore::instance()->CreateLandmarkUndoSet(mqMorphoDigCore::instance()->Getmui_LandmarkMode());		
				   cout << "Render From onLeftButtonDown" << endl;
				   
				   mqMorphoDigCore::instance()->Render();
				   //  }
			   }

		  }
		  //this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);

		  this->L = L_RELEASED;
		  this->J = J_RELEASED;
		  this->K = K_RELEASED;
		  this->Shift = SHIFT_RELEASED;
		  this->Ctrl = CTRL_RELEASED;
	  }//left button down, no landmark 
	  else  if (this->T == T_PRESSED 
		  && this->Ctrl != CTRL_PRESSED
		  )
	  {
		  this->Tag(1);
		  this->Mask();
		  //this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetDefaultRenderer()->AddActor(actor);

		 // this->T = T_RELEASED;
	  }//left button down, no landmark 
	  else  if (this->M == M_PRESSED
		  && this->Ctrl != CTRL_PRESSED
		  )
	  {
		  this->Mask();
		 
	  }//left button down, no landmark 
	  else
	  { // left mouse pressed, no 
		  if (this->T == T_RELEASED || this->M == M_RELEASED)
		  {
			  this->ResetMoveWhat();
		  }
		  

		  if (this->MoveWhat==CAM)
		  {
			  if (this->Alt == ALT_PRESSED)
			  {
				  this->StartSpin();
			  }
			  this->Superclass::OnLeftButtonDown();


		  }
		  else
		  {
			  cout << "here" << endl;
			  this->ActorsPositionsSaved = 0;//allow to save position
			  int x = this->Interactor->GetEventPosition()[0];
			  int y = this->Interactor->GetEventPosition()[1];

			  this->FindPokedRenderer(x, y);
			  //this->FindPickedActor(x, y);
			  if (this->CurrentRenderer == NULL)
			  {
				  return;
			  }

			  this->GrabFocus(this->EventCallbackCommand);

			  cout << "Start Rotate or Spin" << endl;
			  this->NumberOfSelectedActors = this->getNumberOfSelectedActors();
			  if (this->Alt == ALT_PRESSED)
			  {
				  this->StartSpin();
			  }
			  else
			  {
				  cout << "?" << endl;
				  this->StartRotate();
				  cout << "??" << endl;
					  
			  }
			  


		  }
		  
	  }
	  return;
  }
  //REALLY??? 
  this->RubberStart();

  
}

void vtkMDInteractorStyle::DeleteSelectedActors()
{
	this->ActorCollection->DeleteSelectedActors();
	this->VolumeCollection->DeleteSelectedVolumes();
	this->NormalLandmarkCollection->DeleteSelectedActors();
	this->TargetLandmarkCollection->DeleteSelectedActors();
	this->NodeLandmarkCollection->DeleteSelectedActors();
	this->HandleLandmarkCollection->DeleteSelectedActors();
	this->FlagLandmarkCollection->DeleteSelectedActors();
	this->Interactor->Render();
}
int vtkMDInteractorStyle::getNumberOfSelectedActors()
{
	this->ActorCollection->InitTraversal();
	int cpt = 0;
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());

		if (myActor->GetSelected() == 1)
		{
			cpt++;
		}
		
	}
	this->VolumeCollection->InitTraversal();
	
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());

		if (myVolume->GetSelected() == 1)
		{
			cout << "Interactor style, found 1 selected volume" << endl;
			cpt++;
		}

	}
	this->NormalLandmarkCollection->InitTraversal();
	
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());

		if (myActor->GetSelected() == 1)
		{
			cpt++;
		}

	}

	this->TargetLandmarkCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());

		if (myActor->GetSelected() == 1)
		{
			cpt++;
		}

	}
	this->NodeLandmarkCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());

		if (myActor->GetSelected() == 1)
		{
			cpt++;
		}

	}
	this->HandleLandmarkCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());

		if (myActor->GetSelected() == 1)
		{
			cpt++;
		}

	}
	this->FlagLandmarkCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());

		if (myActor->GetSelected() == 1)
		{
			cpt++;
		}

	}
	return cpt;
}

void vtkMDInteractorStyle::SaveSelectedActorsPositions()
{
	if (this->ActorsPositionsSaved == 0)
	{
		cout << "SavePositions" << endl;

		std::string action = "Action!";
		//cout << "State = " << this->State << endl;
		switch (this->State)
		{
		case VTKIS_ROTATE:
			action = "Rotate selected actors";
			//cout << "VTK_IS_ROTATE: action=" << action.c_str() << endl;
			break;
		case VTKIS_PAN:
			action = "Translate selected actors";
			//cout << "VTK_IS_PAN: action=" << action.c_str() << endl;
			break;
		case VTKIS_SPIN:
			action = "Spin selected actors";
			//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
			break;
		}
		cout << "Begin undo set" << action << endl;
		int Count = BEGIN_UNDO_SET(action);

		//cout << action.c_str() << endl;
		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				//cout << "Call myActor Save Position with count"<<Count << endl;
				myActor->SaveState(Count);
			}
		}
		this->VolumeCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
			if (myVolume->GetSelected() == 1)
			{
				cout << "Call myVolume Save Position with count"<<Count << endl;
				myVolume->SaveState(Count);
			}
		}
		this->NormalLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				//cout << "Call myLMActor Save Position with count" << Count << endl;
				myActor->SaveState(Count);
			}
		}
		this->TargetLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				//cout << "Call myLMActor Save Position with count" << Count << endl;
				myActor->SaveState(Count);
			}
		}
		this->NodeLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
			//	cout << "Call myLMActor Save Position with count" << Count << endl;
				myActor->SaveState(Count);
			}
		}
		this->HandleLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				cout << "Call myLMActor Save Position with count" << Count << endl;
				myActor->SaveState(Count);
			}
		}

		this->FlagLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				//cout << "Call myLMActor Save Position with count" << Count << endl;
				myActor->SaveState(Count);
			}
		}
		//vtkMorphoDigCore::instance()->getUndoStack()->
		
		END_UNDO_SET();
		this->ActorsPositionsSaved = 1;
		cout << "End undo set" << action << endl;
	}

}
//--------------------------------------------------------------------------
void vtkMDInteractorStyle::OnMouseMove()
{
	
  if (this->CurrentMode != VTKISMD_SELECT &&  this->CurrentMode != VTKISMD_TAGMASKPENCIL)
  {
	 // this->ResetMoveWhat();
	  //if (this->Ctrl != CTRL_PRESSED)
	  if (this->MoveWhat == CAM)
	  {
		  //let the parent class handle it
		  this->Superclass::OnMouseMove();
		  if (this->LM_Button == LBUTTON_DOWN)
		  {
			  //cout << "Activage clipping plane!!" << endl;
			  mqMorphoDigCore::instance()->ActivateClippingPlane();
		  }
	  }
	  else // MoveWhat = objects
	  {
		 
		  //copied from Trackball Actor
		  int x = this->Interactor->GetEventPosition()[0];
		  int y = this->Interactor->GetEventPosition()[1];
		  //should be only once!
		  if (this->NumberOfSelectedActors>0 )
		  {
			  
			  this->SaveSelectedActorsPositions();
			  // cout << "We should increment the GLOBAL undo stack number now!";
		  }
		  switch (this->State)
		  {
		  case VTKIS_ROTATE:
			  this->FindPokedRenderer(x, y);
			  //cout << "ROTATE" << endl;
			  this->RotateActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;

		  case VTKIS_PAN:
			  this->FindPokedRenderer(x, y);
			  //cout << "PAN" << endl;
			  this->PanActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;		  

		  case VTKIS_SPIN:
			  this->FindPokedRenderer(x, y);
			  //cout << "SPIN" << endl;
			  this->SpinActors();
			  this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			  break;

		
		  }
	  }
    return;
  }

  if (!this->Interactor || !this->Moving)
  {
    return;
  }

  if (this->CurrentMode == VTKISMD_SELECT)
  {

	  this->EndPosition[0] = this->Interactor->GetEventPosition()[0];
	  this->EndPosition[1] = this->Interactor->GetEventPosition()[1];
	  int *size = this->Interactor->GetRenderWindow()->GetSize();
	  if (this->EndPosition[0] > (size[0] - 1))
	  {
		  this->EndPosition[0] = size[0] - 1;
	  }
	  if (this->EndPosition[0] < 0)
	  {
		  this->EndPosition[0] = 0;
	  }
	  if (this->EndPosition[1] > (size[1] - 1))
	  {
		  this->EndPosition[1] = size[1] - 1;
	  }
	  if (this->EndPosition[1] < 0)
	  {
		  this->EndPosition[1] = 0;
	  }
	  this->RedrawRubberBand();
  }
  
  if (this->CurrentMode == VTKISMD_TAGMASKPENCIL)
  {
	  cout << "Mouse move tag mask pencil" << endl;
	  this->TagMaskPencilCenterPosition[0] = this->Interactor->GetEventPosition()[0];
	  this->TagMaskPencilCenterPosition[1] = this->Interactor->GetEventPosition()[1];
	  int *size = this->Interactor->GetRenderWindow()->GetSize();
	  if (this->TagMaskPencilCenterPosition[0] > (size[0] - 1))
	  {
		  this->TagMaskPencilCenterPosition[0] = size[0] - 1;
	  }
	  if (this->TagMaskPencilCenterPosition[0] < 0)
	  {
		  this->TagMaskPencilCenterPosition[0] = 0;
	  }
	  if (this->TagMaskPencilCenterPosition[1] > (size[1] - 1))
	  {
		  this->TagMaskPencilCenterPosition[1] = size[1] - 1;
	  }
	  if (this->TagMaskPencilCenterPosition[1] < 0)
	  {
		  this->TagMaskPencilCenterPosition[1] = 0;
	  }
	 // cout << "this->LM_Button=" << this->LM_Button << endl;
	  if (this->LM_Button == LBUTTON_DOWN && this->T ==T_PRESSED)
	  {
		  cout << "Tag inside mousmove" << endl;
		  this->Tag(0);
		  this->Mask();
	  }
	  if (this->RM_Button == RBUTTON_DOWN && this->T == T_PRESSED)
	  {
		  this->Mask();
		  this->Tag(1);
	  }
	  if(this->LM_Button == LBUTTON_DOWN && this->M == M_PRESSED)
	  {
		  cout << "Mask inside mousmove" << endl;
		  this->Mask();
	  }
	  if (this->RM_Button == RBUTTON_DOWN && this->M == M_PRESSED)
	  {
		  this->Mask();
	  }
	  this->RedrawTagMaskPencilCircle();
  }
}

void vtkMDInteractorStyle::RubberStop()
{
	mqMorphoDigCore::instance()->setCurrentCursor(this->MoveWhat);
	if (!this->Interactor || !this->Moving)
	{
		return;
	}

//otherwise record the rubber band end coordinate and then fire off a pick
if ((this->StartPosition[0] != this->EndPosition[0])
	|| (this->StartPosition[1] != this->EndPosition[1]))
{
	this->Pick();
}
this->Moving = 0;
}
void vtkMDInteractorStyle::TagMaskPencilStop()
{
	this->TagMaskPencilStarted = 0;
	
	//mqMorphoDigCore::instance()->setCurrentCursor(this->MoveWhat);
	if (!this->Interactor || !this->Moving)
	{
		return;
	}

	this->Moving = 0;
}
void vtkMDInteractorStyle::OnRightButtonUp()
{
	this->RM_Button = RBUTTON_UP;
	this->Alt = ALT_RELEASED;
	if (this->CurrentMode != VTKISMD_SELECT)
	{
		
		//this->ResetMoveWhat();

		//if (this->Ctrl != CTRL_PRESSED)
		if (this->MoveWhat == CAM)
		{
			//if not in rubber band mode,  let the parent class handle it
			
			
			this->Superclass::OnRightButtonUp();
		}
		else
		{
			 // we allow to resave the positions later!
			switch (this->State)
			{
			case VTKIS_PAN:
				this->EndPan();
				break;

			case VTKIS_SPIN:
				this->EndSpin();
				break;

			case VTKIS_ROTATE:
				//cout << "End rotate" << endl;
				this->EndRotate();
				break;
			}

			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
		}
		//if not in rubber band mode,  let the parent class handle it
		
		return;
	}
	this->RubberStop();	
	cout << "Reset mode to 0 onRightButtonUp" << endl;

	this->CurrentMode = VTKISMT_ORIENT;
}
//--------------------------------------------------------------------------
void vtkMDInteractorStyle::OnLeftButtonUp()
{
	this->LM_Button = LBUTTON_UP;
	this->Alt = ALT_RELEASED;
  if (this->CurrentMode != VTKISMD_SELECT)
  {
	 // this->ResetMoveWhat();

	  //if (this->Ctrl != CTRL_PRESSED)
	  if (this->MoveWhat == CAM)
	  {	  
	  
		  //if not in rubber band mode,  let the parent class handle it
		  this->Superclass::OnLeftButtonUp();
	  }
	else
	{
	  switch (this->State)
	  {
	  case VTKIS_PAN:
		  this->EndPan();
		  break;

	  case VTKIS_SPIN:
		  this->EndSpin();
		  break;

	  case VTKIS_ROTATE:
		  //cout << "End rotate" << endl;
		  this->EndRotate();
		  break;
	  }

	  if (this->Interactor)
	  {
		  this->ReleaseFocus();
	  }
  }
    return;
  }
  this->RubberStop();
  

}

void vtkMDInteractorStyle::OnMiddleButtonDown()
{
	this->ResetMoveWhat();

	//if (this->Ctrl != CTRL_PRESSED)
	if (this->MoveWhat == CAM)
	{
		
		//if not in rubber band mode,  let the parent class handle it
		this->Superclass::OnMiddleButtonDown();
	}
	else
	{
		
		this->ActorsPositionsSaved = 0;//allow to save position
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];

		this->FindPokedRenderer(x, y);
		//this->FindPickedActor(x, y);
		if (this->CurrentRenderer == NULL 
			|| this->ActorCollection == NULL
			)
		{
			return;
		}

		this->GrabFocus(this->EventCallbackCommand);
		this->NumberOfSelectedActors = this->getNumberOfSelectedActors();
		this->StartPan();
		
	}
}

//----------------------------------------------------------------------------
void vtkMDInteractorStyle::OnMiddleButtonUp()
{
	if (this->CurrentMode != VTKISMD_SELECT)
	{
		//this->ResetMoveWhat();

		//if (this->Ctrl != CTRL_PRESSED)
		if (this->MoveWhat == CAM)
		{

			//if not in rubber band mode,  let the parent class handle it
			this->Superclass::OnMiddleButtonUp();
		}
		else
		{
			switch (this->State)
			{
			case VTKIS_PAN:
				this->EndPan();
				break;

			case VTKIS_SPIN:
				this->EndSpin();
				break;

			case VTKIS_ROTATE:
				//cout << "End rotate" << endl;
				this->EndRotate();
				break;
			}

			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
		}
		//if not in rubber band mode,  let the parent class handle it

		return;
	}

		
}

//--------------------------------------------------------------------------
void vtkMDInteractorStyle::RedrawRubberBand()
{
  //update the rubber band on the screen
	
  int *size = this->Interactor->GetRenderWindow()->GetSize();

  vtkUnsignedCharArray *tmpPixelArray = vtkUnsignedCharArray::New();
  tmpPixelArray->DeepCopy(this->PixelArray);
  unsigned char *pixels = tmpPixelArray->GetPointer(0);

  int min[2], max[2];

  min[0] = this->StartPosition[0] <= this->EndPosition[0] ?
    this->StartPosition[0] : this->EndPosition[0];
  if (min[0] < 0) { min[0] = 0; }
  if (min[0] >= size[0]) { min[0] = size[0] - 1; }

  min[1] = this->StartPosition[1] <= this->EndPosition[1] ?
    this->StartPosition[1] : this->EndPosition[1];
  if (min[1] < 0) { min[1] = 0; }
  if (min[1] >= size[1]) { min[1] = size[1] - 1; }

  max[0] = this->EndPosition[0] > this->StartPosition[0] ?
    this->EndPosition[0] : this->StartPosition[0];
  if (max[0] < 0) { max[0] = 0; }
  if (max[0] >= size[0]) { max[0] = size[0] - 1; }

  max[1] = this->EndPosition[1] > this->StartPosition[1] ?
    this->EndPosition[1] : this->StartPosition[1];
  if (max[1] < 0) { max[1] = 0; }
  if (max[1] >= size[1]) { max[1] = size[1] - 1; }

  int i;
  for (i = min[0]; i <= max[0]; i++)
  {
	  // i = x1  [x1, y2]
	  //     4(y1*taillex)+x1)
	  //  4(y2*taillex)+x1)
	  // i =x2    [x2, y2
	  //      4(y2*taillex)+x2)

    pixels[4*(min[1]*size[0]+i)] = 255 ^ pixels[4*(min[1]*size[0]+i)];
    pixels[4*(min[1]*size[0]+i)+1] = 255 ^ pixels[4*(min[1]*size[0]+i)+1];
    pixels[4*(min[1]*size[0]+i)+2] = 0 ^ pixels[4*(min[1]*size[0]+i)+2];
    pixels[4*(max[1]*size[0]+i)] = 255 ^ pixels[4*(max[1]*size[0]+i)];
    pixels[4*(max[1]*size[0]+i)+1] = 255 ^ pixels[4*(max[1]*size[0]+i)+1];
    pixels[4*(max[1]*size[0]+i)+2] = 0^ pixels[4*(max[1]*size[0]+i)+2];
  }
  for (i = min[1]+1; i < max[1]; i++)
  {
	  // i = y1+1 
	  //     4(y1*taillex)+x1)
	  // i =x2    [x2, y2
	  //      4(y2*taillex)+x2)
    pixels[4*(i*size[0]+min[0])] = 255 ^ pixels[4*(i*size[0]+min[0])];
    pixels[4*(i*size[0]+min[0])+1] = 255 ^ pixels[4*(i*size[0]+min[0])+1];
    pixels[4*(i*size[0]+min[0])+2] = 0 ^ pixels[4*(i*size[0]+min[0])+2];
    pixels[4*(i*size[0]+max[0])] = 255 ^ pixels[4*(i*size[0]+max[0])];
    pixels[4*(i*size[0]+max[0])+1] = 255 ^ pixels[4*(i*size[0]+max[0])+1];
    pixels[4*(i*size[0]+max[0])+2] =0 ^ pixels[4*(i*size[0]+max[0])+2];
  }

  this->Interactor->GetRenderWindow()->SetRGBACharPixelData(0, 0, size[0]-1, size[1]-1, pixels, 0);
  this->Interactor->GetRenderWindow()->Frame();

  tmpPixelArray->Delete();
}
void vtkMDInteractorStyle::RedrawTagMaskPencilCircle()
{
	// assume "R" is 
	//update the circle drawn on the screen
	int *size = this->Interactor->GetRenderWindow()->GetSize();

	vtkUnsignedCharArray *tmpPixelArray = vtkUnsignedCharArray::New();
	tmpPixelArray->DeepCopy(this->PixelArray2);
	unsigned char *pixels = tmpPixelArray->GetPointer(0);

	// adaptation of the midpoint circle algorithm to know which pixels form the outline of a circle of center x0,Y0 and radius "radius"
	int radius = (int) mqMorphoDigCore::instance()->Getmui_PencilSize();
	if (radius < 10) { radius = 10; }
	int x0 = this->TagMaskPencilCenterPosition[0];
	int y0 = this->TagMaskPencilCenterPosition[1];
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);
	while (x >= y)
	{
		if (((x0 + x)<=(size[0]-1)) && ((y0 + y)<=(size[1] - 1))) {
			int cx = x0 + x;
			int cy = y0 + y;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4*(size[0]*cy + cx) ];
			pixels[4 * (size[0]*cy + cx) +1] = 255 ^ pixels[4*(size[0]*cy + cx) +1];
			pixels[4 * (size[0]*cy + cx) +2] = 0 ^ pixels[4*(size[0]*cy + cx) +2];
			
		}
		if ((((x0 + y)<=(size[0] - 1)) && ((y0 + x)<=(size[1] - 1)))) {
			int cx = x0 + y;
			int cy = y0 + x;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4 * (size[0]*cy + cx)];
			pixels[4 * (size[0]*cy + cx) + 1] = 255 ^ pixels[4 * (size[0]*cy + cx) + 1];
			pixels[4 * (size[0]*cy + cx) + 2] = 0 ^ pixels[4 * (size[0]*cy + cx) + 2];
			//putpixel(x0 + y, y0 + x);
		}
		if (((x0 -y)>=0) && ((y0 + x)<=(size[1] - 1))) {
			int cx = x0 - y;
			int cy = y0 + x;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4 * (size[0]*cy + cx)];
			pixels[4 * (size[0]*cy + cx) + 1] = 255 ^ pixels[4 * (size[0]*cy + cx) + 1];
			pixels[4 * (size[0]*cy + cx) + 2] = 0 ^ pixels[4 * (size[0]*cy + cx) + 2];
		}
		if (((x0 - x) >= 0) && ((y0 + y) <= (size[1] - 1))) {
			int cx = x0 - x;
			int cy = y0 + y;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4 * (size[0]*cy + cx)];
			pixels[4 * (size[0]*cy + cx) + 1] = 255 ^ pixels[4 * (size[0]*cy + cx) + 1];
			pixels[4 * (size[0]*cy + cx) + 2] = 0 ^ pixels[4 * (size[0]*cy + cx) + 2];
			//putpixel(x0 - x, y0 + y);
		}
		if (((x0 - x) >= 0) && ((y0 - y)>=0 )) {
			int cx = x0 - x;
			int cy = y0 - y;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4 * (size[0]*cy + cx)];
			pixels[4 * (size[0]*cy + cx) + 1] = 255 ^ pixels[4 * (size[0]*cy + cx) + 1];
			pixels[4 * (size[0]*cy + cx) + 2] = 0 ^ pixels[4 * (size[0]*cy + cx) + 2];
			
		}
		if (((x0 - y) >= 0) && ((y0 - x) >=0)) {
			int cx = x0 - y;
			int cy = y0 - x;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4 * (size[0]*cy + cx)];
			pixels[4 * (size[0]*cy + cx) + 1] = 255 ^ pixels[4 * (size[0]*cy + cx) + 1];
			pixels[4 * (size[0]*cy + cx) + 2] = 0 ^ pixels[4 * (size[0]*cy + cx) + 2];
		}
		if (((x0 + y) <=(size[0] - 1)) && ((y0 -x ) >=0)) {
			int cx = x0 + y;
			int cy = y0 - x;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4 * (size[0]*cy + cx)];
			pixels[4 * (size[0]*cy + cx) + 1] = 255 ^ pixels[4 * (size[0]*cy + cx) + 1];
			pixels[4 * (size[0]*cy + cx) + 2] = 0 ^ pixels[4 * (size[0]*cy + cx) + 2];
			
		}
		if (((x0 + x) <= (size[0] - 1)) && ((y0 - y) >= 0)) {
			int cx = x0 + x;
			int cy = y0 - y;
			pixels[4 * (size[0]*cy + cx)] = 255 ^ pixels[4 * (size[0]*cy + cx)];
			pixels[4 * (size[0]*cy + cx) + 1] = 255 ^ pixels[4 * (size[0]*cy + cx) + 1];
			pixels[4 * (size[0]*cy + cx) + 2] = 0 ^ pixels[4 * (size[0]*cy + cx) + 2];
			
		}

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}

	
	this->Interactor->GetRenderWindow()->SetRGBACharPixelData(0, 0, size[0] - 1, size[1] - 1, pixels, 0);
	this->Interactor->GetRenderWindow()->Frame();

	tmpPixelArray->Delete();
}
//--------------------------------------------------------------------------
void vtkMDInteractorStyle::Pick()
{
  //find rubber band lower left, upper right and center
  double rbcenter[3];
  int *size = this->Interactor->GetRenderWindow()->GetSize();
  int min[2], max[2];
  min[0] = this->StartPosition[0] <= this->EndPosition[0] ?
    this->StartPosition[0] : this->EndPosition[0];
  if (min[0] < 0) { min[0] = 0; }
  if (min[0] >= size[0]) { min[0] = size[0] - 2; }

  min[1] = this->StartPosition[1] <= this->EndPosition[1] ?
    this->StartPosition[1] : this->EndPosition[1];
  if (min[1] < 0) { min[1] = 0; }
  if (min[1] >= size[1]) { min[1] = size[1] - 2; }

  max[0] = this->EndPosition[0] > this->StartPosition[0] ?
    this->EndPosition[0] : this->StartPosition[0];
  if (max[0] < 0) { max[0] = 0; }
  if (max[0] >= size[0]) { max[0] = size[0] - 2; }

  max[1] = this->EndPosition[1] > this->StartPosition[1] ?
    this->EndPosition[1] : this->StartPosition[1];
  if (max[1] < 0) { max[1] = 0; }
  if (max[1] >= size[1]) { max[1] = size[1] - 2; }

  rbcenter[0] = (min[0] + max[0])/2.0;
  rbcenter[1] = (min[1] + max[1])/2.0;
  rbcenter[2] = 0;

  if (this->State == VTKIS_NONE)
  {
    //tell the RenderWindowInteractor's picker to make it happen
    vtkRenderWindowInteractor *rwi = this->Interactor;

    vtkAssemblyPath *path = NULL;
    rwi->StartPickCallback();
    vtkAbstractPropPicker *picker =
      vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker());
    if ( picker != NULL )
    {
      //vtkRenderedAreaPicker *areaPicker = vtkRenderedAreaPicker::SafeDownCast(picker);
		vtkAreaPicker *areaPicker = vtkAreaPicker::SafeDownCast(picker);
      if (areaPicker != NULL)
      {
		  cout << "AreaPick from interactor style" << endl;
        areaPicker->AreaPick(min[0], min[1], max[0], max[1],
                             this->CurrentRenderer);
      }
      else
      {
		  cout << "Picker pick from interactor style" << endl;
        picker->Pick(rbcenter[0], rbcenter[1],
                     0.0, this->CurrentRenderer);
      }
      path = picker->GetPath();
    }
    if ( path == NULL )
    {
      this->HighlightProp(NULL);
      this->PropPicked = 0;
    }
    else
    {
      //highlight the one prop that the picker saved in the path
      //this->HighlightProp(path->GetFirstNode()->GetViewProp());
      this->PropPicked = 1;
    }
    rwi->EndPickCallback();
  }
  cout << "Render from Pick" << endl;
  this->Interactor->Render();
}
void vtkMDInteractorStyle::GetCenterOfMassOfSelectedActors(double com[3])
{
	com[0] = 0;
	com[1] = 0;
	com[2] = 0;
	//cout << "start a com:" << endl;
	//double com[3] = { 0, 0,0 };	
	//Conception weakness : call COM before VN otherwise computation may not have been triggered!!
	


	int nv = 0;
	double *coma = this->ActorCollection->GetCenterOfMassOfSelectedActors();
	int nva = this->ActorCollection->GetGlobalSelectedVN();
	if (nva>0) {
		com[0] += coma[0] * nva ;
		com[1] += coma[1] * nva ;
		com[2] += coma[2] * nva ;
	}
	//cout << " com:" << com[0] << "," << com[1] << "," << com[2] << endl;
	//cout << "selected nva:" << nva << endl;
	//cout << "start  lm com:" << endl;
	//Conception weakness : call COM before VN otherwise computation may not have been triggered!!
	nv = nva;
	double *Normalcomlm = this->NormalLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Normalnvlm = this->NormalLandmarkCollection->GetGlobalSelectedVN();
	if (Normalnvlm > 0) {
		//cout << "nvlm>0" << endl;
		
		com[0] += Normalcomlm[0] * Normalnvlm;
		com[1] += Normalcomlm[1] * Normalnvlm;
		com[2] += Normalcomlm[2] * Normalnvlm;
	}
	nv += Normalnvlm;

	double *Targetcomlm = this->TargetLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Targetnvlm = this->TargetLandmarkCollection->GetGlobalSelectedVN();
	if (Targetnvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Targetcomlm[0] * Targetnvlm;
		com[1] += Targetcomlm[1] * Targetnvlm;
		com[2] += Targetcomlm[2] * Targetnvlm;
	}
	nv += Targetnvlm;

	double *Nodecomlm = this->NodeLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Nodenvlm = this->NodeLandmarkCollection->GetGlobalSelectedVN();
	if (Nodenvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Nodecomlm[0] * Nodenvlm;
		com[1] += Nodecomlm[1] * Nodenvlm;
		com[2] += Nodecomlm[2] * Nodenvlm;
	}
	nv += Nodenvlm;

	double *Handlecomlm = this->HandleLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Handlenvlm = this->HandleLandmarkCollection->GetGlobalSelectedVN();
	if (Handlenvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Handlecomlm[0] * Handlenvlm;
		com[1] += Handlecomlm[1] * Handlenvlm;
		com[2] += Handlecomlm[2] * Handlenvlm;
	}
	nv += Handlenvlm;

	double *Flagcomlm = this->FlagLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Flagnvlm = this->FlagLandmarkCollection->GetGlobalSelectedVN();
	if (Flagnvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Flagcomlm[0] * Flagnvlm;
		com[1] += Flagcomlm[1] * Flagnvlm;
		com[2] += Flagcomlm[2] * Flagnvlm;
	}
	nv += Flagnvlm;

	if (nv > 0) { com[0] /= nv; com[1] /= nv; com[2] /= nv;
	}
	if (this->VolumeCollection->GetNumberOfSelectedVolumes() > 0)
	{
		double *Volcom = this->VolumeCollection->GetCenterOfMassOfSelectedVolumes();
		com[0] += Volcom[0];
		com[1] += Volcom[1];
		com[2] += Volcom[2];
		if (nv > 0) {
			com[0] /= 2;
			com[1] /= 2;
			com[2] /= 2;
		}
	}
	//cout << "global selected com:" << com[0] << ","<<com[1] << "," << com[2] << endl;
	//cout << "global selected nv:" << nv << endl;
	//return com;
}

double vtkMDInteractorStyle::GetBoundingBoxLengthOfSelectedActors()
{
	double boundsa[6];
	this->ActorCollection->GetBoundingBoxSelected(boundsa);
	double boundsv[6];
	this->VolumeCollection->GetBoundingBoxSelected(boundsv);

	double Normalboundslm[6];
	this->NormalLandmarkCollection->GetBoundingBoxSelected(Normalboundslm);
	double Targetboundslm[6];
	this->TargetLandmarkCollection->GetBoundingBoxSelected(Targetboundslm);
	double Nodeboundslm[6];
	this->NodeLandmarkCollection->GetBoundingBoxSelected(Nodeboundslm);
	double Handleboundslm[6];
	this->HandleLandmarkCollection->GetBoundingBoxSelected(Handleboundslm);
	double Flagboundslm[6];
	this->FlagLandmarkCollection->GetBoundingBoxSelected(Flagboundslm);



	double largestboundsselected[6];
	largestboundsselected[0] = DBL_MAX;
	largestboundsselected[1] = -DBL_MAX;
	largestboundsselected[2] = DBL_MAX;
	largestboundsselected[3] = -DBL_MAX;
	largestboundsselected[4] = DBL_MAX;
	largestboundsselected[5] = -DBL_MAX;

	if (boundsa[0] < largestboundsselected[0]) { largestboundsselected[0] = boundsa[0]; }
	if (boundsa[1] > largestboundsselected[1]) { largestboundsselected[1] = boundsa[1]; }
	if (boundsa[2] < largestboundsselected[2]) { largestboundsselected[2] = boundsa[2]; }
	if (boundsa[3] > largestboundsselected[3]) { largestboundsselected[3] = boundsa[3]; }
	if (boundsa[4] < largestboundsselected[4]) { largestboundsselected[4] = boundsa[4]; }
	if (boundsa[5] > largestboundsselected[5]) { largestboundsselected[5] = boundsa[5]; }

	if (boundsv[0] < largestboundsselected[0]) { largestboundsselected[0] = boundsv[0]; }
	if (boundsv[1] > largestboundsselected[1]) { largestboundsselected[1] = boundsv[1]; }
	if (boundsv[2] < largestboundsselected[2]) { largestboundsselected[2] = boundsv[2]; }
	if (boundsv[3] > largestboundsselected[3]) { largestboundsselected[3] = boundsv[3]; }
	if (boundsv[4] < largestboundsselected[4]) { largestboundsselected[4] = boundsv[4]; }
	if (boundsv[5] > largestboundsselected[5]) { largestboundsselected[5] = boundsv[5]; }


	if (Normalboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Normalboundslm[0]; }
	if (Normalboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Normalboundslm[1]; }
	if (Normalboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Normalboundslm[2]; }
	if (Normalboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Normalboundslm[3]; }
	if (Normalboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Normalboundslm[4]; }
	if (Normalboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Normalboundslm[5]; }

	if (Targetboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Targetboundslm[0]; }
	if (Targetboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Targetboundslm[1]; }
	if (Targetboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Targetboundslm[2]; }
	if (Targetboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Targetboundslm[3]; }
	if (Targetboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Targetboundslm[4]; }
	if (Targetboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Targetboundslm[5]; }

	if (Nodeboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Nodeboundslm[0]; }
	if (Nodeboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Nodeboundslm[1]; }
	if (Nodeboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Nodeboundslm[2]; }
	if (Nodeboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Nodeboundslm[3]; }
	if (Nodeboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Nodeboundslm[4]; }
	if (Nodeboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Nodeboundslm[5]; }

	if (Handleboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Handleboundslm[0]; }
	if (Handleboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Handleboundslm[1]; }
	if (Handleboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Handleboundslm[2]; }
	if (Handleboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Handleboundslm[3]; }
	if (Handleboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Handleboundslm[4]; }
	if (Handleboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Handleboundslm[5]; }

	if (Flagboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Flagboundslm[0]; }
	if (Flagboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Flagboundslm[1]; }
	if (Flagboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Flagboundslm[2]; }
	if (Flagboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Flagboundslm[3]; }
	if (Flagboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Flagboundslm[4]; }
	if (Flagboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Flagboundslm[5]; }

	double A[3];
	double B[3];
	double diag[3];
	A[0] = largestboundsselected[0];
	A[1] = largestboundsselected[2];
	A[2] = largestboundsselected[4];
	B[0] = largestboundsselected[1];
	B[1] = largestboundsselected[3];
	B[2] = largestboundsselected[5];
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	double lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));
	return lengthxyz;

}
void vtkMDInteractorStyle::TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]) {
	double pointPred[4]; double pointNew[4] = { 0, 0, 0, 0 };
	pointPred[0] = pointin[0];
	pointPred[1] = pointin[1];
	pointPred[2] = pointin[2];
	pointPred[3] = 1;

	matrix->MultiplyPoint(pointPred, pointNew);
	pointout[0] = pointNew[0];
	pointout[1] = pointNew[1];
	pointout[2] = pointNew[2];
}



void vtkMDInteractorStyle::RotateActors()
{
	//cout << "ROTATE! " << endl;
	if (this->CurrentRenderer == NULL 
		|| this->ActorCollection == NULL
		)
	{
		return;
	}
	cout << "Rotate Actors" << endl;
	vtkRenderWindowInteractor *rwi = this->Interactor;
	vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

	// First get the origin of the assembly
	/*double *obj_center = this->InteractionProp->GetCenter();*/
	double rot_center[3] = { 0,0,0 };
	
	this->GetCenterOfMassOfSelectedActors(rot_center);
	cout << "Rotation center: " << rot_center[0] << "," << rot_center[1] << "," << rot_center[2] << endl;
	double boundRadius = this->GetBoundingBoxLengthOfSelectedActors();
	cout << "Bound Radius: " << boundRadius << endl;
	if (boundRadius == std::numeric_limits<double>::infinity())
	{
		boundRadius = 60;
	}
	else
	{
		boundRadius *= 0.5;
	}
	// GetLength gets the length of the diagonal of the bounding box
	/*double boundRadius = this->InteractionProp->GetLength() * 0.5;*/


	// Get the view up and view right vectors
	double view_up[3], view_look[3], view_right[3];

	cam->OrthogonalizeViewUp();
	cam->ComputeViewPlaneNormal();
	cam->GetViewUp(view_up);
	vtkMath::Normalize(view_up);
	cam->GetViewPlaneNormal(view_look);
	vtkMath::Cross(view_up, view_look, view_right);
	vtkMath::Normalize(view_right);

	// Get the furtherest point from object position+origin
	double outsidept[3];

	outsidept[0] = rot_center[0] + view_right[0] * boundRadius;
	outsidept[1] = rot_center[1] + view_right[1] * boundRadius;
	outsidept[2] = rot_center[2] + view_right[2] * boundRadius;

	// Convert them to display coord
	double disp_obj_center[3];

	this->ComputeWorldToDisplay(rot_center[0], rot_center[1], rot_center[2],
		disp_obj_center);

	this->ComputeWorldToDisplay(outsidept[0], outsidept[1], outsidept[2],
		outsidept);

	double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center,
		outsidept));
	double nxf = (rwi->GetEventPosition()[0] - disp_obj_center[0]) / radius;

	double nyf = (rwi->GetEventPosition()[1] - disp_obj_center[1]) / radius;

	double oxf = (rwi->GetLastEventPosition()[0] - disp_obj_center[0]) / radius;

	double oyf = (rwi->GetLastEventPosition()[1] - disp_obj_center[1]) / radius;
	cout << "here" << endl;
	if (((nxf * nxf + nyf * nyf) <= 1.0) &&
		((oxf * oxf + oyf * oyf) <= 1.0))
	{
		cout << "and there" << endl;
		double newXAngle = vtkMath::DegreesFromRadians(asin(nxf));
		double newYAngle = vtkMath::DegreesFromRadians(asin(nyf));
		double oldXAngle = vtkMath::DegreesFromRadians(asin(oxf));
		double oldYAngle = vtkMath::DegreesFromRadians(asin(oyf));

		double scale[3];
		scale[0] = scale[1] = scale[2] = 1.0;

		double **rotate = new double*[2];

		rotate[0] = new double[4];
		rotate[1] = new double[4];

		rotate[0][0] = newXAngle - oldXAngle;
		rotate[0][1] = view_up[0];
		rotate[0][2] = view_up[1];
		rotate[0][3] = view_up[2];

		rotate[1][0] = oldYAngle - newYAngle;
		rotate[1][1] = view_right[0];
		rotate[1][2] = view_right[1];
		rotate[1][3] = view_right[2];

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				//cout << "Apply prop3Dtransform" << endl;
				for (vtkIdType j = 0; j < 2; j++)
				{
					for (vtkIdType k = 0; k < 4; k++)
					{
						//cout << "rotate["<<j<<"]"<<"["<<k<<"]="<< rotate[j][k] << endl;

					}
				}
				
				//cout << "scale:" << scale[0] << ","<< scale[1] << ","<< scale[2] << endl;
				
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				myActor->SetChanged(1);
			}
		}
		cout << "rotate volume" << endl;
		this->VolumeCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myVolume);

			vtkProp3D *myPropr2 = vtkProp3D::SafeDownCast(myVolume->GetOutlineActor());
			vtkProp3D *myPropr3 = vtkProp3D::SafeDownCast(myVolume->GetSliceXY2());
			vtkProp3D *myPropr4 = vtkProp3D::SafeDownCast(myVolume->GetSliceXZ2());
			vtkProp3D *myPropr5 = vtkProp3D::SafeDownCast(myVolume->GetSliceYZ2());
			//vtkProp3D *myPropr3 = vtkProp3D::SafeDownCast(myVolume->GetBox());
			if (myVolume->GetSelected() == 1)
			{
				//cout << "Apply prop3Dtransform" << endl;
				for (vtkIdType j = 0; j < 2; j++)
				{
					for (vtkIdType k = 0; k < 4; k++)
					{
						//cout << "rotate["<<j<<"]"<<"["<<k<<"]="<< rotate[j][k] << endl;

					}
				}

				//cout << "scale:" << scale[0] << ","<< scale[1] << ","<< scale[2] << endl;

				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				
				this->Prop3DTransform(myPropr2,
					rot_center,
					2,
					rotate,
					scale);
				this->Prop3DTransform(myPropr3,
					rot_center,
					2,
					rotate,
					scale);
				this->Prop3DTransform(myPropr4,
					rot_center,
					2,
					rotate,
					scale);
				this->Prop3DTransform(myPropr5,
					rot_center,
					2,
					rotate,
					scale);
				/*this->Prop3DTransform(myPropr3,
					rot_center,
					2,
					rotate,
					scale);*/
				myVolume->SetChanged(1);
			}
		}
		this->NormalLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);		
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->TargetLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->NodeLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->HandleLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		this->FlagLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}

		delete[] rotate[0];
		delete[] rotate[1];
		delete[] rotate;

		if (this->AutoAdjustCameraClippingRange)
		{
			this->CurrentRenderer->ResetCameraClippingRange();
			mqMorphoDigCore::instance()->ActivateClippingPlane();
		}
		cout << "Render from RotateActors" << endl;
		rwi->Render();
	}
}

//----------------------------------------------------------------------------
void vtkMDInteractorStyle::SpinActors()
{
	if (this->CurrentRenderer == NULL
		|| this->ActorCollection == NULL
		)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;
	vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

	// Get the axis to rotate around = vector from eye to origin

	double spin_center[3] = { 0,0,0 };
	this->GetCenterOfMassOfSelectedActors(spin_center);
	//double obj_center[3] = { 0,0, 0 };
	
	double motion_vector[3];
	double view_point[3];

	if (cam->GetParallelProjection())
	{
		// If parallel projection, want to get the view plane normal...
		cam->ComputeViewPlaneNormal();
		cam->GetViewPlaneNormal(motion_vector);
	}
	else
	{
		// Perspective projection, get vector from eye to center of actor
		cam->GetPosition(view_point);
		motion_vector[0] = view_point[0] - spin_center[0];
		motion_vector[1] = view_point[1] - spin_center[1];
		motion_vector[2] = view_point[2] - spin_center[2];
		vtkMath::Normalize(motion_vector);
	}

	double disp_obj_center[3];

	this->ComputeWorldToDisplay(spin_center[0], spin_center[1], spin_center[2],
		disp_obj_center);

	double newAngle =
		vtkMath::DegreesFromRadians(atan2(rwi->GetEventPosition()[1] - disp_obj_center[1],
			rwi->GetEventPosition()[0] - disp_obj_center[0]));

	double oldAngle =
		vtkMath::DegreesFromRadians(atan2(rwi->GetLastEventPosition()[1] - disp_obj_center[1],
			rwi->GetLastEventPosition()[0] - disp_obj_center[0]));

	double scale[3];
	scale[0] = scale[1] = scale[2] = 1.0;

	double **rotate = new double*[1];
	rotate[0] = new double[4];

	rotate[0][0] = newAngle - oldAngle;
	rotate[0][1] = motion_vector[0];
	rotate[0][2] = motion_vector[1];
	rotate[0][3] = motion_vector[2];

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor= vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			myActor->SetChanged(1);
		}
		
	}
	this->VolumeCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myVolume);
		vtkProp3D *myPropr2 = vtkProp3D::SafeDownCast(myVolume->GetOutlineActor());
		vtkProp3D *myPropr3 = vtkProp3D::SafeDownCast(myVolume->GetSliceXY2());
		vtkProp3D *myPropr4 = vtkProp3D::SafeDownCast(myVolume->GetSliceXZ2());
		vtkProp3D *myPropr5 = vtkProp3D::SafeDownCast(myVolume->GetSliceYZ2());
		//vtkProp3D *myPropr3 = vtkProp3D::SafeDownCast(myVolume->GetBox());
		if (myVolume->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			this->Prop3DTransform(myPropr2,
				spin_center,
				1,
				rotate,
				scale);
			this->Prop3DTransform(myPropr3,
				spin_center,
				1,
				rotate,
				scale);
			this->Prop3DTransform(myPropr4,
				spin_center,
				1,
				rotate,
				scale);
			this->Prop3DTransform(myPropr5,
				spin_center,
				1,
				rotate,
				scale);
			/*this->Prop3DTransform(myPropr3,
				spin_center,
				1,
				rotate,
				scale);*/
			myVolume->SetChanged(1);
		}

	}
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);			
			myActor->SetChanged(1);
		}
	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
			myActor->SetChanged(1);
		}
	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
			myActor->SetChanged(1);
		}
	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
			myActor->SetChanged(1);
		}
	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			this->Prop3DTransform(myPropr,
				spin_center,
				1,
				rotate,
				scale);
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
			myActor->SetChanged(1);
		}
	}

	delete[] rotate[0];
	delete[] rotate;

	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
		mqMorphoDigCore::instance()->ActivateClippingPlane();
	}
	cout << "Render from SpinActors" << endl;
	rwi->Render();
}

//----------------------------------------------------------------------------
void vtkMDInteractorStyle::PanActors()
{
	if (this->CurrentRenderer == NULL 
		|| this->ActorCollection == NULL)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;

	// Use initial center as the origin from which to pan
	double pan_center[3];
	this->GetCenterOfMassOfSelectedActors(pan_center);

	double disp_obj_center[3], new_pick_point[4];
	double old_pick_point[4], motion_vector[3];

	this->ComputeWorldToDisplay(pan_center[0], pan_center[1], pan_center[2],
		disp_obj_center);

	this->ComputeDisplayToWorld(rwi->GetEventPosition()[0],
		rwi->GetEventPosition()[1],
		disp_obj_center[2],
		new_pick_point);

	this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
		rwi->GetLastEventPosition()[1],
		disp_obj_center[2],
		old_pick_point);

	motion_vector[0] = new_pick_point[0] - old_pick_point[0];
	motion_vector[1] = new_pick_point[1] - old_pick_point[1];
	motion_vector[2] = new_pick_point[2] - old_pick_point[2];

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			myActor->SetChanged(1);
		}
	}
	this->VolumeCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myVolume);
		vtkProp3D *myPropr2 = vtkProp3D::SafeDownCast(myVolume->GetOutlineActor());
		vtkProp3D *myPropr3 = vtkProp3D::SafeDownCast(myVolume->GetSliceXY2());
		vtkProp3D *myPropr4 = vtkProp3D::SafeDownCast(myVolume->GetSliceXZ2());
		vtkProp3D *myPropr5 = vtkProp3D::SafeDownCast(myVolume->GetSliceYZ2());
		//vtk3DWidget *myPropr3 = vtk3DWidget::SafeDownCast(myVolume->GetBox());
		
		if (myVolume->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			if (myPropr2->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr2->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr2->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr2->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}

			if (myPropr3->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr3->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr3->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr3->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			if (myPropr4->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr4->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr2->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr4->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			if (myPropr5->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr2->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr5->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr5->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}

			if (myVolume->GetBox() != NULL)
			{
				//cout << "Transform!" << endl;
				
				//myVolume->GetBox()->GetTransform(t);
				// ici �a ne va pas: il faudrait faire : 1) place widget (remet � 0 tout, m�me les plans) 2) get transform (car c'est transform� par le place widget) 3) translation de la valeur 
				//t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				//myVolume->GetBox()->SetTransform(t);

				/*vtkTransform *t = vtkTransform::New();
				vtkSmartPointer<vtkMatrix4x4> Mat = myVolume->GetMatrix();
				vtkSmartPointer<vtkMatrix4x4> translationMat = vtkSmartPointer<vtkMatrix4x4>::New();
				double tx, ty, tz;
				tx = Mat->GetElement(0, 3);
				ty = Mat->GetElement(1, 3);
				tz = Mat->GetElement(2, 3);
				translationMat->SetElement(0, 3, tx);
				translationMat->SetElement(1, 3, ty);
				translationMat->SetElement(2, 3, tz);
				cout << "tx" << tx << "ty" << ty << "tz" << tz << endl;
				myVolume->GetBox()->GetTransform(t);
				t->SetMatrix(translationMat);
				myVolume->GetBox()->SetTransform(t);
				//M03 M13 M23 = translation
				//cout << "Box Matrix:" << endl;
				//cout << t->GetMatrix() << endl;
				//myVolume->GetBox()->PlaceWidget();
				//cout << "myPropr3 box is not null" << endl;
				t->Delete();*/
			}
			else
			{
				//cout << "myPropr3 box is null (unfortunately)" << endl;
			}
			//myVolume->GetBox()->
			myVolume->SetChanged(1);
		}
	}
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);		
		if (myActor->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

			myActor->SetChanged(1);
		}
	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

			myActor->SetChanged(1);
		}
	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		int nchanged = 0;
		if (myActor->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

			myActor->SetChanged(1);
			nchanged = 1;
		}
		//if (nchanged == 1) { this->NodeLandmarkCollection->SetChanged(1); }
	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

			myActor->SetChanged(1);
		}
	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
		if (myActor->GetSelected() == 1)
		{
			if (myPropr->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(myPropr->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				myPropr->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
			}
			this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

			myActor->SetChanged(1);
		}
	}
	

	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
		mqMorphoDigCore::instance()->ActivateClippingPlane();
	}
	cout << "Render from PanActors" << endl;
	rwi->Render();
}

void vtkMDInteractorStyle::ChangeAttachmentPoint(vtkMatrix4x4 *NewMat, vtkLMActor *LMActor)
{
	vtkBillboardTextActor3D *LMLabel = LMActor->GetLMLabelActor3D();
	double init_pos = 1;
	double mult = 1;
	if (LMActor->GetLMType() == TARGET_LMK || LMActor->GetLMType() == HANDLE_LMK)
	{
		mult = 1.4;
	}
	if (LMActor->GetLMBodyType() == SPHERE)
	{
		init_pos = 0.5*1.1*mult*LMActor->GetLMSize();
	}
	else
	{
		if (LMActor->GetLMType() == FLAG_LMK)
		{
			init_pos = 1.1*mult*LMActor->GetLMSize();
		}
		else
		{
			init_pos = 3 * 1.1*mult*LMActor->GetLMSize();
		}
	}

	double ap[3] = { init_pos,
		0,
		0 };

	double apt[3];

	this->TransformPoint(NewMat, ap, apt);
	LMLabel->SetPosition(apt);
}

//--------------------------------------------------------------------------
void vtkMDInteractorStyle::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

/*void vtkMDInteractorStyle::AttachmentPointTransform(double *ap, vtkProp3D *prop3D, double apt[3])
{
	vtkTransform *Transform = vtkTransform::New();	
	Transform->SetMatrix(prop3D->GetMatrix());
		vtkSmartPointer<vtkPoints>  data = vtkSmartPointer<vtkPoints> ::New();
		data->SetDataTypeToDouble();
		data->SetNumberOfPoints(1);
		data->SetPoint(0, ap);
		vtkSmartPointer<vtkPolyData> pdata = vtkSmartPointer<vtkPolyData>::New();
		pdata->SetPoints(data);
		vtkSmartPointer<vtkTransformFilter> transformFilter =
		vtkSmartPointer<vtkTransformFilter>::New();
		transformFilter->SetInputData(pdata);
		transformFilter->SetTransform(Transform);
		transformFilter->Update();
		vtkSmartPointer<vtkPoints> Tdata = vtkSmartPointer<vtkPoints>::New();
		Tdata = transformFilter->GetOutput()->GetPoints();

		//get transformed center!
		double *tcenter = Tdata->GetPoint(0);
		apt[0] = tcenter[0];
		apt[1] = tcenter[1];
		apt[2] = tcenter[2];
}*/

/*void vtkMDInteractorStyle::AttachmentPointTransform(double* attachmentPoint, double *boxCenter,
	int numRotation,
	double **rotate,
	double *scale, 
	double newAttachmentPoint[3])
{
	vtkMatrix4x4 *oldMatrix = vtkMatrix4x4::New();
	
	double orig[3] = {attachmentPoint[0],attachmentPoint[1],attachmentPoint[2] };	
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(oldMatrix);
	newTransform->Translate(-(boxCenter[0]), -(boxCenter[1]), -(boxCenter[2]));

	for (int i = 0; i < numRotation; i++)
	{
		newTransform->RotateWXYZ(rotate[i][0], rotate[i][1],
			rotate[i][2], rotate[i][3]);
	}

	if ((scale[0] * scale[1] * scale[2]) != 0.0)
	{
		newTransform->Scale(scale[0], scale[1], scale[2]);
	}

	newTransform->Translate(boxCenter[0], boxCenter[1], boxCenter[2]);

	// now try to get the composit of translate, rotate, and scale
	newTransform->Translate(-(orig[0]), -(orig[1]), -(orig[2]));
	newTransform->PreMultiply();
	newTransform->Translate(orig[0], orig[1], orig[2]);



	newAttachmentPoint[0]= newTransform->GetPosition()[0];
	newAttachmentPoint[1] = newTransform->GetPosition()[1];
	newAttachmentPoint[2] = newTransform->GetPosition()[2];

	oldMatrix->Delete();
	newTransform->Delete();
	
}*/

void vtkMDInteractorStyle::Prop3DTransform(vtkProp3D *prop3D,
	double *boxCenter,
	int numRotation,
	double **rotate,
	double *scale)
{
	vtkMatrix4x4 *oldMatrix = vtkMatrix4x4::New();
	prop3D->GetMatrix(oldMatrix);

	double orig[3];
	prop3D->GetOrigin(orig);

	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	if (prop3D->GetUserMatrix() != NULL)
	{
		newTransform->SetMatrix(prop3D->GetUserMatrix());
	}
	else
	{
		newTransform->SetMatrix(oldMatrix);
	}

	newTransform->Translate(-(boxCenter[0]), -(boxCenter[1]), -(boxCenter[2]));

	for (int i = 0; i < numRotation; i++)
	{
		newTransform->RotateWXYZ(rotate[i][0], rotate[i][1],
			rotate[i][2], rotate[i][3]);
	}

	if ((scale[0] * scale[1] * scale[2]) != 0.0)
	{
		newTransform->Scale(scale[0], scale[1], scale[2]);
	}

	newTransform->Translate(boxCenter[0], boxCenter[1], boxCenter[2]);

	// now try to get the composit of translate, rotate, and scale
	newTransform->Translate(-(orig[0]), -(orig[1]), -(orig[2]));
	newTransform->PreMultiply();
	newTransform->Translate(orig[0], orig[1], orig[2]);

	if (prop3D->GetUserMatrix() != NULL)
	{
		// apply new transform to prop3D
		newTransform->GetMatrix(prop3D->GetUserMatrix());
	}
	else
	{
		prop3D->SetPosition(newTransform->GetPosition());
		prop3D->SetScale(newTransform->GetScale());
		prop3D->SetOrientation(newTransform->GetOrientation());
	}
	oldMatrix->Delete();
	newTransform->Delete();
}

/*
// Calculates rotation matrix given euler angles.
Mat eulerAnglesToRotationMatrix(Vec3f &theta)
{
	// Calculate rotation about x axis
	Mat R_x = (Mat_<double>(3,3) <<
			   1,       0,              0,
			   0,       cos(theta[0]),   -sin(theta[0]),
			   0,       sin(theta[0]),   cos(theta[0])
			   );

	// Calculate rotation about y axis
	Mat R_y = (Mat_<double>(3,3) <<
			   cos(theta[1]),    0,      sin(theta[1]),
			   0,               1,      0,
			   -sin(theta[1]),   0,      cos(theta[1])
			   );

	// Calculate rotation about z axis
	Mat R_z = (Mat_<double>(3,3) <<
			   cos(theta[2]),    -sin(theta[2]),      0,
			   sin(theta[2]),    cos(theta[2]),       0,
			   0,               0,                  1);


	// Combined rotation matrix
	Mat R = R_z * R_y * R_x;

	return R;

}

// Checks if a matrix is a valid rotation matrix.
bool isRotationMatrix(Mat &R)
{
	Mat Rt;
	transpose(R, Rt);
	Mat shouldBeIdentity = Rt * R;
	Mat I = Mat::eye(3,3, shouldBeIdentity.type());

	return  norm(I, shouldBeIdentity) < 1e-6;

}

// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
Vec3f rotationMatrixToEulerAngles(Mat &R)
{

	assert(isRotationMatrix(R));

	float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );

	bool singular = sy < 1e-6; // If

	float x, y, z;
	if (!singular)
	{
		x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
		y = atan2(-R.at<double>(2,0), sy);
		z = atan2(R.at<double>(1,0), R.at<double>(0,0));
	}
	else
	{
		x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
		y = atan2(-R.at<double>(2,0), sy);
		z = 0;
	}
	return Vec3f(x, y, z);



}

*/