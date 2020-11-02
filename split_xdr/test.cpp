#include <stdio.h>                                                                                                 
#include <stdlib.h>
#include <iostream>
#include "test.h"
using namespace std;

SXdrIndex::SXdrIndex(){
    memset(this, 0x00, sizeof(*this));
}

SXdrIndex::SXdrIndex(SXdr* pXdr){
    if(pXdr == NULL)
        return ;
    m_llStartTime = pXdr->get_startTime();
    m_pXdr = pXdr;
    m_pHead = NULL;
    m_pNext = NULL;
    m_pNextGroup = NULL;
}
SXdrIndex::~SXdrIndex(){
    DELETE_PTR(m_pXdr);
    DELETE_PTR(m_pHead);
    DELETE_PTR(m_pNext);
}
SXdrIndex* SXdrIndex::push(SXdr* pXdr){
    if(!pXdr)
        return NULL;
    if(!m_pHead )
        m_pHead = this;            //设置第一个节点的m_pHead
    SXdrIndex* pNewXdr = new SXdrIndex(pXdr);
    pNewXdr->m_pHead = this->m_pHead;
    SXdrIndex* pCmpXdr = m_pHead;
    SXdrIndex* pPreXdr = NULL;
    while(pCmpXdr){
        if(pNewXdr->m_llStartTime < pCmpXdr->m_llStartTime){    //pNewXdr插在pCmpXdr前
            if(!pPreXdr){           //插入头节点
                pNewXdr->m_pNext = m_pHead;
                pNewXdr->m_pHead = pNewXdr;
                pPreXdr = pNewXdr->m_pNext;
                while(pPreXdr){     //更新所有头结点信息
                    pPreXdr->m_pHead = pNewXdr;
                    pPreXdr = pPreXdr->m_pNext;
                }
            }
            else{                   //插入中间结点
                pPreXdr->m_pNext = pNewXdr;
                pNewXdr->m_pNext = pCmpXdr;
            }
            break;
        }
        pPreXdr = pCmpXdr;
        pCmpXdr = pCmpXdr->m_pNext;
    }
    if(!pCmpXdr)
        this->m_pNext = pNewXdr;    //pNewXdr插在尾节点

    if(this->m_pNext)
        return this->m_pNext;       //新增XDR为尾节点
    else
        return this;                //this 还是尾节点
}
void SXdrIndex::print(){
    printf("---------- print -----------------\n");
    //SXdrIndex* pPrint = this->m_pHead;
    SXdrIndex* pPrint = this;
    SXdrIndex* pNext = NULL;
    SXdr* pX = NULL;
    bool bNew = true;
    while(pPrint){
    if(pPrint->m_pNext){
        pNext =  pPrint->m_pNext;
        bNew = false;
    }
    else{
        pNext =  pPrint->m_pNextGroup;
        if(pNext)
            bNew = true;
        else
            bNew = false;
    }
//        if(pNext)
//            printf("\t[test] m_llStartTime = %lld, next m_llStartTime = %lld\n",pPrint->m_pXdr->get_startTime(),pNext->m_pXdr->get_startTime());
//        else
//            printf("\t[test] m_llStartTime = %lld\n",pPrint->m_pXdr->get_startTime());
        pX = pPrint->m_pXdr;
        printf("\t[test] startTime = %lld,triggerType = %d,billDate = %lld,treatFlag = %d\n",
            pX->startTime,pX->triggerType,pX->billDate,pX->treatFlag);
        if(bNew)
            printf("new Group:\n");
        pPrint = pNext;
    }
}


int64 get_triggerType(SXdrIndex* pXdrIndexList){
    return pXdrIndexList->m_pXdr->get_triggerType();
}
int64 get_billDate(SXdrIndex* pXdrIndexList){
    //取总话单
    return pXdrIndexList->m_pXdr->get_billDate();
}
int64 get_treatFlag(SXdrIndex* pXdrIndexList){
    //取总话单
    return pXdrIndexList->m_pXdr->get_treatFlag();
}
typedef int64 (*get_FieldFunc)(SXdrIndex* pXdrIndexList);

