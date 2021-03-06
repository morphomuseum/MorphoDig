/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqOpenRawDialog_h
#define _mqOpenRawDialog_h


#include <QDialog>

namespace Ui
{
class mqOpenRawDialog;
}




/**
* mqOpenRawDialog is the dialog used by MorphoDig to create an OpenRaw from the 1st selected volume
*/
class  mqOpenRawDialog : public QDialog
{
  Q_OBJECT

public:
  mqOpenRawDialog(QWidget* Parent);
  
  ~mqOpenRawDialog();
  void OpenRaw();
  void setFileName(QString fileName);
  void RecomputeRequested(int dataType, int dimX, int dimY, int dimZ, double headerSize);
  
  public slots:
  
	  virtual void slotDataTypeChanged(int newDataType);
	  
	  virtual void slotBigEndianClicked(bool checked);
	  virtual void slotLittleEndianClicked(bool isChecked);
	  virtual void slotFrontToBackClicked(bool isChecked);
	  virtual void slotBackToFrontClicked(bool isChecked);
	  

	  virtual void slotDimXChanged(int newDimX);
	  virtual void slotDimYChanged(int newDimY);
	  virtual void slotDimZChanged(int newDimZ);
	  virtual void slotHeaderSizeChanged(double newHeaderSize);
	  virtual void slotVoxelSizeXChanged(double newsVoxelSizeX);
	  virtual void slotVoxelSizeYChanged(double newsVoxelSizeY);
	  virtual void slotVoxelSizeZChanged(double newsVoxelSizeZ);
  virtual void slotOpenRaw();
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqOpenRawDialog)
  Ui::mqOpenRawDialog* const Ui;
  QString myFileName;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
