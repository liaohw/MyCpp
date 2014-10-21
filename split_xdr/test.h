
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

//�û�XDR�б�����
#define DELETE_PTR(ptr) if(ptr != NULL){ delete ptr; ptr = NULL; }
class SXdrIndex{
public:
    SXdrIndex();
    SXdrIndex(SXdr* pXdr);
    ~SXdrIndex();
    SXdrIndex* push(SXdr* pXdr);//����������ݲ��룬return ֵΪ���һ����Ч�ڵ� 
    void print();
    
    int64           m_llStartTime;  
    SXdr*           m_pXdr;         //����ָ��
    SXdrIndex*      m_pHead;        //���л���ͷ���
    SXdrIndex*      m_pNext;        //ͬһ�������һ���ڵ�(ͬ���������һ���ڵ�ĸ��ֶ�ΪNULL)
    SXdrIndex*      m_pNextGroup;   //��һ�������ָ��ڵ�(���һ������ĸ��ֶ�ΪNULL)
};

