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





void mqSaveDataReaction::SavePOS()
{
	
	
	vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at one single surface to use this option.");
		msgBox.exec();
		return;
	}
	else if (num_selected_meshes > 1)
	{
		QMessageBox msgBox;
		msgBox.setText("More than one surface are currently selected. Please select at one single surface to use this option.");		
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

		mqMorphoDigCore::instance()->SavePOS(Mat, fileName);
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
			stream << "Curve_segment_" << i << "_length(mm):" << mqMorphoDigCore::instance()->getBezierCurveSource()->GetCurveSegmentLength(i)<<endl;
		}

	}
	file.close();
	


}

void mqSaveDataReaction::SaveShapeMeasures(int mode)
{
	//mode: 1: area and volume 
	//mode: 2: normalized shape index area and volume	
	//mode: 3: convex hull area_ratio and ch_normalized_shape_index, area, volume, ch_area, ch_volume
	QString myText;
	if (mode == 1)
	{
		cout << "Save normalized shape index" << endl;
		myText = tr("Save normalized shape index");
	}
	else if (mode == 2)
	{
		cout << "Save mean readius normalized shape index" << endl;
		myText = tr("Save mean readius normalized shape index");
	}
	else 
	{
		cout << "Save convex hull normalized shape index" << endl;
		myText = tr("Save convex hull normalized shape index ");
	}



	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
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
	
	QString myText;
	
		cout << "Save  mesh size " << endl;
		myText = tr("Save mesh size");
	



	QString fileName = QFileDialog::getSaveFileName(this->MainWindow,
		myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir(),
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






