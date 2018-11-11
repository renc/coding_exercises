// Test 2, answer from canjiang.ren@outlook.com 

// These assumptions are made:
// 1. There is a client which communicate with market/exchange server.
// For example this client has a trader api to send requests and get responds from the server, 
// when the client do that, it invoke the Listener to do order tracking. 
//
// 2. Requests and responds are asynchronous, synchronization is needed, 
// however the test donot suggest whether synchronization is done at client before invoking these callbacks
// or synchronization should be done by derived Listener, here the second case is used.
// 
// 3. Part A. X, Y are not changed for one instance object. They could be different values between objects.
// 

// Result:
// Part A. Operation 1. finished. 
//         Operation 2. donot finish since not understand this operation.
// Part B. All operations are finished.

#include <iostream> 
#include <assert.h>
#include <thread>
#include <queue>
#include <memory>
#include <deque>
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
        tRequestAcknowledged, tRequestRejectd, tOrderFilled 
}; 
class BasicMsg {
public:
    virtual MsgType msgType() const = 0; 
};
class MsgInsertOrderRequest : public BasicMsg {
public: 
    MsgType msgType() const { return MsgType::tInsertOrderRequest;}
    int m_id = -1;      // should be private data memeber, and public getXX and setXX(), 
    char m_cSide = 'U'; // unknown
    double m_fPrice; 
    int m_iQuantity;
};
class MsgReplaceOrderRequest : public BasicMsg {
public: 
    MsgType msgType() const { return MsgType::tReplaceOrderRequest;}
    int m_iOldId = -1; 
    int m_iNewId = -1;
    int m_iDeltaQuantity = 0; 
};
class MsgRequestAcknowledged: public BasicMsg {
public:
    MsgType msgType() const { return MsgType::tRequestAcknowledged; }
    int m_iId = -1;
};
class MsgRequestRejected: public BasicMsg {
public:
    MsgType msgType() const { return MsgType::tRequestRejectd; }
    int m_iId = -1;
};
class MsgOrderFilled: public BasicMsg {
public: 
    MsgType msgType() const { return MsgType::tOrderFilled; }
    int m_iId = -1;
    int m_iQuantityFilled = -1;
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
    double m_fPrice = -1;
    int m_iQuantity = -1; 
    OrderStatus m_orderStatus = OrderStatus::sInit;
    std::chrono::system_clock::time_point m_requestTime;
};

class DerivedListener : public Listener
{
public:
    DerivedListener(int iX, int iY);
    ~DerivedListener();

    // Interfaces fo Listener 
    void OnInsertOrderRequest(int id, char side, double price, int quantity) override;
    void OnReplaceOrderRequest ( int oldId, int newId, int deltaQuantity) override;     
    void OnRequestAcknowledged( int id) override;
    void OnRequestRejected( int id)override;
    void OnOrderFilled( int id, int quantityFilled) override;
    
    void processMessage();
private:
    int m_iXNumRequest = -1; // from ctor,
    int m_iYNumSeconds = -1; // from ctor, 
    std::thread m_thread; // thread to processMessage
    std::queue<std::shared_ptr<BasicMsg> > m_msgQueue;
    std::deque<OrderInfo> m_aOrderInfo;
};

DerivedListener::DerivedListener(int iX, int iY) 
    : m_iXNumRequest(iX), m_iYNumSeconds(iY)
{
    m_thread = std::thread(&DerivedListener::processMessage, this);
}
DerivedListener::~DerivedListener()
{
    m_thread.join();
}
void DerivedListener::OnInsertOrderRequest(int id, char side, double price, int quantity)
{
    std::shared_ptr<MsgInsertOrderRequest> pMsg = std::make_shared<MsgInsertOrderRequest>();
    pMsg->m_id = id;
    pMsg->m_cSide = side;
    pMsg->m_fPrice = price;
    pMsg->m_iQuantity = quantity;
    m_msgQueue.push(pMsg);
}
void DerivedListener::OnReplaceOrderRequest(int oldId, int newId, int deltaQuantity)
{
    std::shared_ptr<MsgReplaceOrderRequest> pMsg = std::make_shared<MsgReplaceOrderRequest>();
    pMsg->m_iOldId = oldId;
    pMsg->m_iNewId = newId;
    pMsg->m_iDeltaQuantity = deltaQuantity;
    m_msgQueue.push(pMsg);
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
void DerivedListener::processMessage()
{
    // this function runs in a thread to handle messages.
    while (!m_msgQueue.empty())
    {
        std::shared_ptr<BasicMsg> pMsg = m_msgQueue.front();
        if (pMsg->msgType() == MsgType::tInsertOrderRequest)
        {
            std::shared_ptr<MsgInsertOrderRequest> pM = std::dynamic_pointer_cast<MsgInsertOrderRequest>(pMsg);
        }
    }
}