// Test 2 from Eclipse Trading online test, answer from xx@outlook.com 
// First, thx for sharing this test, really interesting. 

// These assumptions are made:
// 1. There is a client which communicate with market/exchange server.
// For example this client has a trader api to send requests and get responds from the server, 
// when the client do that, it invoke the Listener to do order tracking. 
//
// 2. Requests and responds are asynchronous, synchronization is needed, 
// however this Test donot suggest whether synchronization is done at client before invoking these callbacks
// or synchronization should be done by derived Listener, here the second case is used.
//  
// 

// Result:
// Part A. (1). finished. Please check function DerivedListener::hasEnoughXRequestsForLastYSeconds() for details. 
//         (2). The exact time depends, for example I try to invoke DerivedListener::hasEnoughXRequestsForLastYSeconds()
//              and get times like: 
//              Test 1 took time: 0.000249115s. Test 1 took time: 0.000371103s.
// Part B. All operations are finished.

#include <iostream> 
#include <assert.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
//#include <boost/lockfree/queue.hpp>
#include <memory>
#include <deque>
#include <unordered_map>
#include <chrono>

class Listener
{
public:
    // These two callbacks represent client requents. 

    // Indicates the client has sent a new order request to the market. Exactly one 
    // callback will follow:
    // * 'OnRequestAcknowledged', in which case order 'id' is active in the market; or
    // * 'OnRequestRejected', in which case the order was never active in the market. 
    virtual void OnInsertOrderRequest(
        int id,
        char side, // 'B' for bid, 'O' ofr offer 
        double price, 
        int quantity) = 0;
    
    // Indicates the client has sent a request to change the quantity of an order. 
    // Exactly one callback will follow:
    // * 'OnRequestAcknowledged', in which case the order quantity was modified and the 
    //  order is now tracked by ID 'newId'; or
    // * 'OnRequestRejected', in which case the order was not modified and remains 
    //  tracked by ID 'oldId'.
    virtual void OnReplaceOrderRequest (
        int oldId,                  // The existing order to modify 
        int newId,                  // The new order ID to use if the modification succeeds
        int deltaQuantity) = 0;     // How much the quantity should be increased/decreased
    

    // These three callbacks represent market confirmations.

    // Indicates the insert or modify request was accepted.
    virtual void OnRequestAcknowledged(
        int id) = 0;

    // Indicates the insert or modify request was rejected.
    virtual void OnRequestRejected(
        int id) = 0;

    // Indicates that the order quantity was reduced (and filled) by 'quantityFilled'.
    virtual void OnOrderFilled(
        int id, 
        int quantityFilled) = 0;
     
};

// Different messages are put into a queue,
// one message type for one callback. 
enum class MsgType {
        tInsertOrderRequest, tReplaceOrderRequest, 
        tRequestAcknowledged, tRequestRejectd, tOrderFilled, 
        tHasXRequestForLastYSeconds
}; 
class BasicMsg {
public:
    virtual MsgType msgType() const = 0; 
};
class MsgInsertOrderRequest : public BasicMsg {
public: 
    MsgType msgType() const override { return MsgType::tInsertOrderRequest;}
    int m_id = -1;      // should be private data memeber, and public getXX and setXX(), 
    char m_cSide = 'U'; // unknown
    double m_fPrice; 
    int m_iQuantity;
    std::chrono::steady_clock::time_point m_requestTime;
};
class MsgReplaceOrderRequest : public BasicMsg {
public: 
    MsgType msgType() const override { return MsgType::tReplaceOrderRequest;}
    int m_iOldId = -1; 
    int m_iNewId = -1;
    int m_iDeltaQuantity = 0; 
    std::chrono::steady_clock::time_point m_requestTime;
};
class MsgRequestAcknowledged: public BasicMsg {
public:
    MsgType msgType() const override { return MsgType::tRequestAcknowledged; }
    int m_iId = -1;
};
class MsgRequestRejected: public BasicMsg {
public:
    MsgType msgType() const override { return MsgType::tRequestRejectd; }
    int m_iId = -1;
};
class MsgOrderFilled: public BasicMsg {
public: 
    MsgType msgType() const override { return MsgType::tOrderFilled; }
    int m_iId = -1;
    int m_iQuantityFilled = -1;
};
class MsgHasXRequestForLastYSeconds : public BasicMsg 
{
public:
    MsgType msgType() const override { return MsgType::tHasXRequestForLastYSeconds; }
    int m_iXNumRequest = -1;
    int m_iYNumSeconds = -1;
    std::chrono::steady_clock::time_point m_requestOperationTime;
    int m_iQueryId = -1;
};
// to keep track of the order status    
enum class OrderStatus {
    sInit, sSubmited, sAcknowledged, sRejected, sPartialFilled, sAllFilled 
};
class OrderInfo
{
public:
    OrderInfo(int id, char side, double price, int quantity) 
        : m_iId(id), m_cSide(side), m_fPrice(price), m_iQuantity(quantity) {}
//private: 
    int m_iId = -1;
    char m_cSide = 'U'; // unknown
    double m_fPrice = 0;
    int m_iQuantity = 0; 
    OrderStatus m_orderStatus = OrderStatus::sInit;
    std::chrono::steady_clock::time_point m_requestTime;

