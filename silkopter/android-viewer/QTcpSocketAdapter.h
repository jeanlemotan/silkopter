#pragma once

#include <mutex>
#include <vector>
#include <memory>
#include <deque>

#include <QObject>
#include <QTcpSocket>

class QTcpSocketAdapter : public QObject
{
    Q_OBJECT

    typedef std::vector<uint8_t> TX_Buffer_t;
    typedef std::vector<uint8_t> RX_Buffer_t;
public:
    QTcpSocketAdapter()
        : m_socket(new QTcpSocket(this))
    {
    }

    QTcpSocket& getSocket()
    {
        return *m_socket;
    }
    QTcpSocket const& getSocket() const
    {
        return *m_socket;
    }

    void start()
    {
        QObject::connect(m_socket.get(), &QIODevice::readyRead, this, &QTcpSocketAdapter::handleReceive);
        QObject::connect(m_socket.get(), &QIODevice::bytesWritten, this, &QTcpSocketAdapter::handleSend);
    }

    template<class Container>
    void read(Container& dst)
    {
        std::lock_guard<std::mutex> lg(m_rxMutex);
        size_t off = dst.size();
        dst.resize(off + m_rxBuffer.size());
        std::copy(m_rxBuffer.begin(), m_rxBuffer.end(), dst.begin() + off);
        m_rxBuffer.clear();
    }
    void write(void const* data, size_t size)
    {
        if (size)
        {
            std::shared_ptr<TX_Buffer_t> bufferPtr = getTxBuffer();
            bufferPtr->resize(size);
            std::copy(reinterpret_cast<uint8_t const*>(data), reinterpret_cast<uint8_t const*>(data) + size, bufferPtr->begin());

            {
                std::lock_guard<std::mutex> lg(m_txBufferQueueMutex);
                m_txBufferQueue.push_back(bufferPtr);
            }

            if (m_isSending.exchange(true) == false)
            {
                m_socket->write(reinterpret_cast<char const*>(data), size);
            }
        }
    }

public Q_SLOTS:
    void handleSend(uint64_t /*bytes*/)
    {
        std::shared_ptr<TX_Buffer_t> bufferForPoolPtr;

        {
            std::lock_guard<std::mutex> lg(m_txBufferQueueMutex);
            bufferForPoolPtr = m_txBufferQueue.front();
            m_txBufferQueue.pop_front();

            if (!m_txBufferQueue.empty())
            {
                std::shared_ptr<TX_Buffer_t> bufferPtr = m_txBufferQueue.front();
                m_socket->write(reinterpret_cast<char const*>(bufferPtr->data()), bufferPtr->size());
            }
            else
            {
                m_isSending.exchange(false);
            }
        }

        {
            std::lock_guard<std::mutex> lg(m_txBufferPoolMutex);
            m_txBufferPool.push_back(bufferForPoolPtr);
        }
    }

    void handleReceive()
    {
        uint64_t bytes = m_socket->bytesAvailable();
        if (bytes > 0)
        {
            std::lock_guard<std::mutex> lg(m_rxMutex);
            size_t offset = m_rxBuffer.size();
            m_rxBuffer.resize(offset + bytes);
            int64_t r = m_socket->read(reinterpret_cast<char*>(m_rxBuffer.data()) + offset, bytes);
            if (r >= 0)
            {
                m_rxBuffer.resize(offset + r);
            }
            else
            {
                m_rxBuffer.resize(offset);
            }
        }
    }

private:
    std::shared_ptr<TX_Buffer_t> getTxBuffer()
    {
        std::lock_guard<std::mutex> lg(m_txBufferPoolMutex);
        std::shared_ptr<TX_Buffer_t> buffer_ptr;
        if (!m_txBufferPool.empty())
        {
            buffer_ptr = m_txBufferPool.back();
            m_txBufferPool.pop_back();
        }
        else
        {
            buffer_ptr = std::make_shared<TX_Buffer_t>();
        }
        return buffer_ptr;
    }

    std::unique_ptr<QTcpSocket> m_socket;
    RX_Buffer_t m_rxBuffer;
    std::mutex m_rxMutex;

    std::atomic_bool m_isSending = { false };
    std::deque<std::shared_ptr<TX_Buffer_t>> m_txBufferQueue;
    std::mutex m_txBufferQueueMutex;

    std::vector<std::shared_ptr<TX_Buffer_t>> m_txBufferPool;
    std::mutex m_txBufferPoolMutex;
};
