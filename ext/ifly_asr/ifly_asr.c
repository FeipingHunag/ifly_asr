#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "qisr.h"
#include <ruby.h>

#define kBufferSize 1024*4

static VALUE ifly_asr_iat(VALUE klass, VALUE fileName) {
  char *audioFileName = StringValue(fileName);

  int ret = 0, len, status = 2, ep_status = 0, rec_status = 0, rslt_status = 0, i = 0;
    char buff[kBufferSize], text[kBufferSize];
    FILE *fin;

    ret = QISRInit("appid=50ebc3c9,vad_enable=0");
    if (ret != 0) {
      rb_raise(rb_eRuntimeError, "ISR Initialize Error");
    }

    const char *sess_id = QISRSessionBegin("", "ssm=1,sub=iat,auf=audio/L16;rate=8000,aue=amr;-1,ent=sms8k,rst=plain,rse=utf8", &ret);

    if (ret != 0){
      rb_raise(rb_eRuntimeError, "ISR Session Begin Error");
    }

    fin = fopen(audioFileName, "rb");
    if (!fin) {
      rb_raise(rb_eRuntimeError, "ISR Can not open audio input file");
    }

    while(!feof(fin)) {
        len = fread(buff, 1, kBufferSize, fin);

        ret = QISRAudioWrite(sess_id, buff, len, status, &ep_status, &rec_status);

        if (ret != 0) break;

        usleep(200000);
    }
    fclose(fin);

    status = 4;
    ret = QISRAudioWrite(sess_id, buff, 1, status, &ep_status, &rec_status);
    if (ret != 0) {
      rb_raise(rb_eRuntimeError, "ISR Audio Last Frame Write Error");
    }

    do {
        const char *result = QISRGetResult(sess_id, &rslt_status, 0, &ret);
        if (ret != 0) break;
        if (rslt_status != 1 && result) strcat(text, result);
        usleep(500000);
    } while (rslt_status != 5 && ++i < 30);

    QISRSessionEnd(sess_id, NULL);
    QISRFini();

    return rb_str_new(text, strlen(text));
}

/* ruby calls this to load the extension */
void Init_ifly_asr(void) {
  VALUE module = rb_define_module("IflyAsr");

  /* the hola_bonjour function can be called
   * from ruby as "IflyAsr.iat" */
  rb_define_singleton_method(module, "iat", ifly_asr_iat, 1);
}
