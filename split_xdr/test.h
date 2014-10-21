
typedef int int32;
typedef long long int64;

class SXdr{
public:
    SXdr(){memset(this , 0x00, sizeof(*this));}
    SXdr(int64 a,int32 b,int64 c,int32 d):startTime(a),triggerType(b),billDate(c),treatFlag(d){}
    ~SXdr(){}

    int64 get_startTime(){return startTime;}
    int32 get_triggerType(){return triggerType;}
    int64 get_billDate(){return billDate;}
    int32 get_treatFlag(){return treatFlag;}
    
    int64 startTime;
    int32 triggerType;
    int64 billDate;
    int32 treatFlag;
};

//用户XDR列表排序
#define DELETE_PTR(ptr) if(ptr != NULL){ delete ptr; ptr = NULL; }
class SXdrIndex{
public:
    SXdrIndex();
    SXdrIndex(SXdr* pXdr);
    ~SXdrIndex();
    SXdrIndex* push(SXdr* pXdr);//带排序的数据插入，return 值为最后一个有效节点 
    void print();
    
    int64           m_llStartTime;  
    SXdr*           m_pXdr;         //话单指针
    SXdrIndex*      m_pHead;        //所有话单头结点
    SXdrIndex*      m_pNext;        //同一分组的下一个节点(同分组内最后一个节点的该字段为NULL)
    SXdrIndex*      m_pNextGroup;   //下一个分组的指针节点(最后一个分组的该字段为NULL)
};

