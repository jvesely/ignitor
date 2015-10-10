#include "myRandom.h"
#include "optionDialog.h"
#include <QLayout>
#include <QToolButton>
#include <QSlider>
#include <QColor>
#include <QColorDialog>


OptionDialog::OptionDialog(fractal * par):QDialog(par,Qt::Dialog){
	setModal(true);
	setSizeGripEnabled(false);
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	setFixedSize(330,300);
	
	setWindowTitle(tr("Fractal Options"));
	myFractal = par;
	mainOpt = new GeneralOptions(myFractal);
	tabs = createTabs();
	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(mainOpt);
  layout->addWidget(tabs);
  layout->addWidget(buttonBox);
  setLayout(layout);	
	exec();
}
QTabWidget * OptionDialog::createTabs(){
	 tabs = new QTabWidget(this);
	for (int i = 0; i< myFractal->_funcCount;++i)
		tabs->addTab(new FunctionTab(myFractal->_functions[i]),QString::number(i+1));
	QToolButton *newTabButton = new QToolButton(tabs);
  newTabButton->setCursor(Qt::ArrowCursor);
  newTabButton->setAutoRaise(true);
	newTabButton->setIcon(QIcon(":/images/addtab.png"));
  newTabButton->setToolTip(tr("Add function"));
	tabs->setCornerWidget(newTabButton, Qt::TopLeftCorner);

	QToolButton *closeTabButton = new QToolButton(tabs);
    closeTabButton->setCursor(Qt::ArrowCursor);
  closeTabButton->setAutoRaise(true);
	closeTabButton->setIcon(QIcon(":/images/closetab.png"));
  closeTabButton->setToolTip(tr("Remove function"));
	tabs->setCornerWidget(closeTabButton, Qt::TopRightCorner);

	

	tabs->repaint();
	connect(closeTabButton,SIGNAL(clicked()),this,SLOT(remFunc()));
	connect(newTabButton,SIGNAL(clicked()),this,SLOT(addFunc()));
	//connect(tabs,SIGNAL(currentChanged(int)),this,SLOT(tabChange(false)));
	return tabs;
}
void OptionDialog::addFunc(){

	tabs->addTab(new FunctionTab(Function()),QString::number(tabs->count()+1));
	tabs->setCurrentIndex(tabs->count()-1);
	tabChange(false);
}
void OptionDialog::remFunc(){
	delete tabs->currentWidget();
	tabChange(true);
}
void OptionDialog::tabChange(bool deleted){
	if(tabs->count()<1)
		addFunc();
	if (deleted)
		for (int i =0;i<tabs->count();++i)
			tabs->setTabText(i,QString::number(i+1));
	
	mainOpt->setFuncCount(tabs->count());
}
void OptionDialog::accept(){
	myFractal->_iterations = mainOpt->getIterations();
	myFractal->_height = mainOpt->getHeight();
	myFractal->_width = mainOpt->getWidth();
	myFractal->delFunc();
	for (int i = 0;i<tabs->count();++i){
		myFractal->addFunc(*((FunctionTab *)tabs->widget(i)));	
	}
	reject();
}
FunctionTab::FunctionTab(const Function &from){
	setupUi(this);
	doubleSpinBoxXX->setValue(from._coefs[0]);
	doubleSpinBoxXY->setValue(from._coefs[1]);
	doubleSpinBoxXA->setValue(from._coefs[2]);
	doubleSpinBoxYX->setValue(from._coefs[3]);
	doubleSpinBoxYY->setValue(from._coefs[4]);
	doubleSpinBoxYA->setValue(from._coefs[5]);
	comboVariations->setCurrentIndex(from._nonLinear);

	colour = QColor(from._red*255,from._green*255,from._blue*255);
	if (colour.isValid()) {
		colourButton->setText(colour.name());
    colourButton->setPalette(QPalette(colour));
    colourButton->setAutoFillBackground(true);
	}
	connect(colourButton,SIGNAL(clicked()),this,SLOT(changeColour()));
	

}
FunctionTab::operator Function(){
	double coefs[6];
	coefs[0]=doubleSpinBoxXX->value();
	coefs[1]=doubleSpinBoxXY->value();
	coefs[2]=doubleSpinBoxXA->value();
	coefs[3]=doubleSpinBoxYX->value();
	coefs[4]=doubleSpinBoxYY->value();
	coefs[5]=doubleSpinBoxYA->value();

	Function me(coefs,colour.redF(),colour.greenF(),colour.blueF(),comboVariations->currentIndex());
	return me;
}
void FunctionTab::changeColour(){
	colour = QColorDialog::getColor(colour,this);
  if (colour.isValid()) {
     colourButton->setText(colour.name());
     colourButton->setPalette(QPalette(colour));
     colourButton->setAutoFillBackground(true);
	}
}
GeneralOptions::GeneralOptions(fractal *origin){
	//setFixedSize(185,130);
	setupUi(this);
	iterationSlider->setValue(origin->getIterations());
	spinBoxWidth->setValue(origin->getWidth());
	spinBoxHeight->setValue(origin->getHeight());
	functionsCount->setNum(origin->_funcCount);
}
int GeneralOptions::getIterations(){
	return iterationSlider->value();
}
int GeneralOptions::getWidth(){
	return spinBoxWidth->value();
}
int GeneralOptions::getHeight(){
	return spinBoxHeight->value();
}
void GeneralOptions::setFuncCount(int n){
	functionsCount->setNum(n);
}
