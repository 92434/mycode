#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hex_dump(char *data, int max){
	int i = 0;
	bool b_hex = true;
	int i_char_start = 0;
	while(i < max){
		if(i && !(i % 16) || i == max - 1){
			printf("\n");
			if(b_hex == true){
				b_hex = false;
				i = i_char_start;
			} else {
				b_hex = true;	
				i_char_start = i;
			}
		} else if(i){
			printf(" ");
		}

		if(b_hex == 1){
			printf("%02x", (unsigned char)data[i]);
		} else {
			//printf("%c ", (unsigned char)data[i]);
		}

		i++;
	}
	printf("\n");
}

void parse_config(const char *cp_config){
	FILE *f=fopen(cp_config, "r");
	char line[256];
	while (!feof(f))
	{
		if (!fgets(line, 256, f))
			break;
		if (strlen(line) && line[strlen(line)-1]=='\r')
			line[strlen(line)-1]=0;
		if (!line[0])
			break;

		{
			char name[256], value[256], value1[256];
			int id, id1, id2;
			sscanf(line, "%[^:]:%[^:]:%[^:]", name, value, value1);
			sscanf(name, "%s", name);
			sscanf(value, "%d", &id1);
			sscanf(value1, "%d", &id2);
			printf(">>>%s,%s,%s\n", name, value, value1);
			printf(">>>%s,%d,%d\n", name, id1, id2);
		}
	}
	fclose(f);
}

int main(int argc, char *argv[]){
	parse_config("config.conf");
}
