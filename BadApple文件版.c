#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

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
char info[] = {"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^` ."};
int len;

#define name "frame\\badapple"
#define frame 5216
#define SPACING 2  //间距
#define SleepTime 41
#pragma comment(lib, "WINMM.LIB")



int ReadImg(const char *bmpName);
int WriteInfo(FILE *fp);
int Run(const char *bmpName);     /*函数原型*/
void Pos(int x, int y);



int main(int argc, char *argv[]) 
{
	out = (char *)malloc(sizeof(char) * 40000);
	system("mode con cols=220 lines=150");
	system("color F0");
	len = strlen(info) - 1;
	Run(name);
	free(out);
	return 0;
}

int WriteInfo(FILE *fp)
{
	register int i,j;
	int x = 0;
	Pos(0,0);
	for(i = 17;i < bmpHeight - 17;++i)
	{
		if(i % SPACING != 0)
			continue;
		for(j = 0;j < bmpWidth;++j)
		{
			if(j % SPACING == 0)
				out[x++] = info[(int)(a[i][j]/256.0*len)];
		}
		out[x++] = '\n';	
	}
	out[x] = '\0';
	fwrite(out,sizeof(char),x,fp);
	return x;
}
int Run(const char *bmpName)
{
	int i;
	int count = 0,temp = frame;
	FILE *fp = fopen("Info","wb+");
	char format[15];
	char path[30];
	clock_t first,last;
	if(fp == NULL)
	{
		puts("文件打开失败\n");
		return 0;
	}
	//每次读一张图片，并把信息写入文件。全部读完后停一下。
	while(temp > 0)
	{
		++count;
		temp /= 10;
	}
	sprintf(format,"%s%dd%s","%s%0",count,"%s");
	
	for(i = 0;i < frame;++i)
	{
		//依次把文件名和i拼接起来
		//Pos(0,0);
		//printf("正在写入......\n已经写入%d / %d",i+1,frame);
		path[0] = '\0';
		sprintf(path,format,bmpName,i,".BMP");
		ReadImg(path);
		count = WriteInfo(fp);//写入文件
	}
	
	//count = 6643;
	rewind(fp);
	system("cls");
	printf("写入完毕！\n");
	system("pause");
	PlaySound("BadApple.wav", NULL, SND_FILENAME | SND_ASYNC);
	first = clock();
	while(1)
	{
		last = clock();
		if(last - first < SleepTime)
		{
			continue;
		}
		else
		{
			if(fread(out,sizeof(char),count,fp) != 0)
			{
				first = last;
				Pos(0,0);
				printf("%s",out);
				//Sleep(SleepTime);
			}
			else
				break;
		}
	}
	fclose(fp);
	if(Pre)
		free(Pre);
	system("cls");
	puts("蟹蟹欣赏~");
	return 1; 
} 
int ReadImg(const char *bmpName)
{
	FILE *fp=fopen(bmpName,"rb");              /*二进制读方式打开指定的图像文件*/
	register int i,j;
	unsigned char * pix = NULL;
	if(fp==NULL) 
		return 0;
	fread(&fileHead,sizeof(BITMAPFILEHEADER),1,fp);    /*读文件信息头*/
	fread(&infoHead,sizeof(BITMAPINFOHEADER),1,fp);    /*读位图信息头*/
	bmpWidth=infoHead.biWidth;     /*获取图像宽，高，每像素所占位数等信息 */
	bmpHeight=infoHead.biHeight;
	biBitCount=infoHead.biBitCount;
	lineByte=(bmpWidth*biBitCount/8+3)/4*4;    /*计算每行字节数(必须是4的倍数)*/
	if(Pre == NULL)
	{
		pix=(unsigned char *)malloc(lineByte+1);
		Pre = pix;
	}
	else
		pix = Pre;
	if(pix == NULL)
		return 0;
	for(i = 0;i < bmpHeight;++i)
	{
		fread(pix,1,lineByte,fp);
		for(j = 0;j < bmpWidth;++j)
		{
			a[bmpHeight-i-1][j] = (int)((unsigned char)(pix[j*3+2] * 0.3 + pix[j*3+1] * 0.59 + pix[j*3] * 0.11));
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