    int m_iQuantityFilled = 0; // use to calc the number of quantities are not filled yet. 
    int m_iOldId = -1, m_iNewId = -1; // used to link the original insert request and the replace request.
};


template <typename T>
class QueueCV {
public:
    void push(const T& n)
    {
        {
            std::lock_guard<std::mutex> lk(m_mtx);
            m_queue.push(n);
        } // Important, unlock before notify, otherwise, 
        // the waiting thread wait up before the mutx is unlock.
        m_cv.notify_all();
        // vs notify_one() ? multiply customers.
    }
    // T pop() // i use this function signature before, but change to
    // Using a reference parameter to receive the result is used to transfer ownership
    //  out of the queue in order to avoid the exception safety issues 
    // of returning data by-value: if the copy constructor of a by-value return throws, 
    // then the data has been removed from the queue, but is lost, 
    // whereas with this approach, the potentially problematic copy is performed 
    // prior to modifying the queue (see Herb Sutter's Guru Of The Week #8 
    // for a discussion of the issues). 
    void pop(T &destination)
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        m_cv.wait(lk, [this](){ return m_queue.empty() == false;});
        // same as
        // while ( ! (q.empty() == false) ) 
        //     m_cv.wait(lk);
        //
        // condition variables can be subject to spurious wake-ups, 
        // so it is important to check the actual condition being waited for 
        // when the call to wait returns.
        // 
        // if we are here, q is not empty and mtx is locked.
        
        //T iRet = m_queue.front();
        destination = m_queue.front();
        m_queue.pop();   
        //return iRet;     
        // this could be blocking (not no-blocking)
    }
    T& front() {
        // why return type is reference ? 
        // https://en.cppreference.com/w/cpp/container/queue/front
        std::lock_guard<std::mutex> lk(m_mtx);
        return m_queue.front();
    } 
    // const T& front() const {
    //     std::lock_guard<std::mutex> lk(m_mtx);
    //     return m_queue.front();
    // }
    // renc, why to remove front() and pop() for multiply customers ?
    // tbb concurrent_queue does not have front() and pop() functions.
    // https://www.threadingbuildingblocks.org/tutorial-intel-tbb-concurrent-containers 
    // https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
    // 
    bool try_pop(T &destination) {
        std::lock_guard<std::mutex> lk(m_mtx);
        if (m_queue.empty())
            return false;

        destination = m_queue.front();
        m_queue.pop();  
        return true;
    }
    bool empty()  {
        std::lock_guard<std::mutex> lk{m_mtx};
        return m_queue.empty();
    }
    int size()  {
        std::lock_guard<std::mutex> lk{m_mtx};
        return m_queue.size();
    }
