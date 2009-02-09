#ifndef MESHLOG_H
#define MESHLOG_H

#include <QtGui>
#include "../Core/MeshCore.h"

class MeshLog : public QListWidget
{
    Q_OBJECT

public:
    MeshLog(QWidget *parent = 0);
    ~MeshLog();

private:
	bool filter(ELogSeverity severity, const char* file, const char* function, int line);
	void output(ELogSeverity severity, const char* function, const char* msg);

	static Boolean LogFilter(ELogSeverity severity, const char* file, const char* function, int line, void* pUserArg);
	static void LogOutput(ELogSeverity severity, const char* function, const char* msg, void* pUserArg);
};

#endif // MESHLOG_H
