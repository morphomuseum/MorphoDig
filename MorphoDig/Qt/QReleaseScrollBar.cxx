#include "QReleaseScrollBar.h"
#include <cmath>

QReleaseScrollBar::QReleaseScrollBar(QWidget *parent) :
	QScrollBar(parent)
{
	connect(this, SIGNAL(sliderReleased()), this, SLOT(slotRefresh()));

}
void QReleaseScrollBar::slotRefresh()
{
	this->Refresh();
}


void QReleaseScrollBar::Refresh()
{
	//std::cout << "Refresh to 0!" << endl;
	this->setValue(0);
}