private:
    std::mutex m_mtx;
    std::condition_variable m_cv;

    std::queue<T> m_queue; // un-bound
};

class DerivedListener : public Listener
{
public:
    DerivedListener(int iXNumRequest, int iYNumSeconds);
    ~DerivedListener();

    // Note, call this before attach this listener to receive request. 
    void startThreadProcessMessage(); 
    void processMessage();

    // Interfaces fo Listener 
    void OnInsertOrderRequest(int id, char side, double price, int quantity) override;
    void OnReplaceOrderRequest ( int oldId, int newId, int deltaQuantity) override;     
    void OnRequestAcknowledged( int id) override;
    void OnRequestRejected( int id)override;
    void OnOrderFilled( int id, int quantityFilled) override;
    
    // Interfaces for PartA
    // has more than x requrest been received in the last y seconds? 
    bool hasEnoughXRequestsForLastYSeconds() ; // (x,y) given in ctor.
    bool hasEnoughXRequestsForLastYSeconds(int iXNumRequest, double iYNumSeconds) ;

private:
    int m_iXNumRequest = -1; // from ctor,
    int m_iYNumSeconds = -1; // from ctor, 
    
    std::thread m_thread; // thread to processMessage
    std::atomic_bool m_bStopThread{false};
    //std::queue<std::shared_ptr<BasicMsg> > m_msgQueue;
    QueueCV<std::shared_ptr<BasicMsg> > m_msgQueue;
    
    std::deque<OrderInfo> m_aOrderInfo;
    std::unordered_map<int, int> m_mapOrderId2Idx; //map the order id to index of m_aOrderInfo.

    std::atomic_bool m_bHasEnoughXRequestForLastYSecond{ false};
    std::atomic<int> m_bFinishOperationHasEnoughXRequestForLastYSecond { -1};
    std::mutex m_mtxXRequestYSecond;
    std::condition_variable m_cvXRequestYSecond;


};

