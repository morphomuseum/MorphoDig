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
  void RecomputeRequested(int dataType, int dimX, int dimY, int dimZ);
  
  public slots:
  
  
	  virtual void slotRecomputeRequested();
  virtual void slotOpenRaw();
  virtual void slotDataTypeChanged(int newDataType);
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqOpenRawDialog)
  Ui::mqOpenRawDialog* const Ui;
  QString myFileName;
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
