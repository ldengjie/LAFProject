#ifndef ICONV_CONST
# define ICONV_CONST const
#endif

#include "libchardet-0.0.4/src/chardet.h"
#include  <iostream>
#include  "iconv.h"
using namespace std;

char* GetLocalEncoding(const char* in_str, unsigned int str_len, char* out_encode){
    chardet_t chardect=NULL;
    if(chardet_create(&chardect)==CHARDET_RESULT_OK){
        if(chardet_handle_data(chardect, in_str, (unsigned int)str_len) == CHARDET_RESULT_OK)
            if(chardet_data_end(chardect) == CHARDET_RESULT_OK)
                chardet_get_charset(chardect, out_encode, CHARDET_MAX_ENCODING_NAME);
    }
    if(chardect)
        chardet_destroy(chardect);
    return out_encode;
}
int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);
    if(cd==0) return -1;
    memset(outbuf,0,outlen);
    if(iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
    iconv_close(cd);
    return 0;
}
int t2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
int g2t(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}
int u2t(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("unicode","utf-8",inbuf,inlen,outbuf,outlen);
}
int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("unicode","gb2312",inbuf,inlen,outbuf,outlen);
}
/*
   int charsetConvert(const char *from_charset,const char *to_charset, const char *src, const int srclen, char* save,int savelen) {

   if(save==NULL||srclen == 0) {
   return -1;
   }

   save[0] = 0;
   if (strcmp(from_charset, to_charset) == 0) {
   if(savelen<=srclen)
   strncat(save, src, savelen);
   else
   strncat(save, src, srclen);
   return savelen>srclen ? srclen : savelen;
   }

//convert
iconv_t cd;
int status = 0; //result
char *outbuf = save;//iconv outptr begin
ICONV_CONST char* inptr = src;
char* outptr = outbuf;
size_t insize = srclen;
size_t outsize = savelen;

cd = iconv_open(to_charset, from_charset);
if((iconv_t)(-1) == cd){
return -1;
}
iconv(cd, NULL, NULL, NULL, NULL);
while (insize > 0) {
size_t res = iconv(cd, (ICONV_CONST char**) &inptr, &insize, &outptr,&outsize);
if (outptr != outbuf) {
outbuf=outptr;
 *outbuf=0;
 }
 if (res == (size_t) (-1)) {
 if (errno == EILSEQ) {
 int one = 1;
 iconvctl(cd, ICONV_SET_DISCARD_ILSEQ, &one);
 status = -3;
 } else if (errno == EINVAL) {
 if (srclen == 0) {
 status = -4;
 goto done;
 } else {
 break;
 }
 } else if (errno == E2BIG) {
 status = -5;
 goto done;
 } else {
 status = -6;
 goto done;
 }
 }
 }
 status = strlen(save);// ===  outbuf - save ;
done:
iconv_close(cd);
return status;
}


int ConvertToGb(const char* in_str, int str_len,char* out_str, int out_str_len){
char encode[CHARDET_MAX_ENCODING_NAME];
encode[0]=0;
GetLocalEncoding(in_str,str_len,encode);
if(encode[0]==0){
//I'll try UTF-8 ,If you think it dosn't matter about undetect encode ,return -1 is ok
sprintf(encode,"%s","UTF-8");
//return -1;
}

return charsetConvert(encode,"GB18030",in_str,str_len,out_str,out_str_len);
}
    */
int main(int argc, char *argv[])
{
    return 1;
}