DerivedListener::DerivedListener(int iXNumRequest, int iYNumSeconds) 
    : m_iXNumRequest(iXNumRequest), m_iYNumSeconds(iYNumSeconds)
{ 
}
DerivedListener::~DerivedListener()
{ 
    std::cout << "Dtor.\n";
    if (m_bStopThread == false)
        m_bStopThread = true;
    m_thread.join();
}
void DerivedListener::OnInsertOrderRequest(int id, char side, double price, int quantity)
{
    std::shared_ptr<MsgInsertOrderRequest> pMsg = std::make_shared<MsgInsertOrderRequest>();
    pMsg->m_id = id;
    pMsg->m_cSide = side;
    pMsg->m_fPrice = price;
    pMsg->m_iQuantity = quantity;
    pMsg->m_requestTime = std::chrono::steady_clock::now();
    m_msgQueue.push(pMsg);
    std::cout << "IOR, qsize " << m_msgQueue.size() << std::endl;
}
void DerivedListener::OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity)
{
    std::shared_ptr<MsgReplaceOrderRequest> pMsg = std::make_shared<MsgReplaceOrderRequest>();
    pMsg->m_iOldId = oldId;
    pMsg->m_iNewId = newId;
    pMsg->m_iDeltaQuantity = deltaQuantity;
    pMsg->m_requestTime = std::chrono::steady_clock::now();
    m_msgQueue.push(pMsg);
    std::cout << "ROR, qsize " << m_msgQueue.size() << std::endl;
}
void DerivedListener::OnRequestAcknowledged(int id)
{
    std::shared_ptr<MsgRequestAcknowledged> pMsg = std::make_shared<MsgRequestAcknowledged>();
    pMsg->m_iId = id;
    m_msgQueue.push(pMsg);
}
void DerivedListener::OnRequestRejected(int id) 
{
    std::shared_ptr<MsgRequestRejected> pMsg = std::make_shared<MsgRequestRejected>();
    pMsg->m_iId = id;
    m_msgQueue.push(pMsg);
}
void DerivedListener::OnOrderFilled(int id, int quantityFilled) 
{
    std::shared_ptr<MsgOrderFilled> pMsg = std::make_shared<MsgOrderFilled>();
    pMsg->m_iId = id;
    pMsg->m_iQuantityFilled = quantityFilled;
    m_msgQueue.push(pMsg);
}
void DerivedListener::startThreadProcessMessage()
{
    // when this function is invoke, "this" object should been constructed. 
    m_thread = std::thread(&DerivedListener::processMessage, this);
}
void DerivedListener::processMessage()
{
    // this function runs in a thread to handle messages.
    std::cout << "processMsg start.\n";
 
    while (!m_bStopThread)
    {
        //// std::queue
        // if (m_msgQueue.empty()) {
        //     //std::cout << ".q0.";
        //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // }
        // else 
        // {
        //     std::shared_ptr<BasicMsg> pMsg = m_msgQueue.front();
        //     m_msgQueue.pop(); 
        
        std::shared_ptr<BasicMsg> pMsg;
        if (m_msgQueue.try_pop(pMsg)== false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } else {
            if (pMsg->msgType() == MsgType::tInsertOrderRequest)
            {
                std::shared_ptr<MsgInsertOrderRequest> pM = std::dynamic_pointer_cast<MsgInsertOrderRequest>(pMsg);
                OrderInfo oi(pM->m_id, pM->m_cSide, pM->m_fPrice, pM->m_iQuantity);
                oi.m_requestTime = pM->m_requestTime;
                oi.m_orderStatus = OrderStatus::sSubmited;

                m_mapOrderId2Idx[oi.m_iId] = m_aOrderInfo.size();
                m_aOrderInfo.push_back(oi);
                std::cout << "Insert oi: " << oi.m_iId << ", at[" << m_aOrderInfo.size() -1 <<"]." << std::endl;
            }   
            else if (pMsg->msgType() == MsgType::tReplaceOrderRequest)
            {
                std::shared_ptr<MsgReplaceOrderRequest> pM = std::dynamic_pointer_cast<MsgReplaceOrderRequest>(pMsg);
                assert(pM->m_iOldId != -1);
                if (m_mapOrderId2Idx.find(pM->m_iOldId) != m_mapOrderId2Idx.end())
                {
                    assert(m_mapOrderId2Idx[pM->m_iOldId] < m_aOrderInfo.size());
                    OrderInfo &oldOI = m_aOrderInfo[m_mapOrderId2Idx[pM->m_iOldId]];
    
                    OrderInfo newOI(pM->m_iNewId, oldOI.m_cSide, oldOI.m_fPrice, oldOI.m_iQuantity - oldOI.m_iQuantityFilled + pM->m_iDeltaQuantity);
                    newOI.m_requestTime = pM->m_requestTime;
                    newOI.m_orderStatus = OrderStatus::sSubmited;
                    newOI.m_iOldId = pM->m_iOldId;
                    oldOI.m_iNewId = pM->m_iNewId;

                    m_mapOrderId2Idx[newOI.m_iId] = m_aOrderInfo.size();
                    m_aOrderInfo.push_back(newOI); 
                    std::cout << "Replace oi: " << newOI.m_iId << ", at[" << m_aOrderInfo.size() -1 <<"]." << std::endl;
                } else {
                    // todo: something goes wrong
                }
            }
            else if (pMsg->msgType() == MsgType::tHasXRequestForLastYSeconds)
            {
                m_bFinishOperationHasEnoughXRequestForLastYSecond = -1;
                m_bHasEnoughXRequestForLastYSecond = false;

                std::shared_ptr<MsgHasXRequestForLastYSeconds> pM = std::dynamic_pointer_cast<MsgHasXRequestForLastYSeconds>(pMsg);

                const int iOrderRequestCount = m_aOrderInfo.size();
                std::cout << "hasXY(X"<< pM->m_iXNumRequest << ", Y" << pM->m_iYNumSeconds <<"), oi size " << iOrderRequestCount << std::endl;
                if (iOrderRequestCount < pM->m_iXNumRequest)
                    m_bHasEnoughXRequestForLastYSecond = false;

                const OrderInfo &oi = m_aOrderInfo[iOrderRequestCount - pM->m_iXNumRequest];
                std::chrono::duration<double> tPastTimeDurtation = pM->m_requestOperationTime - oi.m_requestTime;
                double fPastTime = tPastTimeDurtation.count();
                std::cout << "Past time: " << fPastTime << std::endl;
                if (fPastTime < pM->m_iYNumSeconds)
                    m_bHasEnoughXRequestForLastYSecond = true;

                m_bFinishOperationHasEnoughXRequestForLastYSecond = pM->m_iQueryId;
                m_cvXRequestYSecond.notify_all();
            }
        }
    } // end of while 
    std::cout << "processMsg end.\n";
}

