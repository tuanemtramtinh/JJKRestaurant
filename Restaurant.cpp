#include "main.h"

class customerOrder{
public:
    Restaurant::customer* data;
    customerOrder* next;
    customerOrder(Restaurant::customer* data, customerOrder* next = nullptr): data(data), next(next){}
    ~customerOrder(){
        delete data;
        delete next;
    }
};

class imp_res : public Restaurant
{
protected:
    customer* TableOrder; //! DanhSachLienKetVong
    customer* CustomerX; //! Khách hàng ở vị trí X
    customer* GuestQueue; //! Hàng chờ
    //! Head và tail cho thứ tự khách hàng
    customerOrder* orderHead;
    customerOrder* orderTail;
    //! Head và tail cho thứ tự khách hàng
    int GuestInQueue;
    int GuestInTable;
public:
    bool CheckDuplicate(customer*); //!"thiên thượng thiên hạ, duy ngã độc tôn"

    /* Hàm sử dụng cho khách ngồi trong bàn ăn */
    void AppendGuestTable(customer*); //!Quăng khách vào bàn
    void RemoveGuestOrder(int);
    /* Hàm sử dụng cho khách ngồi trong bàn ăn */

    /*Hàm sử dụng cho khách nằm trong hàng chờ*/
    void AppendGuestQueue(customer*); //!Quăng khách vào hàng chờ
    customer* PopGuest(); //!Xóa phần tử đầu trong hàng chờ
    /*Hàm sử dụng cho khách nằm trong hàng chờ*/

    /*Hàm sử dụng để kiểm tra thứ tự khách hàng*/

    /*Hàm sử dụng để kiểm tra thứ tự khách hàng*/
public:
    imp_res() {
        TableOrder = nullptr;
        GuestQueue = nullptr;
        GuestInTable = 0;
        GuestInQueue = 0;
        CustomerX = nullptr;
    };
    void RED(string name, int energy)
    {
        cout << name << " " << energy << endl;
        customer *cus = new customer (name, energy, nullptr, nullptr);
        if (cus->energy == 0) return; //! Không phải chú thuật sư hay chú linh
        if (CheckDuplicate(cus)) return; //! "thiên thượng thiên hạ, duy ngã độc tôn"
        if (GuestInTable == MAXSIZE){ //! Thêm khách vào hàng chờ
            if (GuestInQueue == MAXSIZE) return;
            AppendGuestQueue(cus);
            return;
        }
        AppendGuestTable(cus); //! Thêm khách vào bàn ăn
    }
    void BLUE(int num)
    {
        cout << "blue "<< num << endl;
        if (GuestInTable == 0) return;
        if (num >= MAXSIZE || num >= GuestInTable) RemoveGuestOrder(GuestInTable);
        else RemoveGuestOrder(num);
        while (GuestInQueue != 0 && GuestInTable < MAXSIZE){
            customer* FirstGuest = PopGuest();
            AppendGuestTable(FirstGuest);
        }
    }
    void PURPLE()
    {
        cout << "purple"<< endl;
    }
    void REVERSAL()
    {
        cout << "reversal" << endl;
    }
    void UNLIMITED_VOID()
    {
        cout << "unlimited_void" << endl;
    }
    void DOMAIN_EXPANSION()
    {
        cout << "domain_expansion" << endl;
    }
    void LIGHT(int num)
    {
        cout << "light " << num << endl;
        if (num > 0){
            customer* temp = CustomerX;
            for (int i = 0; i < GuestInTable; i++) {
                temp->print();
                temp = temp->next;
            }
        }
        else if (num < 0){
            customer* temp = CustomerX;
            for (int i = 0; i < GuestInTable; i++){
                temp->print();
                temp = temp->prev;
            }
        }
        else{
            customer* temp = GuestQueue;
            for (int i = 0; i < GuestInQueue; i++){
                temp->print();
                temp = temp -> next;
            }
        }
    }
};

