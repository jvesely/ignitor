#ifndef _FRACTAL_H_
#define _FRACTAL_H_
#include <QWidget>
#include <QVector>
#include <QThread>
#include "function.h"

class engineThread:public QThread{

		Q_OBJECT

		bool _abort;
		int _width;
		int _height;
		int _i;
		int _iterations;
		int _funcCount;
		double * red;
		double * green;
		double * blue;
		int * hits;
		QVector<Function> _functions;
	

		void put(const double x, const double y, const double r, const double g, const double b);
	public:
		engineThread(const int width, const int height, const int iterations,
									const QVector<Function> &functions);
		~engineThread();
		void run();
		int getWidth() const;
		int getHeight() const;
		QColor get(const int pos) const;
		void sendStatus();
	signals:
		void nextIteration(int min, int n, int max);
		void instantPreview();

};
class fractal:public QWidget{
		friend class OptionDialog;
		friend class GeneralOptions;
		
		Q_OBJECT

		QImage *picture;
		QVector<Function> _functions;
		engineThread *engine;


		double max;
		int _width;
		int _height;
		int _iterations;
		int _funcCount;

		bool rendered;
		bool _active;
		bool _running;
		
		void addFunction();
		void genFunctions();
		void delFunc();
		void addFunc(const Function & func);
		QDomElement toXML(QDomDocument &doc) const;
		bool fromXML(const QDomDocument &func);
	protected:
		void paintEvent(QPaintEvent * event);


	signals:
		void setMin(int min);
		void setMax(int max);
		void setCurrent(int cur);
		void changed();
	public slots:
		void paint();
		void stop();
		void pointIterated(int min, int n, int max);
		void activated(fractal * active);
		void engineDied();
	public:
		fractal(const int width = 1024,const int height = 768);
		~fractal();
		void setResolution(const int fWidth =1024,const int fHeight = 768);
		bool render();
		bool display();
		void write(const QString &text);
		bool loadXML(const QString &fileName);
		void saveXML(const QString &fileName) const;
		bool savePict(const QString &fileName) const;
		void setFuncCount(int max);
		void setIterations(int num);
		void sendStatus();
		bool getRendered();
		bool isRunning();
		int getIterations();
		int getWidth();
		int getHeight();
		


};
#endif