/*
*   根据函数func得到的条件进行分组拆分(话单拆分)
*   pXdrIndexList在第二次调用时需要区分m_pNext和m_pNextGroup
*   分组后非尾节点的m_pNextGroup变m_pNext，m_pNext变为NULL
*/
void split_xdrByFunc(
    SXdrIndex* pXdrIndexList,
    get_FieldFunc func,
    bool is_TreatFlag = false
){
    SXdrIndex* pXdrPreIndex = pXdrIndexList; 
    SXdrIndex* pXdrPreRight = pXdrPreIndex; 
    SXdrIndex* pXdrCmpIndex = pXdrPreIndex->m_pNext ? pXdrPreIndex->m_pNext : pXdrPreIndex->m_pNextGroup; 
    while(pXdrCmpIndex){
    if(!is_TreatFlag && func(pXdrPreIndex) != func(pXdrCmpIndex)){
        //分组
        pXdrPreIndex->m_pNext = NULL;
        pXdrPreIndex->m_pNextGroup = pXdrCmpIndex;
    }
    else if(is_TreatFlag){
        if( func(pXdrCmpIndex) != 0 ){
                //错单分组
                pXdrPreIndex->m_pNext = NULL;
                pXdrPreIndex->m_pNextGroup = pXdrCmpIndex;
            }
            else{
                if( func(pXdrPreIndex) != 0 ){
                        //pXdrCmpIndex正确话单且pXdrPreIndex是错单，需要考虑pXdrCmpIndex与前一条正确话单pXdrPreRight组合到同组
                        //比较pXdrPreRight和pXdrCmpIndex
                        if(get_triggerType(pXdrCmpIndex) == get_triggerType(pXdrPreRight) 
                            && get_billDate(pXdrCmpIndex) == get_billDate(pXdrPreRight)){
                            //组合到pXdrPreRight组
                            //pXdrCmpIndex从pXdrPreIndex后去除
                            pXdrPreIndex->m_pNext = pXdrCmpIndex->m_pNext;
                            pXdrPreIndex->m_pNextGroup = pXdrCmpIndex->m_pNextGroup;
                            //pXdrCmpIndex合并到pXdrPreRight之后
                            pXdrPreRight->m_pNext = pXdrCmpIndex;
                            pXdrCmpIndex->m_pNextGroup = pXdrPreRight->m_pNextGroup;
                            pXdrPreRight->m_pNextGroup = NULL;
                            pXdrCmpIndex->m_pNext = NULL;
                            //将pXdrPreIndex置为原pXdrCmpIndex的前一个节点，用于后面下一对的偏移
                            pXdrCmpIndex = pXdrPreIndex;
                        }
                        else{
                            //新分组
                        pXdrPreIndex->m_pNext = NULL;
                        pXdrPreIndex->m_pNextGroup = pXdrCmpIndex;
                        }
                    }
                //pXdrCmpIndex变成新的前一个
                pXdrPreRight = pXdrCmpIndex;
            }
    }
        //下一对
        pXdrPreIndex = pXdrCmpIndex;
        pXdrCmpIndex = pXdrPreIndex->m_pNext ? pXdrPreIndex->m_pNext : pXdrPreIndex->m_pNextGroup; 
    }   
}

//从pXdrIndexList位置开始处理
void split_xdrList(SXdrIndex* pXdrIndexList)
{
    if(!pXdrIndexList) return ;
    //按TRIGGER_TYPE进行话单拆分
    split_xdrByFunc(pXdrIndexList, get_triggerType);
    //按BILL_DATE进行话单拆分
    split_xdrByFunc(pXdrIndexList, get_billDate);
    //按TREAT_FLAG进行话单拆分
    split_xdrByFunc(pXdrIndexList, get_treatFlag, true);
    
}

int main(int argc, char **argv)
{
    cout<<"\n------------- test begin----------------"<<endl;
    SXdr sXdr1(20100101120000,1,20100301120000,0);
    SXdr sXdr2(20100101120005,2,20100301120001,0);
    SXdr sXdr3(20100101120003,1,20100301120001,0);
    SXdr sXdr4(20100101120001,1,20100301120001,0);
    SXdr sXdr5(20100101120006,2,20100301120001,0);
    SXdr sXdr6(20100101120007,2,20100301120001,1);
    SXdr sXdr7(20100101120008,2,20100301120001,1);
    SXdr sXdr8(20100101120009,2,20100301120001,0);
    SXdr sXdr9(20100101120002,1,20100301120001,0);
    SXdr sXdr10(20100101120010,2,20100301120002,0);
    SXdr sXdr11(20100101120011,2,20100301120002,1);
    SXdr sXdr12(20100101120012,2,20100301120002,0);

    SXdrIndex* pHead = new SXdrIndex(&sXdr1);
    SXdrIndex* pXdrIndexList = pHead;
    pXdrIndexList = pXdrIndexList->push(&sXdr2);
    pXdrIndexList = pXdrIndexList->push(&sXdr3);
    pXdrIndexList = pXdrIndexList->push(&sXdr4);
    pXdrIndexList = pXdrIndexList->push(&sXdr5);
    pXdrIndexList = pXdrIndexList->push(&sXdr6);
    pXdrIndexList = pXdrIndexList->push(&sXdr7);
    pXdrIndexList = pXdrIndexList->push(&sXdr8);
    pXdrIndexList = pXdrIndexList->push(&sXdr9);
    pXdrIndexList = pXdrIndexList->push(&sXdr10);
    pXdrIndexList = pXdrIndexList->push(&sXdr11);
    pXdrIndexList = pXdrIndexList->push(&sXdr12);

    pHead->print();

    split_xdrList(pXdrIndexList->m_pHead);

    pHead->print();
    cout<<"------------- test end----------------"<<endl;
    return 0;
}

