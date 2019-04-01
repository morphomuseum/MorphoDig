/*=========================================================================

   Program: MorphoDig
   Module:    mqOpenDataReaction.cxx

========================================================================*/
#include "mqOpenDataReaction.h"

#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>




#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//-----------------------------------------------------------------------------
mqOpenDataReaction::mqOpenDataReaction(QAction* parentObject,  int _mode)
  : Superclass(parentObject)
{
  this->Mode = _mode;
  this->MainWindow = mqMorphoDigCore::instance()->GetMainWindow();
  
}

//@@TODO
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void mqOpenDataReaction::OpenMesh()
{
 //mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open mesh!" << endl;
	
	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load surface"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("surfaces (*.ply *.stl *.vtk *.obj)"));

	

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	
	mqMorphoDigCore::instance()->OpenMesh(fileName);
}
void mqOpenDataReaction::OpenLMK_or_VER(int mode)
{
	//mode 0=> inside normal landmarks
	// 1 => inside target landmarks
	//2 => inside node landmarks
	//3 => inside handle landmarks
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open LMK or VER!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load landmarks"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("landmark files(*.ver *.lmk)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	
	std::string VERext(".ver");
	std::string VERext2(".VER");	
	std::string LMKext(".lmk");
	std::string LMKext2(".LMK");

	int type = 0; //0 =  ver, 1 lmk
	std::size_t found = fileName.toStdString().find(VERext);
	std::size_t found2 = fileName.toStdString().find(VERext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 0; //VER
	}

	
	found = fileName.toStdString().find(LMKext);
	found2 = fileName.toStdString().find(LMKext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 1; //LMK
	}

	if (type == 0)
	{
		
		mqMorphoDigCore::instance()->OpenVER(fileName, mode);
	}
	else 
	{
		mqMorphoDigCore::instance()->OpenLMK(fileName, mode);
	}

}
void mqOpenDataReaction::OpenSTV()
{
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open STV!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load MorphoDig landmarks/curve files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("Landmark files(*.stv)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;

	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	std::string STVext(".stv");
	std::string STVext2(".STV");
	

	int type = 0;
	std::size_t found = fileName.toStdString().find(STVext);
	std::size_t found2 = fileName.toStdString().find(STVext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 0; //STV
	}



	if (type == 0)
	{

		mqMorphoDigCore::instance()->OpenSTV(fileName);
	}
	

}
void mqOpenDataReaction::OpenMAP()
{
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open MAP!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Import MorphoDig color maps"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("color map files(*.map)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;

	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	std::string MAPext(".map");
	std::string MAPext2(".MAP");


	int type = 0;
	std::size_t found = fileName.toStdString().find(MAPext);
	std::size_t found2 = fileName.toStdString().find(MAPext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 0; //MAP
	}



	if (type == 0)
	{

		mqMorphoDigCore::instance()->OpenMAP(fileName);
	}


}

void mqOpenDataReaction::OpenPOS(int mode)
{
	if (mode < 1) { mode = 1; }
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags
	
	if (mode == 1)
	{
		vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}
	}
	else
	{
		vtkIdType num_selected_landmarks = 0;
		num_selected_landmarks+= mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();

		if (num_selected_landmarks == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark/flag selected. Please select at least one landmark or flag to use this option.");
			msgBox.exec();
			return;
		}
	}
	//open and applies position 
	
	cout << "Open POS" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load position file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("position file (*.pos)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;	
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMorphoDigCore::instance()->OpenPOS(fileName,mode);
	
}

void mqOpenDataReaction::OpenPOSTrans(int mode)
{
	if (mode < 1) { mode = 1; }
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags

	if (mode == 1)
	{
		vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}
	}
	else
	{
		vtkIdType num_selected_landmarks = 0;
		num_selected_landmarks += mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
		num_selected_landmarks += mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();

		if (num_selected_landmarks == 0) {
			QMessageBox msgBox;
			msgBox.setText("No landmark/flag selected. Please select at least one landmark or flag to use this option.");
			msgBox.exec();
			return;
		}
	}


	//open and applies transpoed position 
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open transposed POS" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load transposed position file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("transposed position file (*.pos)"));

	cout << fileName.toStdString() << endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	mqMorphoDigCore::instance()->OpenPOSTrans(fileName, mode);

}

void mqOpenDataReaction::OpenORI()
{
	
	cout << "Open ORI" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load orientation file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("orientation file (*.ori)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	mqMorphoDigCore::instance()->OpenORI(fileName);

}
void mqOpenDataReaction::OpenFLG()
{

	cout << "Open FLG" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load flag file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("flag file (*.flg)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMorphoDigCore::instance()->OpenFLG(fileName);

}
void mqOpenDataReaction::OpenCUR()
{

	cout << "Open CUR" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load CUR file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("curve file (*.cur)"));

	cout << fileName.toStdString()<<endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMorphoDigCore::instance()->OpenCUR(fileName);

}
void mqOpenDataReaction::OpenTAGMAP()
{

	cout << "Open TAGMAP" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load TAG MAP file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("tag file (*.tag *.tgp)"));

	cout << fileName.toStdString() << endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	mqMorphoDigCore::instance()->OpenTAGMAP(fileName);

}
void mqOpenDataReaction::OpenData()
{
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open Data!" << endl;
	
	
	QStringList filenames = QFileDialog::getOpenFileNames(this->MainWindow,
		tr("Load data"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("MorphoDig data or project (*.ntw *.ver *.cur *.stv *.tag *.tgp *.pos *.ori *.flg *.lmk *.ply *.stl *.vtk *.obj *.vtp)"));

	if (!filenames.isEmpty())
	{
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

			int type = 0; //0 = stl, 1 = vtk,  2 = ply, 3 = ntw, 4 ver, 5 cur, 6 flg, 7 lmk, 8 tag, 9 stv, 10 ori, 11 pos
			std::size_t found = fileName.toStdString().find(STLext);
			std::size_t found2 = fileName.toStdString().find(STLext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 0;
				//STL
			}

			found = fileName.toStdString().find(VTKext);
			found2 = fileName.toStdString().find(VTKext2);
			std::size_t found3 = fileName.toStdString().find(VTKext3);
			std::size_t found4 = fileName.toStdString().find(VTKext4);
			if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
			{
				type = 1; //VTK
			}

			//std::cout << "2Type= " <<type<< std::endl;
			found = fileName.toStdString().find(PLYext);
			found2 = fileName.toStdString().find(PLYext2);

			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 2; //PLY
			}

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
				type =5; //VER
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


			if (type < 4)
			{
				mqMorphoDigCore::instance()->OpenMesh(fileName);
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
		}
	}
	

	/*QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load data"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("MorphoDig data or project (*.ntw *.ver *.cur *.stv *.tag *.pos *.ori *.flg *.lmk *.ply *.stl *.vtk *.obj *.vtp)"));*/

	
}

void mqOpenDataReaction::OpenNTW()
{
//char param1[1000];
	int i = 0;
	/*
	mqMorphoDigCore::instance()->Getmui_LastUsedDir()
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	*/
	QString NTWfile = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load project"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("NTW (*.ntw)"));
	
	cout << NTWfile.toStdString() << endl;
	if (NTWfile.isEmpty()) return;
	
	QFileInfo fileInfo(NTWfile);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	

	mqMorphoDigCore::instance()->OpenNTW(NTWfile);
}




//-----------------------------------------------------------------------------


