/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqOpenTiff3DDialog_h
#define _mqOpenTiff3DDialog_h

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <QDialog>

namespace Ui
{
class mqOpenTiff3DDialog;
}




/**
* mqOpenTiff3DDialog is the dialog used by MorphoDig to create an OpenTiff3D from the 1st selected volume
*/
class  mqOpenTiff3DDialog : public QDialog
{
  Q_OBJECT

public:
  mqOpenTiff3DDialog(QWidget* Parent);
  
  ~mqOpenTiff3DDialog();
  void OpenTiff3D();
  void setFileName(QString fileName);
  void setDimensions(int dimX, int dimY, int dimZ);
  void setDataType(int dataType);
  void setInputAsStack();
  void setInputAs3DFile();
  void set2DStackInput(vtkSmartPointer<vtkImageData> input);
  //void RecomputeRequested(int dataType, int dimX, int dimY, int dimZ, double headerSize);
  
  public slots:
  
  
	  /*virtual void slotDimXChanged(int newDimX);
	  virtual void slotDimYChanged(int newDimY);
	  virtual void slotDimZChanged(int newDimZ);
	  virtual void slotHeaderSizeChanged(double newHeaderSize);*/
	  virtual void slotVoxelSizeXChanged(double newsVoxelSizeX);
  virtual void slotOpenTiff3D();
 // virtual void slotDataTypeChanged(int newDataType);
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqOpenTiff3DDialog)
  Ui::mqOpenTiff3DDialog* const Ui;
  QString myFileName;
  int inputAsStack;
  int myDimX;
  int myDimY;
  int myDimZ;
  int myDataType;
  vtkSmartPointer<vtkImageData> myInput; //only for 2D stack!
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
