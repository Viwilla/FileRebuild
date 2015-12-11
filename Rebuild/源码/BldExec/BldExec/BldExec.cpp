#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
void writefile(char arr[][200]);
char orderby[][200] = { "Lock.rll" };
int n = 0;
FILE *fpr, *fpw;
char c;

int main(int argc, char *argv[])
{
	long file;
	struct _finddata_t find;
	char skippath[5] = "..";
	

	_chdir(".\\FILE\\");
	if ((file = _findfirst("*.*", &find)) == -1L)
	{
		printf("空白!\n");
		exit(0);
	}
	while (_findnext(file, &find) == 0)
	{
		//printf("%s", orderby[0]);
		if (!strcmp(find.name, skippath))
		{
			continue;
		}
		const char lock[50] = "Lock.rll";
		if (!strcmp(find.name,lock))
		{
			char Encryption[200] = ("..\\Encryption.exe Lock.rll unLock.rll 123");
			system(Encryption);
			char *lockfile = "Lock.rll";
			strcpy(orderby[0], "unLock.rll");
			n++;
			continue;
		}
		/****将分割后的文件根据标志按顺序放入数组orderby[][200]中***/
		char filename[200] = "";
		strcat(filename, find.name);
		FILE *x;
		x = fopen("Lock.rll","rb");
		fpr = fopen(filename, "rb");
		if (!fpr)
			printf("打开失败");
		c = fgetc(fpr);
		int i;
		char sig[4] = "001";
		for (i = 0; i < 3; i++)
		{
			sprintf(&sig[i],"%c",c);
			c = fgetc(fpr);
		}
		int j=int(sig[0]-48)+int(sig[1]-48)*10+int(sig[2]-48);
		j--;
		strcpy(orderby[j], filename);
		n++;
		fclose(fpr);
		//printf("j=%d\n", j);
		//printf("%s\n", orderby[j]);
	}
	_findclose(file);
	writefile(orderby);
	return 0;
}

void writefile(char orderby[][200])
{
	char PEname[200] = "Rebuild.exe";
	fpw = fopen(PEname, "wb");
	int i,j;
	for (j = 0; j < n; j++)
	{
		fpr = fopen(orderby[j], "rb");
		for (i = 0; i < 6; i++)
		{
			c = fgetc(fpr);
		}
		c = fgetc(fpr);
		while (!feof(fpr))
		{
			fputc(c, fpw);
			fflush(fpw);
			c = fgetc(fpr);
		}
		fclose(fpr);
	}
	
	fclose(fpw);
	system(PEname);
	remove(orderby[0]);
	return ;
}