package main

import (
	"fmt"
)

func main() {
	fmt.Println("")

	// 前景 背景 颜色
	// ---------------------------------------
	// 30  40  黑色
	// 31  41  红色
	// 32  42  绿色
	// 33  43  黄色
	// 34  44  蓝色
	// 35  45  紫红色
	// 36  46  青蓝色
	// 37  47  白色
	//
	// 代码 意义
	// -------------------------
	//  0  终端默认设置
	//  1  高亮显示
	//  4  使用下划线
	//  5  闪烁
	//  7  反白显示
	//  8  不可见

	for b := 40; b <= 47; b++ { // 背景色彩 = 40-47
		for f := 30; f <= 37; f++ { // 前景色彩 = 30-37
			for d := range []int{0, 1, 4, 5, 7, 8} { // 显示方式 = 0,1,4,5,7,8
				fmt.Printf(" %c[%d;%d;%dm%s(d=%d,b=%d,f=%d)%c[0m ", 0x1B, d, b, f, "", d, b, f, 0x1B)
			}
			fmt.Println("")
		}
		fmt.Println("")
	}

	fmt.Printf(" %c[1;40;31m%s%c[0m %s\n", 0x1B, "this is colorful text.", 0x1B, "this is uncolorful text?")
}
