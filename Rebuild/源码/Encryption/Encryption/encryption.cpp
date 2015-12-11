#define TRUE 1  
#define FALSE 0  

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <io.h>     // �����/usr/include/�Ҳ���,������/usr/include/sys/���ƹ�ȥ  


// �����Ϣ  
void msg_log(char *str);

// �ж��ļ��Ƿ����  
int file_exists(char *filename);


// ������  
int main(int argc, char *argv[]){

	int keylen, index = 0;
	char *source, *dest, *key, fBuffer[1], tBuffer[20], ckey;

	FILE *fSource, *fDest;

	source = argv[1]; // ԭ�ļ�  
	dest = argv[2];   // Ŀ���ļ�  
	key = argv[3];    // �����ִ�  

	// ������  
	if (source == NULL || dest == NULL || key == NULL){
		msg_log("param error\nusage:xor_encrypt source dest key\ne.g ./xor_encrypt o.txt d.txt 123456");
		exit(0);
	}

	// �ж�ԭ�ļ��Ƿ����  
	if (file_exists(source) == FALSE){
		sprintf(tBuffer, "%s not exists", source);
		msg_log(tBuffer);
		exit(0);
	}

	// ��ȡkey����  
	keylen = strlen(key);

	fSource = fopen(source, "rb");
	fDest = fopen(dest, "wb");

	while (!feof(fSource)){

		fread(fBuffer, 1, 1, fSource);    // ��ȡ1�ֽ�  

		if (!feof(fSource)){
			ckey = key[index%keylen];     // ѭ����ȡkey  
			*fBuffer = *fBuffer ^ ckey;   // xor encrypt  
			fwrite(fBuffer, 1, 1, fDest); // д���ļ�  
			index++;
		}

	}

	fclose(fSource);
	fclose(fDest);

	msg_log("Encrypt or Decrypt success��");

	exit(0);
}

//�����Ϣ  
void msg_log(char *str){
	printf("%s\n", str);
}

// �ж��ļ��Ƿ����  
int file_exists(char *filename){
	return (_access(filename, 0) == 0);
}