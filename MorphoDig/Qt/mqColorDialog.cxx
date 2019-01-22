/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqColorDialog.h"
#include "ui_mqColorDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QHeaderView>

#include <sstream>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

//-----------------------------------------------------------------------------
mqColorDialog::mqColorDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqColorDialog())
{
  this->Ui->setupUi(this);
  this->setObjectName("mqColorDialog");
  QColor myColor;
  QColor myBGColor;
  QColor myBGColor2;
  double meshcolor[4];
  double backgroundcolor[3];
  double backgroundcolor2[3];
  this->Ui->meshColorButton->setShowAlphaChannel(true);
  mqMorphoDigCore::instance()->Getmui_MeshColor(meshcolor);
  mqMorphoDigCore::instance()->Getmui_BackGroundColor(backgroundcolor);
  mqMorphoDigCore::instance()->Getmui_BackGroundColor2(backgroundcolor2);
  myColor.setRedF(meshcolor[0]);
  myColor.setGreenF(meshcolor[1]);
  myColor.setBlueF(meshcolor[2]);
  myColor.setAlphaF(meshcolor[3]);
  myBGColor.setRedF(backgroundcolor[0]);
  myBGColor.setGreenF(backgroundcolor[1]);
  myBGColor.setBlueF(backgroundcolor[2]);
  
  myBGColor2.setRedF(backgroundcolor2[0]);
  myBGColor2.setGreenF(backgroundcolor2[1]);
  myBGColor2.setBlueF(backgroundcolor2[2]);



  this->Ui->meshColorButton->setChosenColor(myColor);
  connect(this->Ui->meshColorButton, SIGNAL(colorChosen()), this, SLOT(slotMeshColorChanged()));
 
  this->Ui->backgroundColorButton->setChosenColor(myBGColor);
  connect(this->Ui->backgroundColorButton, SIGNAL(colorChosen()), this, SLOT(slotBackGroundColorChanged()));
  this->Ui->backgroundColorButton2->setChosenColor(myBGColor2);
  connect(this->Ui->backgroundColorButton2, SIGNAL(colorChosen()), this, SLOT(slotBackGroundColorChanged2()));

  connect(this->Ui->reinitcolorsButton, SIGNAL(clicked()), this, SLOT(slotReinitializeColors()));

  this->Ui->ambient->setValue(mqMorphoDigCore::instance()->Getmui_Ambient());
  this->Ui->diffuse->setValue(mqMorphoDigCore::instance()->Getmui_Diffuse());
  this->Ui->specular->setValue(mqMorphoDigCore::instance()->Getmui_Specular());
  this->Ui->specularPower->setValue(mqMorphoDigCore::instance()->Getmui_SpecularPower());

  connect(this->Ui->ambient, SIGNAL(valueChanged(int)), this, SLOT(slotAmbient(int)));
  connect(this->Ui->diffuse, SIGNAL(valueChanged(int)), this, SLOT(slotDiffuse(int)));
  connect(this->Ui->specular, SIGNAL(valueChanged(int)), this, SLOT(slotSpecular(int)));
  connect(this->Ui->specularPower, SIGNAL(valueChanged(double)), this, SLOT(slotSpecularPower(double)));




  /*this->Ui->VersionLabel->setText(
	  QString("<html><b>Version: <i>%1</i></b></html>")
	  .arg(QString(MORPHODIG_VERSION) + " " + QString(ENVIRONMENT) + "-bit"));*/
 
}




//-----------------------------------------------------------------------------
mqColorDialog::~mqColorDialog()
{
	QColor myColor = this->Ui->meshColorButton->chosenColor();
	double meshcolor[4];
	myColor.getRgbF(&meshcolor[0], &meshcolor[1], &meshcolor[2], &meshcolor[3]);

	QColor myBGColor = this->Ui->backgroundColorButton->chosenColor();
	double bgcolor[3];
	myBGColor.getRgbF(&bgcolor[0], &bgcolor[1], &bgcolor[2]);

	QColor myBGColor2 = this->Ui->backgroundColorButton2->chosenColor();
	double bgcolor2[3];
	myBGColor2.getRgbF(&bgcolor2[0], &bgcolor2[1], &bgcolor2[2]);

	//cout << "meshcolor[0]="<<meshcolor[0] << endl;
	//cout << "meshcolor[1]=" << meshcolor[1] << endl;
	//cout << "meshcolor[2]=" << meshcolor[2] << endl;
	//cout << "Color dialog closes : meshcolor[3]=" << meshcolor[3] << endl;

	mqMorphoDigCore::instance()->Setmui_MeshColor(meshcolor);
	mqMorphoDigCore::instance()->Setmui_BackGroundColor(bgcolor);
	mqMorphoDigCore::instance()->Setmui_BackGroundColor2(bgcolor2);
	
  delete this->Ui;
}

