/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqLandmarkDialog_h
#define _mqLandmarkDialog_h

#include <QDialog>

namespace Ui
{
class mqLandmarkDialog;
}




/**
* mqLandmarkDialog is the Landmark dialog used by MorphoDig.
* It sets  Landmark and Flag rendering settings
*/
class  mqLandmarkDialog : public QDialog
{
  Q_OBJECT

public:
  mqLandmarkDialog(QWidget* Parent);
  
  ~mqLandmarkDialog();

  public slots:
  virtual void slotFlagColorChanged();
  virtual void slotAdjustLandmarkRenderingSizeChanged();
  virtual void slotAdjustScaleFactorChanged();
  virtual void slotFontSizeChanged();
  virtual void slotLandmarkRenderingSizeChanged();
  virtual void slotFlagRenderingSizeChanged();
  virtual void slotLandmarkBodyTypeChanged();
  virtual void slotReinitializeColors();
  virtual void slotRender();
protected:
	
private:
  Q_DISABLE_COPY(mqLandmarkDialog)
  Ui::mqLandmarkDialog* const Ui;
};

#endif // !_mqLandmarkDialog_h
