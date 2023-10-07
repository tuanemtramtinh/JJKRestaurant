#include "main.h"

class customerOrder{
public:
    Restaurant::customer* data;
    customerOrder* next;
    customerOrder* prev;
    bool QueueCheck;
    customerOrder(Restaurant::customer* data, bool QueueCheck = false, customerOrder* next = nullptr, customerOrder* prev = nullptr):
            data(data), QueueCheck(QueueCheck), next(next), prev(prev){}
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
    void RemoveGuestOrder(int, customerOrder*&, customerOrder*&);
    /* Hàm sử dụng cho khách ngồi trong bàn ăn */

    /*Hàm sử dụng cho khách nằm trong hàng chờ*/
    void AppendGuestQueue(customer*); //!Quăng khách vào hàng chờ
    customer* PopGuest(); //!Xóa phần tử đầu trong hàng chờ
    customer* PopGuest(customer*);
    /*Hàm sử dụng cho khách nằm trong hàng chờ*/

    /*Hàm sử dụng để kiểm tra thứ tự khách hàng*/
    void AppendOrder(customerOrder*&, customerOrder*&, customer* );
    void ClearOrder(customerOrder*&, customerOrder*&);
    /*Hàm sử dụng để kiểm tra thứ tự khách hàng*/

    void DeleteType(customerOrder*& head, customerOrder*& tail, int &length);
    bool JujutsuCheckTable(); //!Kiểm tra trong bàn ăn toàn chú thuật sư
    bool CursedSpiritTable(); //!Kiểm tra trong bàn ăn toàn oán linh
public:
    imp_res() {
        TableOrder = nullptr;
        GuestQueue = nullptr;
        GuestInTable = 0;
        GuestInQueue = 0;
        CustomerX = nullptr;
        orderHead = orderTail = nullptr;
    };
    void RED(string name, int energy)
    {
        //cout << name << " " << energy << endl;
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
        //cout << "blue "<< num << endl;
        if (GuestInTable == 0) return;
        if (num >= MAXSIZE || num >= GuestInTable) RemoveGuestOrder(GuestInTable, orderHead, orderTail);
        else RemoveGuestOrder(num, orderHead, orderTail);
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
        if (GuestInTable <= 3) return;
        if (GuestInTable == 4){
            customer* temp = CustomerX;
            customer* res = nullptr;
            int MinEnergy = INT_MAX;
            for (int i = 0; i < GuestInTable; i++){
                if (MinEnergy > temp -> energy){
                    MinEnergy = temp -> energy;
                    res = temp;
                }
                temp = temp -> next;
            }
            for (int i = 0; i < GuestInTable; i++){
                res -> print();
                res = res -> next;
            }
            return;
        }
        if (JujutsuCheckTable()){
            customer* temp = CustomerX;
            int TotalEnergy = 0;
            for (int i = 0; i < 4; i++){
                TotalEnergy += temp -> energy;
                temp = temp -> next;
            }
            customer* p = CustomerX;
            customer* ansStart = CustomerX; customer* ansEnd = temp;
            int MinEnergyI = TotalEnergy;
            for (int i = 4; i < GuestInTable; i++){
                if (TotalEnergy - p -> energy + temp -> energy < MinEnergyI){
                    ansStart = p -> next;
                    ansEnd = temp;
                    MinEnergyI = TotalEnergy - p -> energy + temp -> energy;
                }
                TotalEnergy = TotalEnergy - p -> energy + temp -> energy;
                p = p -> next;
                temp = temp -> next;
            }
            int MinEnergy = INT_MAX;
            customer* MinIndex = nullptr;
            customer* Res = nullptr;
            for (int i = 0; i < 4; i++){
                customer* newCus = new customer(ansStart -> name, ansStart -> energy, nullptr, nullptr);
                if (Res == nullptr){
                    Res = newCus;
                    Res -> next = Res -> prev = Res;
                }
                else{
                    Res -> prev -> next = newCus;
                    newCus -> prev = Res -> prev;
                    newCus -> next = Res;
                    Res -> prev = newCus;
                }
                ansStart = ansStart -> next;
            }
            customer* tempRes = Res;
            int MinRes = INT_MAX;
            for (int i = 0; i < 4; i++){
                if (MinRes > tempRes -> energy){
                    MinRes = tempRes -> energy;
                    MinIndex = tempRes;
                }
                tempRes = tempRes -> next;
            }
            for (int i = 0; i < 4; i++){
                MinIndex -> print();
                MinIndex = MinIndex -> next;
            }
            return;
        }
        if (CursedSpiritTable()){
            customer* MinIndex = nullptr;
            int MinEnergy = INT_MAX;
            customer* temp = CustomerX;
            for (int i = 0; i < GuestInTable; i++){
                if (MinEnergy > temp -> energy){
                    MinEnergy = temp -> energy;
                    MinIndex = temp;
                }
                temp = temp -> next;
            }
            for (int i = 0; i < GuestInTable; i++){
                MinIndex -> print();
                MinIndex = MinIndex -> next;
            }
            return;
        }
        int MinEnergy = INT_MAX, AnsElementCount = 0;
        customer* temp = CustomerX;
        customer* start = nullptr; customer* end = nullptr;
        for (int i = 0; i < GuestInTable - 3; i++){
            int TotalEnergy = 0, ElementCount = 0;
            customer* temp_J = temp;
            for (int j = 0; j < GuestInTable; j++){
                TotalEnergy += temp -> energy;
                ElementCount += 1;
                if (ElementCount >= 4){
                    if (start == nullptr && end == nullptr){
                        start = temp; end = temp_J;
                        MinEnergy = TotalEnergy;
                        AnsElementCount = ElementCount;
                    }
                    else if (MinEnergy > TotalEnergy){
                        MinEnergy = TotalEnergy;
                        start = temp; end = temp_J;
                        AnsElementCount = ElementCount;
                    }
                }
                temp_J = temp_J -> next;
            }
            temp = temp -> next;
        }
        customer* Res = nullptr; //! Dãy con liên tiếp dài nhất có tổng nhỏ nhất
        int cnt = 0;
        for (int i = 0; i < AnsElementCount; i++){
            customer* newCus = new customer(start -> name, start -> energy, nullptr, nullptr);
            if (Res == nullptr){
                Res = newCus;
                Res -> next = Res -> prev = Res;
            }
            else{
                Res -> prev -> next = newCus;
                newCus -> prev = Res -> prev;
                newCus -> next = Res;
                Res -> prev = newCus;
            }
            start = start -> next;
        }
        customer* tempRes = Res;
        customer* MinIndex = nullptr;
        int MinRes = INT_MAX;
        for (int i = 0; i < AnsElementCount; i++){
            if (MinRes > tempRes -> energy){
                MinRes = tempRes -> energy;
                MinIndex = tempRes;
            }
            tempRes = tempRes -> next;
        }
        for (int i = 0; i < AnsElementCount; i++){
            MinIndex -> print();
            MinIndex = MinIndex -> next;
        }
    }
    void DOMAIN_EXPANSION()
    {
        //cout << "domain_expansion" << endl;
        if (GuestInTable == 0) return;
        int JujutsuSorcerers = 0; //! Số lượng Chú thuật sư
        int CursedSpirits = 0; //! Số lượng Oán linh
        int TotalSocererEnergy = 0, TotalSpiritEnergy = 0;
        //! Tách ra thành 2 danh sách chú thuật sư và oán linh
        customerOrder* JujutsuSorcererHead = nullptr;
        customerOrder* JujutsuSorcererMid = nullptr;
        customerOrder* JujutsuSorcererTail = nullptr;
        customerOrder* CursedSpiritsHead = nullptr;
        customerOrder* CursedSpiritsMid = nullptr;
        customerOrder* CursedSpiritsTail = nullptr;
        customerOrder* tempOrder = orderHead;
        for (int i = 0; i < GuestInTable; i++){
            if (tempOrder -> data -> energy > 0){
                AppendOrder(JujutsuSorcererHead, JujutsuSorcererTail, tempOrder -> data);
                TotalSocererEnergy += JujutsuSorcererTail -> data -> energy;
                JujutsuSorcerers += 1;
            }
            else {
                AppendOrder(CursedSpiritsHead, CursedSpiritsTail, tempOrder -> data);
                TotalSpiritEnergy += abs(CursedSpiritsTail -> data -> energy);
                CursedSpirits += 1;
            }
            tempOrder = tempOrder -> next;
        }
        JujutsuSorcererMid = JujutsuSorcererTail; CursedSpiritsMid = CursedSpiritsTail;
        customer* tempQueue = GuestQueue; //! Lấy luôn cả hàng chờ
        for (int i = 0; i < GuestInQueue; i++){
            if (tempQueue -> energy > 0){
                AppendOrder(JujutsuSorcererHead, JujutsuSorcererTail, tempQueue);
                JujutsuSorcererTail -> QueueCheck = true;
                TotalSocererEnergy += JujutsuSorcererTail -> data -> energy;
                JujutsuSorcerers += 1;
            }
            else{
                AppendOrder(CursedSpiritsHead, CursedSpiritsTail, tempQueue);
                CursedSpiritsTail -> QueueCheck = true;
                TotalSpiritEnergy += abs(CursedSpiritsTail -> data -> energy);
                CursedSpirits += 1;
            }
            tempQueue = tempQueue -> next;
        }
        //! Tổng ENERGY của tất cả chú thuật sư lớn hơn hoặc bằng tổng trị tuyệt đối ENERGY
        //của tất cả chú linh có mặt tại nhà hàng
        if (TotalSocererEnergy >= TotalSpiritEnergy){
            DeleteType(CursedSpiritsHead, CursedSpiritsTail, CursedSpirits);
        }
        else{
            DeleteType(JujutsuSorcererHead, JujutsuSorcererTail, JujutsuSorcerers);
        }
        ClearOrder(orderHead, orderTail); //Xóa thứ tự khách ban đầu
        if (JujutsuSorcerers){
            if (JujutsuSorcererMid != nullptr){
                orderHead = JujutsuSorcererHead;
                orderTail = JujutsuSorcererMid;
                ClearOrder(JujutsuSorcererMid -> next, JujutsuSorcererTail);
                JujutsuSorcererMid -> next = nullptr;
            }
            else ClearOrder(JujutsuSorcererHead, JujutsuSorcererTail);
            while (GuestInQueue != 0 && GuestInTable < MAXSIZE){
                customer* Guest = PopGuest();
                AppendGuestTable(Guest);
            }
        }
        else{
            if (CursedSpiritsMid != nullptr){
                orderHead = CursedSpiritsHead;
                orderTail = CursedSpiritsMid;
                ClearOrder(CursedSpiritsMid -> next, CursedSpiritsTail);
                CursedSpiritsMid -> next = nullptr;
            }
            else ClearOrder(CursedSpiritsHead, CursedSpiritsTail);
            while (GuestInQueue != 0 && GuestInTable < MAXSIZE){
                customer* Guest = PopGuest();
                AppendGuestTable(Guest);
            }
        }
        TableOrder = orderHead -> data;
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

void imp_res::AppendOrder(customerOrder *& head, customerOrder *& tail, customer* Customer) {
    if (!head && !tail){
        head = tail = new customerOrder(Customer);
        return;
    }
    tail -> next = new customerOrder(Customer);
    tail -> next -> prev = tail;
    tail = tail -> next;
}

void imp_res::ClearOrder(customerOrder*& head, customerOrder*& tail) {
    while(head != nullptr){
        customerOrder * temp = head;
        head = head -> next;
        temp -> data = nullptr;
        temp -> next = temp -> prev = nullptr;
        delete temp;
    }
    head = tail = nullptr;
}

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
        AppendOrder(orderHead, orderTail, CustomerX);
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
    AppendOrder(orderHead, orderTail, CustomerX);
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

Restaurant::customer* imp_res::PopGuest(Restaurant::customer * cus) {
    customer* tempGuest = nullptr;
    if (GuestQueue == 0) return nullptr;
    if ((GuestInQueue == 1 && cus == GuestQueue) || cus == GuestQueue){
        tempGuest = PopGuest();
        return tempGuest;
    }
    cus -> prev -> next = cus -> next;
    cus -> next -> prev = cus -> prev;
    tempGuest = cus;
    tempGuest -> next = tempGuest -> prev = nullptr;
    GuestInQueue -= 1;
    return tempGuest;
}

void imp_res::RemoveGuestOrder(int num, customerOrder*& head, customerOrder*& tail) {
    //if (!head && !tail) return;
    for (int i = 0; i < num; i++){
        customerOrder* tempOrder = head;
        head = head -> next;
        if (tempOrder -> data == TableOrder && tempOrder -> next != nullptr) TableOrder = tempOrder -> next -> data;
        if (tempOrder -> data -> energy > 0) CustomerX = tempOrder -> data -> next;
        else CustomerX = tempOrder -> data -> prev;
        tempOrder -> data -> prev -> next = tempOrder -> data -> next;
        tempOrder -> data -> next -> prev = tempOrder -> data -> prev;
        tempOrder -> data -> next = tempOrder -> data -> prev = nullptr;
        tempOrder -> next = tempOrder -> prev = nullptr;
        delete tempOrder;
        GuestInTable -= 1;
        if (GuestInTable == 0){
            CustomerX = TableOrder = nullptr;
            ClearOrder(orderHead, orderTail); //! Phòng trường hợp bất trắc
        }
    }
}

void imp_res::DeleteType(customerOrder *&head, customerOrder *&tail, int &length) {
    customerOrder* temp = tail;
    for (int i = 0; i < length; i++){
        temp -> data -> print();
        temp = temp -> prev;
    }
    for (int i = 0; i < length; i++){
        if (head != nullptr && head -> QueueCheck){
            temp = head;
            head = head -> next;
            PopGuest(temp -> data);
            temp -> next = temp -> prev = nullptr;
            delete temp;
        }
        else{
            RemoveGuestOrder(1, head, tail);
        }
    }
    length = 0;
}

bool imp_res::JujutsuCheckTable() {
    customer* temp = CustomerX;
    for (int i = 0; i < GuestInTable; i++){
        if (temp->energy < 0) return false;
        temp = temp -> next;
    }
    return true;
}

bool imp_res::CursedSpiritTable() {
    customer* temp = CustomerX;
    for (int i = 0; i < GuestInTable; i++){
        if (temp->energy > 0) return false;
        temp = temp -> next;
    }
    return true;
}