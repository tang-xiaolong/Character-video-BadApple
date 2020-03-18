#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
//读取位图文件相关代码
#pragma comment(lib, "WINMM.LIB")
BITMAPFILEHEADER fileHead;      //文件信息头
BITMAPINFOHEADER infoHead;      //位图信息头
RGBQUAD pColorTable[256];       //颜色表指针
unsigned char *pBmpBuf;         //图像数据指针
int bmpWidth;          //图像的宽
int bmpHeight;        //图像的高
int biBitCount;       //每像素位数
int lineByte;         //每行的字节数



unsigned char a[1000][1000];
unsigned char *Pre = NULL;
char *out;
char info[] = { "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^` ." };
int len;//记录Info长度
clock_t startTime;//记录开始时间
float timeFramRate = 0;//frame/timeSum  得到帧数和总时间的比例，避免每次计算


#define name "frame\\badapple"
#define frame 5216
//通过输出间隔和上下边距控制打印出来的信息多少  避免输出太多显示不下
#define Interval 2  //每隔几行输出一行
#define Margin 10	//上下边距 

//函数声明
int Read(const char *bmpName);
int Run(const char *bmpName);     /*函数原型*/
void Print();
void Pos(int x, int y);



int main(int argc, char *argv[])
{
	out = (char *)malloc(sizeof(char) * 40000);
	system("mode con cols=220 lines=150");
	system("color F0");

	timeFramRate = frame / 219000.0f;//3:39秒即219秒  即219000微妙
	len = strlen(info);
	Run(name);
	free(out);
	return 0;
}
//获取当前时间应该播放的图片 通过当前时间减去开始时间得到经过了多久，然后乘以比例的到第几张，减1变成下标
int GetIndex()
{
	return (int)((clock() - startTime) * timeFramRate - 1);
}

int Run(const char *bmpName)
{
	int nowIndex, preIndex = -1;
	int count = 0, temp = frame;
	char path[30] = "\0";
	char format[15];
	while (temp > 0)
	{
		++count;
		temp /= 10;
	}
	sprintf(format, "%s%dd%s", "%s%0", count, "%s");
	PlaySound("BadApple.wav", NULL, SND_FILENAME | SND_ASYNC);
	startTime = clock();
	while ((nowIndex = GetIndex()) < frame)
	{
		if (nowIndex != preIndex)
		{
			preIndex = nowIndex;
			path[0] = '\0';
			sprintf(path, format, bmpName, nowIndex, ".BMP");
			Read(path);
			Print();
		}
	}
	if (Pre)
		free(Pre);
	system("cls");
	printf("总共是花了%d秒\n", (clock() - startTime) / CLK_TCK);
	puts("蟹蟹欣赏~ ");
	return 1;
}
void Print()
{
	int i, j;
	int x = 0;
	Pos(0, 0);
	int maxRow = bmpHeight - Margin;
	for (i = Margin; i < maxRow; ++i)
	{
		if (i % Interval != 0)
			continue;
		for (j = 0; j < bmpWidth; ++j)
		{
			out[x++] = info[(int)(a[i][j] / 256.0*len)];
		}
		out[x++] = '\n';
	}
	out[x] = '\0';
	printf("%s", out);
}
int Read(const char *bmpName)
{
	FILE *fp = fopen(bmpName, "rb");              /*二进制读方式打开指定的图像文件*/
	register int i, j;
	unsigned char * pix = NULL;
	if (fp == NULL)
		return 0;
	fread(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);    /*读文件信息头*/
	fread(&infoHead, sizeof(BITMAPINFOHEADER), 1, fp);    /*读位图信息头*/
	bmpWidth = infoHead.biWidth;     /*获取图像宽，高，每像素所占位数等信息 */
	bmpHeight = infoHead.biHeight;
	biBitCount = infoHead.biBitCount;
	lineByte = (bmpWidth*biBitCount / 8 + 3) / 4 * 4;    /*计算每行字节数(必须是4的倍数)*/

	if (Pre == NULL)
	{
		pix = (unsigned char *)malloc(lineByte + 1);
		Pre = pix;
	}
	else
		pix = Pre;
	if (pix == NULL)
		return 0;
	for (i = 0; i < bmpHeight; ++i)
	{
		fread(pix, 1, lineByte, fp);
		for (j = 0; j < bmpWidth; ++j)
		{
			a[bmpHeight - i - 1][j] = (int)((unsigned char)(pix[j * 3 + 2] * 0.3 + pix[j * 3 + 1] * 0.59 + pix[j * 3] * 0.11));
		}
	}
	fclose(fp);
	return 1;
}

void Pos(int x, int y)//设置光标位置
{//要注意这里的x和y与我们数组的x和y是反的
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//返回标准的输入、输出或错误的设备的句柄，也就是获得输入、输出/错误的屏幕缓冲区的句柄
	SetConsoleCursorPosition(hOutput, pos);
}
