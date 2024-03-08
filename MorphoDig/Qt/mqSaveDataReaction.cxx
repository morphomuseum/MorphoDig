/*=========================================================================

   Program: MorphoDig
   Module:    mqSaveDataReaction.cxx

========================================================================*/
#include "mqSaveDataReaction.h"

#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "mqMorphoDigCore.h"
#include "vtkMDActor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <vtkPLYReader.h>
#include <vtkMath.h>
#include <vtkSTLReader.h>
#include <vtkCleanPolyData.h>
#include <vtkFloatArray.h>

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataNormals.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//-----------------------------------------------------------------------------
mqSaveDataReaction::mqSaveDataReaction(QAction* parentObject,  int _mode)
  : Superclass(parentObject)
{
  this->Mode = _mode;
  this->MainWindow = mqMorphoDigCore::instance()->GetMainWindow();
  
}



void mqSaveDataReaction::ExportLandmarksAsFlags()
{
	QString proposedName = "";
	vtkIdType num_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	vtkIdType num_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfItems();
	if (num_meshes == 1 || num_volumes == 1)
	{
		if (num_meshes == 1)
		{
			mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
			proposedName += QDir::separator();
			proposedName += myActor->GetName().c_str();
		}
		else
		{
			mqMorphoDigCore::instance()->getVolumeCollection()->InitTraversal();
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(mqMorphoDigCore::instance()->getVolumeCollection()->GetNextVolume());
			proposedName += QDir::separator();
			proposedName += myVolume->GetName().c_str();

		}
	}
	else
	{
		vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 1)
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			proposedName += QDir::separator();
			proposedName += +mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str();
		}
		else
		{
			vtkIdType num_selected_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
			if (num_selected_volumes == 1)
			{
				mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
				proposedName += QDir::separator();
				proposedName += +mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str();
			}

		}
	}

	QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		tr("Export landmarks as flags"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + proposedName,
		tr("MorphoDig flag file (*.flg)"), NULL
		//, QFileDialog::DontConfirmOverwrite
	);


	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	cout << "Export Normal landmarks as flags" << endl;

	mqMorphoDigCore::instance()->ExportLandmarksAsFlags(fileName, 0);


}
void mqSaveDataReaction::ExportAvizoLandmarks()
{
	/*ExportAvizoLandmarks
	if (this->m_lmktype==0)// Normal landmarks
	{
		num_landmarks = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
	}
	else if (this->m_lmktype == 1) // Target landmarks
	{
		num_landmarks = mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();

	}
	*/
	QString proposedName = "";
	vtkIdType num_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	vtkIdType num_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfItems();
	if (num_meshes == 1 || num_volumes == 1)
	{
		if (num_meshes == 1)
		{
			mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
			proposedName += QDir::separator();
			proposedName += myActor->GetName().c_str();
		}
		else
		{
			mqMorphoDigCore::instance()->getVolumeCollection()->InitTraversal();
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(mqMorphoDigCore::instance()->getVolumeCollection()->GetNextVolume());
			proposedName += QDir::separator();
			proposedName += myVolume->GetName().c_str();

		}
	}
	else
	{
		vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 1)
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			proposedName += QDir::separator();
			proposedName += +mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str();
		}
		else
		{
			vtkIdType num_selected_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
			if (num_selected_volumes == 1)
			{
				mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
				proposedName += QDir::separator();
				proposedName += +mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str();
			}

		}
	}

	QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
		tr("Export Avizo/Amira Landmark files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + proposedName,
		tr("Avizo/Amira landmark file (*.lm *.landmarkAscii)"), NULL
		//, QFileDialog::DontConfirmOverwrite
	);


	cout << fileName.toStdString();
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	cout << "Export Avizo File" << endl;

	mqMorphoDigCore::instance()->ExportAvizoLandmarks(fileName);


}
void mqSaveDataReaction::SavePOS(int mode)
{
	//mode = 0 : for selected surfaces
	//mode = 1 : for selected volumes
	
	if (mode == 0)
	{
		vtkIdType num_selected_surfaces = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_surfaces == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select one single surface to use this option.");
			msgBox.exec();
			return;
		}
		else if (num_selected_surfaces > 1)
		{
			QMessageBox msgBox;
			msgBox.setText("Several surfaces are currently selected. Please select one single surface to use this option.");
			int ret = msgBox.exec();
			return;

		}
		vtkMDActor *FirstSelectedActor = mqMorphoDigCore::instance()->GetFirstSelectedActor();
		if (FirstSelectedActor != NULL) {

			vtkSmartPointer<vtkMatrix4x4> Mat = FirstSelectedActor->GetMatrix();
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();

			QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save POS files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("Pos file (*.pos)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);


			cout << fileName.toStdString();
			if (fileName.isEmpty()) return;
			QFileInfo fileInfo(fileName);
			mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


			//Save and applies position 
			//mqMorphoDigCore::instance()->getUndoStack();
			cout << "Save POS" << endl;
			double BoxBounds[6] = { 0,0,0,0,0,0 };
			if (FirstSelectedActor->GetdisplayROI() == 1)
			{
				FirstSelectedActor->GetBoxBounds(BoxBounds);
			}
			mqMorphoDigCore::instance()->SavePOS(Mat, fileName, BoxBounds);
		}
	}
	else
	{
		vtkIdType num_selected_surfaces = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_surfaces != 0) {
			QMessageBox msgBox;
			msgBox.setText("Some surfaces are selected. Please unselect selected surfaces to save volume position.");
			msgBox.exec();
			return;
		}
		
		vtkIdType num_selected_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
		if (num_selected_volumes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No volume selected. Please select one single volume to use this option.");
			msgBox.exec();
			return;
		}
		else if (num_selected_volumes > 1)
		{
			QMessageBox msgBox;
			msgBox.setText("Several volumes are currently selected. Please select one single volume to use this option.");
			int ret = msgBox.exec();
			return;

		}
		vtkMDVolume *FirstSelectedVolume = mqMorphoDigCore::instance()->GetFirstSelectedVolume();
		if (FirstSelectedVolume != NULL) {

			vtkSmartPointer<vtkMatrix4x4> Mat = FirstSelectedVolume->GetMatrix();
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();

			QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
				tr("Save POS files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str(),
				tr("Pos file (*.pos)"), NULL
				//, QFileDialog::DontConfirmOverwrite
			);


			cout << fileName.toStdString();
			if (fileName.isEmpty()) return;
			QFileInfo fileInfo(fileName);
			mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


			//Save and applies position 
			//mqMorphoDigCore::instance()->getUndoStack();
			cout << "Save POS" << endl;
			double BoxBounds[6] = { 0,0,0,0,0,0 };
			if (FirstSelectedVolume->GetdisplayROI() == 1)
			{
				FirstSelectedVolume->GetBoxBounds(BoxBounds);
			}
			mqMorphoDigCore::instance()->SavePOS(Mat, fileName, BoxBounds);
		}
	}
	
}


