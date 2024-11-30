#include "sdovalueplot.h"
#include "sdovalue.h"
#include <algorithm>
#include <QDebug>



SDOValuePlot::SDOValuePlot(const QString& newName, CoValuesHolder* valsHolder, QWidget* parent)
    :SignalPlot(newName, parent)
{
    m_valsHolder = nullptr;
    setValuesHolder(valsHolder);
}

SDOValuePlot::~SDOValuePlot()
{
    if(m_valsHolder){
        for(auto& valData: m_sdoValues){
            m_valsHolder->delSdoValue(valData.sdoval);
        }
        m_sdoValues.clear();
    }
}

CoValuesHolder* SDOValuePlot::valuesHolder() const
{
    return m_valsHolder;
}

void SDOValuePlot::setValuesHolder(CoValuesHolder* newValuesHolder)
{
    if(m_valsHolder){
        disconnect(m_valsHolder, &CoValuesHolder::updateBegin, this, &SDOValuePlot::sdovalsUpdating);
    }
    m_valsHolder = newValuesHolder;
    if(m_valsHolder){
        connect(newValuesHolder, &CoValuesHolder::updateBegin, this, &SDOValuePlot::sdovalsUpdating);
    }
}

bool SDOValuePlot::addSDOValue(CO::NodeId newNodeId, CO::Index newIndex, CO::SubIndex newSubIndex, COValue::Type type,
                               const QString& newName, const QColor& newColor, const qreal& z)
{
    if(m_valsHolder == nullptr) return false;

    size_t typeSize = COValue::typeSize(type);
    if(typeSize == 0) return false;

    int signal_num = addSignal(newName, newColor, z);
    if(signal_num == -1) return false;

    auto sdoValPtr = m_valsHolder->addSdoValue(newNodeId, newIndex, newSubIndex, typeSize);

    if(sdoValPtr == nullptr){
        removeSignal(signal_num);
        return false;
    }

    m_sdoValues.append({sdoValPtr, true, type, signal_num, QElapsedTimer()});

    connect(sdoValPtr, &SDOValue::readed, this, &SDOValuePlot::sdovalueReaded);

    return true;
}

int SDOValuePlot::SDOValuesCount() const
{
    return m_sdoValues.size();
}

CoValuesHolder::HoldedSDOValuePtr SDOValuePlot::SDOValue(int n) const
{
    if(n < 0 || n >= m_sdoValues.size()) return nullptr;

    return m_sdoValues[n].sdoval;
}

void SDOValuePlot::delSDOValue(CoValuesHolder::HoldedSDOValuePtr sdoval)
{
    if(auto it = std::find_if(m_sdoValues.begin(), m_sdoValues.end(), [sdoval](const auto& val){ return val.sdoval == sdoval; }); it != m_sdoValues.end()){
        m_valsHolder->delSdoValue(it->sdoval);
        m_sdoValues.erase(it);
    }
}

COValue::Type SDOValuePlot::SDValueType(int n) const
{
    if(n < 0 || n >= m_sdoValues.size()) return COValue::Type();

    return m_sdoValues[n].type;
}

int SDOValuePlot::SDOValueSignalNumber(int n) const
{
    if(n < 0 || n >= m_sdoValues.size()) return -1;

    return m_sdoValues[n].signal_num;
}

void SDOValuePlot::sdovalueReaded()
{
    auto sdoval = qobject_cast<CoValuesHolder::HoldedSDOValuePtr>(sender());
    if(sdoval == nullptr) return;

    int n = 0;
    auto it = m_sdoValues.begin();
    for(; it != m_sdoValues.end(); ++ it, n ++){
        if(it->sdoval == sdoval) break;
    }

    if(it != m_sdoValues.end()){
        putValue(&(*it));
        it->elapsedTimer.start();
        it->readed = true;
    }
}

void SDOValuePlot::sdovalsUpdating()
{
    //qDebug() << "sdovalsUpdating";
    for(auto it = m_sdoValues.begin(); it != m_sdoValues.end(); ++ it){
        if(!it->readed){
            putValue(&(*it));
            it->elapsedTimer.start();
        }
        it->readed = false;
    }

    replot();
}

void SDOValuePlot::putValue(SDOValItem* sdoItem)
{
    qreal dt = 0.0;
    if(sdoItem->elapsedTimer.isValid()) dt = static_cast<qreal>(sdoItem->elapsedTimer.elapsed()) / 1000;
    putSample(sdoItem->signal_num, COValue::valueAs<qreal>(sdoItem->sdoval->data(), sdoItem->type, 0.0), dt);
}
