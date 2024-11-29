#ifndef CANOPENWIN_H
#define CANOPENWIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CanOpenWin; }
QT_END_NAMESPACE


class QGridLayout;
class SLCanOpenNode;
class CoValuesHolder;
class SDOValue;
class SDOValuePlot;
class TrendPlotEditDlg;
class SignalCurveEditDlg;


class CanOpenWin : public QMainWindow
{
    Q_OBJECT

public:
    CanOpenWin(QWidget *parent = nullptr);
    ~CanOpenWin();

private slots:
    void on_actQuit_triggered(bool checked);
    void on_actDebugExec_triggered(bool checked);
    void on_actConnect_triggered(bool checked);
    void on_actDisconnect_triggered(bool checked);

    void CANopen_connected();
    void CANopen_disconnected();
private:
    Ui::CanOpenWin *ui;
    SDOValuePlot* m_plot;
    SLCanOpenNode* m_slcon;
    CoValuesHolder* m_valsHolder;
    QGridLayout* m_layout;

    int m_updIntervalms;

    TrendPlotEditDlg* m_trendDlg;
    SignalCurveEditDlg* m_signalCurveEditDlg;
};
#endif // CANOPENWIN_H
