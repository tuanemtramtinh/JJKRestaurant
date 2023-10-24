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
        if (data != nullptr) delete data;
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
    customerOrder* findDeleteCusOrder();
    void RemoveGuestOrder(int, customerOrder*&, customerOrder*&);
    void AppendFromQueueToTable(){
        if (GuestInQueue > 0) {
            while (GuestInQueue != 0 && GuestInTable < MAXSIZE) {
                customer *FirstGuest = PopGuest();
                customerOrder * tempOrder = orderHead;
                for (int i = 0; i < GuestInTable + GuestInQueue; i++) {
                    if (tempOrder->data == FirstGuest){
                        break;
                    }
                    tempOrder = tempOrder->next;
                }
                tempOrder -> QueueCheck = false;
                AppendGuestTable(FirstGuest);
            }
        }
    }
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

    void DeleteType(customerOrder*&, customerOrder*&, int&);
    void SwapNode(customer*&, customer*&, customer*&);
    customer* findMin(customer*, customer*, int);
    customer* at(customer*, int);
    bool checkCus(customer*, customer*);
    int InsSort(customer*&, int, int);
    int ShellSort(customer*&, int);
public:
    imp_res() {
        TableOrder = nullptr;
        GuestQueue = nullptr;
        GuestInTable = 0;
        GuestInQueue = 0;
        CustomerX = nullptr;
        orderHead = orderTail = nullptr;
    };
    ~imp_res(){
        while(orderHead){
            customerOrder*temp = orderHead;
            orderHead = orderHead -> next;
            delete temp;
        }
        TableOrder = nullptr;
        GuestQueue = nullptr;
        GuestInTable = 0;
        GuestInQueue = 0;
        CustomerX = nullptr;
        orderHead = orderTail = nullptr;
    }
    void RED(string name, int energy)
    {
        //cout << name << " " << energy << endl;
        customer *cus = new customer (name, energy, nullptr, nullptr);
        if (cus->energy == 0){
            delete cus;
            return; //! Không phải chú thuật sư hay chú linh
        }
        if (CheckDuplicate(cus)){
            delete cus;
            return; //! "thiên thượng thiên hạ, duy ngã độc tôn"
        }
        if (GuestInTable == MAXSIZE){ //! Thêm khách vào hàng chờ
            if (GuestInQueue == MAXSIZE){
                delete cus;
                return;
            }
            AppendGuestQueue(cus);
            AppendOrder(orderHead, orderTail, GuestQueue -> prev);
            orderTail -> QueueCheck = true;
            return;
        }
        AppendGuestTable(cus); //! Thêm khách vào bàn ăn
        AppendOrder(orderHead, orderTail, CustomerX);

    }
    void BLUE(int num)
    {
        //cout << "blue "<< num << endl;
        if (GuestInTable == 0) return;
        if (num >= MAXSIZE || num >= GuestInTable) RemoveGuestOrder(GuestInTable, orderHead, orderTail);
        else RemoveGuestOrder(num, orderHead, orderTail);
        AppendFromQueueToTable();
    }
    void PURPLE()
    {
        //cout << "purple"<< endl;
        if (GuestInQueue == 0 || GuestInQueue == 1) return;
        customerOrder* temp = orderTail;
        customer* MaxAbsEnergy = nullptr;
        for (int i = 0; i < GuestInTable + GuestInQueue; i++){
            if (temp -> QueueCheck && MaxAbsEnergy == nullptr) {
                MaxAbsEnergy = temp->data;
            }
            else if (temp -> QueueCheck && abs(temp -> data -> energy) > abs(MaxAbsEnergy -> energy)){
                MaxAbsEnergy = temp -> data;
            }
            temp = temp -> prev;
        }
        int SizeToSort = 0;
        if (MaxAbsEnergy != nullptr) {
            customer *tempQueue = GuestQueue;
            for (int i = 0; i < GuestInQueue; i++) {
                if (tempQueue == MaxAbsEnergy) {
                    SizeToSort = i + 1;
                    break;
                }
                tempQueue = tempQueue->next;
            }
        }
        int NUM = ShellSort(GuestQueue, SizeToSort);
        //cout << NUM << endl;
        BLUE(NUM % MAXSIZE);
    }
    void REVERSAL()
    {
        //cout << "reversal" << endl;
        if (GuestInTable == 1 || GuestInTable == 0) return;
        int Sorcerers = 0, Spirits = 0;
        customer* temp = CustomerX;
        for (int i = 0; i < GuestInTable; i++){
            if (temp -> energy > 0) Sorcerers += 1;
            else Spirits += 1;
            temp = temp -> next;
        }
        temp = CustomerX;
        customer* head = nullptr; customer* tail = nullptr;
        customer* FirstJujutsu = CustomerX;
        for (int i = 0; i < GuestInTable; i++){
            if (FirstJujutsu -> energy > 0) break;
            FirstJujutsu = FirstJujutsu -> prev;
        }
        //! Đảo ngược oán linh trước
        for (int i = 0; i < GuestInTable; i++){
            if (temp -> energy < 0){
                head = temp;
                break;
            }
            temp = temp -> prev;
        }
        int num = 0;
        if (head != CustomerX){
            temp = CustomerX;
            num = GuestInTable;
        }
        else{
            temp = CustomerX -> next;
            num = GuestInTable - 1;
        }
        for (int i = 0; i < num; i++){
            if (temp -> energy < 0){
                tail = temp;
                break;
            }
            temp = temp -> next;
        }
        if (head != tail && Spirits > 1) {
            while(true){
                SwapNode(TableOrder,head, tail);
                head = head -> prev;
                while(head -> energy > 0){
                    head = head -> prev;
                }
                if (head == tail) break;
                tail = tail -> next;
                while(tail -> energy > 0){
                    tail = tail -> next;
                }
                if (tail == head) break;
            }
            SwapNode(TableOrder,head, tail);
            while (true){
                if (head == tail) break;
                if (head->energy < 0 && tail->energy < 0) {
                    SwapNode(TableOrder,head, tail);
                    head = head->prev;
                    if (head == tail) break;
                    tail = tail->next;
                    if (head == tail) break;
                }
                if (head -> energy > 0) head = head->prev;
                if (head == tail) break;
                if (tail -> energy > 0) tail = tail->next;
                if (head == tail) break;
            }
        }
        //! Đảo ngược oán linh trước
        head = nullptr; tail = nullptr; num = 0; temp = FirstJujutsu;
        //! Đảo ngược chú thuật sư
        for (int i = 0; i < GuestInTable; i++){
            if (temp -> energy > 0){
                head = temp;
                break;
            }
            temp = temp -> prev;
        }
        if (head != FirstJujutsu){
            temp = FirstJujutsu;
            num = GuestInTable;
        }
        else{
            temp = FirstJujutsu -> next;
            num = GuestInTable - 1;
        }
        for (int i = 0; i < num; i++){
            if (temp -> energy > 0){
                tail = temp;
                break;
            }
            temp = temp -> next;
        }
        if (head != tail && Sorcerers > 1) {
            while(true){
                SwapNode(TableOrder,head, tail);
                head = head -> prev;
                while (head -> energy < 0){
                    head = head -> prev;
                }
                if (head == tail) break;
                tail = tail -> next;
                while (tail -> energy < 0){
                    tail = tail -> next;
                }
                if (tail == head) break;
            }
        }
        //! Đảo ngược chú thuật sư
    }
    void UNLIMITED_VOID()
    {
        //cout << "unlimited_void" << endl;
        if (GuestInTable <= 3) return;
        int MinEnergy = 2147483647, AnsElementCount = 0;
        customer* temp = CustomerX;
        customer* start = nullptr; customer* end = nullptr;
        int ElementCount = 0;
        for (int i = 0; i < GuestInTable; i++){
            int TotalEnergy = 0;
            ElementCount = 0;
            customer* temp_J = temp;
            for (int j = 0; j < GuestInTable; j++){
                TotalEnergy += temp_J -> energy;
                ElementCount += 1;
                if (ElementCount >= 4){
                    if (MinEnergy > TotalEnergy || (MinEnergy == TotalEnergy && ElementCount >= AnsElementCount)){
                        start = temp; end = temp_J;
                        MinEnergy = TotalEnergy;
                        AnsElementCount = ElementCount;
                    }
                }
                temp_J = temp_J -> next;
            }
            temp = temp -> next;
        }
        customer* MinIndex = findMin(start, end, AnsElementCount);
        customer* tempMinIndex = MinIndex;
        do{
            tempMinIndex -> print();
            tempMinIndex = tempMinIndex -> next;
        }while(tempMinIndex != end -> next);
        while(start != MinIndex){
            start -> print();
            start = start -> next;
        }
    }
    void DOMAIN_EXPANSION()
    {
        //cout << "domain_expansion" << endl;
        if (GuestInTable == 0 && GuestQueue == 0) return;
        int JujutsuSorcerers = 0; //! Số lượng Chú thuật sư
        int CursedSpirits = 0; //! Số lượng Oán linh
        int TotalSocererEnergy = 0, TotalSpiritEnergy = 0;
        //! Tách ra thành 2 danh sách chú thuật sư và oán linh
        customerOrder* JujutsuSorcererHead = nullptr;
        customerOrder* JujutsuSorcererTail = nullptr;
        customerOrder* CursedSpiritsHead = nullptr;
        customerOrder* CursedSpiritsTail = nullptr;
        customerOrder* tempOrder = orderHead;
        for (int i = 0; i < GuestInTable + GuestInQueue; i++){
            if (tempOrder -> data -> energy > 0 ){
                AppendOrder(JujutsuSorcererHead, JujutsuSorcererTail, tempOrder -> data);
                if (tempOrder -> QueueCheck) JujutsuSorcererTail -> QueueCheck = true;
                TotalSocererEnergy += JujutsuSorcererTail -> data -> energy;
                JujutsuSorcerers += 1;
            }
            else if (tempOrder -> data -> energy < 0) {
                AppendOrder(CursedSpiritsHead, CursedSpiritsTail, tempOrder -> data);
                if (tempOrder -> QueueCheck) CursedSpiritsTail -> QueueCheck = true;
                TotalSpiritEnergy += CursedSpiritsTail -> data -> energy;
                CursedSpirits += 1;
            }
            tempOrder = tempOrder -> next;
        }
        TotalSpiritEnergy += TotalSocererEnergy;
        TotalSpiritEnergy = abs(TotalSpiritEnergy);
        //! Tổng ENERGY của tất cả chú thuật sư lớn hơn hoặc bằng tổng trị tuyệt đối ENERGY
        //của tất cả chú linh có mặt tại nhà hàng
        if (TotalSocererEnergy >= TotalSpiritEnergy) DeleteType(CursedSpiritsHead, CursedSpiritsTail, CursedSpirits);
        else DeleteType(JujutsuSorcererHead, JujutsuSorcererTail, JujutsuSorcerers);
        ClearOrder(orderHead, orderTail); //Xóa thứ tự khách ban đầu
        if (JujutsuSorcerers){
            orderHead = JujutsuSorcererHead;
            orderTail = JujutsuSorcererTail;
            AppendFromQueueToTable();
        }
        else{
            orderHead = CursedSpiritsHead;
            orderTail = CursedSpiritsTail;
            AppendFromQueueToTable();
        }
        customerOrder * newTempOrder = orderHead;
        while(newTempOrder != nullptr){
            if (newTempOrder -> QueueCheck == false){
                TableOrder = newTempOrder -> data;
                break;
            }
            newTempOrder = newTempOrder -> next;
        }
    }
    void LIGHT(int num)
    {
        //cout << "light " << num << endl;
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
    customerOrder* newCus = new customerOrder(Customer);
    tail -> next = newCus;
    
    newCus -> prev = tail;
    tail = newCus;
}

