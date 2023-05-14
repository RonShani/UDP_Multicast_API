#include "MultiCaster.h"
#include <stdio.h>

void MyHandler(MultiCaster *_caster, void *_data, size_t _length)
{
	printf("%s\n",(char*)_data);
}

int main(int argc, char *argv[])
{
	MultiCaster *caster = CreateCaster("224.0.0.30",3000, 256, MyHandler);
	if(strcmp("send",argv[1]) == 0){
		SendMulti(caster,argv[2]);
	}
	else if(strcmp("listen",argv[1]) == 0){
		ListenMulti(caster);
	}
    return 0;
}
