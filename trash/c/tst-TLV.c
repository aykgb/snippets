/*
1、TLV简介：
在通信系统中，两个设备之前必然存在消息交互，消息的格式也存在各种编码类型，
本文仅描述TLV编码的消息格式。Type-length-value（TLV）格式中T、L的长度固定，
通常为1-8个4个字节，V的长度不固定，由L的值表示，V的内容也可以嵌套子TLV格式。
举例：假设消息按大端模式存放，T占4个字节，L占2个字节，下面的消息：
 unsigned char pMsg[] = {0x09, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01,0x01,0x01, 0x01}
 T = {0x09, 0x00, 0x00, 0x00}，值为9。
 L = {0x04, 0x00},值为4。
 V = {0x01,0x01,0x01, 0x01} ，长度为4，每个字节的值均为1。
2、代码实现：a、按T的值由小到大排序一个消息（假定消息中不存在T相同的信元）；
             b、在两个消息中查找相同的信元（T、L、V）均相同，并输出信元个数。

作者:Socrates
日期:2014-08-05
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define TLV_T_LEN (4)
#define TLV_L_LEN (2)

/*错误码*/
enum _RetCode {
    ERR = -1, /*失败*/
    OK = 0    /*成功*/
} RETCODE;

/*信元TLV结构*/
typedef struct _stIE {
    unsigned int ulTag;    /*T*/
    unsigned short usLen;  /*L*/
    unsigned char *pValue; /*V*/
} IE;

/*消息链表*/
typedef struct _stMsg {
    IE ie;
    struct _stMsg *pNext;
} Msg;

/*
功能:创建链表
*/
int CreateMsgList(Msg *&pList)
{
    pList = (Msg *)malloc(sizeof(Msg));
    if (NULL == pList) {
        return ERR;
    }
    memset(&(pList->ie), 0, sizeof(IE));
    pList->pNext = NULL;

    return OK;
}

/*
功能:销毁链表
*/
void DestoryMsgList(Msg *pList)
{
    if (NULL == pList) {
        return;
    }

    Msg *p = pList;
    while(NULL != p) {
        p = p->pNext;
        free(pList);
        pList = p;
    }
    free(pList);
    pList = NULL;
    return;
}

/*
功能:向消息链表中插入信元，并保持按Tag递增
*/
int InsertIEToMsgList(Msg *pMsgList, const IE *pIE)
{
    if ((NULL == pMsgList)
            || (NULL == pIE)) {
        return ERR;
    }

    /*链表销毁时释放*/
    Msg *pInsertMsg = (Msg *)malloc(sizeof(Msg));
    if (NULL == pInsertMsg) {
        return ERR;
    }

    /*创建链表结点*/
    memcpy(&(pInsertMsg->ie), pIE, sizeof(IE));
    pInsertMsg->pNext = NULL;

    /*按Tag递增插入结点，保持链表有序，不带头结点*/
    Msg *p = pMsgList;
    while(NULL != p->pNext) {
        if ((p->pNext->ie.ulTag) > (pIE->ulTag)) {
            break;
        }
        p = p->pNext;
    }

    pInsertMsg->pNext = p->pNext;
    p->pNext = pInsertMsg;

    return OK;
}

/*
功能:获取指定消息中的第一个信元
*/
IE *GetIEFromMsg(const unsigned char *pInMsg)
{
    if (NULL == pInMsg) {
        return NULL;
    }

    /*链表销毁时释放*/
    IE *pIE = (IE *)malloc(sizeof(IE));
    if (NULL == pIE) {
        return NULL;
    }
    memset(pIE, 0, sizeof(IE));

    pIE->ulTag = *(unsigned int *)pInMsg;
    pIE->usLen = *(unsigned short *)(pInMsg + TLV_T_LEN);
    pIE->pValue = (unsigned char *)(pInMsg + TLV_T_LEN + TLV_L_LEN);

    return pIE;
}

/*
功能:构造有序消息链表
*/
int CreateSortMsgList(unsigned char *pInMsg, unsigned int ulMsgLen, Msg *&pOutMsgList)
{

    if ((NULL == pInMsg)
            ||(0 == ulMsgLen)) {
        return ERR;
    }

    /*建立链表*/
    if (ERR == CreateMsgList(pOutMsgList)) {
        return ERR;
    }

    unsigned int iTmpMsgLen = 0;
    IE *pIE = NULL;

    /*遍历消息，注意获取信元并插入消息链表*/
    while(iTmpMsgLen < ulMsgLen) {
        pIE = GetIEFromMsg(pInMsg);
        if (NULL == pIE) {
            return ERR;
        }

        if(ERR == InsertIEToMsgList(pOutMsgList, pIE)) {
            return ERR;
        }

        pInMsg += (TLV_T_LEN + TLV_L_LEN + pIE->usLen);
        iTmpMsgLen += (TLV_T_LEN + TLV_L_LEN + pIE->usLen);
    }

    return OK;
}

