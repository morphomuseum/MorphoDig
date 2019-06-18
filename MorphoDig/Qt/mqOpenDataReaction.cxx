/*=========================================================================

   Program: MorphoDig
   Module:    mqOpenDataReaction.cxx

========================================================================*/
#include "mqOpenDataReaction.h"
#include "mqOpenRawDialog.h"
#include "mqOpenTiff3DDialog.h"

#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include <vtkImageAppend.h>

#include <vtkTIFFReader.h>
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

	
	int ok = mqMorphoDigCore::instance()->OpenMesh(fileName);
	if (ok == 1) { mqMorphoDigCore::instance()->CreateSurfaceUndoSet(1); }
}

void mqOpenDataReaction::OpenVolume()
{
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open MHA MHD VTI!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load MHD/MHA volume"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("surfaces (*.mhd *.mha)"));



	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;

	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	mqMorphoDigCore::instance()->OpenVolume(fileName);
}

void mqOpenDataReaction::OpenLandmark(int mode)
{
	//mode 0=> inside normal landmarks
	// 1 => inside target landmarks
	//2 => inside node landmarks
	//3 => inside handle landmarks
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open LMK VER TPS PTS!" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Load landmarks"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("landmark files(*.ver *.lmk *.pts *.tps)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	
	std::string VERext(".ver");
	std::string VERext2(".VER");	
	std::string LMKext(".lmk");
	std::string LMKext2(".LMK");
	std::string PTSext(".pts");
	std::string PTSext2(".PTS");
	std::string TPSext(".tps");
	std::string TPSext2(".TPS");

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
	found = fileName.toStdString().find(PTSext);
	found2 = fileName.toStdString().find(PTSext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 2; //PTS
	}
	found = fileName.toStdString().find(TPSext);
	found2 = fileName.toStdString().find(TPSext2);
	if (found != std::string::npos || found2 != std::string::npos)
	{
		type = 3; //TPS
	}
	if (type == 0)
	{
		
		mqMorphoDigCore::instance()->OpenVER(fileName, mode);
	}
	else if (type == 1)
	{
		mqMorphoDigCore::instance()->OpenLMK(fileName, mode);
	}
	else if (type == 2)
	{
		mqMorphoDigCore::instance()->OpenPTS(fileName, mode);
	}
	else if (type == 3)
	{
		mqMorphoDigCore::instance()->OpenTPS(fileName, mode);
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
void mqOpenDataReaction::ImportAvizoLandmarks()
{

	cout << "Import Avizo LM" << endl;

	QString fileName = QFileDialog::getOpenFileName(this->MainWindow,
		tr("Import Avizo/Amira Landmarks inside Normal and Target landmark lists"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("Avizo/Amira landmarks (*.lm *.landmarkAscii)"));

	cout << fileName.toStdString() << endl;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());
	mqMorphoDigCore::instance()->ImportAvizoLandmarks(fileName);

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
void mqOpenDataReaction::OpenRAW()
{
	QString fileName = QFileDialog::getOpenFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		tr("Open raw data"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("raw data (*.raw);; other (*.*)"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	mqOpenRawDialog OpenRaw_dialog(mqCoreUtilities::mainWidget());
	OpenRaw_dialog.setFileName(fileName);
	OpenRaw_dialog.exec();
}
void mqOpenDataReaction::OpenTIFF3D()
{
	QString fileName = QFileDialog::getOpenFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		tr("Open tiff 3D file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("tiff 3D files (*.tiff *.tif )"));

	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	mqOpenTiff3DDialog OpenTiff3D_dialog(mqCoreUtilities::mainWidget());
	OpenTiff3D_dialog.setFileName(fileName);
	OpenTiff3D_dialog.exec();
}
void mqOpenDataReaction::OpenTIFF2DStack()
{
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open Data!" << endl;


	QStringList filenames = QFileDialog::getOpenFileNames(this->MainWindow,
		tr("Load 2D tiff stack"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("2D TIFF files (*.tif *.tiff )"));
	int cpt_tiff = 0;
	int tiff_3D = 1;

	if (!filenames.isEmpty())
	{
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
				
		

		
		if (cpt_tiff > 0)
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
				std::size_t found = fileName.toStdString().find(TIFext);
				std::size_t found2 = fileName.toStdString().find(TIFext2);
				std::size_t found3 = fileName.toStdString().find(TIFext3);
				std::size_t found4 = fileName.toStdString().find(TIFext4);

				if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
				{

					QFile file(fileName);
					QString name = "";
					if (file.exists()) {
						dimZ++;

						name = file.fileName(); // Return only a file name		
						vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
						vtkSmartPointer <vtkTIFFReader> tiffReader = vtkSmartPointer<vtkTIFFReader>::New();
						tiffReader->SetFileName(fileName.toLocal8Bit());
						//tiffReader->GetF
						tiffReader->Update();
						input = tiffReader->GetOutput();
						int dim[3];
						input->GetDimensions(dim);

						if (dim[2] != 1)
						{
							found_3Dtiff = 1;
							//mettre un message : pas possible de mettre un 3D tiff 
							QMessageBox msgBox;
							msgBox.setText("Error: 3D TIFF file found. Please only open 2D TIFF files when using this menu.");
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
									QString msg = "At lease one 2D image differs in dimensions from those of the first opened tiff image(" + QString(dimX) + "," + QString(dimY) + "). These files will be ignored.";
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
	}

}
void mqOpenDataReaction::OpenDicomFolder()
{
	cout << "Open Data!" << endl;
	QString dcmDir = QFileDialog::getExistingDirectory(this->MainWindow, tr("Select DICOM Folder"), mqMorphoDigCore::instance()->Getmui_LastUsedDir());
	if (dcmDir.isEmpty()) return;

	mqMorphoDigCore::instance()->OpenDicomFolder(dcmDir);

	

}
void mqOpenDataReaction::OpenData()
{
	//mqMorphoDigCore::instance()->getUndoStack();
	cout << "Open Data!" << endl;
	
	
	QStringList filenames = QFileDialog::getOpenFileNames(this->MainWindow,
		tr("Load data"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("MorphoDig data or project (*.ntw *.ver *.cur *.stv *.tag *.tgp *.pos *.ori *.flg *.lmk *.pts *.tps *.ply *.stl *.vtk *.obj *.vtp *.mha *.mhd *.vti *.raw *.tif *.tiff )")); 
	int cpt_tiff = 0;
	int tiff_3D = 1;
	
	if (!filenames.isEmpty())
	{
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
			std::string TPSext(".tps");
			std::string TPSext2(".TPS");
			std::string PTSext(".pts");
			std::string PTSext2(".PTS");
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


			int type = 0; //0 = stl, 1 = vtk,  2 = ply, 3 = ntw, 4 ver, 5 cur, 6 flg, 7 lmk, 8 tag, 9 stv, 10 ori, 11 pos 13 mhd mha vti
			std::size_t found = fileName.toStdString().find(STLext);
			std::size_t found2 = fileName.toStdString().find(STLext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 0;
				//STL
			}

			found = fileName.toStdString().find(VTKext);
			found2 = fileName.toStdString().find(VTKext2);
			if (found != std::string::npos || found2 != std::string::npos )
			{
				type = 1; //VTK
			}
			std::size_t found3 = fileName.toStdString().find(VTKext3);
			std::size_t found4 = fileName.toStdString().find(VTKext4);
			if (found3 != std::string::npos || found4 != std::string::npos)
			{
				type = 1; //VTP
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
				cout << "MHD MHA or VTI" << endl;
				type = 13; //MHA MHD VTI
			}
			found = fileName.toStdString().find(PTSext);
			found2 = fileName.toStdString().find(PTSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 14; //PTS
				cout << "PTS" << endl;
			}

			found = fileName.toStdString().find(TPSext);
			found2 = fileName.toStdString().find(TPSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				cout << "TPS" << endl;
				type = 15; //TPS
			}

			found = fileName.toStdString().find(RAWext);
			found2 = fileName.toStdString().find(RAWext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				cout << "RAW" << endl;
				type = 16; //RAW
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
			if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
			{
				type = 17;
				//Tif TIFF
			}
		
			if (type < 4)
			{
				int ok = mqMorphoDigCore::instance()->OpenMesh(fileName);
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
			
		}
		if (cpt_tiff > 1)
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
				std::size_t found = fileName.toStdString().find(TIFext);
				std::size_t found2 = fileName.toStdString().find(TIFext2);
				std::size_t found3 = fileName.toStdString().find(TIFext3);
				std::size_t found4 = fileName.toStdString().find(TIFext4);

				if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
				{

					QFile file(fileName);
					QString name = "";
					if (file.exists()) {
						dimZ++;

						name = file.fileName(); // Return only a file name		
						vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
						vtkSmartPointer <vtkTIFFReader> tiffReader = vtkSmartPointer<vtkTIFFReader>::New();
						tiffReader->SetFileName(fileName.toLocal8Bit());
						//tiffReader->GetF
						tiffReader->Update();
						input = tiffReader->GetOutput();
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
									QString msg = "At lease one 2D image differs in dimensions from those of the first opened TIFF image(" + QString(dimX) + "," + QString(dimY) + "). These files will be ignored.";
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
	}
	



	
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


