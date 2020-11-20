// utils.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024

/* 固定长度——处理行数最多不超过10000行的代码文件 */
struct rec {
    char str[1024];
    unsigned int len;
} Rec[10000];

/* 字符串匹配算法——常规 */
int str_match(const char *src, const char *tep)
{
    int sLen = strlen(src);
    int tLen = strlen(tep);
    if (sLen > tLen) {
        int j = 0;
        while(j++ <= sLen - tLen)
            if ((memcmp(tep, src++, tLen)) == 0)	// 寻找匹配字符串
                return j;
    }
    return -1;
}

/* 将文本在某行处移动m行(向前/后) */
int move_text(struct rec *Rec, int num_lines, int cur_index, int steps, bool dir)
{
    int i = 0;
    if (dir) {	// dir = true ，表示删除操作-删除包含当前行的steps行（向下删除）
        while(i < num_lines - cur_index) {
            Rec[i] = Rec[i + steps];
            i++;
        }
        while (steps-- > 0)
            Rec[i++].str[0] = '\0';
    } else {	// 增加操作——在当前行下方插入
        i = num_lines - cur_index;
        while (i--)
            Rec[i + steps] = Rec[i];
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc <= 1) exit(-1);
    FILE *fp_src;
    if ((fp_src = fopen(argv[1], "r+")) == NULL) {
        printf("****** Invalid File Name! ******\n");
        exit(-1);
    }
    /* 将整个文件读入到内存中 */
    char buf[MAX_LEN];
    memset(buf, 0, MAX_LEN);
    int num_line = 0; 	// 记录行数
    int num_str = 0;	// 记录当前行的字符数
    while (fgets(buf, MAX_LEN - 1, fp_src) != NULL) {
        while (buf[num_str] != '\n' && buf[num_str] != 0) num_str++;	// 统计本行的字符数
        Rec[num_line].len = num_str;
        memcpy(Rec[num_line].str, buf, num_str);
        num_line++;
        num_str = 0;
    }

    /* 处理文件内容 */
    int i;
    for (i = 0; i < num_line; i++) {
        if (Rec[i + 1].str[4] == '{' && (str_match(Rec[i].str, "if(isSsm2Card())")) != -1) {
            /* 插入文本 */
            // int steps = 3;		// 插入文本的行数
            // int lines = i + 2;	// 起始行标
            // move_text(&Rec[lines], num_line, lines, steps, false);
            // num_line += steps;
            // strcpy(Rec[lines].str, "        HAL_INFO_PRINT(\"ATTENTION: This is ssm2 cardType and the oam function block has been removed.\");");
            // strcpy(Rec[lines + 1].str, "        HAL_INFO_PRINT(\"           The operation of oam will not be effective!\");");
            // strcpy(Rec[lines + 2].str, "        return HAL_SUCCESS;");

            /* 删除文本 */
            int steps = 3;		// 删除文本的行数
            int lines = i + 2;	// 起始行标
            move_text(&Rec[lines], num_line, lines, steps, true);
            num_line -= steps;
        }
    }

    FILE *fp_des = fopen("b.cpp", "w+");
    printf("total: %d\n", num_line);
    for (i = 0; i < num_line; i++) {
        fputs(Rec[i].str, fp_des);
        fputc('\n', fp_des);
    }

    fclose(fp_src);
    fclose(fp_des);
    return 0;
}
