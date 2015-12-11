#define BYTE 0
#define PIECE 1 

#include<stdio.h>
#include<stdlib.h> 
#include<string.h>

int a; 
char string[][99] = { "Lock.rll", "activeds.tlb", "wica.ini", "fms.dll.mui", "stand.tmp", "Binge.lsp", "funs.tmp", "11256.cpx", "lodsd.bl" };
void main(int argc, char **argv)
{
	void fun(char *s); /*** 扩展名自加函数 ***/
	FILE *fp_write = NULL, *fp_read = NULL, *fp_bat = NULL;
	long num_in = 0, byte_piece, pc_byte, total_byte;
	int i, buffer = 0, len, pc_fn = 0, pc_float = 0, byte_rest = 0, sort_div;
	char *fn_in, fn_out[50], p[] = ".000", fn_bat[100], fn_obj_bat[100], str_rest_byte[4], str_pc_fn[4], ch;

	switch (argc) /*** 命令行检测 ***/
	{
	case 3: break;
	case 2:
		printf("\n ERROR! you forgot to enter the number\n"); exit(0);
	case 1:
		printf("\n ERROR! you forgot to enter the file name and the number\n");
		exit(0);
	}

	fn_in = argv[1];
	if ((fp_read = fopen(fn_in, "rb")) == NULL) /*** 打开被分割的文件 ***/
	{
		printf("\n ERROR! the file not exists\n"); 
		exit(0);
	}
	fseek(fp_read, 0L, 2);
	total_byte = ftell(fp_read); 
	rewind(fp_read);

	if (*argv[2] == '(') /*** 检测分割类型 ***/
	{
		sort_div = PIECE;
		i = 1;
		while (argv[2][i] >= '0'&&argv[2][i] <= '9')
			num_in = num_in * 10 + argv[2][i++] - '0'; /*** n 为输入的块数 ***/
		if (argv[2][i] != ')'&&argv[2][i])
		{
			printf("\n ERROR! the entered NUMBER is invalid\n"); 
			exit(0);
		}
		if (num_in>999)
		{
			printf("\n ERROR! can not creat more than 999 files\n"); 
			exit(0);
		}
		byte_rest = total_byte%num_in;
		byte_piece = total_byte / num_in; 
		if (byte_rest>0)
			byte_piece++;
	}
	else
	{
		sort_div = BYTE;
		i = 0;
		while (argv[2][i] >= '0'&&argv[2][i] <= '9') 
		{ 
			num_in = num_in * 10 + argv[2][i] - '0'; 
			i++;
		}
		if (argv[2][i] == '.')
		{
			i++; 
			while (argv[2][i] >= '0' && argv[2][i] <= '9') 
			{ 
				num_in = num_in * 10 + argv[2][i] - '0'; 
				i++;
				pc_float++; 
			}
		}
		ch = argv[2][i];
		if (ch == 'K' || ch == 'k') 
			num_in *= 1024; 
		if (ch == 'M' || ch == 'm') 
			num_in = num_in * 1024 * 1024;
		if (ch&&ch != '.'&&ch != 'k'&&ch != 'K'&&ch != 'm'&&ch != 'M')
		{
			printf("\n ERROR! the entered NUMBER is invalid\n"); 
			exit(0);
		}
		for (i = 1; i <= pc_float; i++) 
			num_in = num_in / 10;
		byte_piece = num_in;
		if (total_byte%byte_piece == 0) 
			pc_fn = total_byte / byte_piece; 
		else 
			pc_fn = total_byte / byte_piece + 1;
		if (pc_fn>999)
		{
			printf("\n ERROR! can not creat more than 999 files\n"); 
			exit(0);
		}
	}
	
	i = 0; /*** d:\dire\file.nnn --->d:\dire\file#nnn ***/
	while (*(fn_in + i))
	{
		if (*(fn_in + i) != '.') *(fn_out + i) = *(fn_in + i); 
		else *(fn_out + i) = '#'; 
		i++;
	}
	*(fn_out + i) = '\0';
	strcpy(fn_bat, fn_out); 
	strcpy(fn_obj_bat, fn_out); 
	strcat(fn_bat, ".bat");
	/*** d:\dire\file#nnn --> d:\dire\file#nnn.bat ***/
	strcat(fn_out, p); 
	len = strlen(fn_out);
	/*** d:\dire\file#nnn --->dire#nnn.000 ***/

	/*** 分割 ***/

	fread(&buffer, 1, 1, fp_read);
	pc_fn = 0;
	int n = 0;
	int x;
	char flag[7];
	while (!feof(fp_read))
	{
		n++;
		fun(fn_out + len - 3); /*** 调用扩展名自加函数 ***/
		fp_write = fopen(fn_out, "wb");

		pc_byte = 0;
		sprintf(flag, "%03d", n);
		strcat(flag, "sig");
		int ss = strlen(flag);
		for (x = 0; x < ss; x++)
		{
			fwrite(&flag[x], 1, 1, fp_write);
			fflush(fp_write);
		}
		while (!feof(fp_read))
		{
			fwrite(&buffer, 1, 1, fp_write);
			++pc_byte;
			fread(&buffer, 1, 1, fp_read);
			if (pc_byte == byte_piece) 
			{ 
				fclose(fp_write); 
				break; 
			}
		}
		pc_fn++;
		if (sort_div == PIECE) 
			if (pc_fn == byte_rest)
				byte_piece--;
		/*** 若按块分割，前 byte_rest 个块文件均比后面的块文件多 1 个字节 ***/
	}
	fclose(fp_read);
	fclose(fp_write);

	/**** 生成拷贝文件 ***/
	
	if (pc_fn <= 31)
	{
		fp_bat = fopen(fn_bat, "w"); 
		fputs("copy /b ", fp_bat);
		len = strlen(fn_obj_bat);
		while (fn_obj_bat[len - 1] != '\\'&&len - 1 >= 0) 
			len--; 
		i = 0;
		while (fn_obj_bat[len + i]) 
		{ 
			fn_obj_bat[i] = fn_obj_bat[len + i]; 
			i++; 
		} 
		fn_obj_bat[i] = '\0';
		/*** 分离出目标文件名，(取出 fn_in 的关键词) ***/
		strcat(fn_obj_bat, ".000");
		len = strlen(fn_obj_bat);
		for (i = 1; i <= pc_fn; i++)
		{
			fun(fn_obj_bat + len - 3); /*** 调用扩展名自加函数 ***/
			if (i>1)
				fputs(" + ", fp_bat);
			fputs(fn_obj_bat, fp_bat);
		}
		len = strlen(fn_in);
		while (fn_in[len - 1] != '\\'&&len - 1 >= 0) 
			len--; 
		i = 0;
		while (fn_in[len + i]) 
		{ 
			fn_in[i] = fn_in[len + i];
			i++;
		} 
		
		fn_in[i] = '\0';
		fputs("", fp_bat); 
		fputs(fn_in, fp_bat);
		
		fclose(fp_bat);


	}	
	/*** 报告分割结果 ***/

	printf("\n OK! %d files have been created", pc_fn);
	if (pc_fn <= 31) 
		printf(" AND the batch file\n");
	else 
		printf(" NO the batch file\n");
	if (byte_rest>0)
		byte_piece++;
	if (byte_piece == pc_byte) 
	{
		printf("\n ALL files are %ld B\n", byte_piece);  
	}

	str_pc_fn[0] = pc_fn / 100 + '0'; 
	str_pc_fn[1] = pc_fn / 10 % 10 + '0';
	str_pc_fn[2] = pc_fn % 10 + '0';
	str_pc_fn[3] = '\0';
	switch (sort_div)
	{
	case PIECE:
		if (byte_rest)
		{
			str_rest_byte[0] = byte_rest / 100 + '0'; 
			str_rest_byte[1] = byte_rest / 10 % 10 + '0';
			str_rest_byte[2] = byte_rest % 10 + '0'; 
			str_rest_byte[3] = '\0';
		}
		if (byte_rest == 1) 
			printf("\n NO.001 is %ld B\n", byte_piece);
		if (byte_rest>1) 
			printf("\n NO.001 --> NO.%s are %ld B\n", str_rest_byte, byte_piece);
		if (pc_fn - byte_rest == 1) 
			printf("\n NO.%s is %ld B\n", str_pc_fn, pc_byte);
		if (pc_fn - byte_rest>1)
		{
			fun(str_rest_byte); 
			printf("\n NO.%s --> NO.%s are %ld B\n", 
			str_rest_byte, str_pc_fn, pc_byte);
		}
		break;
	case BYTE:
		if (pc_fn == 2) 
			printf("\n NO.001 is %ld B\n", byte_piece);
		else
		{
			/*** 借用 str_rest_byte 存储 pc_fn-- ***/
			pc_fn--;
			str_rest_byte[0] = pc_fn / 100 + '0'; 
			str_rest_byte[1] = pc_fn / 10 % 10 + '0'; 
			str_rest_byte[2] = pc_fn % 10 + '0';
			printf("\n NO.001 --> NO.%s are %ld B\n", str_rest_byte, byte_piece);
		}
		printf("\n NO.%s is %ld B\n", str_pc_fn, pc_byte);
	}

	len = strlen(fn_obj_bat);
	fn_obj_bat[len-3] = '0';
	fn_obj_bat[len-2] = '0';
	fn_obj_bat[len-1] = '1';
	
	char Encryption[200] = (".\\Encryption.exe ");
	strcat(Encryption, ".\\FILE\\");
	strcat(Encryption, fn_obj_bat);
	strcat(Encryption, " .\\FILE\\11");
	strcat(Encryption, fn_obj_bat);
	strcat(Encryption, " ");
	strcat(Encryption, "123");
	system(Encryption);
	char MZpart[100] = (".\\FILE\\");
	strcat(MZpart, fn_obj_bat);
	remove(MZpart);

	/*****重命名****/
	int j;
	for (j = 1; j <= pc_fn; j++)
	{
		char newname[100] = (".\\FILE\\");
		strcat(newname, string[j - 1]);
		if (j == 1)
		{

			char oldname[100] = (".\\FILE\\11");
			strcat(oldname, fn_obj_bat);
			rename(oldname, newname);
		}
		else{
			sprintf(&fn_obj_bat[len - 3], "%d", j / 100);
			sprintf(&fn_obj_bat[len - 2], "%d", j / 10);
			sprintf(&fn_obj_bat[len - 1], "%d", j % 10);
			char oldname[100] = (".\\FILE\\");
			strcat(oldname, fn_obj_bat);
			rename(oldname, newname);
		}
	}
	char oldexe[100] = (".\\FILE\\");
	strcat(oldexe, fn_in);
	remove(oldexe);
	remove(fn_bat);
	return;
}


/*** 定义扩展名自加函数 ***/
void fun(char *s)
{
	if (s[2]<'9') 
		s[2]++;
	else if (s[1]<'9') 
	{ 
		s[2] -= 9; 
		s[1]++; 
	}
	else {
		s[2] -= 9; 
		s[1] -= 9;
		s[0]++; 
	}
}