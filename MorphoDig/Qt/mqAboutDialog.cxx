/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqAboutDialog.h"
#include "mqMorphoDigCore.h"
#include "ui_mqAboutDialog.h"
#include "MorphoDigVersion.h"


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
mqAboutDialog::mqAboutDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqAboutDialog())
{
  this->Ui->setupUi(this);
  this->setObjectName("mqAboutDialog");
  

  this->Ui->VersionLabel->setText(
	  QString("<html><b>Version: <i>%1</i></b></html>")
	  .arg(QString(MORPHODIG_VERSION) + " " + QString(ENVIRONMENT) + "-bit"));
  this->AddClientInformation();
    /*
	/ get extra information and put it in
  vtkProcessModule* pm = vtkProcessModule::GetProcessModule();
  pqOptions* opts = pqOptions::SafeDownCast(pm->GetOptions());

  std::ostringstream str;
  vtkIndent indent;
  opts->PrintSelf(str, indent.GetNextIndent());
  str << ends;
  QString info = str.str().c_str();
  int idx = info.indexOf("Runtime information:");
  info = info.remove(0, idx);

  this->Ui->VersionLabel->setText(
    QString("<html><b>Version: <i>%1</i></b></html>")
      .arg(QString(PARAVIEW_VERSION_FULL) + " " + QString(PARAVIEW_BUILD_ARCHITECTURE) + "-bit"));
	*/
}

inline void addItem(QTreeWidget* tree, const QString& key, const QString& value)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(tree);
	item->setText(0, key);
	item->setText(1, value);
}
inline void addItem(QTreeWidget* tree, const QString& key, int value)
{
	::addItem(tree, key, QString("%1").arg(value));
}

void mqAboutDialog::AddClientInformation()
{
	//vtkProcessModule* pm = vtkProcessModule::GetProcessModule();
	//pqOptions* opts = pqOptions::SafeDownCast(pm->GetOptions());

	QTreeWidget* tree = this->Ui->ClientInformation;
	
	::addItem(tree, "MorphoDig Version",
		QString(MORPHODIG_VERSION) + " " + QString(ENVIRONMENT) + "-bit");
	::addItem(tree, "Qt Version", QT_VERSION_STR);
	::addItem(tree, "VTK Version", mqMorphoDigCore::instance()->GetVTKVersion());
	::addItem(tree, "OpenGL Version", mqMorphoDigCore::instance()->GetOpenGLVersion());

	

	//::addItem(tree, "Data Directory", opts->GetDataDirectory());

	//vtkNew<vtkMTOpenGLInformation> OpenGLInfo;
	//OpenGLInfo->CopyFromObject(NULL);

	//::addItem(tree, "OpenGL Vendor", QString::fromStdString(OpenGLInfo->GetVendor()));
	//::addItem(tree, "OpenGL Version", QString::fromStdString(OpenGLInfo->GetVersion()));
	//::addItem(tree, "OpenGL Renderer", QString::fromStdString(OpenGLInfo->GetRenderer()));
	//glGetString(GL_VERSION);
	//std::string MyVersion = (std::string)((char*)glGetString(GL_VERSION));

	//::addItem(tree, "OpenGL Version", QString::fromStdString(MyVersion));

#if QT_VERSION >= 0x050000
	tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
	tree->header()->setResizeMode(QHeaderView::ResizeToContents);
#endif
}

//-----------------------------------------------------------------------------
mqAboutDialog::~mqAboutDialog()
{
  delete this->Ui;
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