/*
功能:消息排序
*/
int Sort(unsigned char *pInMsg, unsigned int ulMsgLen, unsigned char *pOutMsg)
{
    if ((NULL == pInMsg)
            || (NULL == pOutMsg)
            || (0 == ulMsgLen)) {
        return ERR;
    }

    /*建立有序消息链表*/
    unsigned char *pTmp = pOutMsg;
    Msg *pMsgList = NULL;
    if (ERR == CreateSortMsgList(pInMsg, ulMsgLen, pMsgList)) {
        DestoryMsgList(pMsgList);
        return ERR;
    }

    /*输出排序后的消息*/
    Msg *pList = pMsgList->pNext;
    while(NULL != pList) {
        memcpy(pTmp, &(pList->ie), TLV_T_LEN + TLV_L_LEN);
        memcpy(pTmp + TLV_T_LEN + TLV_L_LEN, pList->ie.pValue, pList->ie.usLen);
        pTmp += (TLV_T_LEN + TLV_L_LEN + pList->ie.usLen);
        pList = pList->pNext;
    }

    DestoryMsgList(pMsgList);
    return OK;
}

/*
功能:比较两个信元是否相同
*/
int IsSameIE(IE *pIE1, IE *pIE2)
{
    if ((NULL == pIE1)
            || (NULL == pIE2)) {
        return ERR;
    }

    if ((pIE1->ulTag == pIE2->ulTag)
            && (pIE1->usLen == pIE2->usLen)
            && (0 == memcmp(pIE1->pValue, pIE2->pValue, pIE1->usLen))) {
        return OK;
    }

    return ERR;
}

/*
功能:比较两个消息，并输出相同信元个数
*/
int CompareMsg(unsigned char *pMsg1,
               unsigned int ulMsgLen1,
               unsigned char *pMsg2,
               unsigned int ulMsgLen2,
               unsigned int *ulSameNum)

{
    /*创建有序消息链表1*/
    Msg *pMsgList1 = NULL;
    if (ERR == CreateSortMsgList(pMsg1, ulMsgLen1, pMsgList1)) {
        DestoryMsgList(pMsgList1);
        return ERR;
    }

    /*创建有序消息链表2*/
    Msg *pMsgList2 = NULL;
    if (ERR == CreateSortMsgList(pMsg2, ulMsgLen2, pMsgList2)) {
        DestoryMsgList(pMsgList1);
        DestoryMsgList(pMsgList2);
        return ERR;
    }

    Msg *p = pMsgList1->pNext;
    Msg *q = NULL;
    unsigned int iCount = 0;

    /*比较消息*/
    while(NULL != p) {
        q = pMsgList2->pNext;
        while(NULL != q) {
            if (OK == IsSameIE(&(p->ie), &(q->ie))) {
                iCount++;
            }

            q = q->pNext;
        }
        p = p->pNext;
    }

    DestoryMsgList(pMsgList1);
    DestoryMsgList(pMsgList2);

    *ulSameNum = iCount;

    return OK;

}


int main(int argc, char* argv[])
{
    unsigned char pMsg[] = {0x09, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01,0x01,0x01, 0x01,
                            0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x02,0x02,0x02, 0x02, 0x02, 0x02, 0x02,
                            0x05, 0x00, 0x00, 0x00, 0x05, 0x00, 0x03,0x03,0x03, 0x03, 0x03,
                            0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01,0x01,0x01, 0x01,
                            0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x08,0x08,
                            0x01, 0x00, 0x01, 0x00, 0x04, 0x00, 0x09,0x09,0x09, 0x09
                           };

    unsigned char pMsg2[] = {0x09, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01,0x01,0x01, 0x01,
                             0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x02,0x02,0x02, 0x02, 0x02, 0x02, 0x02,
                             0x05, 0x00, 0x00, 0x00, 0x05, 0x00, 0x03,0x03,0x03, 0x03, 0x03,
                             0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01,0x01,0x01, 0x01,
                             0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x08,0x08,
                             0x01, 0x00, 0x01, 0x00, 0x04, 0x00, 0x09,0x09,0x09, 0x09
                            };
    int iLen = sizeof(pMsg) / sizeof(pMsg[0]);
    for (int i = 0; i < iLen; i++) {
        printf("0x%x,", pMsg[i]);
    }
    printf("\n\n");

    unsigned char *pSortMsg = (unsigned char *)malloc(iLen);
    if (NULL == pSortMsg) {
        return ERR;
    }

    if (ERR != Sort(pMsg, iLen, pSortMsg)) {
        for (int i = 0; i < iLen; i++) {
            printf("0x%x,", pSortMsg[i]);
        }
    }

    int iLen2 = sizeof(pMsg2) / sizeof(pMsg2[0]);
    unsigned int iSameNum = 0;

    if (ERR != CompareMsg(pMsg, iLen, pMsg2, iLen2, &iSameNum)) {
        printf("\nSame Number is %d", iSameNum);
    }

    getchar();
    return 0;
}