bool imp_res::CheckDuplicate(customer* cus) {
    customer* tempTable = TableOrder;
    customer* tempQueue = GuestQueue;
    for (int i = 0; i < GuestInTable; i++){
        if (tempTable -> name == cus -> name) return true;
        tempTable = tempTable -> next;
    }
    for (int i = 0; i < GuestInQueue; i++){
        if (tempQueue -> name == cus -> name) return true;
        tempQueue = tempQueue -> next;
    }
    return false;
}

void imp_res::AppendGuestTable(customer* cus) {
    if (TableOrder == nullptr){ //! Truờng hơợp bàn ăn trống
        TableOrder = cus;
        TableOrder -> next = TableOrder -> prev = TableOrder;
        GuestInTable = 1;
        CustomerX = TableOrder;
        orderHead = orderTail = new customerOrder(CustomerX);
        return;
    }
    //! Trường hợp bàn ăn không trống
    if (GuestInTable >= MAXSIZE / 2.0){
        customer* tempTable = CustomerX;
        int MaxDiff = -999999;
        for (int i = 0; i < GuestInTable; i++){
            if (abs(cus->energy - tempTable->energy) > MaxDiff){
                MaxDiff = abs(cus->energy - tempTable->energy);
                CustomerX = tempTable;
            }
            tempTable = tempTable -> next;
        }
    }
    if (cus->energy >= CustomerX->energy){
        cus -> next = CustomerX -> next;
        CustomerX -> next -> prev = cus;
        CustomerX -> next = cus;
        cus -> prev = CustomerX;
    }
    else{
        CustomerX -> prev -> next = cus;
        cus -> prev = CustomerX -> prev;
        cus -> next = CustomerX;
        CustomerX -> prev = cus;
    }
    CustomerX = cus;
    orderTail -> next = new customerOrder(CustomerX);
    orderTail = orderTail -> next;
    GuestInTable += 1;
}

void imp_res::AppendGuestQueue(customer* cus) {
    if (GuestQueue == nullptr){
        GuestQueue = cus;
        GuestQueue -> next = GuestQueue -> prev = GuestQueue;
        GuestInQueue = 1;
        return;
    }
    GuestQueue -> prev -> next  = cus;
    cus -> prev = GuestQueue -> prev;
    cus -> next = GuestQueue;
    GuestQueue -> prev = cus;
    GuestInQueue += 1;
}

Restaurant::customer* imp_res::PopGuest() {
    customer* tempGuest = nullptr;
    if (GuestInQueue == 0) return nullptr;
    if (GuestInQueue == 1){
        tempGuest = GuestQueue;
        tempGuest -> next = tempGuest -> prev = nullptr;
        GuestQueue = nullptr;
        GuestInQueue = 0;
        return tempGuest;
    }
    tempGuest = GuestQueue;
    GuestQueue = GuestQueue -> next;
    tempGuest -> prev -> next = tempGuest -> next;
    tempGuest -> next -> prev = tempGuest -> prev;
    tempGuest -> next = tempGuest -> prev = nullptr;
    GuestInQueue -= 1;
    return tempGuest;
}

//! BLUE FUNCTION
void imp_res::RemoveGuestOrder(int num) {
    for (int i = 0; i < num; i++){
        customerOrder* tempOrder = orderHead;
        orderHead = orderHead -> next;
        if (tempOrder -> data == TableOrder && tempOrder -> next != nullptr) TableOrder = tempOrder -> next -> data;
        if (tempOrder -> data -> energy > 0) CustomerX = tempOrder -> data -> next;
        else CustomerX = tempOrder -> data -> prev;
        tempOrder -> data -> prev -> next = tempOrder -> data -> next;
        tempOrder -> data -> next -> prev = tempOrder -> data -> prev;
        tempOrder -> data -> next = tempOrder -> data -> prev = nullptr;
        tempOrder -> next = nullptr;
        delete tempOrder;
        GuestInTable -= 1;
        if (GuestInTable == 0){
            CustomerX = TableOrder = nullptr;
            orderHead = orderTail = nullptr;
        }
    }
}
//! BLUE FUNCTION