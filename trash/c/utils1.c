#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LEN 1024

typedef struct node {
    char *data;
    struct node *next;
} Node;

typedef struct list {
    Node *head;
    Node *tail;
    unsigned int len;
} List;

Node* node_malloc()
{
    Node *pNode;
    if ((pNode = (Node *)malloc(sizeof(Node))) == NULL) {
        printf("Node malloc failed!\n");
        exit(-1);
    }
    return pNode;
}

char* str_malloc(unsigned int size)
{
    char *pTmp;
    if ((pTmp = (char *)malloc(sizeof(char) * (size + 1))) == NULL) {
        printf("String malloc faild!\n");
        exit(-1);
    }
    return pTmp;
}

// 根据序数定位指定的链表节点的内存位置——负责检查序数输入的正确与否和链表是否为空
// ord是从1开始的
Node* locate(List *list, unsigned int ord)
{
    if (list->head == NULL)	// 链表检查在前-避免因检查到ord为0直接退出
        return NULL;
    if (ord > list->len) {
        printf("Ordinal must be less than the length of a list!\n");
        exit(-1);
    }
    if (ord == 0) {
        printf("Ordinal must be greater than 0!\n");
        exit(-1);
    }

    if (ord == list->len)
        return list->tail;
    Node *cur = list->head;
    unsigned int i;
    for (i = 1; i < ord; cur = cur->next, i++)
        ;
    return cur;
}

void delete_one_line(List *list, unsigned int ord)
{
    Node *cur, *prev;
    cur = list->head;
    prev = NULL;

    if (list->len == 1) {
        free(cur);
        list->head = NULL;
        list->tail = NULL;
        list->len = 0;
        return;
    }
    if (ord == 1) {
        list->head = cur->next;
        free(cur->data);
        free(cur);
        list->len--;
        return;
    }

    prev = locate(list, ord - 1);
    cur = locate(list, ord);
    prev->next = cur->next;
    free(cur->data);
    free(cur);
    list->len--;
}

void insert_one_line(List *list, unsigned int ord, const char *src)
{
    Node *cur = locate(list, ord);
    Node *pNode = node_malloc();
    pNode->data = str_malloc(strlen(src));
    if (cur == NULL) {							// 链表为空
        pNode->next = NULL;
        list->head = pNode;
        list->tail = pNode;
    } else if (cur == list->tail) {					// 非空-从尾部插入(list的尾部改变，需要特殊处理)
        pNode->next = NULL;
        cur->next = pNode;
        list->tail = pNode;
    } else if (cur == list->head) {					// 非空-从头部插入
        pNode->next = cur;
        list->head = pNode;
    } else {										// 非空-其他情况
        pNode->next = cur->next;
        cur->next = pNode;
    }
    list->len++;
    strcpy(pNode->data, src);
}

/* 字符串匹配算法——常规 */
int str_match(const char *src, const char *tep)
{
    int sLen = strlen(src);
    int tLen = strlen(tep);
    if (sLen > tLen) {
        int j = 0;
        while (j++ <= sLen - tLen)
            if ((memcmp(tep, src++, tLen)) == 0)	// 寻找匹配字符串
                return j;
    }
    return -1;
}

/* TODO: ＫＭＰ算法 */

int main(int argc, char *argv[])
{
    if (argc <= 1) exit(-1);
    FILE *fp_src = NULL;
    if ((fp_src = fopen(argv[1], "r+")) == NULL) {
        printf("****** Invalid File Name! ******\n");
        exit(-1);
    }

    char buf[MAX_LEN] = "\0";
    List file;
    file.head = NULL;
    file.tail = NULL;
    file.len = 0;
    /* 读入文件到内存 */
    while (fgets(buf, MAX_LEN - 1, fp_src) != NULL)
        insert_one_line(&file, file.len, buf);		// 从尾部插入
    printf("%d\n", (int)file.len);

    /* 修改文件 */
    unsigned int i;
    for (i = 0; i < file.len; i++) {
        Node* cur = locate(&file, i + 1);
        // if ((str_match(cur->data, " OSM2_HAL_Tm::")) != -1)
        // {
        // Node* tmp = cur->next;
        // bool ff = false;
        // int ii;
        // for(ii = 0; ii < 5 && !ff; ii++)
        // {
        // if(tmp->data[0] == '{')
        // ff = true;
        // tmp = tmp->next;
        // }
        // if(ff)
        // {
        // // unsigned int steps = 1;		// 插入或删除文本的行数
        // // unsigned int ord = i + 1;	// 起始行标
        // /* 插入文本 */
        // insert_one_line(&file, i + ii + 1, "    HAL_INFO_PRINT(\"%s\", __FUNCTION__);\n");
        // /* 删除文本 */
        // // while (steps--)
        // // delete_one_line(&file, ord + steps);
        // }
        // }
        int index;
        if ((index = str_match(cur->data, "PSM")) != -1) {
            int j;
            char tmp[MAX_LEN] = "\0";
            index--;
            for(j = 0; j < index; j++) {
                tmp[j] = cur->data[j];
            }
            tmp[index] = 'O';
            tmp[index + 1] = 'S';
            tmp[index + 2] = 'M';
            tmp[index + 3] = '2';
            for(j = index + 4; j < strlen(cur->data); j++) {
                tmp[j] = cur->data[j - 1];
            }
            tmp[j] = '\n';
            delete_one_line(&file, i+1);
            insert_one_line(&file, i, tmp);
        }
    }

    /* 将修改写入新文件 */
    Node *cur = file.head;
    FILE *fp_des = fopen("b.cpp", "w+");
    for (i = 0; i < file.len; i++) {
        fputs(cur->data, fp_des);
        // fputc('\n', fp_des);
        cur = cur->next;
    }

    /* 清理内存 */
    while (file.len > 0)
        delete_one_line(&file, 1);
    fclose(fp_des);
    fclose(fp_src);

    return 0;
}