void mqSaveDataReaction::SaveORI()
{
	
	cout << "Save ORI" << endl;

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		tr("Save orientation file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("orientation file (*.ori)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	mqMorphoDigCore::instance()->SaveORI(fileName);
	

}

void mqSaveDataReaction::SaveCURInfos()
{

	cout << "Save CUR infos" << endl;

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		tr("Save curve infos (length per curve segment)"), mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
		tr("text file (*.txt)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	QString CurSegName;

	std::string TXText = ".txt";
	std::string TXText2 = ".TXT";
	std::size_t found = fileName.toStdString().find(TXText);
	std::size_t found2 = fileName.toStdString().find(TXText2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".txt");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		int num_seg = mqMorphoDigCore::instance()->getBezierCurveSource()->GetCurveSegmentNumber();
		for (int i = 1; i <= num_seg; i++)
		{
			stream << "Curve_segment_" << i << "_length:	" << mqMorphoDigCore::instance()->getBezierCurveSource()->GetCurveSegmentLength(i) << Qt::endl;
		}

	}
	file.close();
	


}

void mqSaveDataReaction::SaveSelectedSurfaceScalars()
{
	int numsel = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (numsel == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Select at least one surface ");
		msgBox.exec();
		return;

	}
	else
	{
		vtkMDActor * myActor = mqMorphoDigCore::instance()->GetFirstSelectedActor();

		QString actorName = QString(myActor->GetName().c_str());
		QString myText;

		myText = tr("Save scalars of ") + actorName;




		QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
			myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + actorName + "_scalars.txt",
			tr("text file (*.txt)"));

		cout << fileName.toStdString() << endl;;
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


		std::string TXText = ".txt";
		std::string TXText2 = ".TXT";
		std::size_t found = fileName.toStdString().find(TXText);
		std::size_t found2 = fileName.toStdString().find(TXText2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".txt");
		}

		mqMorphoDigCore::instance()->SaveSelectedSurfaceScalars(myActor, fileName);

	}
}
/*void mqSaveDataReaction::SaveSelectedSurfaceSummaryAlongDirection()
{
	int numsel = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (numsel == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	if (numsel > 0 && mqMorphoDigCore::instance()->Getmui_ActiveArray()->NumComp == 1 &&

		(mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_FLOAT
			|| mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_DOUBLE
			)
		)

	{

		QString activeScalar = mqMorphoDigCore::instance()->Getmui_ActiveArray()->Name;
		QString myText;

		myText = tr("Save summary of scalars along direction") + activeScalar;




		QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
			myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + activeScalar + "_summary_along_direction.txt",
			tr("text file (*.txt)"));

		cout << fileName.toStdString() << endl;;
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


		std::string TXText = ".txt";
		std::string TXText2 = ".TXT";
		std::size_t found = fileName.toStdString().find(TXText);
		std::size_t found2 = fileName.toStdString().find(TXText2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".txt");
		}

		mqMorphoDigCore::instance()->SaveActiveScalarSummaryAlongDirection(fileName, 0, "", 0, 20, 0);
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select at least one surface and activate existing scalars to use this option");
		msgBox.exec();
		return;

	}
}*/

