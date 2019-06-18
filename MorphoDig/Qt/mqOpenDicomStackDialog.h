/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqOpenDicomStackDialog_h
#define _mqOpenDicomStackDialog_h

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <QDialog>

namespace Ui
{
class mqOpenDicomStackDialog;
}




/**
* mqOpenDicomStackDialog is the dialog used by MorphoDig to create an OpenDicomStack from the 1st selected volume
*/
class  mqOpenDicomStackDialog : public QDialog
{
  Q_OBJECT

public:
  mqOpenDicomStackDialog(QWidget* Parent);
  
  ~mqOpenDicomStackDialog();
  void OpenDicomStack();
  void setFileName(QString fileName);
  void setDimensions(int dimX, int dimY, int dimZ);
  void setSpacing(double spacingX, double spacingY, double spacingZ);
  void setDataType(int dataType);
  void setInputAsStack();
  void setInputAs3DFile();
  void set2DStackInput(vtkSmartPointer<vtkImageData> input);

public slots:
  
  
	 

  virtual void slotOpenDicomStack();
 // virtual void slotDataTypeChanged(int newDataType);
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqOpenDicomStackDialog)
  Ui::mqOpenDicomStackDialog* const Ui;
  QString myFileName;
  int inputAsStack;
  int myDimX;
  int myDimY;
  int myDimZ;
  double mySpacingX;
  double mySpacingY;
  double mySpacingZ;
  int myDataType;
  vtkSmartPointer<vtkImageData> myInput; 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
