#include <QtWidgets>
//#include "QDoubleSlider.h"
#include "QReleaseSlider.h"
#include "QReleaseSliderValue.h"

QReleaseSliderValue::QReleaseSliderValue(Qt::Orientation orientation, const QString &title,
	QWidget *parent)
	//: QGroupBox(title, parent)
	: QWidget(parent)
{
	//this->setFlat(true);
	this->spinbox = new QSpinBox;
	this->spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->spinbox->setDisabled(true);
	//this->spinbox->setMaximum(90);
	//this->spinbox->setMinimum(-90);
	//this->spinbox->setFixedWidth(20);
	//this->spinbox->setFixedSize(15, 15);
	this->spinbox->setFixedSize(16, 15);
	QFont font(this->spinbox->font());
	font.setPointSize(4);
	this->spinbox->setFont(font);

	this->slider = new QReleaseSlider;
	//this->slider = new QSlider(orientation);
	//this->slider = new QDoubleSlider;
	//this->slider = new QDoubleSlider;
	//this->slider->setFocusPolicy(Qt::StrongFocus);
	//this->slider->setTickPosition(QSlider::TicksBothSides);
	//this->slider->setTickInterval(10);
	this->slider->setSingleStep(0.2);

	
	
	connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
	connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));

	QBoxLayout::Direction direction;

	if (orientation == Qt::Vertical)
	{
		direction = QBoxLayout::TopToBottom;
	}
	else
	{
		direction = QBoxLayout::LeftToRight;
	}

	//QBoxLayout *slidersLayout = new QBoxLayout(direction);
	QVBoxLayout *layout2 = new QVBoxLayout;
	layout2->setAlignment(Qt::AlignCenter);
	layout2->addWidget(slider);
	layout2->addWidget(spinbox);
	setLayout(layout2);
	//this->setContentsMargins(0, 0, 0, 0);
	//this->setFixedWidth(30);
}

void QReleaseSliderValue::setValue(int value)
{
	slider->setValue(value);
}

void QReleaseSliderValue::setMinimum(int value)
{
	slider->setMinimum(value);
	spinbox->setMinimum(value);
}

void QReleaseSliderValue::setMaximum(int value)
{
	slider->setMaximum(value);
	
	spinbox->setMaximum(value);
}

void QReleaseSliderValue::invertAppearance(bool invert)
{
	slider->setInvertedAppearance(invert);
	//spinbox->setInvertedAppearance(invert);
	
}

void QReleaseSliderValue::invertKeyBindings(bool invert)
{
	slider->setInvertedControls(invert);
	//spinbox->setInvertedControls(invert);
	
}