bool DerivedListener::hasEnoughXRequestsForLastYSeconds()  
{
    return hasEnoughXRequestsForLastYSeconds(m_iXNumRequest, m_iYNumSeconds);
}
bool DerivedListener::hasEnoughXRequestsForLastYSeconds(int iXNumRequest, double iYNumSeconds)  
{ 
    static int iQueryId = 0;
    
    std::shared_ptr<MsgHasXRequestForLastYSeconds> pMsg = std::make_shared<MsgHasXRequestForLastYSeconds>();
    pMsg->m_iXNumRequest = iXNumRequest;
    pMsg->m_iYNumSeconds = iYNumSeconds;
    pMsg->m_requestOperationTime = std::chrono::steady_clock::now();
    pMsg->m_iQueryId = iQueryId++; // use the query id to support multi calls of this func at the similar time. 
    m_msgQueue.push(pMsg);
    std::cout << "Insert xy query, " << pMsg->m_iQueryId << ".\n";
    if (iQueryId > INT32_MAX - 1)
        iQueryId = 0; 

    // wait until the calc finish
	std::unique_lock<std::mutex> lk(m_mtxXRequestYSecond);
	//m_cvXRequestYSecond.wait(lk, [&] { return m_bFinishOperationHasEnoughXRequestForLastYSecond == pMsg->m_iQueryId; }); 
    while (m_bFinishOperationHasEnoughXRequestForLastYSecond != pMsg->m_iQueryId)
    {
        m_cvXRequestYSecond.wait(lk);
    }
    m_bFinishOperationHasEnoughXRequestForLastYSecond = -1; // Note: this
		    
    return m_bHasEnoughXRequestForLastYSecond;
}

int main()
{
    DerivedListener *pListener = (new DerivedListener(10, 2));
    pListener->startThreadProcessMessage();
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // Test for Part A.
    {
    
        for (int i = 0; i < 11; ++i)
            pListener->OnInsertOrderRequest(i, 'B', 1.0, 1);

        auto tStartTest1 = std::chrono::steady_clock::now();
        bool bTest1Result = pListener->hasEnoughXRequestsForLastYSeconds(); // Answer of Part A (1). 
        auto tEndTest1 = std::chrono::steady_clock::now();
        if (bTest1Result == false)
            std::cout << "Test fail 1.\n";
        else
            std::cout << "Test ok 1.\n";
        // Answer of Part A (2).  Test 1 took time: 0.000249115s. Test 1 took time: 0.000371103s.
        std::cout << "Test 1 took time: " << std::chrono::duration<double>(tEndTest1 - tStartTest1).count() << "s.\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        if (pListener->hasEnoughXRequestsForLastYSeconds() == false)
            std::cout << "Test ok 2.\n";
        else    
            std::cout << "test fail 2.\n";
        if (pListener->hasEnoughXRequestsForLastYSeconds() == false)
            std::cout << "Test ok 3.\n";
        else    
            std::cout << "test fail 3.\n";
    }
    // Test for Part B. 
    {

    }
    return 0;
}