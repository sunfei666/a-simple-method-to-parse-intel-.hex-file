#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_num(char c){
	if(c >= 'a' && c <= 'f'){
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}else if(c >= '0' && c <= '9'){
		return c - '0';
	}
	printf("invalid char to change to hex!\n");
	return -1;
}

int get_hex(char *str, int index){
	return get_num(str[index+1]) + get_num(str[index+0]) * 0x10;
}

/*
 * paras:
 *      file  : *.hex type file name
 *      buffer: buffer to get data
 *      len   : len to get data
 *
 * */
int file_to_hex(char *file, char *buffer, int len)
{
	FILE *infile;
	int index = 0, cnt=0, i;
	char line[1024];

	if(!file || !buffer){
		return -1;
	}

	memset(buffer, 0, len);

	infile=fopen(file, "r");
	if(infile == NULL) {
		printf("cant open file:%s\n", file);
		return -2;
	}

	while (!feof(infile)) {
		memset(line, 0, sizeof(line));
		/* get one line  */
		fgets(line, 1024, infile);

		/* get type of current line, if not 0:DATA record, goto next */
		if(get_hex(line, 7) != 0) {
			continue;
		}

		cnt = get_hex(line, 1);/* get byte cnt in current line */

		for (i=0; i<cnt*2; i++) {
			if((i&0x1) == 0) {
				/* get a byte data */
				buffer[index++] = get_hex(line, i+9);
				if(index >= len) {
					goto END;
				}
			}
		}
	}

END:
	fclose(infile);
	return 0;
}

int main(void)
{
	char *in_name = "test.hex";
	char out_buffer[16*1024];

	memset(out_buffer, 0, sizeof(out_buffer));

	if(file_to_hex(in_name, out_buffer, 4400)) {
		printf("get bin fail!\n");
	}

	int i;
	for(i=0; i<4400; i++) {
		printf("%#2x,", ((unsigned char *)out_buffer)[i]);
		if(i%16 == 0)
			printf("\n");
	}

	return 0;
}