void imp_res::ClearOrder(customerOrder*& head, customerOrder*& tail) {
    while(head != nullptr){
        customerOrder * temp = head;
        head = head -> next;
        temp -> data = nullptr;
        delete temp;
    }
    head = tail = nullptr;
}

bool imp_res::CheckDuplicate(customer* cus) {
    customer* tempTable = CustomerX;
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
        return;
    }
    //! Trường hợp bàn ăn không trống
    if (GuestInTable >= MAXSIZE / 2){
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

customerOrder* imp_res::findDeleteCusOrder(){
    customerOrder* tempOrder = orderHead;
    for (int i = 0; i < GuestInTable + GuestInQueue; i++){
        if (tempOrder -> QueueCheck == false) return tempOrder;
        tempOrder = tempOrder -> next;
    }
    return nullptr;
}

void imp_res::RemoveGuestOrder(int num, customerOrder*& head, customerOrder*& tail) {
    //if (!head && !tail) return;
    for (int i = 0; i < num; i++){
        customerOrder* tempOrder = findDeleteCusOrder();
        if (TableOrder == tempOrder -> data && GuestInTable > 1){
            customerOrder* tableTempPos = tempOrder -> next;
            while(GuestInTable > 1 && tableTempPos != nullptr){
                if (tableTempPos -> QueueCheck == false){
                    TableOrder = tableTempPos -> data;
                }
                tableTempPos = tableTempPos -> next;
            }
        }
        if (tempOrder -> data -> energy > 0) CustomerX = tempOrder -> data -> next;
        else if (tempOrder -> data -> energy < 0) CustomerX = tempOrder -> data -> prev;
        tempOrder -> data -> prev -> next = tempOrder -> data -> next;
        tempOrder -> data -> next -> prev = tempOrder -> data -> prev;
        if (tempOrder == head){
            head = head -> next;
            delete tempOrder;
        }
        else {
            tempOrder->prev->next = tempOrder->next;
            if (tempOrder == tail) {
                tail = tempOrder->prev;
            } else tempOrder->next->prev = tempOrder->prev;
            delete tempOrder;
        }
        GuestInTable -= 1;
        if (GuestInTable == 0){
            CustomerX = TableOrder = nullptr;
            if (GuestInQueue == 0){
                head = tail = nullptr;
            }
        }
    }
    if (head && tail){
        head -> prev = nullptr;
        tail -> next = nullptr;
    }
}

void imp_res::DeleteType(customerOrder *&head, customerOrder *&tail, int &length) {
    customerOrder* temp = tail;
    if (length == 0) return;
    for (int i = 0; i < length; i++){
        temp -> data -> print();
        temp = temp -> prev;
    }
    for (int i = 0; i < length; i++){
        if (head != nullptr && head -> QueueCheck){
            temp = head;
            PopGuest(temp -> data);
            head = head -> next;
            delete temp;
        }
        else {
            customerOrder* tempOrder = head;
            if (tempOrder->data->energy > 0) CustomerX = tempOrder->data->next;
            else if (tempOrder->data->energy < 0) CustomerX = tempOrder->data->prev;
            tempOrder->data->prev->next = tempOrder->data->next;
            tempOrder->data->next->prev = tempOrder->data->prev;
            if (tempOrder == head) {
                head = head->next;
                delete tempOrder;
            }
            GuestInTable -= 1;
            if (GuestInTable == 0) {
                CustomerX = TableOrder = nullptr;
                if (GuestInQueue == 0){
                    GuestQueue = nullptr;
                }
            }
            if (GuestInQueue == 0){
                GuestQueue = nullptr;
            }
        }
    }
    head = tail = nullptr;
    length = 0;
}

Restaurant::customer* imp_res::findMin(Restaurant::customer *start, Restaurant::customer *end, int length) {
    int min_val = 2147483647;
    customer* MinIndex = nullptr;
    for (int i = 0; i < length; i++){
        if (min_val > start -> energy){
            MinIndex = start;
            min_val = start -> energy;
        }
        start = start -> next;
    }
    return MinIndex;
}

void imp_res::SwapNode(Restaurant::customer*& head, Restaurant::customer *& Cus1, Restaurant::customer *& Cus2) {
    if (head == Cus1) head = Cus2;
    else if (head == Cus2) head = Cus1;
    customer * temp = Cus1 -> next;
    Cus1 -> next = Cus2 -> next;
    Cus2 -> next = temp;
    Cus1 -> next -> prev = Cus1;
    Cus2 -> next -> prev = Cus2;
    temp = Cus1 -> prev;
    Cus1 -> prev = Cus2 -> prev;
    Cus2 -> prev = temp;
    Cus1 -> prev -> next = Cus1;
    Cus2 -> prev -> next = Cus2;
    temp = Cus1;
    Cus1 = Cus2;
    Cus2 = temp;
}

Restaurant::customer* imp_res::at(customer* head, int index) {
    customer* temp = head;
    for (int i = 0; i < index; i++)
        temp = temp -> next;
    return temp;
}

bool imp_res::checkCus(Restaurant::customer * Cus1, Restaurant::customer *Cus2) {
    customerOrder *temp = orderHead;
    for(int i = 0; i < GuestInTable + GuestInQueue; i++){
        if (temp -> data == Cus1) return true;
        else if (temp -> data == Cus2) return false;
        temp = temp -> next;
    }
    return false;
}

int imp_res::InsSort(Restaurant::customer*& list, int size, int incr) {
    int SwapCount = 0; //! Đếm số lần Swap
    for (int i = incr; i < size; i++){
        for (int j = i; j >= incr /*&& abs(at(list, j) -> energy) > abs(at(list, j - incr) -> energy)*/; j -= incr){
            customer* Cus1 = at(list, j);
            customer* Cus2 = at(list, j - incr);
            if (abs(Cus1 -> energy) > abs(Cus2 -> energy) ||
                abs(Cus1 -> energy) == abs(Cus2 -> energy) && checkCus(Cus1, Cus2) /*IndexOf(Cus1) < IndexOf(Cus2)*/){
                if (Cus1 == GuestQueue) GuestQueue = Cus2;
                else if (Cus2 == GuestQueue) GuestQueue = Cus1;
                SwapNode(list, Cus1, Cus2);
                SwapCount += 1;
            }
            else break;
        }
    }
    return SwapCount;
}

int imp_res::ShellSort(Restaurant::customer *& list, int size) {
    int NumCount = 0; //! Đếm NUM để thực hiện lệnh BLUE <NUM>
    for (int i = size / 2; i > 2; i /= 2){
        for (int j = 0; j < i; j++){
            customer* pos = at(list, j);
            NumCount += InsSort(pos, size - j, i);
        }
    }
    customer* pos = at(list, 0);
    NumCount += InsSort(pos, size, 1);
    return NumCount;
}