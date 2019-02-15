#ifndef LUMI_CMD_H_
#define LUMI_CMD_H_

#include <ZW_typedefs.h>

#define CMD\
    COMMON  lumi_common;\
    CONTACT lumi_contact;\
    CURTAIN lumi_curtain;\
    DIMMER  lumi_dimmer;\

typedef struct _COMMON_ {
    BYTE no;     /* Device No */
    BYTE type;   /* Device Type */
} COMMON;

typedef struct _CONTACT_ {
    BYTE no;
    BYTE type;
    BYTE val;
} CONTACT;

typedef struct _CURTAIN_ {
    BYTE no;
    BYTE type;
    BYTE state;
    BYTE level;
} CURTAIN;

typedef struct _DIMMER_ {
    BYTE no;
    BYTE type;
    BYTE level;
} DIMMER;

typedef union _CMD_BUFFER_ {
    CMD 
} CMD_BUFFER, *CMD_BUFFER_P;

#endif /* !LUMI_CMD_H_ */