/*Not used any longer : now called from mqScalarsInfosDialogReaction*/
void mqSaveDataReaction::SaveActiveScalarSummary()
{
	int numsel = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (numsel == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	if (numsel>0&& mqMorphoDigCore::instance()->Getmui_ActiveArray()->NumComp == 1 &&

		(mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_FLOAT
			|| mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_DOUBLE
			)
		)

	{

		QString activeScalar = mqMorphoDigCore::instance()->Getmui_ActiveArray()->Name;
		QString myText;
	
		myText = tr("Save summary of scalars ")+activeScalar;
		



		QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
			myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir()+ QDir::separator()+activeScalar+"_summary.txt",
			tr("text file (*.txt)"));

		cout << fileName.toStdString() << endl;;
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

		
		std::string TXText = ".txt";
		std::string TXText2 = ".TXT";
		std::size_t found = fileName.toStdString().find(TXText);
		std::size_t found2 = fileName.toStdString().find(TXText2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".txt");
		}

		mqMorphoDigCore::instance()->SaveActiveScalarSummary(fileName, 0, "");
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select at least one surface and activate existing scalars to use this option");
		msgBox.exec();
		return;

	}

}

void mqSaveDataReaction::SaveShapeMeasures(int mode)
{
	//mode: 1: area and volume 
	//mode: 2: normalized shape index area and volume	
	//mode: 3: convex hull area_ratio and ch_normalized_shape_index, area, volume, ch_area, ch_volume

	vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}

	QString myText;
	QString myFileProposition;
	if (mode == 1)
	{
		cout << "Save area and volume" << endl;
		myText = tr("Save area and volume");
		myFileProposition = tr("Area_and_Volume.txt");
	}
	else if (mode == 2)
	{
		cout << "Save normalized shape index" << endl;
		myText = tr("Save normalized shape index");
		myFileProposition = tr("Normalized_shape_index.txt");
	}
	else 
	{
		cout << "Save convex hull area ratio and convex hull normalized shape index" << endl;
		myText = tr("Save convex hull area ratio and convex hull normalized shape index");
		myFileProposition = tr("CH_are_ratio_CH_Normalized_shape_index.txt");
	}



	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + myFileProposition,
		tr("text file (*.txt)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	QString CurSegName;

	std::string TXText = ".txt";
	std::string TXText2 = ".TXT";
	std::size_t found = fileName.toStdString().find(TXText);
	std::size_t found2 = fileName.toStdString().find(TXText2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".txt");
	}

	mqMorphoDigCore::instance()->SaveShapeMeasures(fileName, mode);



}
void mqSaveDataReaction::SaveMeshSize()
{
	vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	QString myText;
	
		cout << "Save  surface size measurements" << endl;
		myText = tr("Save surface size measurements");
	

		QString myFileProposition = tr("Size_measurements.txt");

	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir()+ QDir::separator() + myFileProposition,
		tr("text file (*.txt)"));

	cout << fileName.toStdString() << endl;;
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	

	std::string TXText = ".txt";
	std::string TXText2 = ".TXT";
	std::size_t found = fileName.toStdString().find(TXText);
	std::size_t found2 = fileName.toStdString().find(TXText2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".txt");
	}

	mqMorphoDigCore::instance()->SaveMeshSize(fileName);



}


//-----------------------------------------------------------------------------