void mqColorDialog::slotAmbient(int ambient){
	mqMorphoDigCore::instance()->Setmui_Ambient(ambient);
}
void mqColorDialog::slotDiffuse(int diffuse){
	mqMorphoDigCore::instance()->Setmui_Diffuse(diffuse);
}
void mqColorDialog::slotSpecular(int specular){
	mqMorphoDigCore::instance()->Setmui_Specular(specular);
}
void mqColorDialog::slotSpecularPower(double specularPower){
	mqMorphoDigCore::instance()->Setmui_SpecularPower(specularPower);
}

void mqColorDialog::slotMeshColorChanged()
{
	QColor myColor = this->Ui->meshColorButton->chosenColor();
	double meshcolor[4];
	myColor.getRgbF(&meshcolor[0], &meshcolor[1], &meshcolor[2], &meshcolor[3]);
		
	mqMorphoDigCore::instance()->Setmui_MeshColor(meshcolor);	
	mqMorphoDigCore::instance()->Render();
	cout << "Mesh color changed!" << endl;
}
void mqColorDialog::slotBackGroundColorChanged()
{
	QColor myBGColor = this->Ui->backgroundColorButton->chosenColor();
	double bgcolor[3];
	myBGColor.getRgbF(&bgcolor[0], &bgcolor[1], &bgcolor[2]);
	mqMorphoDigCore::instance()->Setmui_BackGroundColor(bgcolor);	
	mqMorphoDigCore::instance()->Render();
	cout << "Background color changed!" << endl;
}
void mqColorDialog::slotBackGroundColorChanged2()
{
	QColor myBGColor2 = this->Ui->backgroundColorButton2->chosenColor();
	double bgcolor2[3];
	myBGColor2.getRgbF(&bgcolor2[0], &bgcolor2[1], &bgcolor2[2]);
	mqMorphoDigCore::instance()->Setmui_BackGroundColor2(bgcolor2);
	mqMorphoDigCore::instance()->Render();
	cout << "Background color 2 changed!" << endl;
}
void mqColorDialog::slotReinitializeColors()
{
	QColor myDefaultMeshColor;
	QColor myDefaultBGColor;
	QColor myDefaultBGColor2;
	double defaultMeshColor[4];
	double defaultBackGroundColor[3];
	double defaultBackGroundColor2[3];
	
	int defaultAmbient;
	int defaultDiffuse;
	int defaultSpecular;
	double defaultSpecularPower;

	defaultAmbient = mqMorphoDigCore::instance()->Getmui_DefaultAmbient();
	defaultDiffuse = mqMorphoDigCore::instance()->Getmui_DefaultDiffuse();
	defaultSpecular = mqMorphoDigCore::instance()->Getmui_DefaultSpecular();
	defaultSpecularPower = mqMorphoDigCore::instance()->Getmui_DefaultSpecularPower();
	mqMorphoDigCore::instance()->Getmui_DefaultMeshColor(defaultMeshColor);
	mqMorphoDigCore::instance()->Getmui_DefaultBackGroundColor(defaultBackGroundColor);
	mqMorphoDigCore::instance()->Getmui_DefaultBackGroundColor2(defaultBackGroundColor2);

	mqMorphoDigCore::instance()->Setmui_MeshColor(defaultMeshColor);
	mqMorphoDigCore::instance()->Setmui_BackGroundColor(defaultBackGroundColor);
	mqMorphoDigCore::instance()->Setmui_BackGroundColor2(defaultBackGroundColor2);

	mqMorphoDigCore::instance()->Setmui_Ambient(defaultAmbient);
	mqMorphoDigCore::instance()->Setmui_Diffuse(defaultDiffuse);
	mqMorphoDigCore::instance()->Setmui_Specular(defaultSpecular);
	mqMorphoDigCore::instance()->Setmui_SpecularPower(defaultSpecularPower);

	myDefaultMeshColor.setRedF(defaultMeshColor[0]);
	myDefaultMeshColor.setGreenF(defaultMeshColor[1]);
	myDefaultMeshColor.setBlueF(defaultMeshColor[2]);
	myDefaultMeshColor.setAlphaF(defaultMeshColor[3]);
	myDefaultBGColor.setRedF(defaultBackGroundColor[0]);
	myDefaultBGColor.setGreenF(defaultBackGroundColor[1]);
	myDefaultBGColor.setBlueF(defaultBackGroundColor[2]);

	myDefaultBGColor2.setRedF(defaultBackGroundColor2[0]);
	myDefaultBGColor2.setGreenF(defaultBackGroundColor2[1]);
	myDefaultBGColor2.setBlueF(defaultBackGroundColor2[2]);



	this->Ui->meshColorButton->setChosenColor(myDefaultMeshColor);	
	this->Ui->backgroundColorButton->setChosenColor(myDefaultBGColor);	
	this->Ui->backgroundColorButton2->setChosenColor(myDefaultBGColor2);
	this->Ui->ambient->setValue(defaultAmbient);
	this->Ui->diffuse->setValue(defaultDiffuse);
	this->Ui->specular->setValue(defaultSpecular);
	this->Ui->specularPower->setValue(defaultSpecularPower);
	mqMorphoDigCore::instance()->Render();
	
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

