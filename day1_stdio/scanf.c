#include <stdio.h>

int main(void)
{
	int a, b;
	char c, d;
	char buf[64];

	//正常使用
//	scanf("%d %d", &a, &b);
//
//	printf("a = %d\n, b = %d\n", a, b);
//
	//输入三个数字才能结束
	//因为结尾是\n，\n属于空白字符，scanf会跳过空白字符去等待输入，把\n换成空格也是一样的结果
//	scanf("%d %d\n", &a, &b);
//
//	printf("a = %d\n, b = %d\n", a, b);

	//输入时候必须是这样的1,2
//	scanf("%d,%d", &a, &b);
//
//	printf("a = %d\n, b = %d\n", a, b);

	//输入字符的时候转义字符也是有效字符
//	scanf("%c%c", &c, &d);
//
//	putchar(c);
//	putchar(d);

	//这样用是正常的
//	scanf("%c %c", &c, &d);
//
//	putchar(c);
//	putchar(d);

	//输入是I love you的时候只能获取到I
//	scanf("%s", buf);
//
//	printf("%s\n", buf);
	
	//除了\n外的数据都收，知道\n为止
//	scanf("%[^\n]", buf);
//
//	printf("%s\n", buf);

	//1 N 输出结果是:
//    			input：
//			1 N
//			1 空格
//			input：
//			1 N
	//解释：输入1 N，第一个scanf获得1，第二个scanf获得空格
	//	进入第二次循环时候因为缓存中还存在N和换行两个数
	//	所以第一个scanf会因为数据类型不对而出错返回
	//	因此第二各scanf获得N，a的值不变
//	while(c != 'N')	{
//		printf("input:\n");
//		scanf("%d", &a);
//		scanf("%c", &c);
//		printf("%d  %c\n", a,c);
//	}

	//正常  在%c前面加空格或者在%d加空格
	while(c != 'N')	{
		printf("input:\n");
		scanf("%d", &a);
		scanf(" %c", &c);
		printf("%d  %c\n", a,c);
	}
	
	//限制输入长度
	//scanf("%20s", buf);
	//
	//printf("%s\n", buf);

	return 0;
}
















